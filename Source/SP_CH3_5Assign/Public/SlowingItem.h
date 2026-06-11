// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "SlowingItem.generated.h"

UCLASS()
class SP_CH3_5ASSIGN_API ASlowingItem : public ABaseItem
{
	GENERATED_BODY()
public:
	ASlowingItem();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	float SlowingPercent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	float SlowingDuration; 

	virtual void ActivateItem(AActor* OverlapActor) override;
};
