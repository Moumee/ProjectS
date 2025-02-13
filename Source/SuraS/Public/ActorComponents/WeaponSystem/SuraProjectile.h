

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"

#include "ProjectileData.h"
#include "ProjectileType.h"

#include "SuraProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent; // 기본적인 projectile이 구현되어 있는 class임
//class UParticleSystem;
class UNiagaraSystem;

UCLASS(config = Game) //TODO: 무슨 속성인지 알아봐야함
class SURAS_API ASuraProjectile : public AActor
{
	GENERATED_BODY()

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
	//UParticleSystem* ImpactEffect;
	UNiagaraSystem* ImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UMaterialInterface* DecalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage = 0.f;

	UPROPERTY(VisibleAnywhere)
	AActor* ProjectileOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomProjectile")
	float InitialSpeed = 50000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomProjectile")
	float MaxSpeed = 50000.f;

public:	
	// Sets default values for this actor's properties
	ASuraProjectile();
	void InitializeProjectile(AActor* Owner);
	void LoadProjectileData(FName ProjectileID);

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	void SpawnImpactEffect(FVector SpawnLocation, FRotator SpawnRotation);
	void SpawnTrailEffect();
	void SpawnDecalEffect(FVector SpawnLocation, FRotator SpawnRotation);

//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;


};
