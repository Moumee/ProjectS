// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerRunningState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerCrouchingState.h"
#include "Characters/Player/SuraPlayerDashingState.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerJumpingState.h"
#include "Characters/Player/SuraPlayerWalkingState.h"
#include "Components/CapsuleComponent.h"

USuraPlayerRunningState::USuraPlayerRunningState()
{
	StateDisplayName = "Running";
	StateType = EPlayerState::Running;
}


void USuraPlayerRunningState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);

	bShouldUpdateSpeed = true;

	float RunSpeed = Player->GetPlayerMovementData()->GetRunSpeed();


	switch (Player->GetPreviousGroundedState()->GetStateType())
	{
	case EPlayerState::Walking:
		{
			SpeedTransitionTime = Player->GetPlayerMovementData()->GetWalkRunTransitionDuration();
			float WalkSpeed = Player->GetPlayerMovementData()->GetWalkSpeed();
			SpeedChangePerSecond = (RunSpeed - WalkSpeed) / SpeedTransitionTime;
			break;
		}

	case EPlayerState::Dashing:
		{
			SpeedTransitionTime = Player->GetPlayerMovementData()->GetRunDashTransitionDuration();
			float DashSpeed = Player->GetPlayerMovementData()->GetDashSpeed();
			SpeedChangePerSecond = (RunSpeed - DashSpeed) / SpeedTransitionTime;
			break;
		}
	case EPlayerState::Crouching:
		{
			SpeedTransitionTime = Player->GetPlayerMovementData()->GetCrouchRunTransitionDuration();
			float CrouchSpeed = Player->GetPlayerMovementData()->GetCrouchSpeed();
			SpeedChangePerSecond = (RunSpeed - CrouchSpeed) / SpeedTransitionTime;
			break;
		}
	default:
		break;
	}

	Player->DesiredGroundState = Player->RunningState;
}

void USuraPlayerRunningState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);

	if (bShouldUpdateSpeed)
	{
		float CurrentBaseSpeed = Player->GetBaseMovementSpeed();
		float TargetRunSpeed = Player->GetPlayerMovementData()->GetRunSpeed();
		if (SpeedChangePerSecond > 0 && CurrentBaseSpeed < TargetRunSpeed ||
			SpeedChangePerSecond < 0 && CurrentBaseSpeed > TargetRunSpeed)
		{
			float NewBaseMovementSpeed = Player->GetBaseMovementSpeed() + SpeedChangePerSecond * DeltaTime;
			Player->SetBaseMovementSpeed(NewBaseMovementSpeed);
		}
		else
		{
			Player->SetBaseMovementSpeed(Player->GetPlayerMovementData()->GetRunSpeed());
			bShouldUpdateSpeed = false;
		}
	}

	// float NewCapsuleHeight = FMath::FInterpTo(Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
	// 	Player->GetDefaultCapsuleHalfHeight(), DeltaTime, 5.f);
	// Player->GetCapsuleComponent()->SetCapsuleHalfHeight(NewCapsuleHeight);
	//
	// FVector CurrentCameraLocation = Player->GetCamera()->GetRelativeLocation();
	// float NewCameraZ = FMath::FInterpTo(Player->GetCamera()->GetRelativeLocation().Z,
	// 	Player->GetDefaultCameraLocation().Z, DeltaTime, 5.f);
	// Player->GetCamera()->SetRelativeLocation(FVector(CurrentCameraLocation.X, CurrentCameraLocation.X, NewCameraZ));


	if (Player->IsFallingDown())
	{
		Player->ChangeState(Player->FallingState);
		return;
	}
	
	if (Player->ForwardAxisInputValue <= 0.f)
	{
		Player->ChangeState(Player->WalkingState);
		return;
	}

	if (Player->bDashTriggered)
	{
		Player->ChangeState(Player->DashingState);
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
	Player->SetPreviousGroundedState(this);
	bShouldUpdateSpeed = false;
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

