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
	float WalkRunTransitionDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkDashTransitionDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunDashTransitionDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkCrouchTransitionDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrouchRunTransitionDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrouchDashTransitionDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashImpulseTransitionEndDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunSpeed;

	// The Z value of the launch velocity when jumping
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PrimaryJumpZSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DoubleJumpZSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DoubleJumpXYSpeed;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int DashMaxStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashKnockBackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float DashCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlidingAdditionalSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlidingDecreaseSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WallRunningMaxDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxCameraFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxCameraFOVSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWallRunSpeed;
	
};
