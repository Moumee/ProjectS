// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "ActorComponents/DamageComponent/ACDamageSystem.h"

ASuraCharacterEnemyBase::ASuraCharacterEnemyBase()
{
	// Damage system comp
	DamageSystemComp = CreateDefaultSubobject<UACDamageSystem>(TEXT("DamageSystemComponent"));
	AddOwnedComponent(DamageSystemComp);
}

void ASuraCharacterEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (DamageSystemComp)
	{
		GetDamageSystemComp()->OnDamaged.AddDynamic(this, &ASuraCharacterEnemyBase::OnDamagedTriggered);
		GetDamageSystemComp()->OnDeath.AddDynamic(this, &ASuraCharacterEnemyBase::OnDeathTriggered);
	}
}

void ASuraCharacterEnemyBase::OnDamagedTriggered()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("DAMAGED"));
}

void ASuraCharacterEnemyBase::OnDeathTriggered()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("DEATH"));
}
