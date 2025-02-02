// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuraPlayerEnums.h"
#include "Animation/AnimInstance.h"
#include "SuraPlayerAnimInstance.generated.h"

class USuraPlayerBaseState;
class ASuraCharacterPlayer;
/**
 * 
 */
UCLASS()
class SURAS_API USuraPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	ASuraCharacterPlayer* Player;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsCrouching;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Pitch;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	USuraPlayerBaseState* CurrentState;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	EPlayerState CurrentStateType;

	
	virtual void NativeInitializeAnimation() override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	
};
