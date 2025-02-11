// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShakeBase.h"
#include "WeaponCameraShakeBase.generated.h"

class UPerlinNoiseCameraShakePattern;

UCLASS()
class SURAS_API UWeaponCameraShakeBase : public UCameraShakeBase
{
	GENERATED_BODY()
	
public:
	//UWeaponCameraShakeBase(const FObjectInitializer& ObjectInitializer);
	UWeaponCameraShakeBase();



protected:
	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TObjectPtr<UPerlinNoiseCameraShakePattern> ShakePattern_Perlin;

};
