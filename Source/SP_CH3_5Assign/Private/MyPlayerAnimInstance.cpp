// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerAnimInstance.h"
#include "MyPlayer.h"

void UMyPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (const AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner()))
	{
		Speed = Player->GetGroundSpeed();
		bIsFalling = Player->GetIsFalling();
		bIsJumping = Player->GetIsJumping();
	}
}
