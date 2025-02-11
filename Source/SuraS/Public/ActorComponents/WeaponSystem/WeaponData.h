// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/WeaponSystem/WeaponName.h"
#include "ActorComponents/WeaponSystem/ProjectileType.h"
#include "NiagaraSystem.h"
#include "WeaponData.generated.h"

USTRUCT(BlueprintType)
struct SURAS_API FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponName WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EProjectileType ProjectileType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* FireEffect;
};
