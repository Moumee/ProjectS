// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Enemies/AI/Tasks/Movements/BTTask_FindRandomLocation.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Random Location";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEnemyBaseAIController* const EnemyController = Cast<AEnemyBaseAIController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const Enemy = EnemyController->GetPawn())
		{
			if (UNavigationSystemV1* const Navigation = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FVector const Origin = Enemy->GetActorLocation();
				FNavLocation RandomLocation;

				if (Navigation->GetRandomPointInNavigableRadius(Origin, SearchRadius, RandomLocation))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", RandomLocation);

					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}
