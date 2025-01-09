// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SuraCharacterBase.h"
#include "SuraCharacterPlayer.generated.h"

class UACWallRun;

/**
 * 
 */
UCLASS()
class SURAS_API ASuraCharacterPlayer : public ASuraCharacterBase
{
	GENERATED_BODY()
	
	// Wall-run actor component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wall Run", meta = (AllowPrivateAccess = "true"))
	UACWallRun* WallRunComponent;

public:
	ASuraCharacterPlayer();

	/** Returns WallRunComponent subobject **/
	UACWallRun* GetWallRunComponent() const { return WallRunComponent; }
};
