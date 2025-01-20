// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerMantlingState.h"

#include "Characters/Player/SuraCharacterPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

USuraPlayerMantlingState::USuraPlayerMantlingState()
{
	bShouldMantleUp = false;
	bShouldMantleForward = false;
	StateDisplayName = "Mantling";
}

void USuraPlayerMantlingState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);
	Player->GetCharacterMovement()->StopMovementImmediately();
	Player->GetCharacterMovement()->GravityScale = 0.f;
	bShouldMantleUp = true;
	bShouldMantleForward = false;
	UpMovementElapsedTime = 0;
	ForwardMovementElapsedTime = 0;
	StartLocation = Player->GetActorLocation();
	TargetLocation = Player->MantleHitResult.Location + Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * FVector::UpVector;
	UpMantleTarget = FVector(StartLocation.X, StartLocation.Y, TargetLocation.Z);
	
}

void USuraPlayerMantlingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);
	if (bShouldMantleUp)
	{
		if (UpMovementElapsedTime < 0.15f)
		{
			float t = UpMovementElapsedTime / 0.15f;
			UpMovementElapsedTime += DeltaTime;
			FVector NewLocation = FMath::Lerp(StartLocation, UpMantleTarget,
				t);
			Player->SetActorLocation(NewLocation);
		}
		else
		{
			bShouldMantleUp = false;
			bShouldMantleForward = true;
			Player->SetActorLocation(UpMantleTarget);
			
		}
	}

	if (bShouldMantleForward)
	{
		if (ForwardMovementElapsedTime < 0.1f)
		{
			float t = ForwardMovementElapsedTime / 0.1f;
			ForwardMovementElapsedTime += DeltaTime;
			FVector NewLocation = FMath::Lerp(UpMantleTarget, TargetLocation, t);
			Player->SetActorLocation(NewLocation);
		}
		else
		{
			bShouldMantleForward = false;
			Player->SetActorLocation(TargetLocation);
			Player->ChangeState(Player->DesiredGroundState);
		}
	}
}

void USuraPlayerMantlingState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);
	Player->GetCharacterMovement()->GravityScale = 1.f;
	bShouldMantleUp = false;
	bShouldMantleForward = false;
	ForwardMovementElapsedTime = 0;
	UpMovementElapsedTime = 0;
}
