// Fill out your copyright notice in the Description page of Project Settings.


#include "ReverseControllItem.h"
#include "MyPlayer.h"

AReverseControllItem::AReverseControllItem()
{
	ReverseDuration = 5.f;
	ItemType = "ReverseControll";
}

void AReverseControllItem::ActivateItem(AActor* OverlapActor)
{
	Super::ActivateItem(OverlapActor);

	if (OverlapActor && OverlapActor->ActorHasTag("Player"))
	{
		if (AMyPlayer* Player = Cast<AMyPlayer>(OverlapActor))
		{
			Player->SetReverseControll(true, ReverseDuration);
		}
		DestroyItem();
	}
}
