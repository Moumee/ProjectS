// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerRunningState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerCrouchingState.h"
#include "Characters/Player/SuraPlayerDashingState.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerJumpingState.h"
#include "Characters/Player/SuraPlayerWalkingState.h"

USuraPlayerRunningState::USuraPlayerRunningState()
{
	StateDisplayName = "Running";
}

void USuraPlayerRunningState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);

	const float NewBaseMovementSpeed = Player->GetPlayerMovementData()->GetRunSpeed();
	Player->SetBaseMovementSpeed(NewBaseMovementSpeed);
	
	Player->DesiredGroundState = Player->RunningState;
}

void USuraPlayerRunningState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);

	if (Player->IsFallingDown())
	{
		Player->ChangeState(Player->FallingState);
		return;
	}

	if (Player->bDashTriggered)
	{
		Player->ChangeState(Player->DashingState);
		return;
	}

	if (Player->ForwardAxisInputValue <= 0.f)
	{
		Player->ChangeState(Player->WalkingState);
		return;
	}

	if (Player->bCrouchTriggered)
	{
		Player->ChangeState(Player->CrouchingState);
		return;
	}

	if (Player->bJumpTriggered)
	{
		Player->ChangeState(Player->JumpingState);
		return;
	}
	
}

void USuraPlayerRunningState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);
}

void USuraPlayerRunningState::Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Move(Player, InputVector);

	Player->AddMovementInput(Player->GetActorForwardVector(), InputVector.Y);
	Player->AddMovementInput(Player->GetActorRightVector(), InputVector.X);
}

void USuraPlayerRunningState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Look(Player, InputVector);

	Player->AddControllerPitchInput(InputVector.Y);
	Player->AddControllerYawInput(InputVector.X);
}

void USuraPlayerRunningState::StartJumping(ASuraCharacterPlayer* Player)
{
	Super::StartJumping(Player);
	Player->PrimaryJump();

	
}

