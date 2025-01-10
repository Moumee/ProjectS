#pragma once

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Walking UMETA(DisplayName = "Walking"),
	Running UMETA(DisplayName = "Running"),
	Crouching UMETA(DisplayName = "Crouching"),
	Dashing UMETA(DisplayName = "Dashing"),
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	None UMETA(DisplayName = "None"),
	Sliding UMETA(DisplayName = "Sliding"),
	WallRunning UMETA(DisplayName = "WallRunning"),
	Stunned UMETA(DisplayName = "Stunned"),
	Dead UMETA(DisplayName = "Dead"),
};

UENUM(BlueprintType)
enum class EWallSide : uint8
{
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right"),
};