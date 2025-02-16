// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "EnemyActions.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyActions : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SURAS_API IEnemyActions
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Attack(const ASuraCharacterPlayer* Player) = 0;
};
