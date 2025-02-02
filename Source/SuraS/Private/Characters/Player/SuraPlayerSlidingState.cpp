// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerSlidingState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerCrouchingState.h"
#include "Characters/Player/SuraPlayerDashingState.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerJumpingState.h"
#include "Characters/Player/SuraPlayerRunningState.h"
#include "GameFramework/CharacterMovementComponent.h"

USuraPlayerSlidingState::USuraPlayerSlidingState()
{
	StateDisplayName = "Sliding State";
	StateType = EPlayerState::Sliding;
}

void USuraPlayerSlidingState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);

	Player->GetCharacterMovement()->GroundFriction = 0.f;
	Player->GetCharacterMovement()->BrakingDecelerationWalking = 0.f;


	StartSpeed = Player->GetVelocity().Size() + 800.f;
	CurrentSlideSpeed = StartSpeed;
	CrouchSpeed = Player->GetPlayerMovementData()->GetCrouchSpeed();
	
	FVector ForwardVector = Player->GetActorForwardVector();
	FVector RightVector = Player->GetActorRightVector();
	SlideDirection = (ForwardVector * Player->ForwardAxisInputValue + RightVector * Player->RightAxisInputValue).GetSafeNormal();

	Player->GetCharacterMovement()->Velocity = SlideDirection * StartSpeed;
	
}

void USuraPlayerSlidingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);

	if (Player->FindFloorAngle() >= -0.1f)
	{
		CurrentSlideSpeed = FMath::Max(CurrentSlideSpeed - SlideDeltaSpeed * DeltaTime, CrouchSpeed);
	}

	
	Player->GetCharacterMovement()->Velocity = SlideDirection * CurrentSlideSpeed;

	if (CurrentSlideSpeed <= CrouchSpeed)
	{
		Player->ChangeState(Player->CrouchingState);
		return;
	}

	if (Player->IsFallingDown() || !Player->GetCharacterMovement()->CurrentFloor.bBlockingHit)
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
		Player->DesiredGroundState = Player->RunningState;
		Player->ChangeState(Player->JumpingState);
		return;
		
	}

	if (!Player->bCrouchTriggered)
	{
		Player->ChangeState(Player->RunningState);
		return;
	}
}

void USuraPlayerSlidingState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);
	Player->GetCharacterMovement()->GroundFriction = Player->DefaultGroundFriction;
	Player->GetCharacterMovement()->BrakingDecelerationWalking = Player->DefaultBrakingDecelerationWalking;
}


void USuraPlayerSlidingState::StartJumping(ASuraCharacterPlayer* Player)
{
	Super::StartJumping(Player);
	Player->PrimaryJump();
}

void USuraPlayerSlidingState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Look(Player, InputVector);
	Player->AddControllerPitchInput(InputVector.Y);
	Player->AddControllerYawInput(InputVector.X);
}


