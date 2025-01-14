// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerWalkingState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerDashImpulseState.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerJumpingState.h"
#include "Characters/Player/SuraPlayerRunningState.h"
#include "GameFramework/CharacterMovementComponent.h"

USuraPlayerWalkingState::USuraPlayerWalkingState()
{
	StateDisplayName = "Walking";
}

void USuraPlayerWalkingState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);

	const float NewBaseMovementSpeed = Player->GetPlayerMovementData()->GetWalkSpeed();
	Player->SetBaseMovementSpeed(NewBaseMovementSpeed);
	
	Player->DesiredGroundState = Player->WalkingState;
}

void USuraPlayerWalkingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);

	if (Player->IsFallingDown())
	{
		Player->ChangeState(Player->FallingState);
		return;
	}

	if (Player->bDashTriggered)
	{
		Player->ChangeState(Player->DashImpulseState);
		return;
	}

	if (Player->bRunTriggered)
	{
		Player->ChangeState(Player->RunningState);
		return;
	}

	if (Player->bJumpTriggered)
	{
		Player->ChangeState(Player->JumpingState);
		return;
	}
}

void USuraPlayerWalkingState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);
}

void USuraPlayerWalkingState::Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Move(Player, InputVector);

	Player->AddMovementInput(Player->GetActorForwardVector(), InputVector.Y);
	Player->AddMovementInput(Player->GetActorRightVector(), InputVector.X);
	
}

void USuraPlayerWalkingState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Look(Player, InputVector);

	Player->AddControllerPitchInput(InputVector.Y);
	Player->AddControllerYawInput(InputVector.X);
}

void USuraPlayerWalkingState::StartJumping(ASuraCharacterPlayer* Player)
{
	Super::StartJumping(Player);
	Player->PrimaryJump();
}
