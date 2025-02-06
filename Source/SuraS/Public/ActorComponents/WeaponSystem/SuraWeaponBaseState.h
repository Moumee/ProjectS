

#pragma once

#include "CoreMinimal.h"
#include "WeaponStateType.h"
#include "SuraWeaponBaseState.generated.h"

class UACWeapon;
/**
 * 
 */
UCLASS(Abstract)
class SURAS_API USuraWeaponBaseState : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "WeaponState")
	EWeaponStateType WeaponStateType;

	FName StateDisplayName;

	virtual EWeaponStateType GetWeaponStateType() const;

	virtual void EnterState(UACWeapon* Weapon);

	virtual void UpdateState(UACWeapon* Weapon, float DeltaTime);

	virtual void ExitState(UACWeapon* Weapon);


	//TODO: Reloading, Equiping, Unequip -> Switching? (얘는 잘 모르겠네), OutOfAmmo, ZoomIn ZoomOut
	// IdleState
	// FiringState
	// ReloadingState
	

};
