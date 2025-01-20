// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/SuraPlayerBaseState.h"
#include "SuraPlayerDashingState.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API USuraPlayerDashingState : public USuraPlayerBaseState
{
	GENERATED_BODY()

public:

	USuraPlayerDashingState();

	

	virtual void EnterState(ASuraCharacterPlayer* Player) override;

	virtual void UpdateState(ASuraCharacterPlayer* Player, float DeltaTime) override;

	virtual void ExitState(ASuraCharacterPlayer* Player) override;

	
};
