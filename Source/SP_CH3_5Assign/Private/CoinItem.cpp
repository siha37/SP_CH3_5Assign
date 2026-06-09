// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinItem.h"

ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "Default";	
}

void ACoinItem::ActivateItem(AActor* OverlapActor)
{
	if(OverlapActor && OverlapActor->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Red,"Player Hit!");
		DestroyItem();
	}
}
