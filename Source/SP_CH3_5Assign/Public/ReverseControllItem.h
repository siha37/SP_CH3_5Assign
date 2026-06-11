// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "ReverseControllItem.generated.h"

UCLASS()
class SP_CH3_5ASSIGN_API AReverseControllItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AReverseControllItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ReverseDuration;

	virtual void ActivateItem(AActor* OverlapActor) override;
};
