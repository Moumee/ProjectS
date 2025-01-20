// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/SuraPlayerBaseState.h"
#include "SuraPlayerMantlingState.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API USuraPlayerMantlingState : public USuraPlayerBaseState
{
	GENERATED_BODY()

	FTimerHandle MantleTimerHandle;

	bool bShouldMantleUp;
	bool bShouldMantleForward;

	FVector StartLocation;
	FVector TargetLocation;

	FVector UpMantleTarget;

	float UpMovementElapsedTime;
	float ForwardMovementElapsedTime;

public:

	USuraPlayerMantlingState();

	virtual void EnterState(ASuraCharacterPlayer* Player) override;

	virtual void UpdateState(ASuraCharacterPlayer* Player, float DeltaTime) override;

	virtual void ExitState(ASuraCharacterPlayer* Player) override;
	
};
