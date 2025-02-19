// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SuraCharacterEnemyRifle.h"

ASuraCharacterEnemyRifle::ASuraCharacterEnemyRifle()
{
	EnemyType = "Rifle";
}

void ASuraCharacterEnemyRifle::BeginPlay()
{
	Super::BeginPlay();

	Firearm = GetWorld()->SpawnActorDeferred<ASuraFirearmRifle>(RifleClass, GetActorTransform(), this);
	Firearm->InitializeFirearem(this, 150, 30);
	Firearm->FinishSpawning(GetActorTransform(), true);

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	Firearm->AttachToComponent(GetMesh(), AttachmentRules, FName(TEXT("Gun")));
}

void ASuraCharacterEnemyRifle::Attack(const ASuraCharacterPlayer* Player)
{
	if (AttackAnimation)
	{
		UAnimInstance* const EnemyAnimInstance = GetMesh()->GetAnimInstance();
		EnemyAnimInstance->Montage_Play(AttackAnimation);

		Firearm->Fire(this, Player, 0, 0, false);

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("hi"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("not found"));
	}
}