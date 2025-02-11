// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	None UMETA(DisplayName = "None"),
	Idle UMETA(DisplayName = "Idle"),
	Walking UMETA(DisplayName = "Walking"),
	Sprintng UMETA(DisplayName = "Sprinting"),
	Jumping UMETA(DisplayName = "Jumping"),
	Falling UMETA(DisplayName = "Falling"),
	Dashing UMETA(DisplayName = "Dashing"),
	WallRunning UMETA(DisplayName = "WallRunning"),
	Attacking UMETA(DisplayName = "Attacking"),
	Dead UMETA(DisplayName = "Dead")
};
