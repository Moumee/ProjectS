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

	for (int i = 0; i < Player->DashCooldowns.Num(); i++)
	{
		if (Player->DashCooldowns[i] == 0.f)
		{
			Player->DashCooldowns[i] = Player->GetPlayerMovementData()->GetDashCooldown();
			break;
		}
	}

	const float NewBaseMovementSpeed = Player->GetPlayerMovementData()->GetDashSpeed();
	Player->SetBaseMovementSpeed(NewBaseMovementSpeed);

	
	Player->GetCharacterMovement()->GroundFriction = 0.f;

	FVector DashImpulseDirection;
	
	if (Player->ForwardAxisInputValue >= 0.f && Player->RightAxisInputValue == 0.f)
	{
		DashImpulseDirection = Player->GetControlRotation().Vector();
	}
	else
	{
		DashImpulseDirection = (Player->GetActorForwardVector() * Player->ForwardAxisInputValue +
			Player->GetActorRightVector() * Player->RightAxisInputValue).GetSafeNormal();
	}
	
	const float DashImpulseSpeed = Player->GetPlayerMovementData()->GetDashImpulseSpeed();
	const float DashImpulseDuration = Player->GetPlayerMovementData()->GetDashDistance() / DashImpulseSpeed;
	Player->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	Player->GetCharacterMovement()->Velocity = DashImpulseDirection * DashImpulseSpeed;

	GetWorld()->GetTimerManager().SetTimer(Player->DashImpulseTimerHandle, [Player]()
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
		FVector ResetVelocity = CurrentVelocity.GetSafeNormal() * Player->GetCharacterMovement()->MaxWalkSpeed;
		Player->GetCharacterMovement()->Velocity = ResetVelocity;
		Player->GetCharacterMovement()->GroundFriction = Player->DefaultGroundFriction;
		Player->ChangeState(Player->DashMovementState);
	},DashImpulseDuration, false);
	
}

void USuraPlayerDashImpulseState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);
}

void USuraPlayerDashImpulseState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Look(Player, InputVector);

	Player->AddControllerPitchInput(InputVector.Y);
	Player->AddControllerYawInput(InputVector.X);
}
