// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enumerations/Enemies/EEnemyStates.h"
#include "SuraEnemyState.generated.h"

class ASuraCharacterEnemyBase;
/**
 * 
 */
UCLASS(Abstract)
class SURAS_API USuraEnemyState : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Enemy State")
	EEnemyState EnemyState;

	virtual EEnemyState GetState() const;

	virtual void EnterState(ASuraCharacterEnemyBase* Enemy);

	virtual void UpdateState(ASuraCharacterEnemyBase* Enemy);

	virtual void ExitState(ASuraCharacterEnemyBase* Enemy);
};
