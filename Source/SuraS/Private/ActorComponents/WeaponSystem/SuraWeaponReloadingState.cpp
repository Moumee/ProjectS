// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/SuraWeaponReloadingState.h"

USuraWeaponReloadingState::USuraWeaponReloadingState()
{
}

USuraWeaponReloadingState::~USuraWeaponReloadingState()
{
}

void USuraWeaponReloadingState::EnterState(UACWeapon* Weapon)
{
	Super::EnterState(Weapon);

	//TODO:
	// ReloadingTime(혹은 ReloadingRate)변수에 따라 Reloading Anim Montage의 Rate를 설정하고 실행하도록
}

void USuraWeaponReloadingState::UpdateState(UACWeapon* Weapon, float DeltaTime)
{
	Super::UpdateState(Weapon, DeltaTime);
}

void USuraWeaponReloadingState::ExitState(UACWeapon* Weapon)
{
	Super::ExitState(Weapon);
}
