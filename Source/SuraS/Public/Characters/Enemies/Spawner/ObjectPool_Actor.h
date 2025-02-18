// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "ObjectPool_Actor.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SURAS_API AObjectPool_Actor : public AActor
{
	GENERATED_BODY()
	
public:	
	AObjectPool_Actor();

	UFUNCTION(BlueprintCallable, Category = "ObjectPool")
		APawn* SpawnPooledObject();

	UFUNCTION()
		void SpawnWrapper();

	UPROPERTY(EditAnywhere, Category = "ObjectPool")
		TSubclassOf<class APawn> PooledObjectSubclass;

	UPROPERTY(EditAnywhere, Category = "ObjectPool")
		int PoolSize = 5;

protected:
	virtual void BeginPlay() override;

	TArray<APawn*> ObjectPool;

private:
	UPROPERTY(EditAnywhere, Category = "ObjectPool")
		float interval = 20.0f;
	UPROPERTY(EditAnywhere, Category = "ObjectPool")
		int spawnCount = 5;
	UPROPERTY(EditAnywhere, Category = "ObjectPool")
		UBehaviorTree* BehaviorTree;

	FTimerHandle TimerHandler;

	FActorSpawnParameters ActorSpawnParameters;
};