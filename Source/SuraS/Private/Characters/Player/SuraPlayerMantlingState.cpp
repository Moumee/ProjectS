// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerMantlingState.h"

#include "Characters/Player/SuraCharacterPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

USuraPlayerMantlingState::USuraPlayerMantlingState()
{
	bShouldMantle = false;
	StateDisplayName = "Mantling";
}

void USuraPlayerMantlingState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);
	Player->GetCharacterMovement()->StopMovementImmediately();
	bShouldMantle = true;
	StartLocation = Player->GetActorLocation();
	TargetLocation = Player->MantleHitResult.ImpactPoint + Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * FVector::UpVector;
	
	
}

void USuraPlayerMantlingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);
	if (bShouldMantle)
	{
		if (ElapsedTime < 0.2f)
		{
			float t = ElapsedTime / 0.2f;
			ElapsedTime += DeltaTime;
			FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation,
				t);
			Player->SetActorLocation(NewLocation);
		}
		else
		{
			bShouldMantle = false;
			Player->SetActorLocation(TargetLocation);
			Player->ChangeState(Player->DesiredGroundState);
			return;
		}
	}
	
}

void USuraPlayerMantlingState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);
	Player->GetCharacterMovement()->GravityScale = 1.f;
	bShouldMantle = false;
	ElapsedTime = 0;
}
