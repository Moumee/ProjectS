// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponCamSettingValue.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FWeaponCamSettingValue
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere)
	float FOV = 75.f;

	UPROPERTY(EditAnywhere)
	FRotator CameraRelativeRotation = { -10, 0, 0 };

	UPROPERTY(EditAnywhere)
	FVector CameraRelativeLocation = { 0, 90, -15 };

	UPROPERTY(EditAnywhere)
	float FOV_InterpSpeed_ToThisState = 1.2f;

	UPROPERTY(EditAnywhere)
	float CameraRelativeRotation_InterpSpeed_ToThisState = 0.01f;

	UPROPERTY(EditAnywhere)
	float CameraRelativeLocation_InterpSpeed_ToThisState = 1.2f;

	//UPROPERTY(EditAnywhere)
	//float FOV_InterpSpeed_ToOtherState = 1.2f;

	//UPROPERTY(EditAnywhere)
	//float CameraRelativeRotation_InterpSpeed_ToOtherState = 0.01f;

	//UPROPERTY(EditAnywhere)
	//float CameraRelativeLocation_InterpSpeed_ToOtherState = 1.2f;

	FWeaponCamSettingValue()
	{

	}
};

UCLASS()
class SURAS_API APlayerWeaponCamSettingValue : public AActor
{
	GENERATED_BODY()
public:
	APlayerWeaponCamSettingValue();
};
