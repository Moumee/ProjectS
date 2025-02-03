// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerFallingState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerDashingState.h"
#include "Characters/Player/SuraPlayerHangingState.h"
#include "Characters/Player/SuraPlayerMantlingState.h"
#include "Characters/Player/SuraPlayerWallRunningState.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

USuraPlayerFallingState::USuraPlayerFallingState()
{
	StateDisplayName = "Falling";
	StateType = EPlayerState::Falling;
}

void USuraPlayerFallingState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);
	if (Player->GetPreviousGroundedState()->GetStateType() == EPlayerState::Dashing)
	{
		bShouldUpdateSpeed = true;
		SpeedTransitionTime = Player->GetPlayerMovementData()->GetRunDashTransitionDuration();
		float DashSpeed = Player->GetPlayerMovementData()->GetDashSpeed();
		float RunSpeed = Player->GetPlayerMovementData()->GetRunSpeed();
		SpeedChangePerSecond = (RunSpeed - DashSpeed) / SpeedTransitionTime;
	}
	ElapsedTimeFromWallRun = 0.f;
}

void USuraPlayerFallingState::UpdateBaseMovementSpeed(ASuraCharacterPlayer* Player, float DeltaTime)
{
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
}

void USuraPlayerFallingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);

	// float NewCapsuleHeight = FMath::FInterpTo(Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
	// Player->GetDefaultCapsuleHalfHeight(), DeltaTime, 5.f);
	// Player->GetCapsuleComponent()->SetCapsuleHalfHeight(NewCapsuleHeight);
	//
	// FVector CurrentCameraLocation = Player->GetCamera()->GetRelativeLocation();
	// float NewCameraZ = FMath::FInterpTo(Player->GetCamera()->GetRelativeLocation().Z,
	// 	Player->GetDefaultCameraLocation().Z, DeltaTime, 5.f);
	// Player->GetCamera()->SetRelativeLocation(FVector(CurrentCameraLocation.X, CurrentCameraLocation.X, NewCameraZ));

	UpdateBaseMovementSpeed(Player, DeltaTime);
	
	Player->InterpPlayerRoll(0.f, DeltaTime, 7.f);

	FHitResult WallHitResult;
	FCollisionQueryParams WallParams;
	WallParams.AddIgnoredActor(Player);
	
	const FVector WallDetectStart = Player->GetActorLocation();
	const FVector WallDetectEnd = WallDetectStart + Player->GetActorForwardVector() * 75.f;
	const bool bWallHit = GetWorld()->SweepSingleByChannel(WallHitResult, WallDetectStart, WallDetectEnd, FQuat::Identity,
		ECC_GameTraceChannel1, FCollisionShape::MakeCapsule(Player->GetCapsuleComponent()->GetScaledCapsuleRadius(),
			Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.85f), WallParams);
	
	if (bWallHit && Player->GetCharacterMovement()->IsFalling() && Player->GetPreviousState()->GetStateType() == EPlayerState::Jumping)
	{
		Player->WallHitResult = WallHitResult;
		FHitResult LedgeHitResult;
		FCollisionQueryParams LedgeParams;
		LedgeParams.AddIgnoredActor(Player);
		
		FVector LedgeDetectStart = Player->GetCamera()->GetComponentLocation() + FVector::UpVector * 100.f;
		FVector LedgeDetectEnd = FVector(LedgeDetectStart.X, LedgeDetectStart.Y, WallHitResult.ImpactPoint.Z);
	
		bool bLedgeHit = GetWorld()->SweepSingleByChannel(LedgeHitResult, LedgeDetectStart, LedgeDetectEnd, FQuat::Identity,
			ECC_GameTraceChannel2, FCollisionShape::MakeSphere(20.f), LedgeParams);
	
		if (bLedgeHit && Player->GetCharacterMovement()->IsWalkable(LedgeHitResult))
		{
			Player->LedgeHitResult = LedgeHitResult;	
			if (LedgeHitResult.ImpactPoint.Z < Player->GetActorLocation().Z && Player->ForwardAxisInputValue > 0.f)
			{
				Player->ChangeState(Player->MantlingState);
				return;
			}
			Player->ChangeState(Player->HangingState);
			return;
		}
	}

	if (Player->GetPreviousState()->GetStateType() == EPlayerState::WallRunning)
	{
		ElapsedTimeFromWallRun += DeltaTime;
		if (ElapsedTimeFromWallRun > 0.2f)
		{
			if (Player->ShouldEnterWallRunning(Player->WallRunDirection, Player->WallRunSide))
			{
				Player->ChangeState(Player->WallRunningState);
				return;
			}
		}
	}
	else
	{
		if (Player->ShouldEnterWallRunning(Player->WallRunDirection, Player->WallRunSide))
		{
			Player->ChangeState(Player->WallRunningState);
			return;
		}
	}
	
	
	
	

	if (Player->bDashTriggered)
	{
		Player->ChangeState(Player->DashingState);
		return;
	}

	if (Player->bLandedTriggered)
	{
		Player->ChangeState(Player->DesiredGroundState);
		return;
	}

	
}

void USuraPlayerFallingState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);
	bShouldUpdateSpeed = false;
	ElapsedTimeFromWallRun = 0.f;
}

void USuraPlayerFallingState::Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Move(Player, InputVector);

	Player->AddMovementInput(Player->GetActorForwardVector(), InputVector.Y);
	Player->AddMovementInput(Player->GetActorRightVector(), InputVector.X);
	
}

void USuraPlayerFallingState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Look(Player, InputVector);
	
	Player->AddControllerPitchInput(InputVector.Y);
	Player->AddControllerYawInput(InputVector.X);
}

void USuraPlayerFallingState::StartJumping(ASuraCharacterPlayer* Player)
{
	Super::StartJumping(Player);
	
	if (Player->JumpsLeft > 0 && Player->JumpsLeft != Player->MaxJumps ||
		Player->GetPreviousGroundedState()->GetStateType() == EPlayerState::Dashing)
	{
		Player->DoubleJump();
	}
}

void USuraPlayerFallingState::Landed(ASuraCharacterPlayer* Player, const FHitResult& HitResult)
{
	Super::Landed(Player, HitResult);
}
