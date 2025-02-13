// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponStateType.generated.h"

UENUM(BlueprintType)
enum class EWeaponStateType : uint8
{
	WeaponStateType_None UMETA(DisplayName = "None"),
	WeaponStateType_Idle UMETA(DisplayName = "Idle"),
	WeaponStateType_Firing UMETA(DisplayName = "Firing"),
	WeaponStateType_Reloading UMETA(DisplayName = "Reloading"),
	WeaponStateType_Unequipped UMETA(DisplayName = "Unequipped"),
	WeaponStateType_Switching UMETA(DisplayName = "Switching")
};

UCLASS()
class SURAS_API AWeaponStateType : public AActor
{
	GENERATED_BODY()
public:
	AWeaponStateType();
};
