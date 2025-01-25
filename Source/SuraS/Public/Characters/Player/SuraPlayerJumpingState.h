// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerBaseState.h"
#include "SuraPlayerJumpingState.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API USuraPlayerJumpingState : public USuraPlayerBaseState
{
	GENERATED_BODY()

	bool bShouldUpdateSpeed = false;

	float SpeedChangePerSecond = 0;

	float SpeedTransitionTime = 0;

	float ElapsedTimeFromWallRun = 0.f;

public:

	USuraPlayerJumpingState();
	
	virtual void EnterState(ASuraCharacterPlayer* Player) override;
	void UpdateBaseMovementSpeed(ASuraCharacterPlayer* Player, float DeltaTime);

	virtual void UpdateState(ASuraCharacterPlayer* Player, float DeltaTime) override;

	virtual void ExitState(ASuraCharacterPlayer* Player) override;

	virtual void Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector) override;

	virtual void Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector) override;

	virtual void StartJumping(ASuraCharacterPlayer* Player) override;

	
};


