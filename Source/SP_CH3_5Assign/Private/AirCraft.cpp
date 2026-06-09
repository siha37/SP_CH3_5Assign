// Fill out your copyright notice in the Description page of Project Settings.


#include "AirCraft.h"

#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AAirCraft::AAirCraft()
{
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetSimulatePhysics(false);
	RootComponent = Capsule;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Capsule);

	
	Spring = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring"));
	Spring->SetupAttachment(Capsule);
	Spring->TargetArmLength = 600.0f;  
	Spring->bUsePawnControlRotation = false;

	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Spring,USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	
	
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAirCraft::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAirCraft::Move(const FInputActionValue& value)
{
	FVector MoveInput = value.Get<FVector>();

	FVector Location = MoveInput * MoveSpeed * GetWorld()->GetDeltaSeconds();
	
	AddActorLocalOffset(Location, true);
}


void AAirCraft::Look(const FInputActionValue& value)
{
	FVector2d LookDirection = value.Get<FVector2d>();

	float DeltaPitch = LookDirection.Y * PitchSpeed * GetWorld()->GetDeltaSeconds();
	float DeltaYaw = LookDirection.X * YawSpeed * GetWorld()->GetDeltaSeconds();

	AddActorLocalRotation(FRotator(DeltaPitch, 0.0f, 0.0f));
	AddActorWorldRotation(FRotator(0.0f, DeltaYaw, 0.0f));
}

void AAirCraft::Roll(const FInputActionValue& value)
{
	float RollInput = value.Get<float>();

	float DeltaRoll = RollInput * RollSpeed * GetWorld()->GetDeltaSeconds();

	FRotator DeltaRotation(0.0f, 0.0f, DeltaRoll);
	AddActorLocalRotation(DeltaRotation);
}

// Called every frame
void AAirCraft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AAirCraft::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	if(UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if(AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if(PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AAirCraft::Move
					);
			}

			
			if(PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AAirCraft::Look
					);
			}

			if(PlayerController->RollAction)
			{
				EnhancedInput->BindAction(
					PlayerController->RollAction,
					ETriggerEvent::Triggered,
					this,
					&AAirCraft::Roll
					);
			}

		}
	}
}

