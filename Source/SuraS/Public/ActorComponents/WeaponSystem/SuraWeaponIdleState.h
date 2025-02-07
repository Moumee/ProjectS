

#pragma once

#include "CoreMinimal.h"
#include "SuraWeaponBaseState.h"
#include "SuraWeaponIdleState.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API USuraWeaponIdleState : public USuraWeaponBaseState
{
	GENERATED_BODY()
public:
	USuraWeaponIdleState();
	~USuraWeaponIdleState();

	virtual void EnterState(UACWeapon* Weapon) override;

	virtual void UpdateState(UACWeapon* Weapon, float DeltaTime)override;

	virtual void ExitState(UACWeapon* Weapon)override;
};
