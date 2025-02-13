// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/SuraWeaponReloadingState.h"

USuraWeaponReloadingState::USuraWeaponReloadingState()
{
	WeaponStateType = EWeaponStateType::WeaponStateType_Reloading;
}

USuraWeaponReloadingState::~USuraWeaponReloadingState()
{
}

void USuraWeaponReloadingState::EnterState(UACWeapon* Weapon)
{
	Super::EnterState(Weapon);

	//TODO:
	// ReloadingTime(혹은 ReloadingRate)변수에 따라 Reloading Anim Montage의 Rate를 설정하고 실행하도록
	// Reloading이 끝나면 IdleState로 Change하기
	// 근데 이걸 굳이 State에서 실행할 필요가 있나? 그냥 weapon에서 처리하는게 깔끔한 것 같은데
}

void USuraWeaponReloadingState::UpdateState(UACWeapon* Weapon, float DeltaTime)
{
	Super::UpdateState(Weapon, DeltaTime);
}

void USuraWeaponReloadingState::ExitState(UACWeapon* Weapon)
{
	Super::ExitState(Weapon);
}
