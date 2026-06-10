// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingItem.h"

#include "MyPlayer.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* OverlapActor)
{
	Super::ActivateItem(OverlapActor);
	if(OverlapActor && OverlapActor->ActorHasTag("Player"))
	{
		if(AMyPlayer* player = Cast<AMyPlayer>(OverlapActor))
		{
			player->AddHealth(HealAmount);
		}
		DestroyItem();
	}
}
