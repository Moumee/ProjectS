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



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SingleShot")
	float SingleShotDelay = 1.f;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	bool bIsRecoilRecoverAffectedByPlayerInput = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float RecoilAmountPitch = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float RecoilRangeMinPitch = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float RecoilRangeMaxPitch = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float RecoilAmountYaw = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float RecoilRangeMinYaw = -1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float RecoilRangeMaxYaw = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float RecoilSpeed = 4.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float RecoilRecoverSpeed = 3.5f;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float MissileLaunchDelay = 0.2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	int32 MaxTargetNum = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float MaxTargetDetectionRadius = 3000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float MaxTargetDetectionAngle = 80.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float MaxTargetDetectionTime = 8.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TimeToReachMaxTargetDetectionRange = 2.5f;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charging")
	bool bAutoFireAtMaxChargeTime = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charging")
	float ChargeTimeThreshold = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charging")
	float MaxChargeTime = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charging")
	float ChargingAdditionalDamageBase = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charging")
	float ChargingAdditionalRecoilAmountPitchBase = 4.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charging")
	float ChargingAdditionalRecoilAmountYawBase = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charging")
	float ChargingAdditionalProjectileRadiusBase = 20.f;

};
