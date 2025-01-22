#pragma once

UENUM(BlueprintType)
enum class EWallSide : uint8
{
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right"),
};

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	None UMETA(DisplayName = "None"),
	Walking UMETA(DisplayName = "Walking"),
	Running UMETA(DisplayName = "Running"),
	Jumping UMETA(DisplayName = "Jumping"),
	Falling UMETA(DisplayName = "Falling"),
	Crouching UMETA(DisplayName = "Crouching"),
	Dashing UMETA(DisplayName = "Dashing"),
	Hanging UMETA(DisplayName = "Hanging"),
	Mantling UMETA(DisplayName = "Mantling"),
	WallRunning UMETA(DisplayName = "WallRunning"),
};