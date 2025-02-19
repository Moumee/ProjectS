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

	APlayerController* PlayerController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor Components", meta = (AllowPrivateAccess = "true"))
	UACDamageSystem* DamageSystemComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HealthBarWidget;

	FVector2D HealthBarWidgetSize;

	FTimerHandle HideHealthBarHandle;

protected:
	// [protected variables]
	FName EnemyType; // for initializing differently btw enemy types from the DT

	float AttackDamageAmount;

	// [protected functions]
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	virtual void OnDamagedTriggered();

	virtual void OnDeathTriggered();

	virtual void UpdateHealthBarValue();

public:
	ASuraCharacterEnemyBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (RowType = "EnemyAttributesData"))
	FDataTableRowHandle EnemyAttributesDT;

	// ai controller getter
	FORCEINLINE AEnemyBaseAIController* GetAIController() const { return AIController; }

	// player controller getter
	FORCEINLINE APlayerController* GetPlayerController() const { return PlayerController; }

	// widget size getter
	FORCEINLINE FVector2D GetHealthBarWidgetSize() const { return HealthBarWidgetSize; }

	// damage system comp getter
	FORCEINLINE UACDamageSystem* GetDamageSystemComp() const { return DamageSystemComp; }

	// damage system comp getter
	FORCEINLINE float GetAttackDamageAmount() const { return AttackDamageAmount; }

	// behavior tree getter
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

	FORCEINLINE FName GetEnemyType() const { return EnemyType; }

	void SetUpAIController(AEnemyBaseAIController* const NewAIController); // const ptr: the ptr address can't be changed

	virtual bool TakeDamage(const FDamageData& DamageData, const AActor* DamageCauser) override;

	virtual void Attack(const ASuraCharacterPlayer* Player) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* HitAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* DeathAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* AttackAnimation;
};
