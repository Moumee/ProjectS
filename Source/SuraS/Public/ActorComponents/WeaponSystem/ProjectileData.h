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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	//UParticleSystem* TrailEffect;
	UNiagaraSystem* TrailEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	//UParticleSystem* ImpactEffect;
	UNiagaraSystem* ImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UMaterialInterface* HoleDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DefaultDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float HeadShotAdditionalDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialLifeSpan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialRadius = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive")
	bool bIsExplosive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive")
	float MaxExplosiveDamage = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive")
	float MaxExplosionRadius = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Homing")
	float HomingAccelerationMagnitude = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Penetration")
	bool bCanPenetrate = false;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Penetration")
	//int32 NumPenetrableObjects = 4;
};
