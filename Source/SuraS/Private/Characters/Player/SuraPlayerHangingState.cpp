// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerHangingState.h"

#include "Characters/Player/SuraCharacterPlayer.h"

USuraPlayerHangingState::USuraPlayerHangingState()
{
	StateDisplayName = "Hanging";
}

void USuraPlayerHangingState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);
}

void USuraPlayerHangingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);

	// if (Player->ForwardAxisInputValue > 0.f)
	// {
	// 	Player->ChangeState()
	// }
}

void USuraPlayerHangingState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);
}

void USuraPlayerHangingState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Look(Player, InputVector);
}
