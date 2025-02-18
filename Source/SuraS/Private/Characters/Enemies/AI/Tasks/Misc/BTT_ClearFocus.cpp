// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Misc/BTT_ClearFocus.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"

UBTT_ClearFocus::UBTT_ClearFocus(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Clear Focus";
}

EBTNodeResult::Type UBTT_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEnemyBaseAIController* const EnemyController = Cast<AEnemyBaseAIController>(OwnerComp.GetAIOwner()))
	{
		EnemyController->ClearFocus(EAIFocusPriority::Gameplay);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}