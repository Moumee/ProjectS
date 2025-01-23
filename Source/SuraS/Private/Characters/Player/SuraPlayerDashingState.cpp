// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerDashingState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/SuraCharacterPlayer.h"
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

	Player->DashesLeft--;
	
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

	
	Player->GetCharacterMovement()->GroundFriction = 0.f;

	FVector DashImpulseDirection;
	
	if (Player->RightAxisInputValue == 0.f)
	{
		if (Player->ForwardAxisInputValue >= 0.f)
		{
			DashImpulseDirection = Player->GetControlRotation().Vector();
		}
		else
		{
			DashImpulseDirection = Player->GetControlRotation().Vector() * -1.f;
		}
		
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

	GetWorld()->GetTimerManager().SetTimer(Player->DashingTimerHandle, [Player]()
	{
		if (Player->GetCharacterMovement()->IsMovingOnGround())
		{
			Player->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
		else
		{
			Player->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
			if (Player->JumpsLeft > 0)
			{
				Player->JumpsLeft--;
			}
			
		}
		
		const FVector CurrentVelocity = Player->GetCharacterMovement()->Velocity;
		FVector ResetVelocity = CurrentVelocity.GetSafeNormal() * Player->GetCharacterMovement()->MaxWalkSpeed;
		Player->GetCharacterMovement()->Velocity = ResetVelocity;
		Player->GetCharacterMovement()->GroundFriction = Player->DefaultGroundFriction;
		Player->ChangeState(Player->RunningState);
	},DashImpulseDuration, false);
	
}

void USuraPlayerDashingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);
	
	float NewCapsuleHeight = FMath::FInterpTo(Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
	Player->GetDefaultCapsuleHalfHeight(), DeltaTime, 10.f);
	Player->GetCapsuleComponent()->SetCapsuleHalfHeight(NewCapsuleHeight);

	FVector CurrentCameraLocation = Player->GetCamera()->GetRelativeLocation();
	float NewCameraZ = FMath::FInterpTo(Player->GetCamera()->GetRelativeLocation().Z,
		Player->GetDefaultCameraLocation().Z, DeltaTime, 10.f);
	Player->GetCamera()->SetRelativeLocation(FVector(CurrentCameraLocation.X, CurrentCameraLocation.X, NewCameraZ));
}

void USuraPlayerDashingState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);
	Player->SetPreviousGroundedState(this);

}

void USuraPlayerDashingState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Look(Player, InputVector);
	Player->AddControllerYawInput(InputVector.X);
	Player->AddControllerPitchInput(InputVector.Y);
}

