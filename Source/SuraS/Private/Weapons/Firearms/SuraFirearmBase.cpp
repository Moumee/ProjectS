// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Firearms/SuraFirearmBase.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ASuraFirearmBase::ASuraFirearmBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FirearmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirearmMesh"));
	FirearmMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASuraFirearmBase::BeginPlay()
{
	Super::BeginPlay();

	if (FirearmMeshAsset)
		FirearmMesh->SetSkeletalMesh(FirearmMeshAsset);
}

void ASuraFirearmBase::InitializeFirearem(const ASuraCharacterEnemyBase* OwnerChar, int32 NewMaxAmmo, int32 NewMagsize)
{
	OwnerCharacter = OwnerChar;
	MaxAmmo = NewMaxAmmo;
	CurrentAmmoInReserve = NewMaxAmmo;
	MagSize = NewMagsize;

	Reload();
}

void ASuraFirearmBase::Fire(AActor* FirearmOwner, const AActor* TargetActor, float AdditionalDamage, float AdditionalRadius, bool bIsHoming)
{
	return;
}

bool ASuraFirearmBase::ShouldReload()
{
	if (CurrentAmmonInMag == 0 && CurrentAmmoInReserve != 0)
		return true;

	return false;
}

bool ASuraFirearmBase::CanReload()
{
	if (CurrentAmmonInMag < MagSize && CurrentAmmoInReserve != 0)
		return true;

	return false;
}

void ASuraFirearmBase::Reload()
{
	int32 TotalAmmo = CurrentAmmoInReserve + CurrentAmmonInMag;

	CurrentAmmonInMag = TotalAmmo % MagSize == 0 ? MagSize : TotalAmmo % MagSize;
	CurrentAmmoInReserve = TotalAmmo - CurrentAmmonInMag;
}