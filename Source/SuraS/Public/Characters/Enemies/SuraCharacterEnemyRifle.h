// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Weapons/Firearms/SuraFirearmRifle.h"
#include "SuraCharacterEnemyRifle.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API ASuraCharacterEnemyRifle : public ASuraCharacterEnemyBase
{
	GENERATED_BODY()
	
	ASuraFirearmRifle* Firearm;

protected:
	virtual void BeginPlay() override;

public:
	ASuraCharacterEnemyRifle();

	UPROPERTY(EditAnywhere, Category = "Firearm")
	TSubclassOf<class ASuraFirearmRifle> RifleClass;

	virtual void Attack(const ASuraCharacterPlayer* Player) override;
};
