// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowingItem.h"
#include "MyPlayer.h"


ASlowingItem::ASlowingItem()
{
	SlowingPercent = 50.f;
	SlowingDuration = 5.f;
	ItemType = "Slowing";
}

void ASlowingItem::ActivateItem(AActor* OverlapActor)
{
	Super::ActivateItem(OverlapActor);
	
	if (OverlapActor && OverlapActor->ActorHasTag("Player"))
	{
		if (AMyPlayer* Player = Cast<AMyPlayer>(OverlapActor))
		{
			Player->SetSlow(SlowingPercent, SlowingDuration);
		}
		DestroyItem();
	}
}
