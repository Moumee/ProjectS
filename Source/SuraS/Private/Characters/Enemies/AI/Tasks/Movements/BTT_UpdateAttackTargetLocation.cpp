// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Enemies/AI/Tasks/Movements/BTT_UpdateAttackTargetLocation.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_UpdateAttackTargetLocation::UBTT_UpdateAttackTargetLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Update Attack Target Location";
}

EBTNodeResult::Type UBTT_UpdateAttackTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASuraCharacterPlayer* const Player = Cast<ASuraCharacterPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", Player->GetActorLocation());

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
