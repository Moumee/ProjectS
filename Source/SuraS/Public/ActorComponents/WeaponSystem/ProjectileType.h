// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileType.generated.h"

UENUM(BlueprintType)
enum class EProjectileType : uint8
{
	Projectile_Rifle UMETA(DisplayName = "Rifle_Projectile"),
	Projectile_ShotGun UMETA(DisplayName = "ShotGun_Projectile"),
	Projectile_BasicRocket UMETA(DisplayName = "BasicRocket_Projectile"),
	Projectile_Missile UMETA(DisplayName = "Missile_Projectile")
};

UCLASS()
class SURAS_API AProjectileType : public AActor
{
	GENERATED_BODY()
public:
	AProjectileType();
};
