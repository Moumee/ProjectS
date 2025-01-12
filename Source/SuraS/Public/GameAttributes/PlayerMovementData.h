// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMovementData.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct SURAS_API FPlayerMovementData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunSpeed;

	// The Z value of the launch velocity when jumping
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpZVelocity;

	// The XY value of the launch velocity when jumping
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpXYVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float AirControl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrouchSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashImpulseSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int DashMaxStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashAdditionalSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashKnockBackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float DashCooldown;
	
};
