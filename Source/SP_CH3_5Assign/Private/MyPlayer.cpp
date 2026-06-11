// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "PlayerGameState.h"
#include "Components/CapsuleComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
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

	OverHeadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverHeadWidget"));
	OverHeadWidget->SetupAttachment(Mesh);
	OverHeadWidget->SetWidgetSpace(EWidgetSpace::Screen);	
	
	MaxHealth =  100;
	Health = MaxHealth;

	
	PrimaryActorTick.bCanEverTick = true;
}



// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverHeadHp();
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
	if(SlowMovePercent != 0)
	{
		CurrentSpeed -= CurrentSpeed * SlowMovePercent / 100;
	}

	FVector2D MoveInput = value.Get<FVector2D>().GetSafeNormal();
	const float InputMagnitude = MoveInput.Size();
	GroundSpeed = CurrentSpeed * InputMagnitude;
	bMoveInputThisFrame = true;
	
	if (bIsReverseControl)
	{
		MoveInput *= -1.f;
	}
	
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
		bIsJumping = true;
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

void AMyPlayer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CallHUDSlowUI(false);
	CallHUDReverseUI(false);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(SlowTimerHandle);
		World->GetTimerManager().ClearTimer(ReverceControllHandle);
	}
	Super::EndPlay(EndPlayReason);
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
		bIsFalling = false;
		bIsJumping = false;
		CurrentVelocity.Z = 0.0f;
		DeltaLocation = CurrentVelocity * DeltaTime;
	}
	else if(!bHit && CurrentVelocity.Z < 0.0f)
	{
		bIsJumping = false;
	}
	else
	{
		bIsFalling =true;
	}

	if (!bMoveInputThisFrame)
	{
		GroundSpeed = FMath::FInterpTo(GroundSpeed, 0.f, DeltaTime, 15.f);
	}
	bMoveInputThisFrame = false;

	AddActorLocalOffset(DeltaLocation,true);
}

float AMyPlayer::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	Health = FMath::Clamp(Health-ActualDamage,0,MaxHealth);
	UE_LOG(LogTemp,Log,TEXT("AMyPlayer Take Damage %f"),ActualDamage);
	UpdateOverHeadHp();
	if(Health <= 0)
	{
		OnDeath();
	}
	return ActualDamage;
}

void AMyPlayer::OnDeath()
{
	APlayerGameState* state = GetWorld()->GetGameState<APlayerGameState>();
	if(state)
	{
		state->OnGameOver();
	}
}

void AMyPlayer::UpdateOverHeadHp()
{
	if(!OverHeadWidget) return;

	UUserWidget* widget = OverHeadWidget->GetUserWidgetObject();
	if(!widget) return;

	if(UProgressBar* HpBar = Cast<UProgressBar>(widget->GetWidgetFromName(TEXT("HPBar"))))
	{
		HpBar->SetPercent(Health/MaxHealth);
	}
}

float AMyPlayer::GetHealth() const
{
	return Health;
}

void AMyPlayer::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health+Amount,0,MaxHealth);
	UpdateOverHeadHp();
}


void AMyPlayer::SetSlow(float percent, float duration)
{
	SlowMovePercent = FMath::Clamp(percent, 0.f, 100.f);
	SlowDurationTotal = duration;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(SlowTimerHandle);
		World->GetTimerManager().SetTimer(
			SlowTimerHandle,
			this,
			&AMyPlayer::ResetSlow,
			duration,
			false
		);
	}

	CallHUDSlowUI(true);
}

void AMyPlayer::ResetSlow()
{
	SlowMovePercent = 0.f;
	SlowDurationTotal = 0.f;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(SlowTimerHandle);
	}

	CallHUDSlowUI(false);
}

void AMyPlayer::SetReverseControll(bool isOn, float duration)
{
	bIsReverseControl = isOn;
	ReverseDurationTotal = isOn ? duration : 0.f;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ReverceControllHandle);

		if (isOn && duration > 0.f)
		{
			World->GetTimerManager().SetTimer(
				ReverceControllHandle,
				this,
				&AMyPlayer::ResetReverseControl,
				duration,
				false
			);
		}
	}

	CallHUDReverseUI(isOn);
}

void AMyPlayer::ResetReverseControl()
{
	bIsReverseControl = false;
	ReverseDurationTotal = 0.f;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ReverceControllHandle);
	}

	CallHUDReverseUI(false);
}

bool AMyPlayer::IsSlowActive() const
{
	return SlowMovePercent > 0.f && GetSlowRemainingTime() > 0.f;
}

bool AMyPlayer::IsReverseActive() const
{
	return bIsReverseControl && GetReverseRemainingTime() > 0.f;
}

float AMyPlayer::GetSlowRemainingTime() const
{
	if (const UWorld* World = GetWorld())
	{
		return World->GetTimerManager().GetTimerRemaining(SlowTimerHandle);
	}
	return 0.f;
}

float AMyPlayer::GetReverseRemainingTime() const
{
	if (const UWorld* World = GetWorld())
	{
		return World->GetTimerManager().GetTimerRemaining(ReverceControllHandle);
	}
	return 0.f;
}

void AMyPlayer::CallHUDSlowUI(bool bOn)
{
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController());
	if (!PlayerController)
	{
		return;
	}

	UUserWidget* HUDWidget = PlayerController->GetHUDWidget();
	if (!HUDWidget)
	{
		return;
	}

	UFunction* Func = HUDWidget->FindFunction(FName("SetSlowUIOnOff"));
	if (!Func)
	{
		return;
	}

	struct FSlowUIParams
	{
		bool bOn;
	};

	FSlowUIParams Params;
	Params.bOn = bOn;
	HUDWidget->ProcessEvent(Func, &Params);
}

void AMyPlayer::CallHUDReverseUI(bool bOn)
{
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController());
	if (!PlayerController)
	{
		return;
	}

	UUserWidget* HUDWidget = PlayerController->GetHUDWidget();
	if (!HUDWidget)
	{
		return;
	}

	UFunction* Func = HUDWidget->FindFunction(FName("SetReverseUIOnOff"));
	if (!Func)
	{
		return;
	}

	struct FReverseUIParams
	{
		bool bOn;
	};

	FReverseUIParams Params;
	Params.bOn = bOn;
	HUDWidget->ProcessEvent(Func, &Params);
}