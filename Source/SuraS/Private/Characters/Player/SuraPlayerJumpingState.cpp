// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerJumpingState.h"

#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerDashImpulseState.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerRunningState.h"
#include "GameFramework/CharacterMovementComponent.h"

USuraPlayerJumpingState::USuraPlayerJumpingState()
{
	StateDisplayName = "Jumping";
}

void USuraPlayerJumpingState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);
}

void USuraPlayerJumpingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);

	if (Player->IsFallingDown())
	{
		Player->ChangeState(Player->FallingState);
		return;
	}

	if (Player->bRunTriggered)
	{
		Player->DesiredGroundState = Player->RunningState;
	}

	if (Player->bDashTriggered)
	{
		Player->ChangeState(Player->DashImpulseState);
		return;
	}

	if (Player->bLandedTriggered)
	{
		Player->ChangeState(Player->DesiredGroundState);
		return;
	}
}

void USuraPlayerJumpingState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);
}

void USuraPlayerJumpingState::Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Move(Player, InputVector);

	Player->AddMovementInput(Player->GetActorForwardVector(), InputVector.Y);
	Player->AddMovementInput(Player->GetActorRightVector(), InputVector.X);
}

void USuraPlayerJumpingState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Look(Player, InputVector);

	Player->AddControllerPitchInput(InputVector.Y);
	Player->AddControllerYawInput(InputVector.X);

	
}

void USuraPlayerJumpingState::StartRunning(ASuraCharacterPlayer* Player)
{
	Super::StartRunning(Player);
}

void USuraPlayerJumpingState::StartJumping(ASuraCharacterPlayer* Player)
{
	Super::StartJumping(Player);
	Player->DoubleJump();
}

