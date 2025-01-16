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

	float DefaultCapsuleHalfHeight;

	float CrouchedCapsuleHalfHeight;

	FVector DefaultCameraLocation;

	FVector CrouchedCameraLocation;

	float CrouchTransitionDuration = 0.1f;

	float CrouchTransitionElapsedTime = 0.f;

	float UnCrouchTransitionElapsedTime = 0.f;

	bool bShouldStartCrouching;

	void UnCrouchToNextState(ASuraCharacterPlayer* Player, USuraPlayerBaseState* NextState, float DeltaTime);

	// TODO: Lerp Camera Height!!
public:

	USuraPlayerCrouchingState();

	virtual void EnterState(ASuraCharacterPlayer* Player) override;

	virtual void UpdateState(ASuraCharacterPlayer* Player, float DeltaTime) override;

	virtual void ExitState(ASuraCharacterPlayer* Player) override;

	virtual void Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector) override;

	virtual void Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector) override;

	virtual void StartJumping(ASuraCharacterPlayer* Player) override;
};
