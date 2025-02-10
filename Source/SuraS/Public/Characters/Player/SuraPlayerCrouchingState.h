// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/SuraPlayerBaseState.h"
#include "SuraPlayerCrouchingState.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API USuraPlayerCrouchingState : public USuraPlayerBaseState
{
	GENERATED_BODY()

	bool bShouldUpdateSpeed = false;

	float SpeedChangePerSecond = 0;

	float SpeedTransitionTime = 0;

	float CrouchedCapsuleHalfHeight = 0.f;

	FVector CrouchedCameraLocation = FVector::ZeroVector;

	float CrouchAnimationSpeed = 5.f;

	APlayerController* PlayerController;
	
public:

	USuraPlayerCrouchingState();

	virtual void EnterState(ASuraCharacterPlayer* Player) override;

	virtual void UpdateState(ASuraCharacterPlayer* Player, float DeltaTime) override;

	virtual void ExitState(ASuraCharacterPlayer* Player) override;

	virtual void Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector) override;

	virtual void Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector) override;

	virtual void StartJumping(ASuraCharacterPlayer* Player) override;
};
