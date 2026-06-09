// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"

#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMyPlayer::AMyPlayer()
{	
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetSimulatePhysics(false);
	RootComponent = Capsule;

	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Capsule);

	
	Spring = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring"));
	Spring->SetupAttachment(Capsule);
	Spring->TargetArmLength = 300.0f;  
	Spring->bUsePawnControlRotation = false;

	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Spring,USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	
	
	PrimaryActorTick.bCanEverTick = true;

}



// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}


void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
					&AMyPlayer::Move
					);
			}

			if(PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Started,
					this,
					&AMyPlayer::StartJump
					);
			}
			
			if(PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPlayer::Look
					);
			}
		}
	}
}

void AMyPlayer::Move(const FInputActionValue& value)
{
	float CurrentSpeed = MoveSpeed;

	//체공 중 속도 조정
	if (bIsFalling)
	{
		CurrentSpeed *= AirControlModifier;
	}
	
	
	FVector2D MoveInput = value.Get<FVector2D>().GetSafeNormal();
	UE_LOG(LogTemp, Display, TEXT("Move %f /%f "), MoveInput.X,MoveInput.Y);
	FRotator CamRot = Spring->GetRelativeRotation();
	FRotator YawRot(0,CamRot.Yaw,0);

	// 전방 좌우 방향 벡터
	FVector ForwardDirection = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X) * MoveInput.X;
	FVector RightDirection   = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y) * MoveInput.Y;
	// 정규화
	FVector MoveDirection = (ForwardDirection + RightDirection).GetSafeNormal();

	// 각도 스무딩
	FRotator TargetRotation = MoveDirection.Rotation();
	FRotator CurrentMeshRot = Mesh->GetRelativeRotation();
	CurrentMeshRot.Yaw += 90.0f;
	
	FRotator SmoothRotation = FMath::RInterpTo(
		 CurrentMeshRot,
		 TargetRotation,
		 GetWorld()->GetDeltaSeconds(),
		 10.0f
	 );
	
	// 최종 이동 방향 벡터
	FVector FinalMoveDirection = FRotationMatrix(SmoothRotation).GetUnitAxis(EAxis::X);
	// 속도 적용
	FVector NewLocation = FinalMoveDirection * CurrentSpeed * GetWorld()->GetDeltaSeconds();
    

	AddActorLocalOffset(NewLocation,true);

	SmoothRotation.Yaw -= 90.0f;
	SmoothRotation.Pitch = 0.0f;
	SmoothRotation.Roll = 0.0f;
	Mesh->SetRelativeRotation(SmoothRotation);
}

void AMyPlayer::StartJump(const FInputActionValue& value)
{
	if(!bIsFalling)
	{
		CurrentVelocity.Z = JumpVelocity;
		bIsFalling = true;
	}
}

void AMyPlayer::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();
	LookInput = LookInput * LookSpeed;

	FRotator newRotation = FRotator(LookInput.Y, LookInput.X,0);
	Spring->AddLocalRotation(newRotation);
	
	FRotator currnt = Spring->GetRelativeRotation();
	currnt.Roll = 0;
	currnt.Pitch = FMath::Clamp(currnt.Pitch, -80.0f, 80.0f);
	Spring->SetRelativeRotation(currnt);
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentVelocity.Z += GravityAcceleration * DeltaTime;
	FVector DeltaLocation = CurrentVelocity * DeltaTime;

	FHitResult Hit;
	FVector StartLocation = GetActorLocation();
	
	
	FVector EndLocation = StartLocation + (FVector::DownVector * TraceDistance);

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit,StartLocation,EndLocation,ECC_Visibility,TraceParams);
	
	if(bHit && CurrentVelocity.Z < 0.0f)
	{
		bIsFalling =false;
		CurrentVelocity.Z = 0.0f;
		DeltaLocation = CurrentVelocity * DeltaTime;
	}
	else
	{
		bIsFalling =true;
	}
	
	AddActorLocalOffset(DeltaLocation,true);
}


