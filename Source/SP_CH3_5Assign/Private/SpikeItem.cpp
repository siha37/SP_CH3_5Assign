// Fill out your copyright notice in the Description page of Project Settings.


#include "SpikeItem.h"

#include "Kismet/GameplayStatics.h"

ASpikeItem::ASpikeItem()
{
	ItemType = "Spike";
	SpikeDamage = 10.f;
}

void ASpikeItem::ActivateItem(AActor* OverlapActor)
{
	if (!OverlapActor || !bCanActivate)
	{
		return;
	}

	bCanActivate = false;

	UGameplayStatics::ApplyDamage(
		OverlapActor,
		SpikeDamage,
		nullptr,
		this,
		UDamageType::StaticClass()
	);
}

void ASpikeItem::OnItemEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		bCanActivate = true;
	}
}
