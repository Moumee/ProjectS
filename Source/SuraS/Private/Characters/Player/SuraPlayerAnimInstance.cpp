// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "ActorComponents/ACPlayerMovmentData.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerBaseState.h"
#include "Characters/Player/SuraPlayerCrouchingState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USuraPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Player = Cast<ASuraCharacterPlayer>(TryGetPawnOwner());

	if (Player)
	{
		RunSpeed = Player->GetPlayerMovementData()->GetRunSpeed();
	}

}

void USuraPlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (Player)
	{
		Velocity = Player->GetCharacterMovement()->Velocity;
		
		GroundSpeed = UKismetMathLibrary::VSizeXY(Player->GetCharacterMovement()->Velocity);

		Direction = UKismetAnimationLibrary::CalculateDirection(Player->GetCharacterMovement()->Velocity,
			Player->GetActorRotation());

		Pitch = UKismetMathLibrary::NormalizeAxis(Player->GetControlRotation().Pitch);

		if (Player->GetCurrentState())
		{
			CurrentState = Player->GetCurrentState();
			CurrentStateType = Player->GetCurrentState()->GetStateType();
			bCrouchTriggered = Player->bCrouchTriggered;
		}
		
		if (Player->GetCharacterMovement()->MovementMode == MOVE_Flying)
		{
			bIsInAir = true;
		}
		else
		{
			if (Player->GetCharacterMovement()->IsFalling())
			{
				bIsInAir = true;
			}
			else
			{
				bIsInAir = false;
			}
		}
		
	}
}
