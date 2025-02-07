// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuraWeaponBaseState.h"
#include "SuraWeaponReloadingState.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API USuraWeaponReloadingState : public USuraWeaponBaseState
{
	GENERATED_BODY()
public:
	USuraWeaponReloadingState();
	~USuraWeaponReloadingState();

	virtual void EnterState(UACWeapon* Weapon) override;

	virtual void UpdateState(UACWeapon* Weapon, float DeltaTime) override;

	virtual void ExitState(UACWeapon* Weapon) override;


};
