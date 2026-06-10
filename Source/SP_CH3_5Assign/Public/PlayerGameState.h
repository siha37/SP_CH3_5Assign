// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Level")
	TArray<FName> LevelMapNames;

	FTimerHandle LevelTimerHandle;
	FTimerHandle HUDDUpdateTimerHandle;
public:
	virtual void BeginPlay() override;
	APlayerGameState();

	void StartLevel();
	
	UFUNCTION(BlueprintPure,Category="Score")
	int32 GetScore() const;

	UFUNCTION(BlueprintCallable,Category="Score")
	void AddScore(int32 Amount);

	
	void OnLevelTimeUp();
	void OnCoinCollected();
	void EndLevel();
	void OnGameOver();
	void UpdateHUD();
};
