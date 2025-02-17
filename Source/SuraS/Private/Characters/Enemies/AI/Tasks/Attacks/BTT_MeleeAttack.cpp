// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Attacks/BTT_MeleeAttack.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_MeleeAttack::UBTT_MeleeAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Melee Attack";
}

EBTNodeResult::Type UBTT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		if (ASuraCharacterPlayer* const Player = Cast<ASuraCharacterPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
		{
			Enemy->Attack(Player);

			TempOwnerComp = &OwnerComp; // for finishing task latently

			FOnMontageEnded OnMontageEnded;
			OnMontageEnded.BindUObject(this, &UBTT_MeleeAttack::OnAttackEnded);

			Enemy->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded);

			FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

void UBTT_MeleeAttack::OnAttackEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (TempOwnerComp)
		TempOwnerComp->OnTaskFinished(this, EBTNodeResult::Succeeded);
	else
		TempOwnerComp->OnTaskFinished(this, EBTNodeResult::Failed);
}