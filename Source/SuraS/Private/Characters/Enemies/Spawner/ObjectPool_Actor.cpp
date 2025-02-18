// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Spawner/ObjectPool_Actor.h"

// Sets default values
AObjectPool_Actor::AObjectPool_Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

AActor* AObjectPool_Actor::SpawnPooledObject()
{
	for (AActor* PoolableActor : ObjectPool)
	{
		if (PoolableActor != nullptr && PoolableActor->IsHidden())
		{
			PoolableActor->TeleportTo(GetActorLocation()+FVector(FMath::RandRange(-50, 50), FMath::RandRange(-50, 50), 0), FRotator(0, 0, 0));
			PoolableActor->SetActorHiddenInGame(false);
			PoolableActor->SetActorEnableCollision(true);
			FString error = (PoolableActor->IsHidden()) ? "true" : "false";
			UE_LOG(LogBlueprint, Warning, TEXT("%s"), *error);
			return PoolableActor;
		}
	}

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		AActor* newPoolableActor = World->SpawnActor<AActor>(PooledObjectSubclass,
			GetActorLocation() + FVector(FMath::RandRange(-50, 50), FMath::RandRange(-50, 50), 0), FRotator().ZeroRotator, ActorSpawnParameters);
		newPoolableActor->SetActorHiddenInGame(false);
		ObjectPool.Add(newPoolableActor);
		return newPoolableActor;
	}

	return nullptr;
}

void AObjectPool_Actor::SpawnWrapper()
{
	for (int i = 0; i < spawnCount; i++)
	{
		SpawnPooledObject();
	}
	//UE_LOG(LogBlueprint, Warning, TEXT("called"));
}


// Called when the game starts or when spawned
void AObjectPool_Actor::BeginPlay()
{
	Super::BeginPlay();


	if (PooledObjectSubclass != nullptr)
	{
		UWorld* const World = GetWorld();
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (World != nullptr)
		{
			for (int i = 0; i < PoolSize; i++)
			{
				AActor* PoolableActor = World->SpawnActor<AActor>(PooledObjectSubclass, 
					GetActorLocation(), FRotator().ZeroRotator);

				if (PoolableActor != nullptr)
				{
					PoolableActor->SetActorHiddenInGame(true);
					PoolableActor->SetActorEnableCollision(false);
					ObjectPool.Add(PoolableActor);
				}
			}
		}

	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &AObjectPool_Actor::SpawnWrapper, interval, true);
	
}


