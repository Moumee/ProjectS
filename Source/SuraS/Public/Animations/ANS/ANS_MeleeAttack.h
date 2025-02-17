// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "ANS_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UANS_MeleeAttack : public UAnimNotifyState
{
	GENERATED_BODY()

	ASuraCharacterEnemyBase* Enemy;

	FORCEINLINE ASuraCharacterEnemyBase* GetEnemyChar() const { return Enemy; }

	bool bCanInflictDamage = true;

public:
	UPROPERTY(EditAnywhere)
	float DamageAmount;

	UPROPERTY(EditAnywhere)
	float AttackRange;

	UPROPERTY(EditAnywhere)
	float AttackRadius;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EvetnRef) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EvetnRef) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EvetnRef) override;
};
