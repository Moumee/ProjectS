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

public:

	USuraPlayerJumpingState();
	
	virtual void EnterState(ASuraCharacterPlayer* Player) override;

	virtual void UpdateState(ASuraCharacterPlayer* Player, float DeltaTime) override;

	virtual void ExitState(ASuraCharacterPlayer* Player) override;

	virtual void Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector) override;

	virtual void Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector) override;

	virtual void StartRunning(ASuraCharacterPlayer* Player) override;

	virtual void StartJumping(ASuraCharacterPlayer* Player) override;

	virtual void Landed(ASuraCharacterPlayer* Player, const FHitResult& HitResult) override;
	
};


