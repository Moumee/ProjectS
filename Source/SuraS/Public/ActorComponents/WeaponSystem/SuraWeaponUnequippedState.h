// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuraWeaponBaseState.h"
#include "SuraWeaponUnequippedState.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API USuraWeaponUnequippedState : public USuraWeaponBaseState
{
	GENERATED_BODY()
public:
	USuraWeaponUnequippedState();
	~USuraWeaponUnequippedState();

	virtual void EnterState(UACWeapon* Weapon) override;

	virtual void UpdateState(UACWeapon* Weapon, float DeltaTime)override;

	virtual void ExitState(UACWeapon* Weapon)override;
};