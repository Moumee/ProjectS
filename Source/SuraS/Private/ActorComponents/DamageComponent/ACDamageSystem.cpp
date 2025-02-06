// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/DamageComponent/ACDamageSystem.h"
#include "Structures/DamageData.h"

// Sets default values for this component's properties
UACDamageSystem::UACDamageSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UACDamageSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UACDamageSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UACDamageSystem::SetHealth(float val)
{
	Health = val;
}

void UACDamageSystem::SetMaxHealth(float val)
{
	MaxHealth = val;
}

void UACDamageSystem::SetIsInterruptible(bool val)
{
	bIsInterruptible = val;
}

void UACDamageSystem::SetIsInvincible(bool val)
{
	bIsInvincible = val;
}

void UACDamageSystem::SetIsDead(bool val)
{
	bIsDead = val;
}

float UACDamageSystem::GetHealth()
{
	return Health;
}

float UACDamageSystem::GetMaxHealth()
{
	return MaxHealth;
}

bool UACDamageSystem::TakeDamage(FDamageData DamageData, AActor* DamageCauser)
{
	if (!bIsDead && (!bIsInvincible || DamageData.bCanForceDamage))
	{
		Health -= DamageData.DamageAmount;

		if (Health <= 0.f)
		{
			bIsDead = true;
			OnDeath.Broadcast(); // Call death event

			return true;
		}

		OnDamaged.Broadcast(); // Call damaged event

		return true;
	}

	return false;
}

