// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SuraCharacterEnemyMelee.h"
#include "Structures/Enemies/EnemyAttributesData.h"
#include "Kismet/KismetMathLibrary.h"

ASuraCharacterEnemyMelee::ASuraCharacterEnemyMelee()
{
	EnemyType = "Melee";
}

void ASuraCharacterEnemyMelee::BeginPlay()
{
	Super::BeginPlay();
}

void ASuraCharacterEnemyMelee::RotateTowardPlayer()
{
	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation());

	SetActorRotation(FMath::RInterpTo(GetActorRotation(), NewRotation, GetWorld()->GetDeltaSeconds(), 15.f));
}

void ASuraCharacterEnemyMelee::Attack(const ASuraCharacterPlayer* AttackTarget)
{
	Player = AttackTarget;

	GetWorldTimerManager().SetTimer(
		RotationHandle,
		this, &ASuraCharacterEnemyMelee::RotateTowardPlayer,
		0.01f,
		true
	);

	if (AttackAnimation)
	{
		UAnimInstance* const EnemyAnimInstance = GetMesh()->GetAnimInstance();
		EnemyAnimInstance->Montage_Play(AttackAnimation);
	}

	FTimerHandle ClearTimerHandle;

	GetWorldTimerManager().SetTimer(
		ClearTimerHandle,
		FTimerDelegate::CreateLambda([&]() { GetWorldTimerManager().ClearTimer(RotationHandle); }),
		1.f,
		false
	);
}
