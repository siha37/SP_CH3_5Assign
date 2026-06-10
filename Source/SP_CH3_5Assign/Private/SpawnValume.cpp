// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnValume.h"

#include "ItemSpawnRow.h"
#include "Components/BoxComponent.h"

// Sets default values
ASpawnValume::ASpawnValume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	SpawningBox = CreateDefaultSubobject<UBoxComponent>("SpawningBox");
	SpawningBox->SetupAttachment(Root);
}

AActor* ASpawnValume::SpawnRandomItem(UDataTable* ItemDataTable)
{
	if(FItemSpawnRow* SelectedRow = GetRandomItem(ItemDataTable))
	{
		if(UClass* ActualClass = SelectedRow->ItemClass.Get())
		{
			return SpawnItem(ActualClass);
		}
	}
	return nullptr;
}

FItemSpawnRow* ASpawnValume::GetRandomItem(UDataTable* ItemDataTable)
{
	if(!ItemDataTable) return nullptr;

	TArray<FItemSpawnRow*> Rows;
	static const FString ContextString(TEXT("ASpawnValume::GetRandomItem"));
	ItemDataTable->GetAllRows(ContextString,Rows);

	if(Rows.IsEmpty()) return nullptr;

	float TotalChance = 0.0f;

	for(const FItemSpawnRow* Row : Rows)
	{
		if(Row)
		{
			TotalChance += Row->Spawnchance;
		}
	}
	const float RandValue = FMath::RandRange(0.0f, TotalChance);
	float AccumulatedChance = 0.0f;

	for(FItemSpawnRow* Row : Rows)
	{
		AccumulatedChance += Row->Spawnchance;
		if(AccumulatedChance >= RandValue)
			return Row;
	}
	return nullptr;
}

FVector ASpawnValume::GetRandomPointInVolume() const
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();

	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z));
}

AActor* ASpawnValume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if(!ItemClass) return nullptr;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetRandomPointInVolume(),
		FRotator::ZeroRotator);
	return SpawnedActor;
}

