// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerBaseState.h"

#include "Characters/Player/SuraCharacterPlayer.h"

void USuraPlayerBaseState::EnterState(ASuraCharacterPlayer* Player)
{
	Player->ResetTriggeredBooleans();
}

void USuraPlayerBaseState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
}

void USuraPlayerBaseState::ExitState(ASuraCharacterPlayer* Player)
{
	Player->ResetTriggeredBooleans();
}

void USuraPlayerBaseState::Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
}

void USuraPlayerBaseState::StopMoving(ASuraCharacterPlayer* Player)
{
}

void USuraPlayerBaseState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
}

void USuraPlayerBaseState::StartRunning(ASuraCharacterPlayer* Player)
{
}

void USuraPlayerBaseState::StartJumping(ASuraCharacterPlayer* Player)
{
}

void USuraPlayerBaseState::StartDashing(ASuraCharacterPlayer* Player)
{
}

void USuraPlayerBaseState::StartCrouching(ASuraCharacterPlayer* Player)
{
}

void USuraPlayerBaseState::Landed(ASuraCharacterPlayer* Player, const FHitResult& HitResult)
{
}
