// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerCrouchingState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerDashingState.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerJumpingState.h"
#include "Characters/Player/SuraPlayerRunningState.h"
#include "Characters/Player/SuraPlayerWalkingState.h"
#include "Components/CapsuleComponent.h"


USuraPlayerCrouchingState::USuraPlayerCrouchingState()
{
	StateDisplayName = "Crouching";
	StateType = EPlayerState::Crouching;
	
}

void USuraPlayerCrouchingState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);

	bShouldUpdateSpeed = true;

	bShouldUpdateAnimation = true;

	float CrouchSpeed = Player->GetPlayerMovementData()->GetCrouchSpeed();


	switch (Player->GetPreviousGroundedState()->GetStateType())
	{
	case EPlayerState::Walking:
		{
			SpeedTransitionTime = Player->GetPlayerMovementData()->GetWalkCrouchTransitionDuration();
			float WalkSpeed = Player->GetPlayerMovementData()->GetWalkSpeed();
			SpeedChangePerSecond = (CrouchSpeed - WalkSpeed) / SpeedTransitionTime;
			break;
		}

	case EPlayerState::Dashing:
		{
			SpeedTransitionTime = Player->GetPlayerMovementData()->GetCrouchDashTransitionDuration();
			float DashSpeed = Player->GetPlayerMovementData()->GetDashSpeed();
			SpeedChangePerSecond = (CrouchSpeed- DashSpeed) / SpeedTransitionTime;
			break;
		}
	case EPlayerState::Running:
		{
			SpeedTransitionTime = Player->GetPlayerMovementData()->GetCrouchRunTransitionDuration();
			float RunSpeed = Player->GetPlayerMovementData()->GetRunSpeed();
			SpeedChangePerSecond = (CrouchSpeed - RunSpeed) / SpeedTransitionTime;
			break;
		}
	default:
		break;
	}
	
	
}

void USuraPlayerCrouchingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);


	if (bShouldUpdateSpeed)
	{
		float CurrentBaseSpeed = Player->GetBaseMovementSpeed();
		float TargetCrouchSpeed = Player->GetPlayerMovementData()->GetCrouchSpeed();
		if (SpeedChangePerSecond > 0 && CurrentBaseSpeed < TargetCrouchSpeed ||
			SpeedChangePerSecond < 0 && CurrentBaseSpeed > TargetCrouchSpeed)
		{
			float NewBaseMovementSpeed = Player->GetBaseMovementSpeed() + SpeedChangePerSecond * DeltaTime;
			Player->SetBaseMovementSpeed(NewBaseMovementSpeed);
		}
		else
		{
			Player->SetBaseMovementSpeed(Player->GetPlayerMovementData()->GetCrouchSpeed());
			bShouldUpdateSpeed = false;
		}
	}

	// float NewCapsuleHeight = FMath::FInterpTo(Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
	// 	Player->GetDefaultCapsuleHalfHeight() * 0.65f, DeltaTime, 5.f);
	// Player->GetCapsuleComponent()->SetCapsuleHalfHeight(NewCapsuleHeight);
	//
	// FVector CurrentCameraLocation = Player->GetCamera()->GetRelativeLocation();
	// float NewCameraZ = FMath::FInterpTo(Player->GetCamera()->GetRelativeLocation().Z,
	// 	Player->GetDefaultCameraLocation().Z * 0.65f, DeltaTime, 5.f);
	// Player->GetCamera()->SetRelativeLocation(FVector(CurrentCameraLocation.X, CurrentCameraLocation.X, NewCameraZ));
	

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

	if (Player->bCrouchTriggered)
	{
		Player->ChangeState(Player->RunningState);
		return;
	}

	
}

void USuraPlayerCrouchingState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);

	Player->SetPreviousGroundedState(Player->CrouchingState);
	bShouldUpdateSpeed = false;
	
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




