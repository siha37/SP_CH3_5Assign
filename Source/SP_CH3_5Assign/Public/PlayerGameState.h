// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaveRow.h"
#include "GameFramework/GameState.h"
#include "PlayerGameState.generated.h"

/**
 * 
 */
UCLASS()
class SP_CH3_5ASSIGN_API APlayerGameState : public AGameState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Score")
	int32 Score;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Coin")
	int32 	SpawnedCoinCount;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Coin")
	int32 CollectedCoinCount;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Level")
	float LevelDuration;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Level")
	int32 CurrentLevelIndex;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Level")
	int32 MaxLevels;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Level")
	int32 CurrentWaveCount;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Level")
	TArray<FName> LevelMapNames;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Level")
	TArray<UDataTable*> LevelMapData;

	FTimerHandle LevelTimerHandle;
	FTimerHandle HUDDUpdateTimerHandle;

	void ClearGameTimers();
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	APlayerGameState();

	void StartLevel();
	void StartWave();
	void EndWave(bool bClearLevelTimer);
	void ClearSpawnedItems();
	int32 GetMaxWavesForCurrentLevel() const;
	
	UFUNCTION(BlueprintPure,Category="Score")
	int32 GetScore() const;

	UFUNCTION(BlueprintCallable,Category="Score")
	void AddScore(int32 Amount);

	
	void OnLevelTimeUp();
	void OnCoinCollected();
	void EndLevel();
	void OnGameOver();
	void UpdateHUD();
	FWaveRow* GetWaveData() const;
	UDataTable* GetItemData() const;
};

