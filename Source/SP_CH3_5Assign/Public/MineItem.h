// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MineItem.generated.h"

/**
 * 
 */
UCLASS()
class SP_CH3_5ASSIGN_API AMineItem : public ABaseItem
{
	GENERATED_BODY()
public:
	AMineItem();

	USphereComponent* ExplosionCollision;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	float ExplosionDelay;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	float ExplosionRadius;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	int ExplosionDamage;

	FTimerHandle TimerHandle;
	
	virtual void ActivateItem(AActor* OverlapActor) override;

	void Explode();
};
