// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ACPlayerAttributes.h"

#include "GameAttributes/PlayerMovementData.h"

void UACPlayerAttributes::BeginPlay()
{
	Super::BeginPlay();

	PlayerRow = PlayerMovementDT->FindRow<FPlayerMovementData>(FName("Player"), FString(TEXT("")));
	
}

float UACPlayerAttributes::GetWalkSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->WalkSpeed;
	
}

float UACPlayerAttributes::GetRunSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->RunSpeed;
}

float UACPlayerAttributes::GetCrouchSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->CrouchSpeed;
}

float UACPlayerAttributes::GetJumpZVelocity() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->JumpZVelocity;
}

float UACPlayerAttributes::GetJumpXYVelocity() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->JumpXYVelocity;
}

float UACPlayerAttributes::GetAirControl() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->AirControl;
}

float UACPlayerAttributes::GetDashSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->DashSpeed;
}

float UACPlayerAttributes::GetDashImpulseSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->DashImpulseSpeed;
}

float UACPlayerAttributes::GetDashAdditionalSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->DashAdditionalSpeed;
}

float UACPlayerAttributes::GetDashDuration() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->DashDuration;
}

float UACPlayerAttributes::GetDashDistance() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->DashDistance;
}

float UACPlayerAttributes::GetDashCooldown() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->DashCooldown;
}
