// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/WeaponCameraShakeBase.h"
#include "Shakes/PerlinNoiseCameraShakePattern.h"

//UWeaponCameraShakeBase::UWeaponCameraShakeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
//{
//}

UWeaponCameraShakeBase::UWeaponCameraShakeBase() : UCameraShakeBase(FObjectInitializer::Get())
{
	// only shake the camera if it's not already shaking
	bSingleInstance = true;

	ShakePattern_Perlin = CreateDefaultSubobject<UPerlinNoiseCameraShakePattern>(TEXT("PerlinPattern"));

	ShakePattern_Perlin->X.Amplitude = 2.f;
	ShakePattern_Perlin->X.Frequency = 2.f;
	ShakePattern_Perlin->Y.Amplitude = 2.f;
	ShakePattern_Perlin->Y.Frequency = 2.f;
	ShakePattern_Perlin->Z.Amplitude = 2.f;
	ShakePattern_Perlin->Z.Frequency = 2.f;

	ShakePattern_Perlin->Pitch.Amplitude = 2.f;
	ShakePattern_Perlin->Pitch.Frequency = 2.f;
	ShakePattern_Perlin->Yaw.Amplitude = 2.f;
	ShakePattern_Perlin->Yaw.Frequency = 2.f;
	ShakePattern_Perlin->Roll.Amplitude = 2.f;
	ShakePattern_Perlin->Roll.Frequency = 2.f;

	ShakePattern_Perlin->FOV.Amplitude = 2.f;
	ShakePattern_Perlin->FOV.Frequency = 2.f;

	ShakePattern_Perlin->Duration = 0.5f;

	SetRootShakePattern(ShakePattern_Perlin);
}
