// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/SuraWeaponChargingState.h"

USuraWeaponChargingState::USuraWeaponChargingState()
{
}

USuraWeaponChargingState::~USuraWeaponChargingState()
{
}

void USuraWeaponChargingState::EnterState(UACWeapon* Weapon)
{
	Super::EnterState(Weapon);
}

void USuraWeaponChargingState::UpdateState(UACWeapon* Weapon, float DeltaTime)
{
	Super::UpdateState(Weapon, DeltaTime);
}

void USuraWeaponChargingState::ExitState(UACWeapon* Weapon)
{
	Super::ExitState(Weapon);
}
