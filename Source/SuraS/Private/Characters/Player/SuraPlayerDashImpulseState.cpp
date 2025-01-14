// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerDashImpulseState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerDashMovementState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/Player/SuraPlayerRunningState.h"

USuraPlayerDashImpulseState::USuraPlayerDashImpulseState()
{
	StateDisplayName = "Dash Impulse";
}

void USuraPlayerDashImpulseState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);

	Player->bDashOnCooldown = true;

	const float NewBaseMovementSpeed = Player->GetPlayerMovementData()->GetDashSpeed();
	Player->SetBaseMovementSpeed(NewBaseMovementSpeed);

	
	Player->GetCharacterMovement()->GroundFriction = 0.f;
	Player->GetCharacterMovement()->BrakingFrictionFactor = 0.f;
	Player->GetCharacterMovement()->BrakingDecelerationWalking = 0.f;

	FVector DashImpulseDirection;
	FVector InputDirection;

	
	if (Player->ForwardAxisInputValue >= 0.f && Player->RightAxisInputValue == 0.f)
	{
		InputDirection = Player->GetActorForwardVector();
	}
	else
	{
		InputDirection = (Player->GetActorForwardVector() * Player->ForwardAxisInputValue +
			Player->GetActorRightVector() * Player->RightAxisInputValue).GetSafeNormal();
	}

	if (Player->GetCharacterMovement()->IsFalling())
	{
		DashImpulseDirection = InputDirection;
	}
	else
	{
		const FVector FloorNormal = Player->GetCharacterMovement()->CurrentFloor.HitResult.Normal;
		DashImpulseDirection = FVector::VectorPlaneProject(InputDirection, FloorNormal).GetSafeNormal();
	}
	
	const float DashImpulseSpeed = Player->GetPlayerMovementData()->GetDashImpulseSpeed();
	const float DashImpulseDuration = Player->GetPlayerMovementData()->GetDashDistance() / DashImpulseSpeed;
	Player->GetCharacterMovement()->Velocity = DashImpulseDirection * DashImpulseSpeed;

	GetWorld()->GetTimerManager().SetTimer(Player->DashCooldownTimerHandle, [Player]()
	{
		Player->bDashOnCooldown = false;
	}, Player->GetPlayerMovementData()->GetDashCooldown(), false);

	GetWorld()->GetTimerManager().SetTimer(Player->DashMovementTimerHandle, [Player]()
	{
		Player->DesiredGroundState = Player->RunningState;
		Player->SetBaseMovementSpeed(Player->GetPlayerMovementData()->GetRunSpeed());
		if (!Player->GetCharacterMovement()->IsFalling())
		{
			Player->ChangeState(Player->RunningState);
		}
	}, Player->GetPlayerMovementData()->GetDashDuration(), false);

	GetWorld()->GetTimerManager().SetTimer(Player->DashImpulseTimerHandle, [Player]()
	{
		const FVector CurrentVelocity = Player->GetCharacterMovement()->Velocity;
		FVector ResetVelocity = CurrentVelocity.GetSafeNormal() * Player->GetCharacterMovement()->MaxWalkSpeed;
		Player->GetCharacterMovement()->Velocity = ResetVelocity;
		Player->GetCharacterMovement()->GroundFriction = Player->DefaultGroundFriction;
		Player->GetCharacterMovement()->BrakingFrictionFactor = Player->DefaultBrakingFrictionFactor;
		Player->GetCharacterMovement()->BrakingDecelerationWalking = Player->DefaultBrakingDecelerationWalking;
		Player->ChangeState(Player->DashMovementState);
	},DashImpulseDuration, false);
	
}

void USuraPlayerDashImpulseState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Look(Player, InputVector);

	Player->AddControllerPitchInput(InputVector.Y);
	Player->AddControllerYawInput(InputVector.X);
}
