// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinItem.h"
#include "PlayerGameState.h"


ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "Default";	
}

void ACoinItem::ActivateItem(AActor* OverlapActor)
{
	Super::ActivateItem(OverlapActor);
	if(OverlapActor && OverlapActor->ActorHasTag("Player"))
	{
		if(UWorld* World = GetWorld())
		{
			if(APlayerGameState* state = World->GetGameState<APlayerGameState>())
			{
				state->AddScore(PointValue);
				state->OnCoinCollected();
			}
		}
		DestroyItem();
	}
}
