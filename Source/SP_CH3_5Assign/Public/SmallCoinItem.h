// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoinItem.h"
#include "SmallCoinItem.generated.h"

UCLASS()
class SP_CH3_5ASSIGN_API ASmallCoinItem : public ACoinItem
{
	GENERATED_BODY()
public:
	ASmallCoinItem();
protected:
	virtual void ActivateItem(AActor* OverlapActor) override;
};
