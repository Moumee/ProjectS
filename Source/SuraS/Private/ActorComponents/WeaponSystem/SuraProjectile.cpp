

#include "ActorComponents/WeaponSystem/SuraProjectile.h"

#include "Interfaces/Damageable.h"
#include "Structures/DamageData.h"
#include "Enumerations/EDamageType.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
//#include "Particles/ParticleSystem.h"
//#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ASuraProjectile::ASuraProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	//CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->SetCollisionObjectType(ECC_GameTraceChannel1);
	CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore); //Projectile
	CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore); //Weapon
	CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Ignore); //Player
	CollisionComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	CollisionComp->OnComponentHit.AddDynamic(this, &ASuraProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	//TODO: Projectile attribute load 하기 위한 Data Table 구현하기
	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = InitialSpeed;
	ProjectileMovement->MaxSpeed = MaxSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	//ProjectileMesh->SetCollisionObjectType(ECC_GameTraceChannel1);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	ProjectileMesh->SetCastShadow(false);

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	UE_LOG(LogTemp, Warning, TEXT("Projectile is Spawned!!!"));
}

void ASuraProjectile::InitializeProjectile(AActor* OwnerOfProjectile)
{
	if (IsValid(OwnerOfProjectile))
	{
		ProjectileOwner = OwnerOfProjectile;

		if (ProjectileType == EProjectileType::Projectile_Rifle)
		{
			UE_LOG(LogTemp, Warning, TEXT("Rifle Projectile!!!"));
			LoadProjectileData("RifleProjectile");
		}
		else if (ProjectileType == EProjectileType::Projectile_ShotGun)
		{
			UE_LOG(LogTemp, Warning, TEXT("ShotGun Projectile!!!"));
			LoadProjectileData("ShotGunProjectile");
		}

		SpawnTrailEffect();
	}


	UE_LOG(LogTemp, Warning, TEXT("Projectile InitialSpeed: %f"), ProjectileMovement->InitialSpeed);
	UE_LOG(LogTemp, Warning, TEXT("Projectile MaxSpeed: %f"), ProjectileMovement->MaxSpeed);
}

void ASuraProjectile::LoadProjectileData(FName ProjectileID)
{
	ProjectileData = ProjectileDataTable->FindRow<FProjectileData>(ProjectileID, TEXT(""));
	if (ProjectileData)
	{
		TrailEffect = ProjectileData->TrailEffect;
		ImpactEffect = ProjectileData->ImpactEffect;
		DecalMaterial = ProjectileData->HoleDecal;
		Damage = ProjectileData->Damage;
	}
}

void ASuraProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//TODO: Projectile이 다른 actor에게 hit 했을 때, OtherActor의 종류에 따라서 다른 event 발생시키기. Interface 사용하기
	if (OtherActor != nullptr)
	{
		if (OtherActor != ProjectileOwner)
		{
			// Only add impulse and destroy projectile if we hit a physics
			if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
			{
				OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
			}

			//TODO: Effect는 한번만 발생되도록 설정하기
			//TODO: 충돌 Effect는 Projectile Type에 따라 다르게 설정하기
			SpawnImpactEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			SpawnDecalEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());


			FDamageData DefaultDamage;
			DefaultDamage.DamageAmount = Damage;
			DefaultDamage.DamageType = EDamageType::Melee;
			DefaultDamage.bCanForceDamage = false;

			if (OtherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
			{
				Cast<IDamageable>(OtherActor)->TakeDamage(DefaultDamage, this->ProjectileOwner);
			}

			Destroy();
		}
	}
	else
	{
		// Only add impulse and destroy projectile if we hit a physics
		if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		}

		//TODO: Effect는 한번만 발생되도록 설정하기
		//TODO: 충돌 Effect는 Projectile Type에 따라 다르게 설정하기
		SpawnImpactEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		SpawnDecalEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());


		Destroy();
	}
}


void ASuraProjectile::SpawnImpactEffect(FVector SpawnLocation, FRotator SpawnRotation)
{
	if (ImpactEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactEffect, SpawnLocation, SpawnRotation, FVector(1.0f), true);
	}
}

void ASuraProjectile::SpawnTrailEffect()
{
	if (ProjectileMesh && TrailEffect)
	{
		FTransform TrailStartTransform = ProjectileMesh->GetSocketTransform(FName(TEXT("TrailStart")), ERelativeTransformSpace::RTS_Component);
		FTransform TrailEndTransform = ProjectileMesh->GetSocketTransform(FName(TEXT("TrailEnd")), ERelativeTransformSpace::RTS_Component);
		//FRotator TrailRotation = (TrailStartTransform.GetLocation() - TrailEndTransform.GetLocation()).Rotation();
		//TrailRotation.Normalize();
		float DistanceOffset = 80.f;

		FVector TrailLocationOffset = (TrailEndTransform.GetLocation() - TrailStartTransform.GetLocation()).GetSafeNormal() * DistanceOffset;

		TrailEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			TrailEffect,
			ProjectileMesh,
			FName(TEXT("TrailStart")),
			TrailLocationOffset,
			FRotator(0, 0, 0),
			EAttachLocation::KeepRelativeOffset,
			true);
	}
}

void ASuraProjectile::SpawnDecalEffect(FVector SpawnLocation, FRotator SpawnRotation)
{
	if (DecalMaterial)
	{
		FVector DecalSize = FVector(2.0f, 8.0f, 8.0f);     // X: 두께, YZ: 크기

		UDecalComponent* ProjectileDecal =
		UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),          // 월드 컨텍스트
			DecalMaterial,       // 데칼 머티리얼
			DecalSize,           // 데칼 크기
			SpawnLocation,       // 위치
			SpawnRotation,       // 회전
			10.0f                // 수명 (초 단위)
		);
		ProjectileDecal->SetFadeScreenSize(0.0001f);
	}
}

//// Called when the game starts or when spawned
//void ASuraProjectile::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//// Called every frame
//void ASuraProjectile::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}


