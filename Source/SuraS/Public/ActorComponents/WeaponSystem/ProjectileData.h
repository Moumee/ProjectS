// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/WeaponSystem/ProjectileType.h"
#include "NiagaraSystem.h"
#include "ProjectileData.generated.h"


USTRUCT(BlueprintType)
struct SURAS_API FProjectileData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EProjectileType ProjectileType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//UParticleSystem* TrailEffect;
	UNiagaraSystem* TrailEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//UParticleSystem* ImpactEffect;
	UNiagaraSystem* ImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* HoleDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;
};
