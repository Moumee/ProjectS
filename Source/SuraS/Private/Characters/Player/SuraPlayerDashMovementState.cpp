// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerDashMovementState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerDashImpulseState.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerJumpingState.h"
#include "Characters/Player/SuraPlayerRunningState.h"
#include "GameFramework/CharacterMovementComponent.h"

USuraPlayerDashMovementState::USuraPlayerDashMovementState()
{
	StateDisplayName = "Dash Movement";
}

void USuraPlayerDashMovementState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);
	
	Player->DesiredGroundState = Player->DashMovementState;

	GetWorld()->GetTimerManager().SetTimer(Player->DashMovementTimerHandle, [Player]()
	{
		Player->DesiredGroundState = Player->RunningState;
		if (!Player->GetCharacterMovement()->IsFalling())
		{
			Player->ChangeState(Player->RunningState);
		}
	}, Player->GetPlayerMovementData()->GetDashDuration(), false);
	
}

void USuraPlayerDashMovementState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
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

	if (Player->bJumpTriggered)
	{
		Player->ChangeState(Player->JumpingState);
		return;
	}
	
}

void USuraPlayerDashMovementState::Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Move(Player, InputVector);
	Player->AddMovementInput(Player->GetActorForwardVector(), InputVector.Y);
	Player->AddMovementInput(Player->GetActorRightVector(), InputVector.X);
}

void USuraPlayerDashMovementState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Look(Player, InputVector);
	Player->AddControllerPitchInput(InputVector.Y);
	Player->AddControllerYawInput(InputVector.X);
}

void USuraPlayerDashMovementState::StartJumping(ASuraCharacterPlayer* Player)
{
	Super::StartJumping(Player);
	Player->PrimaryJump();
}

