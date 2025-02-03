// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerDashingState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerJumpingState.h"
#include "Characters/Player/SuraPlayerRunningState.h"
#include "GameFramework/CharacterMovementComponent.h"

USuraPlayerDashingState::USuraPlayerDashingState()
{
	StateDisplayName = "Dashing";
	StateType = EPlayerState::Dashing;
}


void USuraPlayerDashingState::EnterState(ASuraCharacterPlayer* Player)
{
	
	Super::EnterState(Player);

	
	DashImpulseElapsedTime = 0.f;
	

	Player->DashesLeft--;
	Player->JumpsLeft = Player->MaxJumps - 1;
	
	for (int i = 0; i < Player->DashCooldowns.Num(); i++)
	{
		if (Player->DashCooldowns[i] == 0)
		{
			Player->DashCooldowns[i] = Player->GetPlayerMovementData()->GetDashCooldown();
			break;
		}
	}

	const float NewBaseMovementSpeed = Player->GetPlayerMovementData()->GetDashSpeed();
	Player->SetBaseMovementSpeed(NewBaseMovementSpeed);


	FVector DashImpulseDirection;
	
	if (FMath::IsNearlyEqual(Player->RightAxisInputValue, 0.f))
	{
		if (Player->ForwardAxisInputValue >= 0.f)
		{
			DashImpulseDirection = Player->GetCamera()->GetForwardVector();
		}
		else
		{
			DashImpulseDirection = Player->GetCamera()->GetForwardVector() * -1.f;
		}
		
	}
	else
	{
		DashImpulseDirection = (Player->GetActorForwardVector() * Player->ForwardAxisInputValue +
			Player->GetActorRightVector() * Player->RightAxisInputValue).GetSafeNormal();
		
		if (Player->ForwardAxisInputValue < 0.f)
		{
			DashImpulseDirection.Z = Player->GetCamera()->GetForwardVector().Z * -1.f;
		}
	}
	Player->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	const float DashImpulseSpeed = Player->GetPlayerMovementData()->GetDashImpulseSpeed();
	DashImpulseDuration = Player->GetPlayerMovementData()->GetDashDistance() / DashImpulseSpeed;
	Player->GetCharacterMovement()->Velocity = DashImpulseDirection * DashImpulseSpeed;
	
}

void USuraPlayerDashingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);
	
	Player->InterpPlayerRoll(0.f, DeltaTime, 7.f);
	
	// float NewCapsuleHeight = FMath::FInterpTo(Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
	// Player->GetDefaultCapsuleHalfHeight(), DeltaTime, 10.f);
	// Player->GetCapsuleComponent()->SetCapsuleHalfHeight(NewCapsuleHeight);
	//
	// FVector CurrentCameraLocation = Player->GetCamera()->GetRelativeLocation();
	// float NewCameraZ = FMath::FInterpTo(Player->GetCamera()->GetRelativeLocation().Z,
	// 	Player->GetDefaultCameraLocation().Z, DeltaTime, 10.f);
	// Player->GetCamera()->SetRelativeLocation(FVector(CurrentCameraLocation.X, CurrentCameraLocation.X, NewCameraZ));

	if (DashImpulseElapsedTime < DashImpulseDuration)
	{
		DashImpulseElapsedTime += DeltaTime;
	}
	else
	{
		if (Player->GetCharacterMovement()->IsMovingOnGround())
		{
			Player->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
		else
		{
			Player->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		}
		
		const FVector CurrentVelocity = Player->GetCharacterMovement()->Velocity;
		FVector ResetVelocity = CurrentVelocity.GetSafeNormal() * Player->GetPlayerMovementData()->GetDashSpeed();
		Player->GetCharacterMovement()->Velocity = ResetVelocity;
		
		if (Player->GetCharacterMovement()->IsFalling())
		{
			if (Player->IsFallingDown())
			{
				Player->ChangeState(Player->FallingState);
				return;
			}
			Player->ChangeState(Player->JumpingState);
			return;
		}
		Player->ChangeState(Player->RunningState);
		return;
	}

	
}

void USuraPlayerDashingState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);
	Player->SetPreviousGroundedState(this);
	DashImpulseElapsedTime = 0.f;
}

void USuraPlayerDashingState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Look(Player, InputVector);
	Player->AddControllerYawInput(InputVector.X);
	Player->AddControllerPitchInput(InputVector.Y);
}

