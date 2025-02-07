// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/SuraWeaponAnimInstance.h"

#include "ActorComponents/WeaponSystem/ACWeapon.h"

void USuraWeaponAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//Weapon = Cast<UACWeapon>(TryGetPawnOwner());
	Weapon = Cast<UACWeapon>(GetOwningComponent());
}

void USuraWeaponAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

}
