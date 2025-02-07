


#include "ActorComponents/WeaponSystem/SuraWeaponBaseState.h"

#include "ActorComponents/WeaponSystem/ACWeapon.h"

EWeaponStateType USuraWeaponBaseState::GetWeaponStateType() const
{
    return WeaponStateType;
}

void USuraWeaponBaseState::EnterState(UACWeapon* Weapon)
{
}

void USuraWeaponBaseState::UpdateState(UACWeapon* Weapon, float DeltaTime)
{
}

void USuraWeaponBaseState::ExitState(UACWeapon* Weapon)
{
}
