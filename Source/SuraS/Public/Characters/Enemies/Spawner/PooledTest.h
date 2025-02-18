// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledTest.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPooledTestDespawn, APooledTest*, PoolActor);

UCLASS()
class SURAS_API APooledTest : public AActor
{
	GENERATED_BODY()
	
public:	
	
	APooledTest();

	UFUNCTION(BlueprintCallable, Category = "PooledObject")
		void Deactivate();

	void SetActive(bool IsActive);
	void SetPoolIndex(int Index);

	bool IsActive();

protected:
	bool Active;
	float LifeSpan = 0.0f;
	int PoolIndex;
};
