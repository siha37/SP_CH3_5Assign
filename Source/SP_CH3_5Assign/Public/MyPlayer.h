// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPlayer.generated.h"

struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;
class UCapsuleComponent;

UCLASS()
class SP_CH3_5ASSIGN_API AMyPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* Capsule;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* Spring;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Move)
	float MoveSpeed = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Move)
	float LookSpeed = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Move)
	float GravityAcceleration = -908.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Move)
	float AirControlModifier = 0.4f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Move)
	float TraceDistance = 94.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Move)
	float JumpVelocity = 500.0f;

	FVector CurrentVelocity = FVector::ZeroVector;
	bool bIsFalling = false;

	

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	
	UFUNCTION()
	void Move(const FInputActionValue& value);
	void StartJump(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
public:	
	virtual void Tick(float DeltaTime) override;

};
