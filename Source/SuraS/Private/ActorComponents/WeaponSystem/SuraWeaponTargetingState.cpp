// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/SuraWeaponTargetingState.h"
#include "ActorComponents/WeaponSystem/ACWeapon.h"

USuraWeaponTargetingState::USuraWeaponTargetingState()
{
}

USuraWeaponTargetingState::~USuraWeaponTargetingState()
{
}

void USuraWeaponTargetingState::EnterState(UACWeapon* Weapon)
{
	Super::EnterState(Weapon);
}

void USuraWeaponTargetingState::UpdateState(UACWeapon* Weapon, float DeltaTime)
{
	Super::UpdateState(Weapon, DeltaTime);

	Weapon->UpdateTargetMarkers();
}

void USuraWeaponTargetingState::ExitState(UACWeapon* Weapon)
{
	Super::ExitState(Weapon);
}
