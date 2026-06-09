// Copyright Epic Games, Inc. All Rights Reserved.

#include "SP_CH3_5AssignGameMode.h"
#include "SP_CH3_5AssignCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASP_CH3_5AssignGameMode::ASP_CH3_5AssignGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
