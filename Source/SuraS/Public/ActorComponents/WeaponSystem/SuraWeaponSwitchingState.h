// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuraWeaponBaseState.h"
#include "SuraWeaponSwitchingState.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API USuraWeaponSwitchingState: public USuraWeaponBaseState
{
	GENERATED_BODY()
public:
	USuraWeaponSwitchingState();
	~USuraWeaponSwitchingState();

	virtual void EnterState(UACWeapon* Weapon) override;

	virtual void UpdateState(UACWeapon* Weapon, float DeltaTime)override;

	virtual void ExitState(UACWeapon* Weapon)override;
};
