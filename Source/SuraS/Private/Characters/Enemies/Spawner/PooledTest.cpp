// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Spawner/PooledTest.h"

// Sets default values
APooledTest::APooledTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APooledTest::Deactivate()
{
	SetActive(false);
	GetWorldTimerManager().ClearAllTimersForObject(this);
}

void APooledTest::SetActive(bool IsActive)
{
	Active = IsActive;
	SetActorHiddenInGame(!IsActive);
}

void APooledTest::SetPoolIndex(int Index)
{
	PoolIndex = Index;
}

bool APooledTest::IsActive()
{
	return Active;
}

