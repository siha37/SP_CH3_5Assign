// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGameState.h"

#include "BaseItem.h"
#include "CoinItem.h"
#include "MyGameInstance.h"
#include "MyPlayerController.h"
#include "SpawnValume.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void APlayerGameState::ClearGameTimers()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearAllTimersForObject(this);
	}
}

void APlayerGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ClearGameTimers();
	Super::EndPlay(EndPlayReason);
}

void APlayerGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDDUpdateTimerHandle,
		this,
		&APlayerGameState::UpdateHUD,
		0.1f,
		true
	);
}

APlayerGameState::APlayerGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
	CurrentWaveCount = 0;
}

void APlayerGameState::StartLevel()
{
	if (APlayerController* Controll = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyControll = Cast<AMyPlayerController>(Controll))
		{
			MyControll->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance))
		{
			CurrentLevelIndex = MyGameInstance->CurrentLevelIndex;
		}
	}

	CurrentWaveCount = 0;
	StartWave();
}

void APlayerGameState::StartWave()
{
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	ClearSpawnedItems();

	FWaveRow* Wave = GetWaveData();
	if (!Wave)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartWave: Wave data not found. Level=%d Wave=%d"),
			CurrentLevelIndex, CurrentWaveCount);
		return;
	}

	UDataTable* ItemData = GetItemData();
	if (!ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartWave: Item spawn data not found."));
		return;
	}

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnValume::StaticClass(), FoundVolumes);

	for (int32 i = 0; i < Wave->ItemAmount; ++i)
	{
		if (FoundVolumes.Num() > 0)
		{
			if (ASpawnValume* SpawnVolume = Cast<ASpawnValume>(FoundVolumes.Last()))
			{
				if (AActor* SpawnedActor = SpawnVolume->SpawnRandomItem(ItemData))
				{
					if (SpawnedActor->IsA(ACoinItem::StaticClass()))
					{
						++SpawnedCoinCount;
					}
				}
			}
		}
	}

	const float Duration = Wave->WaveDuration > 0.f ? Wave->WaveDuration : LevelDuration;

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&APlayerGameState::OnLevelTimeUp,
		Duration,
		false
	);
}

void APlayerGameState::ClearSpawnedItems()
{
	TArray<AActor*> FoundItems;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseItem::StaticClass(), FoundItems);

	for (AActor* Item : FoundItems)
	{
		if (Item)
		{
			Item->Destroy();
		}
	}
}

int32 APlayerGameState::GetMaxWavesForCurrentLevel() const
{
	if (!LevelMapData.IsValidIndex(CurrentLevelIndex))
	{
		return 0;
	}

	const UDataTable* TargetData = LevelMapData[CurrentLevelIndex];
	if (!TargetData)
	{
		return 0;
	}

	TArray<FWaveRow*> Rows;
	static const FString ContextString(TEXT("APlayerGameState::GetMaxWavesForCurrentLevel"));
	TargetData->GetAllRows(ContextString, Rows);
	return Rows.Num();
}

int32 APlayerGameState::GetScore() const
{
	return Score;
}

void APlayerGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance))
		{
			MyGameInstance->AddTorScore(Amount);
		}
	}
}

void APlayerGameState::OnLevelTimeUp()
{
	EndWave(false);
}

void APlayerGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Collected Coin Count: %d"), CollectedCoinCount);

	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndWave(true);
	}
}

void APlayerGameState::EndWave(bool bClearLevelTimer)
{
	if (bClearLevelTimer)
	{
		GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	}

	CurrentWaveCount++;

	const int32 MaxWaves = GetMaxWavesForCurrentLevel();
	if (CurrentWaveCount < MaxWaves)
	{
		StartWave();
		return;
	}

	EndLevel();
}

void APlayerGameState::EndLevel()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(HUDDUpdateTimerHandle);
	}

	CurrentLevelIndex++;

	if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance))
		{
			AddScore(Score);
			MyGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}

	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}

void APlayerGameState::OnGameOver()
{
	ClearGameTimers();
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	GetWorldTimerManager().ClearTimer(HUDDUpdateTimerHandle);
	ClearSpawnedItems();

	if (APlayerController* Controll = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyControll = Cast<AMyPlayerController>(Controll))
		{
			MyControll->SetPause(true);
			MyControll->ShowMainMenu(true);
		}
	}
}

void APlayerGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = MyPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("TIME: %.1f"), RemainingTime)));
				}
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance))
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("SCORE: %d"), MyGameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* LevelText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelText->SetText(FText::FromString(FString::Printf(
						TEXT("LEVEL %d - WAVE %d"),
						CurrentLevelIndex + 1,
						CurrentWaveCount + 1
					)));
				}
			}
		}
	}
}

FWaveRow* APlayerGameState::GetWaveData() const
{
	if (!LevelMapData.IsValidIndex(CurrentLevelIndex))
	{
		return nullptr;
	}

	const UDataTable* TargetData = LevelMapData[CurrentLevelIndex];
	if (!TargetData)
	{
		return nullptr;
	}

	TArray<FWaveRow*> Rows;
	static const FString ContextString(TEXT("APlayerGameState::GetWaveData"));
	TargetData->GetAllRows(ContextString, Rows);

	if (Rows.IsEmpty() || !Rows.IsValidIndex(CurrentWaveCount))
	{
		return nullptr;
	}

	return Rows[CurrentWaveCount];
}

UDataTable* APlayerGameState::GetItemData() const
{
	const FWaveRow* Row = GetWaveData();
	return Row ? Row->SpawnRow : nullptr;
}
