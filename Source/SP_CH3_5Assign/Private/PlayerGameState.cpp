// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGameState.h"

#include "CoinItem.h"
#include "MyGameInstance.h"
#include "MyPlayerController.h"
#include "SpawnValume.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnitConversion.h"

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
	SpawnedCoinCount=0;
	CollectedCoinCount= 0;
	LevelDuration = 30;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
}


void APlayerGameState::StartLevel()
{
	if(APlayerController* controll = GetWorld()->GetFirstPlayerController())
	{
		if(AMyPlayerController* myControll = Cast<AMyPlayerController>(controll))
		{
			myControll->ShowGameHUD();
		}
	}

	
	if(UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		if(MyGameInstance)
		{
			CurrentLevelIndex = MyGameInstance->CurrentLevelIndex;
		}
	}
	
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ASpawnValume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawnCount = 40;

	for (int32 i = 0; i < ItemToSpawnCount; i++)
	{
		if(FoundVolumes.Num()>0)
		{
			ASpawnValume* SpawnValume = Cast<ASpawnValume>(FoundVolumes.Last());
			if(SpawnValume)
			{
				AActor* SpawnedActor = SpawnValume->SpawnRandomItem();
				if(SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					++SpawnedCoinCount;
				}
			}
		}
	}	
	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&APlayerGameState::OnLevelTimeUp,
		LevelDuration,
		false
		);
}

int32 APlayerGameState::GetScore() const
{
	return Score;
}

void APlayerGameState::AddScore(int32 Amount)
{
	if(UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		if(MyGameInstance)
		{
			MyGameInstance->AddTorScore(Amount);
		}
	}
}
void APlayerGameState::OnLevelTimeUp()
{
	EndLevel();
}

void APlayerGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Collected Coin Count: %d"), CollectedCoinCount);

	if(SpawnedCoinCount > 0 &&CollectedCoinCount >= SpawnedCoinCount)
		EndLevel();
		
}

void APlayerGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	CurrentLevelIndex++;

	if(UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		if(MyGameInstance)
		{
			AddScore(Score);
			MyGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}
	if(CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}
	if(LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(),LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}

void APlayerGameState::OnGameOver()
{
	if(APlayerController* controll = GetWorld()->GetFirstPlayerController())
	{
		if(AMyPlayerController* myControll = Cast<AMyPlayerController>(controll))
		{
			myControll->SetPause(true);
			myControll->ShowMainMenu(true);
		}
	}

}

void APlayerGameState::UpdateHUD()
{
	if(APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if(AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			if(UUserWidget* HUDWidget = MyPlayerController->GetHUDWidget())
			{
				if(UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("TIME: %.1f"), RemainingTime)));
				}
				if(UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if(UGameInstance* GameInstance = GetGameInstance())
					{
						if(UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance))
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("SCORE: %d"), MyGameInstance->TotalScore)));
						}
					}
				}
				
				if(UTextBlock* LevelText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelText->SetText(FText::FromString(FString::Printf(TEXT("LEVEL %d"), CurrentLevelIndex+1)));
				}
			}
		}
		
	}
}
