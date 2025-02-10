// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerMantlingState.h"

#include "Characters/Player/SuraCharacterPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

USuraPlayerMantlingState::USuraPlayerMantlingState()
{
	StateDisplayName = "Mantling";
	StateType = EPlayerState::Mantling;
}


void USuraPlayerMantlingState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);
	Player->GetCharacterMovement()->StopMovementImmediately();

	PlayerController = Player->GetController<APlayerController>();

	Player->JumpsLeft = Player->MaxJumps;
	bShouldMantle = true;
	StartLocation = Player->GetActorLocation();
	TargetLocation = Player->LedgeHitResult.ImpactPoint + Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * FVector::UpVector;

	if (Player->LedgeHitResult.ImpactPoint.Z < Player->GetActorLocation().Z)
	{
		MantleDuration = 0.1f;
	}
	else
	{
		MantleDuration = 0.2f;
	}
	
	
	
}

void USuraPlayerMantlingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);

	if (Player->MantleCamShake && PlayerController)
	{
		PlayerController->ClientStartCameraShake(Player->MantleCamShake);
	}
	

	Player->InterpCapsuleHeight(1.f, DeltaTime);

	if (bShouldMantle)
	{
		if (ElapsedTime < MantleDuration)
		{
			float t = ElapsedTime / MantleDuration;
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
