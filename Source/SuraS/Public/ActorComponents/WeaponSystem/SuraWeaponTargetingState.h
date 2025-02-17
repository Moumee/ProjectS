// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuraWeaponBaseState.h"
#include "SuraWeaponTargetingState.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API USuraWeaponTargetingState : public USuraWeaponBaseState
{
	GENERATED_BODY()
public:
	USuraWeaponTargetingState();
	~USuraWeaponTargetingState();

	virtual void EnterState(UACWeapon* Weapon) override;

	virtual void UpdateState(UACWeapon* Weapon, float DeltaTime) override;

	virtual void ExitState(UACWeapon* Weapon) override;
};
