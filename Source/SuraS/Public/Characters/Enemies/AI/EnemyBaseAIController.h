// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Enumerations/Enemies/EEnemyState.h"
#include "EnemyBaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API AEnemyBaseAIController : public AAIController
{
	GENERATED_BODY()
	
	class UAISenseConfig_Sight* SightConfig;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetSighted(AActor* SeenTarget, FAIStimulus const Stimulus);

	EEnemyState CurrentState;

protected:
	virtual void OnPossess(APawn* PossessedPawn) override;

public:
	AEnemyBaseAIController(FObjectInitializer const& ObjectInitializer);

	EEnemyState GetCurrentState() const { return CurrentState; }

	void UpdateCurrentState(EEnemyState NewState);
};
