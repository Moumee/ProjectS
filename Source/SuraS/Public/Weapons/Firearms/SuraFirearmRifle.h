// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Firearms/SuraFirearmBase.h"
#include "SuraFirearmRifle.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API ASuraFirearmRifle : public ASuraFirearmBase
{
	GENERATED_BODY()
	
public:
	virtual void Fire(AActor* FirearmOwner, const AActor* TargetActor, float AdditionalDamage, float AdditionalRadius, bool bIsHoming) override;
};
