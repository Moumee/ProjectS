

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"

#include "Enumerations/EDamageType.h"

#include "ProjectileData.h"
#include "ProjectileType.h"

#include "SuraProjectile.generated.h"

class UACWeapon;

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

DECLARE_DELEGATE(FHeadShotDelegate);
DECLARE_DELEGATE(FBodyShotDelegate);

UCLASS(config = Game) //TODO: 무슨 속성인지 알아봐야함
class SURAS_API ASuraProjectile : public AActor
{
	GENERATED_BODY()
public:
	FHeadShotDelegate OnHeadShot;
	FBodyShotDelegate OnBodyShot;
protected:
	UPROPERTY(EditAnywhere, Category = Projectile)
	EProjectileType ProjectileType = EProjectileType::Projectile_Rifle;

	//TODO: UDataTable, FDataTableRowHandle 둘중에 하나 사용해야함
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
	UDataTable* ProjectileDataTable;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (RowType="ProjectileData"))
	//FDataTableRowHandle ProjectileDataTableHandle;

	FProjectileData* ProjectileData;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* TrailEffect;

	UPROPERTY()
	UNiagaraComponent* TrailEffectComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* ImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UMaterialInterface* DecalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DefaultDamage = 0.f;

	float HeadShotAdditionalDamage = 0.f;

	UPROPERTY(VisibleAnywhere)
	AActor* ProjectileOwner;

	UPROPERTY(VisibleAnywhere)
	UACWeapon* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomProjectile")
	float InitialSpeed = 50000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomProjectile")
	float MaxSpeed = 50000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomProjectile")
	float InitialRadius = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomProjectile")
	bool bIsExplosive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomProjectile")
	float MaxExplosiveDamage = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomProjectile")
	float MaxExplosionRadius = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomProjectile")
	float HomingAccelerationMagnitude = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Penetration")
	bool bCanPenetrate = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Penetration")
	int32 NumPenetrableObjects = 4;
public:	
	ASuraProjectile();
	void InitializeProjectile(AActor* Owner, UACWeapon* OwnerWeapon, float additonalDamage = 0.f, float AdditionalRadius = 0.f, int32 NumPenetrable = 0);
	void LoadProjectileData(FName ProjectileID);
	void SetHomingTarget(bool bIsHoming, AActor* Target);
	void LaunchProjectile();

	void ApplyExplosiveDamage(bool bCanExplosiveDamage, FVector CenterLocation);
	void ApplyDamage(AActor* OtherActor, float DamageAmount, EDamageType DamageType, bool bCanForceDamage);
	bool SearchOverlappedActor(FVector CenterLocation, float SearchRadius, TArray<AActor*>& OverlappedActors);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	void SpawnImpactEffect(FVector SpawnLocation, FRotator SpawnRotation);
	void SpawnTrailEffect(bool bShouldAttachedToWeapon = false);
	void SpawnDecalEffect(FVector SpawnLocation, FRotator SpawnRotation);
protected:
	bool bShouldUpdateTrailEffect = false;
	void UpdateTrailEffect();

//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginDestroy() override;

#pragma region Penetration
protected:
	int32 NumPenetratedObjects = 0;

	float AdditionalDamage = 0.f;
protected:
	void UpdatePenetration();
	void ResetPenetration();
#pragma endregion

#pragma region HeadShot
protected:
	bool CheckHeadHit(const FHitResult& Hit);
	bool CheckHeadOvelap(const AActor* OverlappedActor, const FHitResult& SweepResult);
#pragma endregion

#pragma region Homing
protected:
	//bool CheckIfTargetAccessible();


#pragma endregion

};
