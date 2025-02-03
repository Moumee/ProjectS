// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerBaseState.h"
#include "Characters/Player/SuraPlayerCrouchingState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USuraPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Player = Cast<ASuraCharacterPlayer>(TryGetPawnOwner());
	
}

void USuraPlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (Player)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(Player->GetCharacterMovement()->Velocity);

		Direction = UKismetAnimationLibrary::CalculateDirection(Player->GetCharacterMovement()->Velocity,
			Player->GetActorRotation());

		Pitch = UKismetMathLibrary::NormalizeAxis(Player->GetControlRotation().Pitch);

		if (Player->GetCurrentState())
		{
			CurrentState = Player->GetCurrentState();
			bIsCrouching = Player->GetCurrentState()->IsA(USuraPlayerCrouchingState::StaticClass());
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
