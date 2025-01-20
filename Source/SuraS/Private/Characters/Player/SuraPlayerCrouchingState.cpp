// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerCrouchingState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerDashingState.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerJumpingState.h"
#include "Characters/Player/SuraPlayerRunningState.h"
#include "Characters/Player/SuraPlayerWalkingState.h"


USuraPlayerCrouchingState::USuraPlayerCrouchingState()
{
	StateDisplayName = "Crouching";
	
}

void USuraPlayerCrouchingState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);

	Player->SetBaseMovementSpeed(Player->GetPlayerMovementData()->GetCrouchSpeed());
	
}

void USuraPlayerCrouchingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
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

	if (Player->bJumpTriggered)
	{
		Player->DesiredGroundState = Player->WalkingState;
		Player->ChangeState(Player->JumpingState);
		return;
		
	}

	if (Player->bRunTriggered)
	{
		Player->ChangeState(Player->RunningState);
		return;
	}

	if (Player->bCrouchTriggered)
	{
		Player->ChangeState(Player->WalkingState);
		return;
	}

	
}

void USuraPlayerCrouchingState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);
	
}

void USuraPlayerCrouchingState::Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Player->AddMovementInput(Player->GetActorForwardVector(), InputVector.Y);
	Player->AddMovementInput(Player->GetActorRightVector(), InputVector.X);
}

void USuraPlayerCrouchingState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Player->AddControllerPitchInput(InputVector.Y);
	Player->AddControllerYawInput(InputVector.X);
}

void USuraPlayerCrouchingState::StartJumping(ASuraCharacterPlayer* Player)
{
	Super::StartJumping(Player);
	Player->PrimaryJump();
}




