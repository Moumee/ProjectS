// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerCrouchingState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerDashImpulseState.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerJumpingState.h"
#include "Characters/Player/SuraPlayerRunningState.h"
#include "Characters/Player/SuraPlayerWalkingState.h"
#include "Components/CapsuleComponent.h"


USuraPlayerCrouchingState::USuraPlayerCrouchingState()
{
	StateDisplayName = "Crouching";
	bShouldStartCrouching = false;
	
}

void USuraPlayerCrouchingState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);
	DefaultCapsuleHalfHeight = Player->GetDefaultCapsuleHalfHeight();
	DefaultCameraLocation = Player->GetDefaultCameraLocation();
	CrouchedCapsuleHalfHeight = DefaultCapsuleHalfHeight * 0.6f;
	CrouchedCameraLocation = FVector(DefaultCameraLocation.X, DefaultCameraLocation.Y, DefaultCameraLocation.Z * 0.6f);
	Player->SetBaseMovementSpeed(Player->GetPlayerMovementData()->GetCrouchSpeed());
	Player->DesiredGroundState = Player->CrouchingState;
	CrouchTransitionElapsedTime = 0.f;
	UnCrouchTransitionElapsedTime = 0.f;
	bShouldStartCrouching = true;
}

void USuraPlayerCrouchingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);

	if (bShouldStartCrouching)
	{
		if (CrouchTransitionElapsedTime < CrouchTransitionDuration)
		{
			CrouchTransitionElapsedTime += DeltaTime;
			const float t = CrouchTransitionElapsedTime / CrouchTransitionDuration;
			const float NewHalfHeight = FMath::Lerp(DefaultCapsuleHalfHeight, CrouchedCapsuleHalfHeight, t);
			const FVector NewCameraLocation = FMath::Lerp(DefaultCameraLocation, CrouchedCameraLocation, t);
			Player->GetCapsuleComponent()->SetCapsuleHalfHeight(NewHalfHeight);
			Player->GetCamera()->SetRelativeLocation(NewCameraLocation);
		}
		else
		{
			bShouldStartCrouching = false;
			Player->GetCapsuleComponent()->SetCapsuleHalfHeight(CrouchedCapsuleHalfHeight);
			Player->GetCamera()->SetRelativeLocation(CrouchedCameraLocation);
			CrouchTransitionElapsedTime = 0.f;
		}
	}

	if (Player->IsFallingDown())
	{
		Player->ChangeState(Player->FallingState);
		return;
	}

	if (Player->bDashTriggered)
	{
		UnCrouchToNextState(Player, Player->DashImpulseState, DeltaTime);
	}

	if (Player->bJumpTriggered)
	{
		Player->DesiredGroundState = Player->WalkingState;
		UnCrouchToNextState(Player, Player->JumpingState, DeltaTime);
	}

	if (Player->bRunTriggered)
	{
		UnCrouchToNextState(Player, Player->RunningState, DeltaTime);
	}

	if (Player->bCrouchTriggered)
	{
		UnCrouchToNextState(Player, Player->WalkingState, DeltaTime);
	}

	
}

void USuraPlayerCrouchingState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);

	bShouldStartCrouching = false;
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

void USuraPlayerCrouchingState::UnCrouchToNextState(ASuraCharacterPlayer* Player, USuraPlayerBaseState* NextState, float DeltaTime)
{
	if (UnCrouchTransitionElapsedTime < CrouchTransitionDuration)
	{
		UnCrouchTransitionElapsedTime += DeltaTime;
		const float t = UnCrouchTransitionElapsedTime / CrouchTransitionDuration;
		const float NewHalfHeight = FMath::Lerp(CrouchedCapsuleHalfHeight, DefaultCapsuleHalfHeight, t);
		const FVector NewCameraLocation = FMath::Lerp(CrouchedCameraLocation, DefaultCameraLocation, t);
		Player->GetCapsuleComponent()->SetCapsuleHalfHeight(NewHalfHeight);
		Player->GetCamera()->SetRelativeLocation(NewCameraLocation);
	}
	else
	{
		Player->GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultCapsuleHalfHeight);
		Player->GetCamera()->SetRelativeLocation(DefaultCameraLocation);
		UnCrouchTransitionElapsedTime = 0.f;
		Player->ChangeState(NextState);
		return;
	}
}


