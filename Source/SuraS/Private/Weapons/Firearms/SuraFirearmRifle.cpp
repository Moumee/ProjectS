// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Firearms/SuraFirearmRifle.h"
#include "ActorComponents/WeaponSystem/SuraProjectile.h"

void ASuraFirearmRifle::Fire(AActor* FirearmOwner, const AActor* TargetActor, float AdditionalDamage, float AdditionalRadius, bool bIsHoming)
{
	if (ProjectileClass)
	{
		const FVector SpawnLocation = FirearmMesh->GetSocketLocation(FName(TEXT("Muzzle")));
		const FRotator SpawnRotation = (TargetActor->GetActorLocation() - SpawnLocation).Rotation();

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ASuraProjectile* Projectile = GetWorld()->SpawnActor<ASuraProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

		Projectile->InitializeProjectile(FirearmOwner, nullptr, AdditionalDamage, AdditionalRadius);

		Projectile->LaunchProjectile();
	}
}