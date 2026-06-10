// Fill out your copyright notice in the Description page of Project Settings.


#include "MineItem.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AMineItem::AMineItem()
{
	ExplosionRadius = 300.f;
	ExplosionDelay = 5.f;
	ExplosionDamage = 30.f;
	ItemType = "Mine";
	bHasExploded = false;
	
	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);
}


void AMineItem::ActivateItem(AActor* OverlapActor)
{
	if(bHasExploded) return;
	bHasExploded = true;
	Super::ActivateItem(OverlapActor);
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&AMineItem::Explode,
		ExplosionDelay,
		false);
}

void AMineItem::Explode()
{
	UParticleSystemComponent* Particle = nullptr;
	
	if(ExplosionParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ExplosionParticle,
			GetActorLocation(),
			GetActorRotation(),
			false
			);
	}
	if(ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ExplosionSound,
			GetActorLocation()
			);
	}
	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);

	for(AActor* actor : OverlappingActors)
	{
		if(actor && actor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(
				actor,
				ExplosionDamage,
				nullptr,
				this,
				UDamageType::StaticClass()
				);
		}
	}
	
	DestroyItem();

	if(Particle)
	{
		FTimerHandle DestroyParticleHandle;
		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleHandle,
			[Particle]()
			{
				Particle->DestroyComponent();	
			},
			1.0f,
			false
			);
	}
}
