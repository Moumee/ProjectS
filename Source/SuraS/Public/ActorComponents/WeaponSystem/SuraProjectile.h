

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SuraProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent; // 기본적인 projectile이 구현되어 있는 class임
class UParticleSystem;

UCLASS(config = Game) //TODO: 무슨 속성인지 알아봐야함
class SURAS_API ASuraProjectile : public AActor
{
	GENERATED_BODY()

protected:
	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UMaterialInterface* DecalMaterial;

	UPROPERTY(VisibleAnywhere)
	AActor* ProjectileOwner;

public:	
	// Sets default values for this actor's properties
	ASuraProjectile();

	void InitializeProjectile(AActor* Owner);

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	void SpawnParticleEffect(FVector SpawnLocation, FRotator SpawnRotation);

	void SpawnDecalEffect(FVector SpawnLocation, FRotator SpawnRotation);

//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;


};
