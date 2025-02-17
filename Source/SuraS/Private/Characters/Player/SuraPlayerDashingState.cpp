// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerDashingState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerJumpingState.h"
#include "Characters/Player/SuraPlayerRunningState.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

USuraPlayerDashingState::USuraPlayerDashingState()
{
	StateDisplayName = "Dashing";
	StateType = EPlayerState::Dashing;
}


void USuraPlayerDashingState::EnterState(ASuraCharacterPlayer* Player)
{
	
	Super::EnterState(Player);

	PlayerController = Player->GetController<APlayerController>();
	
	DashElapsedTime = 0.f;
	

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
	
	if (FMath::IsNearlyEqual(Player->RightAxisInputValue, 0.f))
	{
		if (Player->ForwardAxisInputValue >= 0.f)
		{
			DashDirection = Player->GetCamera()->GetForwardVector();
		}
		else
		{
			DashDirection = Player->GetCamera()->GetForwardVector() * -1.f;
		}
		
	}
	else
	{
		DashDirection = (Player->GetActorForwardVector() * Player->ForwardAxisInputValue +
			Player->GetActorRightVector() * Player->RightAxisInputValue).GetSafeNormal();
		
		if (Player->ForwardAxisInputValue < 0.f)
		{
			DashDirection.Z = Player->GetCamera()->GetForwardVector().Z * -1.f;
		}
	}
	Player->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	
	DashImpulseSpeed = Player->GetPlayerMovementData()->GetDashImpulseSpeed();
	DashEndSpeed = Player->GetPlayerMovementData()->GetDashSpeed();

	EndTransitionDuration = Player->GetPlayerMovementData()->GetDashImpulseTransitionEndDuration();

	float DashImpulseDistance = Player->GetPlayerMovementData()->GetDashDistance();
	DashImpulseDuration = DashImpulseDistance / DashImpulseSpeed;
	Player->GetCharacterMovement()->Velocity = DashDirection * DashImpulseSpeed;
	
}

void USuraPlayerDashingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);
	
	Player->InterpCapsuleHeight(1.f, DeltaTime);

	Player->StartCamShake(Player->DashCamShake);
	
	
	
	if (DashElapsedTime < DashImpulseDuration + EndTransitionDuration)
	{
		DashElapsedTime += DeltaTime;
		if (DashElapsedTime >= DashImpulseDuration)
		{
			float Alpha = (DashElapsedTime - DashImpulseDuration) / EndTransitionDuration;
			float TargetSpeed = FMath::Lerp(DashImpulseSpeed, DashEndSpeed, Alpha);
			float NewSpeed = FMath::FInterpTo(Player->GetCharacterMovement()->Velocity.Size(), TargetSpeed, DeltaTime, 10.f);
			Player->GetCharacterMovement()->Velocity = NewSpeed * DashDirection;
		}
		
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
	DashElapsedTime = 0.f;
}

void USuraPlayerDashingState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Look(Player, InputVector);
	Player->AddControllerYawInput(InputVector.X);
	Player->AddControllerPitchInput(InputVector.Y);
}

