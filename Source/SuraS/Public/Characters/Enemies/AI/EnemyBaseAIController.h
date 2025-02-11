// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyBaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API AEnemyBaseAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	virtual void OnPossess(APawn* PossessedPawn) override;

public:
	AEnemyBaseAIController(FObjectInitializer const& ObjectInitializer);
};
