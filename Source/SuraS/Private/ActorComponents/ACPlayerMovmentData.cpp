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

float UACPlayerMovementData::GetJumpZVelocity() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->JumpZVelocity;
}

float UACPlayerMovementData::GetJumpXYVelocity() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->JumpXYVelocity;
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


float UACPlayerMovementData::GetDashDuration() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->DashDuration;
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

