// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerRunningState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/SuraWeaponChargingState.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerCrouchingState.h"
#include "Characters/Player/SuraPlayerDashingState.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerJumpingState.h"
#include "Characters/Player/SuraPlayerSlidingState.h"
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

	PlayerController = Player->GetController<APlayerController>();

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

	Player->InterpCapsuleHeight(1.f, DeltaTime);

	Player->StartCamShake(Player->RunCamShake);
	
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

	if (Player->HasWeapon())
	{
		if (Player->GetWeaponSystemComponent()->GetCurrentWeapon()->GetCurrentState()->GetWeaponStateType() == EWeaponStateType::WeaponStateType_Charging)
		{
			Player->ChangeState(Player->WalkingState);
			return;
		}
	}

	if (Player->bDashTriggered)
	{
		Player->ChangeState(Player->DashingState);
		return;
	}

	if (Player->bCrouchTriggered)
	{
		if (FMath::IsNearlyEqual(Player->GetBaseMovementSpeed(), Player->GetPlayerMovementData()->GetRunSpeed(), 10.f) &&
			Player->FindFloorAngle() <= 0.1f)
		{
			Player->ChangeState(Player->SlidingState);
			return;
		}
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

