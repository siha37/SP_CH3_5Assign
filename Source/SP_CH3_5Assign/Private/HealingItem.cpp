// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingItem.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* OverlapActor)
{
	if(OverlapActor && OverlapActor->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Red,"Player Hit!");
		DestroyItem();
	}
}
