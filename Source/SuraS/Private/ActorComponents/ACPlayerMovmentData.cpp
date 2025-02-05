// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ACPlayerMovmentData.h"

#include "GameAttributes/PlayerMovementData.h"

void UACPlayerMovementData::BeginPlay()
{
	Super::BeginPlay();

	PlayerRow = PlayerMovementDT->FindRow<FPlayerMovementData>(FName("Player"), FString(TEXT("")));
	
}

float UACPlayerMovementData::GetWalkSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->WalkSpeed;
	
}

float UACPlayerMovementData::GetWalkRunTransitionDuration() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->WalkRunTransitionDuration;
}

float UACPlayerMovementData::GetWalkDashTransitionDuration() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->WalkDashTransitionDuration;
}

float UACPlayerMovementData::GetRunDashTransitionDuration() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->RunDashTransitionDuration;
}


float UACPlayerMovementData::GetCrouchRunTransitionDuration() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->CrouchRunTransitionDuration;
}

float UACPlayerMovementData::GetCrouchDashTransitionDuration() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->CrouchDashTransitionDuration;
}

float UACPlayerMovementData::GetWalkCrouchTransitionDuration() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->WalkCrouchTransitionDuration;
}


float UACPlayerMovementData::GetRunSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->RunSpeed;
}

float UACPlayerMovementData::GetCrouchSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->CrouchSpeed;
}

float UACPlayerMovementData::GetPrimaryJumpZSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->PrimaryJumpZSpeed;
}

float UACPlayerMovementData::GetDoubleJumpZSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->DoubleJumpZSpeed;
}

float UACPlayerMovementData::GetDoubleJumpXYSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->DoubleJumpXYSpeed;
}

float UACPlayerMovementData::GetAirControl() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->AirControl;
}

float UACPlayerMovementData::GetDashSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->DashSpeed;
}

float UACPlayerMovementData::GetDashImpulseSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->DashImpulseSpeed;
}

int UACPlayerMovementData::GetDashMaxStack() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->DashMaxStack;
}

float UACPlayerMovementData::GetDashDistance() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->DashDistance;
}

float UACPlayerMovementData::GetDashCooldown() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->DashCooldown;
}

