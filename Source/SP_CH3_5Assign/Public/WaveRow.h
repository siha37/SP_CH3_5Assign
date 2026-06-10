// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.h"
#include "WaveRow.generated.h"

USTRUCT(BlueprintType)
struct FWaveRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ItemAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaveDuration = 30.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* SpawnRow;
	
};