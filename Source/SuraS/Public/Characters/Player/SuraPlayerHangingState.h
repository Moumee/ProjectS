// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/SuraPlayerBaseState.h"
#include "SuraPlayerHangingState.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API USuraPlayerHangingState : public USuraPlayerBaseState
{
	GENERATED_BODY()
	
	float BaseYaw;

	float ElapsedTime;

	bool bShouldMoveToHangPosition;

	FVector StartPosition;

	FVector HangPosition = FVector::ZeroVector;

public:

	USuraPlayerHangingState();

	virtual void EnterState(ASuraCharacterPlayer* Player) override;

	virtual void UpdateState(ASuraCharacterPlayer* Player, float DeltaTime) override;

	virtual void ExitState(ASuraCharacterPlayer* Player) override;

	virtual void Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector) override;
	
};
