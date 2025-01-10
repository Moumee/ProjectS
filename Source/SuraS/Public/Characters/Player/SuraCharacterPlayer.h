// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuraPlayerEnums.h"
#include "Characters/SuraCharacterBase.h"
#include "SuraCharacterPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMovementStateChanged, EMovementState, NewMovementState);

class UACPlayerAttributes;
class UACPlayerWallRun;
class UCameraComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UACWallRun;

/**
 * 
 */
UCLASS()
class SURAS_API ASuraCharacterPlayer : public ASuraCharacterBase
{
	GENERATED_BODY()

private:

	
	
	// Wall-run actor component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wall Run", meta = (AllowPrivateAccess = "true"))
	UACPlayerWallRun* WallRunComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	UACPlayerAttributes* PlayerAttributes;

#pragma region Input
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;

	float ForwardAxisInputValue;
	
	float RightAxisInputValue;

#pragma endregion Input

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug", meta = (AllowPrivateAccess = "true"))
	bool IsDebugMode;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	EMovementState CurrentMovementState;
	UPROPERTY(BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	EActionState CurrentActionState;

	int MaxJumps = 2;
	int JumpsLeft;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Curve", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* SlopeSpeedCurve;

	float BaseMovementSpeed;

	float AdditionalMovementSpeed;

	virtual void BeginPlay() override;
	
	void PrintPlayerDebugInfo() const;

	float FindFloorAngle() const;
	

	virtual void Tick(float DeltaTime) override;

	
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

#pragma region Input Callback Functions
	
	void Move(const FInputActionValue& InputValue);

	void StopMoving();

	void Look(const FInputActionValue& InputValue);

	void StartRunning();
	
	void StartJumping();

	void StartCrouching();

	void Dash();

#pragma endregion Input Callback Functions

	virtual void Landed(const FHitResult& Hit) override;

	FVector FindJumpLaunchVelocity() const;


public:
	ASuraCharacterPlayer();

	/** Returns WallRunComponent subobject **/
	UACPlayerWallRun* GetWallRunComponent() const { return WallRunComponent; }

	UACPlayerAttributes* GetPlayerAttributes() const { return PlayerAttributes; }
	
	FOnMovementStateChanged OnMovementStateChanged;

	void SetMovementState(EMovementState NewMovementState);

	UFUNCTION(BlueprintCallable)
	void SetBaseMovementSpeed(EMovementState NewMovementState);
};


