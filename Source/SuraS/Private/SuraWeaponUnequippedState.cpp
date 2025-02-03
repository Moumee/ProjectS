// Fill out your copyright notice in the Description page of Project Settings.


#include "SuraWeaponUnequippedState.h"

USuraWeaponUnequippedState::USuraWeaponUnequippedState()
{
}

USuraWeaponUnequippedState::~USuraWeaponUnequippedState()
{
}

void USuraWeaponUnequippedState::EnterState(UACWeapon* Weapon)
{
	Super::EnterState(Weapon);
}

void USuraWeaponUnequippedState::UpdateState(UACWeapon* Weapon, float DeltaTime)
{
	Super::UpdateState(Weapon, DeltaTime);
}

void USuraWeaponUnequippedState::ExitState(UACWeapon* Weapon)
{
	Super::ExitState(Weapon);
}
