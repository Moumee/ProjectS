// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerJumpingState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerDashingState.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerHangingState.h"
#include "Characters/Player/SuraPlayerMantlingState.h"
#include "Characters/Player/SuraPlayerRunningState.h"
#include "Characters/Player/SuraPlayerWallRunningState.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

USuraPlayerJumpingState::USuraPlayerJumpingState()
{
	StateDisplayName = "Jumping";
	StateType = EPlayerState::Jumping;
}


void USuraPlayerJumpingState::EnterState(ASuraCharacterPlayer* Player)
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

void USuraPlayerJumpingState::UpdateBaseMovementSpeed(ASuraCharacterPlayer* Player, float DeltaTime)
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

void USuraPlayerJumpingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);

	UpdateBaseMovementSpeed(Player, DeltaTime);

	Player->InterpCapsuleAndCameraHeight(1.f, DeltaTime, 7.f);

	FHitResult WallHitResult;
	FCollisionQueryParams WallParams;
	WallParams.AddIgnoredActor(Player);

	const FVector WallDetectStart = Player->GetActorLocation();
	const FVector WallDetectEnd = WallDetectStart + Player->GetActorForwardVector() * 35.f;
	const bool bWallHit = GetWorld()->SweepSingleByChannel(WallHitResult, WallDetectStart, WallDetectEnd, FQuat::Identity,
		ECC_GameTraceChannel2, FCollisionShape::MakeCapsule(34.f,
			88.f * 0.85f), WallParams);

	if (bWallHit && Player->GetCharacterMovement()->IsFalling())
	{
		Player->WallHitResult = WallHitResult;
		FHitResult LedgeHitResult;
		FCollisionQueryParams LedgeParams;
		LedgeParams.AddIgnoredActor(Player);
		
		FVector LedgeDetectStart = FVector(WallHitResult.ImpactPoint.X, WallHitResult.ImpactPoint.Y,
			Player->GetCamera()->GetComponentLocation().Z + 100.f);
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

	if (Player->bLandedTriggered)
	{
		Player->ChangeState(Player->RunningState);
		return;
	}
}

void USuraPlayerJumpingState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);
	bShouldUpdateSpeed = false;
	ElapsedTimeFromWallRun = 0.f;
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

void USuraPlayerJumpingState::StartJumping(ASuraCharacterPlayer* Player)
{
	Super::StartJumping(Player);
	Player->DoubleJump();
}

