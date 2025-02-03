// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/SuraPlayerBaseState.h"
#include "SuraPlayerSlidingState.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API USuraPlayerSlidingState : public USuraPlayerBaseState
{
	GENERATED_BODY()

	FVector SlideDirection = FVector::ZeroVector;

	float SlideDeltaSpeed = 1500.f;

	float StartSpeed = 0.f;

	float CurrentSlideSpeed = 0.f;
	
	float CrouchSpeed = 0.f;

public:

	USuraPlayerSlidingState();

	virtual void EnterState(ASuraCharacterPlayer* Player) override;

	virtual void UpdateState(ASuraCharacterPlayer* Player, float DeltaTime) override;

	virtual void ExitState(ASuraCharacterPlayer* Player) override;

	virtual void StartJumping(ASuraCharacterPlayer* Player) override;

	virtual void Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector) override;
	
};


