// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AirCraft.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCapsuleComponent;

UCLASS()
class SP_CH3_5ASSIGN_API AAirCraft : public APawn
{
	GENERATED_BODY()

public:
	AAirCraft();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* Capsule;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* Spring;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
	FVector CurrntVelocity= FVector(0,0,0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float MoveSpeed = 2000.f;
	
	// 회전 속도 변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float PitchSpeed = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float YawSpeed = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float RollSpeed = 100.0f;
	
	UFUNCTION()
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void Roll(const FInputActionValue& value);

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
