// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "SuraFirearmBase.generated.h"

UCLASS()
class SURAS_API ASuraFirearmBase : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	const ASuraCharacterEnemyBase* OwnerCharacter;

	int32 MaxAmmo;
	int32 CurrentAmmoInReserve;
	int32 CurrentAmmonInMag;
	int32 MagSize;

public:	
	ASuraFirearmBase();

	USkeletalMeshComponent* FirearmMesh;

	UPROPERTY(EditAnywhere, Category = "Firearm Mesh")
	USkeletalMesh* FirearmMeshAsset;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<class ASuraProjectile> ProjectileClass;

	void InitializeFirearem(const ASuraCharacterEnemyBase* OwnerChar, int32 NewMaxAmmo, int32 NewMagsize); // should be called after being deferred spawned

	virtual void Fire(AActor* FirearmOwner, const AActor* TargetActor, float AdditionalDamage, float AdditionalRadius, bool bIsHoming);

	bool ShouldReload();
	bool CanReload();
	void Reload();
};
