// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SuraWeaponAnimInstance.generated.h"

class UACWeapon;
/**
 * 
 */
UCLASS()
class SURAS_API USuraWeaponAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	UACWeapon* Weapon;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
};
