// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponRecoilStruct.generated.h"

/**
 * 
 */
USTRUCT(Atomic, BlueprintType)
struct FWeaponRecoilStruct
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsRecoilRecoverAffectedByPlayerInput = true;

	UPROPERTY(EditAnywhere)
	float RecoilAmountPitch = 1.5f;

	UPROPERTY(EditAnywhere)
	float RecoilRangeMinPitch = 0.8f;

	UPROPERTY(EditAnywhere)
	float RecoilRangeMaxPitch = 1.2f;

	UPROPERTY(EditAnywhere)
	float RecoilAmountYaw = 0.8f;

	UPROPERTY(EditAnywhere)
	float RecoilRangeMinYaw = -1.f;

	UPROPERTY(EditAnywhere)
	float RecoilRangeMaxYaw = 1.f;

	UPROPERTY(EditAnywhere)
	float RecoilSpeed = 4.f;
	UPROPERTY(EditAnywhere)
	float RecoilRecoverSpeed = 3.5f;

	FWeaponRecoilStruct(
		bool bIsRecoilRecoverAffectedByInput = true,
		float InRecoilAmountPitch = 1.5f,
		float InRecoilRangeMinPitch = 0.8f,
		float InRecoilRangeMaxPitch = 1.2f,
		float InRecoilAmountYaw = 0.8f,
		float InRecoilRangeMinYaw = -1.f,
		float InRecoilRangeMaxYaw = 1.f,
		float InRecoilSpeed = 4.f,
		float InRecoilRecoverSpeed = 3.5f
		):
		bIsRecoilRecoverAffectedByPlayerInput(bIsRecoilRecoverAffectedByInput),
		RecoilAmountPitch(InRecoilAmountPitch),
		RecoilRangeMinPitch(InRecoilRangeMinPitch),
		RecoilRangeMaxPitch(InRecoilRangeMaxPitch),
		RecoilAmountYaw(InRecoilAmountYaw),
		RecoilRangeMinYaw(InRecoilRangeMinYaw),
		RecoilRangeMaxYaw(InRecoilRangeMaxYaw),
		RecoilSpeed(InRecoilSpeed),
		RecoilRecoverSpeed(InRecoilRecoverSpeed)
	{
	}
};

UCLASS()
class SURAS_API AWeaponRecoilValueStruct : public AActor
{
	GENERATED_BODY()
public:
	AWeaponRecoilValueStruct();
};
