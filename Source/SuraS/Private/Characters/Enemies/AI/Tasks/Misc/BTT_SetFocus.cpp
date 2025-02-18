// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Misc/BTT_SetFocus.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_SetFocus::UBTT_SetFocus(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Set Focus";
}

EBTNodeResult::Type UBTT_SetFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEnemyBaseAIController* const EnemyController = Cast<AEnemyBaseAIController>(OwnerComp.GetAIOwner()))
	{
		if (ASuraCharacterPlayer* const Player = Cast<ASuraCharacterPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
		{
			EnemyController->SetFocus(Player, EAIFocusPriority::Gameplay);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}