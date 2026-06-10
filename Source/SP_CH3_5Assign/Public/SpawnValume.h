#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnValume.generated.h"

struct FItemSpawnRow;
class UBoxComponent;

UCLASS()
class SP_CH3_5ASSIGN_API ASpawnValume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnValume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* ItemDataTable;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnRandomItem();
	
	FItemSpawnRow* GetRandomItem();
	FVector GetRandomPointInVolume() const;
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);
};
