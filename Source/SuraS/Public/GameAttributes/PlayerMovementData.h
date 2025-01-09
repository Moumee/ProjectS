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
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HoldToSprintTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrouchedWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrouchSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlidingThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WallRunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsWallRunGravity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetWallRunGravity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WallRunCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WallRunJumpOffVelocity;
};
