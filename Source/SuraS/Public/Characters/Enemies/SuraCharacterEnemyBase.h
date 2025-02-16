// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SuraCharacterBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Characters/Player/SuraCharacterPlayer.h"

#include "Interfaces/Damageable.h"
#include "Structures/DamageData.h"
#include "Interfaces/Enemies/EnemyActions.h"

#include "ActorComponents/DamageComponent/ACDamageSystem.h"
#include "Components/WidgetComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "SuraCharacterEnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API ASuraCharacterEnemyBase : public ASuraCharacterBase, public IDamageable, public IEnemyActions
{
	GENERATED_BODY()

	AEnemyBaseAIController* AIController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor Components", meta = (AllowPrivateAccess = "true"))
	UACDamageSystem* DamageSystemComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HealthBarWidget;

protected:
	// [protected variables]
	FName EnemyType; // for initializing differently btw enemy types from the DT

	// [protected functions]
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

	void SetUpAIController(AEnemyBaseAIController* NewAIController);

	virtual bool TakeDamage(const FDamageData& DamageData, const AActor* DamageCauser) override;

	virtual void Attack(const ASuraCharacterPlayer* Player) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* HitAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* DeathAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* AttackAnimation;
};
