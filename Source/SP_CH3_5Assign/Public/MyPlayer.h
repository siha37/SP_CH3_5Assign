// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPlayer.generated.h"

class UWidgetComponent;
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* Capsule;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* Spring;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component) 
	UWidgetComponent* OverHeadWidget;

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
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float GroundSpeed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsJumping = false;

	bool bMoveInputThisFrame = false;
	
	float SlowMovePercent = 0;
	float SlowDurationTotal = 0.f;
	float ReverseDurationTotal = 0.f;

	FVector CurrentVelocity = FVector::ZeroVector;
	bool bIsFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

	FTimerHandle SlowTimerHandle;
	FTimerHandle ReverceControllHandle;
	
	bool bIsReverseControl = false;

	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	void Move(const FInputActionValue& value);
	void StartJump(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

	
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser);
	void OnDeath();
	void UpdateOverHeadHp();
	void ResetSlow();
	void ResetReverseControl();
	void CallHUDSlowUI(bool bOn);
	void CallHUDReverseUI(bool bOn);
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure,Category="Health")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable,Category="Health")
	void AddHealth(float Amount);
	
	void SetSlow(float percent,float duration);
	void SetReverseControll(bool isOn,float duration);
	
	UFUNCTION(BlueprintPure, Category = "Animation")
	bool GetIsFalling() const { return bIsFalling; }

	UFUNCTION(BlueprintPure, Category = "Animation")
	bool GetIsJumping() const { return bIsJumping; }

	UFUNCTION(BlueprintPure, Category = "Animation")
	float GetGroundSpeed() const { return GroundSpeed; }

	UFUNCTION(BlueprintPure, Category = "Debuff")
	bool IsSlowActive() const;

	UFUNCTION(BlueprintPure, Category = "Debuff")
	bool IsReverseActive() const;

	UFUNCTION(BlueprintPure, Category = "Debuff")
	float GetSlowRemainingTime() const;

	UFUNCTION(BlueprintPure, Category = "Debuff")
	float GetReverseRemainingTime() const;

	UFUNCTION(BlueprintPure, Category = "Debuff")
	float GetSlowDurationTotal() const { return SlowDurationTotal; }

	UFUNCTION(BlueprintPure, Category = "Debuff")
	float GetReverseDurationTotal() const { return ReverseDurationTotal; }

};
