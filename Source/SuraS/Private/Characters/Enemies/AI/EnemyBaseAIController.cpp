// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"

void AEnemyBaseAIController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);

	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(PossessedPawn))
	{
		if (UBehaviorTree* const BehaviorTree = Enemy->GetBehaviorTree())
		{
			UBlackboardComponent* Bboard;
			UseBlackboard(BehaviorTree->BlackboardAsset, Bboard);
			Blackboard = Bboard; // "Blackboard" is an already existing variable name in AAIController class

			RunBehaviorTree(BehaviorTree);
		}
	}
}

AEnemyBaseAIController::AEnemyBaseAIController(FObjectInitializer const& ObjectInitializer)
{
}
