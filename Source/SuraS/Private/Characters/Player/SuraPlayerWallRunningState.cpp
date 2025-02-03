// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerWallRunningState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerJumpingState.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

USuraPlayerWallRunningState::USuraPlayerWallRunningState()
{
	StateDisplayName = "Wall Running";
	StateType = EPlayerState::WallRunning;
}


void USuraPlayerWallRunningState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);

	Player->GetCharacterMovement()->GravityScale = 0.f;
	Player->GetCharacterMovement()->AirControl = 1.f;
	Player->GetCharacterMovement()->SetPlaneConstraintNormal(FVector::UpVector);
	Player->JumpsLeft = Player->MaxJumps;
	WallRunSide = Player->WallRunSide;
	// Need to make variable for minimum wall run velocity!! Currently set to WalkSpeed for testing purposes.
	StateEnterVelocity = FMath::Max(Player->GetCharacterMovement()->Velocity.Size(), Player->GetPlayerMovementData()->GetRunSpeed());
	TargetRoll = Player->WallRunSide == EWallSide::Left ? 15.f : -15.f;

}



void USuraPlayerWallRunningState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);

	Player->InterpPlayerRoll(TargetRoll, DeltaTime, 7.f);

	SetPlayerWallOffsetLocation(Player, DeltaTime);

	
	if (Player->bJumpTriggered)
	{
		Player->ChangeState(Player->JumpingState);
		return;
	}

	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Player);
	FVector LineTraceEnd = FVector::ZeroVector;
	if (Player->WallRunSide == EWallSide::Left)
	{
		LineTraceEnd = Player->GetActorLocation() +
			FVector::CrossProduct(Player->WallRunDirection, FVector::UpVector).GetSafeNormal() * 200.f;
	}
	else if (Player->WallRunSide == EWallSide::Right)
	{
		LineTraceEnd = Player->GetActorLocation() +
			FVector::CrossProduct(Player->WallRunDirection, FVector::DownVector).GetSafeNormal() * 200.f;
	}

	bool bWallHit = GetWorld()->LineTraceSingleByChannel(
		WallHit,
		Player->GetActorLocation(),
		LineTraceEnd,
		ECC_Visibility,
		Params);

	if (bWallHit)
	{
		FVector WallNormalXY = FVector(WallHit.ImpactNormal.X, WallHit.ImpactNormal.Y, 0.f);
		FVector RightVectorXY = FVector(Player->GetActorRightVector().X, Player->GetActorRightVector().Y, 0.f);
		FVector NewWallRunDirection = FVector::ZeroVector;
		EWallSide NewWallRunSide = EWallSide::None;
		if (FVector::DotProduct(WallNormalXY, RightVectorXY) > 0.f)
		{
			NewWallRunDirection = FVector::CrossProduct(WallNormalXY, FVector::UpVector).GetSafeNormal();
			NewWallRunSide = EWallSide::Left;
		}
		else
		{
			NewWallRunDirection = FVector::CrossProduct(WallNormalXY, FVector::DownVector).GetSafeNormal();
			NewWallRunSide = EWallSide::Right;
		}
		
		if (NewWallRunSide == Player->WallRunSide)
		{
			Player->WallRunDirection = NewWallRunDirection;
			Player->WallRunSide = NewWallRunSide;
			Player->GetCharacterMovement()->Velocity = FVector(NewWallRunDirection.X, NewWallRunDirection.Y, 0.f) * StateEnterVelocity;
		}
		else
		{
			Player->ChangeState(Player->FallingState);
			return;
		}
	}
	else
	{
		Player->ChangeState(Player->FallingState);
		return;
	}
	
}

void USuraPlayerWallRunningState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);
	Player->GetCharacterMovement()->GravityScale = 1.f;
	Player->GetCharacterMovement()->AirControl = Player->GetPlayerMovementData()->GetAirControl();
	Player->GetCharacterMovement()->SetPlaneConstraintNormal(FVector::ZeroVector);
	Player->WallRunSide = EWallSide::None;
}

void USuraPlayerWallRunningState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Look(Player, InputVector);
	Player->AddControllerYawInput(InputVector.X);
	Player->AddControllerPitchInput(InputVector.Y);
}

void USuraPlayerWallRunningState::Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Move(Player, InputVector);
}

void USuraPlayerWallRunningState::StartJumping(ASuraCharacterPlayer* Player)
{
	Super::StartJumping(Player);
	
	if (Player->JumpsLeft > 0)
	{
		FVector LaunchDirectionXY = FVector::ZeroVector;
		Player->JumpsLeft--;
		if (Player->WallRunSide == EWallSide::Left)
		{
			LaunchDirectionXY = FVector::CrossProduct(Player->WallRunDirection, FVector::DownVector).GetSafeNormal();
		}
		else if (Player->WallRunSide == EWallSide::Right)
		{
			LaunchDirectionXY = FVector::CrossProduct(Player->WallRunDirection, FVector::UpVector).GetSafeNormal();
		}
		FVector LaunchVector = LaunchDirectionXY * 500.f +
			FVector::UpVector * Player->GetPlayerMovementData()->GetJumpZVelocity();
		Player->LaunchCharacter(LaunchVector, false, true);
	}
}

void USuraPlayerWallRunningState::SetPlayerWallOffsetLocation(ASuraCharacterPlayer* Player, float DeltaTime)
{
	if (WallHit.ImpactNormal != FVector::ZeroVector)
	{
		FVector NewLocation = FMath::VInterpTo(Player->GetActorLocation(),
											   Player->GetActorLocation() + WallHit.ImpactNormal * 30.f, DeltaTime, 15.f);
	}
}