// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SuraCharacterEnemyMelee.h"
#include "Structures/Enemies/EnemyAttributesData.h"

ASuraCharacterEnemyMelee::ASuraCharacterEnemyMelee()
{
	EnemyType = "Melee";
}

void ASuraCharacterEnemyMelee::BeginPlay()
{
	Super::BeginPlay();
}

void ASuraCharacterEnemyMelee::Attack(const ASuraCharacterPlayer* Player)
{
	if (AttackAnimation)
	{
		UAnimInstance* const EnemyAnimInstance = GetMesh()->GetAnimInstance();
		EnemyAnimInstance->Montage_Play(AttackAnimation);
	}
}
