// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "MyGameInstance.h"
#include "PlayerGameState.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

AMyPlayerController::AMyPlayerController()
	: InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	RollAction(nullptr),
	HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr),
	MainMenuWidgetClass(nullptr),
	MainMenuWidgetInstance(nullptr)
{
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if(InputMappingContext)
			{
				// IMC 매핑 / 우선순위 0번
				Subsystem->AddMappingContext(InputMappingContext,0);
			}	
		}
	}

	FString CurrentMapName = GetWorld()->GetMapName();
	if(CurrentMapName.Contains("MenuLevel"))
	{
		ShowMainMenu(false);
	}
}

UUserWidget* AMyPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void AMyPlayerController::ShowGameHUD()
{
	if(HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
	if(MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}
	if(HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if(HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor =  false;
			SetInputMode(FInputModeGameOnly());
		}
	}
	APlayerGameState* PlayerGameState = GetWorld()? GetWorld()->GetGameState<APlayerGameState>():nullptr;
	if(PlayerGameState)
		PlayerGameState->UpdateHUD();
}

void AMyPlayerController::ShowMainMenu(bool bIsRestart)
{
	if(HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
	if(MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}
	if(MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if(MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor =  true;
			SetInputMode(FInputModeUIOnly());
		}
	}
	if(UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
	{
		if(bIsRestart)
		{
			ButtonText->SetText(FText::FromString(TEXT("Restart")));
			UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayerGameOverAnim"));
			if(PlayAnimFunc)
			{
				MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc,nullptr);
			}
		}
		else
		{
			ButtonText->SetText(FText::FromString(TEXT("Start")));
		}
	}
	
	if(bIsRestart)
	{
		UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayerGameOverAnim"));
		if(PlayAnimFunc)
		{
			MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc,nullptr);
		}

		if(UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("TotalScoreText"))))
		{
			if(UMyGameInstance* instance = Cast<UMyGameInstance>(MainMenuWidgetInstance->GetGameInstance()))
			{
				ButtonText->SetText(FText::FromString(FString::Printf(TEXT("SCORE : %d"),instance->TotalScore)));	
			}
		}
	}
}

void AMyPlayerController::StartGame()
{
	if(UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		GameInstance->CurrentLevelIndex =0;
		GameInstance->TotalScore = 0;
	}
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("BasicLevel"));
	SetPause(false);
}
