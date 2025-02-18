// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "ObjectPool_Actor.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SURAS_API AObjectPool_Actor : public AActor
{
	GENERATED_BODY()
	
public:	
	AObjectPool_Actor();

	UFUNCTION(BlueprintCallable, Category = "ObjectPool")
		AActor* SpawnPooledObject();

	UFUNCTION()
		void SpawnWrapper();

	UPROPERTY(EditAnywhere, Category = "ObjectPool")
		TSubclassOf<class AActor> PooledObjectSubclass;

	UPROPERTY(EditAnywhere, Category = "ObjectPool")
		int PoolSize = 5;

protected:
	virtual void BeginPlay() override;

	TArray<AActor*> ObjectPool;

private:
	UPROPERTY(EditAnywhere, Category = "ObjectPool")
		float interval = 5.0f;
		int spawnCount = 5;

	FTimerHandle TimerHandler;

	FActorSpawnParameters ActorSpawnParameters;
};