


#include "ActorComponents/WeaponSystem/SuraWeaponFiringState.h"
//#include "ActorComponents/WeaponSystem/ACWeapon.h"

USuraWeaponFiringState::USuraWeaponFiringState()
{
	WeaponStateType = EWeaponStateType::WeaponStateType_Firing;
}

USuraWeaponFiringState::~USuraWeaponFiringState()
{
}

void USuraWeaponFiringState::EnterState(UACWeapon* Weapon)
{
	Super::EnterState(Weapon);
}

void USuraWeaponFiringState::UpdateState(UACWeapon* Weapon, float DeltaTime)
{
	Super::UpdateState(Weapon, DeltaTime);
}

void USuraWeaponFiringState::ExitState(UACWeapon* Weapon)
{
	Super::ExitState(Weapon);
	//Weapon->AutoReload();
}