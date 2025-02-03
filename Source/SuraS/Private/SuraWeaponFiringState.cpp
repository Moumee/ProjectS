


#include "SuraWeaponFiringState.h"


USuraWeaponFiringState::USuraWeaponFiringState()
{
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
}
