


#include "ActorComponents/WeaponSystem/SuraWeaponIdleState.h"


USuraWeaponIdleState::USuraWeaponIdleState()
{
}

USuraWeaponIdleState::~USuraWeaponIdleState()
{
}

void USuraWeaponIdleState::EnterState(UACWeapon* Weapon)
{
	Super::EnterState(Weapon);
}

void USuraWeaponIdleState::UpdateState(UACWeapon* Weapon, float DeltaTime)
{
	Super::UpdateState(Weapon, DeltaTime);
}

void USuraWeaponIdleState::ExitState(UACWeapon* Weapon)
{
	Super::ExitState(Weapon);
}

