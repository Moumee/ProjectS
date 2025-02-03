// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SuraCharacterBase.h"
#include "Interfaces/Damageable.h"
#include "Structures/DamageData.h"
#include "SuraCharacterEnemyBase.generated.h"

class UACDamageSystem;

/**
 * 
 */
UCLASS()
class SURAS_API ASuraCharacterEnemyBase : public ASuraCharacterBase, public IDamageable
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor Components", meta = (AllowPrivateAccess = "true"))
	UACDamageSystem* DamageSystemComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnDamagedTriggered();
	virtual void OnDeathTriggered();

public:
	ASuraCharacterEnemyBase();

	// damage system comp getter
	UACDamageSystem* GetDamageSystemComp() const { return DamageSystemComp; }

	bool TakeDamage(FDamageData DamageData, AActor* DamageCauser);
};
