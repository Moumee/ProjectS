// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuraPlayerBaseState.generated.h"

class ASuraCharacterPlayer;
/**
 * 
 */
UCLASS(Abstract)
class SURAS_API USuraPlayerBaseState : public UObject
{
	GENERATED_BODY()

public:

	FName StateDisplayName;

	virtual void EnterState(ASuraCharacterPlayer* Player);

	virtual void UpdateState(ASuraCharacterPlayer* Player, float DeltaTime);

	virtual void ExitState(ASuraCharacterPlayer* Player);

	virtual void Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector);

	virtual void StopMoving(ASuraCharacterPlayer* Player);

	virtual void Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector);

	virtual void StartRunning(ASuraCharacterPlayer* Player);

	virtual void StartJumping(ASuraCharacterPlayer* Player);

	virtual void StartDashing(ASuraCharacterPlayer* Player);

	virtual void StartCrouching(ASuraCharacterPlayer* Player);

	virtual void Landed(ASuraCharacterPlayer* Player, const FHitResult& HitResult);
	
};
