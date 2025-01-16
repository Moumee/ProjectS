// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SuraCharacterBase.h"
#include "SuraCharacterPlayer.generated.h"

class USuraPlayerCrouchingState;
class USuraPlayerDashMovementState;
class USuraPlayerDashImpulseState;
class USuraPlayerFallingState;
class USuraPlayerDashingState;
class USuraPlayerJumpingState;
class USuraPlayerRunningState;
class USuraPlayerWalkingState;
class USuraPlayerBaseState;
class UACPlayerMovementData;
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

protected:

	UPROPERTY()
	USuraPlayerBaseState* CurrentState;
	
	// Wall-run actor component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wall Run", meta = (AllowPrivateAccess = "true"))
	UACPlayerWallRun* WallRunComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	UACPlayerMovementData* PlayerMovementData;

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

	

#pragma endregion Input

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug", meta = (AllowPrivateAccess = "true"))
	bool bIsDebugMode;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ArmMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* LegMesh;

	float DefaultCapsuleHalfHeight;

	FVector DefaultCameraLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Curve", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* SlopeSpeedDeltaCurve;

	// Value that influences player speed based on the angle of the slope
	float SlopeSpeedDelta;
	
	float BaseMovementSpeed;

	float AdditionalMovementSpeed;

	bool bShouldUpdateAdditionalMovementSpeed;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Adds on screen debug messages for debugging purposes
	void PrintPlayerDebugInfo() const;

	void UpdateDashCooldowns(float DeltaTime);

	void UpdateAdditionalMovementSpeed(float DeltaTime);

	// Returns the angle of the current floor
	// If floor angle is 45 degrees downward slope, returns -45.f
	float FindFloorAngle() const;

	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


#pragma region Input Action Callback Functions
	
	void Move(const FInputActionValue& InputValue);

	void StopMoving();

	void Look(const FInputActionValue& InputValue);

	void StartRunning();
	
	void StartJumping();

	void StartCrouching();

	void StartDashing();

#pragma endregion Input Action Callback Functions

	virtual void Landed(const FHitResult& Hit) override;


public:
	ASuraCharacterPlayer();

	/** Returns WallRunComponent subobject **/
	UACPlayerWallRun* GetWallRunComponent() const { return WallRunComponent; }

	// Returns Player Movement Data Actor Component
	UACPlayerMovementData* GetPlayerMovementData() const { return PlayerMovementData; }

	UCameraComponent* GetCamera() const { return Camera; }

	// DesiredGroundState is set when player enters Walking, Running, Dashing state
	// It is used to change state to desired ground state when transitioning from Falling State
	UPROPERTY()
	USuraPlayerBaseState* DesiredGroundState;
	
	UPROPERTY()
	USuraPlayerWalkingState* WalkingState;
	UPROPERTY()
	USuraPlayerRunningState* RunningState;
	UPROPERTY()
	USuraPlayerJumpingState* JumpingState;
	UPROPERTY()
	USuraPlayerFallingState* FallingState;
	UPROPERTY()
	USuraPlayerDashImpulseState* DashImpulseState;
	UPROPERTY()
	USuraPlayerDashMovementState* DashMovementState;
	UPROPERTY()
	USuraPlayerCrouchingState* CrouchingState;

	int MaxJumps = 2;
	int JumpsLeft;

	// Input Action Bound Boolean Flags
	bool bJumpTriggered = false;
	bool bRunTriggered = false;
	bool bDashTriggered = false;
	bool bCrouchTriggered = false;
	bool bLandedTriggered = false;

	void ResetTriggeredBooleans();

	int MaxDashes;
	int DashesLeft;

	FTimerHandle DashImpulseTimerHandle;
	FTimerHandle DashMovementTimerHandle;

	TArray<float> DashCooldowns;

	FTimerHandle AdditionalSpeedTimerHandle;

	// Player Input Axis Values
	// W - Sets ForwardAxisInputValue to 1
	// S - Sets ForwardAxisInputValue to -1
	// A - Sets RightAxisInputValue to -1
	// D - Sets RightAxisInputValue to 1
	float ForwardAxisInputValue;
	float RightAxisInputValue;

	float DefaultGroundFriction;

	UFUNCTION(BlueprintCallable)
	void SetBaseMovementSpeed(float MovementSpeed);

	void ChangeState(USuraPlayerBaseState* NewState);

	bool IsFallingDown() const;

	bool HasMovementInput() const;

	void PrimaryJump();
	void StartAdditionalSpeedDuration();

	void DoubleJump();

	float GetDefaultCapsuleHalfHeight() const;

	FVector GetDefaultCameraLocation() const;
	
};


