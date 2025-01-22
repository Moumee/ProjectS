// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerWallRunningState.h"

USuraPlayerWallRunningState::USuraPlayerWallRunningState()
{
	StateDisplayName = "Wall Running";
	StateType = EPlayerState::WallRunning;
}


void USuraPlayerWallRunningState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);
}

void USuraPlayerWallRunningState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);
}

void USuraPlayerWallRunningState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);
}
