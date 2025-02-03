

#pragma once

#include "CoreMinimal.h"
#include "SuraWeaponBaseState.h"
#include "SuraWeaponFiringState.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API USuraWeaponFiringState : public USuraWeaponBaseState
{
	GENERATED_BODY()
public:
	USuraWeaponFiringState();
	~USuraWeaponFiringState();

	virtual void EnterState(UACWeapon* Weapon) override;

	virtual void UpdateState(UACWeapon* Weapon, float DeltaTime)override;

	virtual void ExitState(UACWeapon* Weapon)override;
};