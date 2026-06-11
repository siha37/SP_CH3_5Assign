// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyPlayerAnimInstance.generated.h"

UCLASS()
class SP_CH3_5ASSIGN_API UMyPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Animation")
	float Speed = 0.f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Animation")
	bool bIsFalling = false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Animation")
	bool bIsJumping = false;
};
