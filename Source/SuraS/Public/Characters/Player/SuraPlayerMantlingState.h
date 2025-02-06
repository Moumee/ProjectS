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

	bool bShouldMantle = false;

	FVector StartLocation = FVector::ZeroVector;
	FVector TargetLocation = FVector::ZeroVector;

	float ElapsedTime = 0;

	float MantleDuration = 0;

	APlayerController* PlayerController;

public:

	USuraPlayerMantlingState();

	virtual void EnterState(ASuraCharacterPlayer* Player) override;

	virtual void UpdateState(ASuraCharacterPlayer* Player, float DeltaTime) override;

	virtual void ExitState(ASuraCharacterPlayer* Player) override;
	
};
