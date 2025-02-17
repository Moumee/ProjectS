// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UBTT_MeleeAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UBehaviorTreeComponent* TempOwnerComp;

	void OnAttackEnded(UAnimMontage* AnimMontage, bool bInterrupted);
	
public:
	explicit UBTT_MeleeAttack(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
