// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "SpikeItem.generated.h"

UCLASS()
class SP_CH3_5ASSIGN_API ASpikeItem : public ABaseItem
{
	GENERATED_BODY()

public:
	ASpikeItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float SpikeDamage;

protected:
	bool bCanActivate = true;

	virtual void ActivateItem(AActor* OverlapActor) override;

	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;
};
