// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/SuraWeaponUnequippedState.h"

#include "ActorComponents/WeaponSystem/ACWeapon.h"

USuraWeaponUnequippedState::USuraWeaponUnequippedState()
{
	WeaponStateType = EWeaponStateType::WeaponStateType_Unequipped;
}

USuraWeaponUnequippedState::~USuraWeaponUnequippedState()
{
}

void USuraWeaponUnequippedState::EnterState(UACWeapon* Weapon)
{
	Super::EnterState(Weapon);

	//TODO: 관련함수들 묶어서 관리하는 함수 만들기
	Weapon->ActivateCrosshairWidget(false);
	Weapon->ActivateAmmoCounterWidget(false);
}

void USuraWeaponUnequippedState::UpdateState(UACWeapon* Weapon, float DeltaTime)
{
	Super::UpdateState(Weapon, DeltaTime);
}

void USuraWeaponUnequippedState::ExitState(UACWeapon* Weapon)
{
	Super::ExitState(Weapon);
}
