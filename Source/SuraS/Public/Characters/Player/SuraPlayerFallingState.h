// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/SuraPlayerBaseState.h"
#include "SuraPlayerFallingState.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API USuraPlayerFallingState : public USuraPlayerBaseState
{
	GENERATED_BODY()
	
	bool bShouldUpdateSpeed = false;

	float SpeedChangePerSecond = 0;

	float SpeedTransitionTime = 0;

	float ElapsedTimeFromWallRun = 0.f;

	UPROPERTY()
	APlayerController* PlayerController;

	FVector DesiredSlidingDirection = FVector::ZeroVector;

	
	
public:

	USuraPlayerFallingState();

	FVector GetDesiredSlidingDirection() const { return DesiredSlidingDirection; }

	virtual void EnterState(ASuraCharacterPlayer* Player) override;
	
	void UpdateBaseMovementSpeed(ASuraCharacterPlayer* Player, float DeltaTime);
	
	void CacheSlidingDirection(ASuraCharacterPlayer* Player);

	virtual void UpdateState(ASuraCharacterPlayer* Player, float DeltaTime) override;

	virtual void ExitState(ASuraCharacterPlayer* Player) override;

	virtual void Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector) override;

	virtual void Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector) override;

	virtual void StartJumping(ASuraCharacterPlayer* Player) override;

	virtual void Landed(ASuraCharacterPlayer* Player, const FHitResult& HitResult) override;
	
};
