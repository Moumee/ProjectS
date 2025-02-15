// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SuraCharacterBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Interfaces/Damageable.h"
#include "Structures/DamageData.h"
#include "ActorComponents/DamageComponent/ACDamageSystem.h"
#include "Components/WidgetComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "SuraCharacterEnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API ASuraCharacterEnemyBase : public ASuraCharacterBase, public IDamageable
{
	GENERATED_BODY()

	AEnemyBaseAIController* AIController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor Components", meta = (AllowPrivateAccess = "true"))
	UACDamageSystem* DamageSystemComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HealthBarWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	UFUNCTION()
	virtual void OnDamagedTriggered();

	UFUNCTION()
	virtual void OnDeathTriggered();

	virtual void UpdateHealthBarValue();

public:
	ASuraCharacterEnemyBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (RowType = "EnemyAttributesData"))
	FDataTableRowHandle EnemyAttributesDT;

	// damage system comp getter
	UACDamageSystem* GetDamageSystemComp() const { return DamageSystemComp; }

	// behavior tree getter
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

	bool TakeDamage(FDamageData DamageData, AActor* DamageCauser);

	void SetUpAIController(AEnemyBaseAIController* NewAIController);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* HitAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* DeathAnimation;
};
