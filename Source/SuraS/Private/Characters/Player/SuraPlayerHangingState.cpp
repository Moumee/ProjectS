// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerHangingState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerJumpingState.h"
#include "Characters/Player/SuraPlayerMantlingState.h"
#include "GameFramework/CharacterMovementComponent.h"


USuraPlayerHangingState::USuraPlayerHangingState()
{
	StartPosition = FVector::ZeroVector;
	ElapsedTime = 0.f;
	StateDisplayName = "Hanging";
	StateType = EPlayerState::Hanging;
	bShouldMoveToHangPosition = false;
}

void USuraPlayerHangingState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);

	StartPosition = Player->GetActorLocation();
	Player->JumpsLeft = Player->MaxJumps;
	Player->GetCharacterMovement()->StopMovementImmediately();
	ElapsedTime = 0.f;
	bShouldMoveToHangPosition = true;
	Player->GetCharacterMovement()->GravityScale = 0.f;
	HangPosition = FVector(Player->WallHitResult.ImpactPoint.X, Player->WallHitResult.ImpactPoint.Y, Player->LedgeHitResult.ImpactPoint.Z - 30.f) +
		Player->WallHitResult.ImpactNormal * 60.f;
}

void USuraPlayerHangingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);
	
	Player->InterpPlayerRoll(0.f, DeltaTime, 7.f);
	
	// float NewCapsuleHeight = FMath::FInterpTo(Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
	// Player->GetDefaultCapsuleHalfHeight(), DeltaTime, 5.f);
	// Player->GetCapsuleComponent()->SetCapsuleHalfHeight(NewCapsuleHeight);
	//
	// FVector CurrentCameraLocation = Player->GetCamera()->GetRelativeLocation();
	// float NewCameraZ = FMath::FInterpTo(Player->GetCamera()->GetRelativeLocation().Z,
	// 	Player->GetDefaultCameraLocation().Z, DeltaTime, 5.f);
	// Player->GetCamera()->SetRelativeLocation(FVector(CurrentCameraLocation.X, CurrentCameraLocation.X, NewCameraZ));

	if (Player->ForwardAxisInputValue > 0.f)
	{
		Player->ChangeState(Player->MantlingState);
		return;
	}

	if (bShouldMoveToHangPosition)
	{
		if (ElapsedTime < 0.2f)
		{
			ElapsedTime += DeltaTime;
			FVector NewPosition = FMath::Lerp(StartPosition, HangPosition, ElapsedTime / 0.2f);
			Player->SetActorLocation(NewPosition);
		}
		else
		{
			bShouldMoveToHangPosition = false;
			Player->SetActorLocation(HangPosition);
			Player->GetCharacterMovement()->StopMovementImmediately();
		}
	}

	if (Player->bJumpTriggered)
	{
		Player->ChangeState(Player->JumpingState);
		return;
	}
	

	
}

void USuraPlayerHangingState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);
	Player->GetCharacterMovement()->GravityScale = 1.f;
	bShouldMoveToHangPosition = false;
	ElapsedTime = 0.f;
}

void USuraPlayerHangingState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Look(Player, InputVector);
	Player->AddControllerPitchInput(InputVector.Y);
	Player->AddControllerYawInput(InputVector.X);
	
	
}

void USuraPlayerHangingState::StartJumping(ASuraCharacterPlayer* Player)
{
	Super::StartJumping(Player);
	if (Player->JumpsLeft > 0)
	{
		Player->JumpsLeft -= 1;
		FVector JumpXY = FVector(Player->WallHitResult.ImpactNormal.X, Player->WallHitResult.ImpactNormal.Y, 0.f);
		FVector JumpVector = JumpXY * 500.f + FVector::UpVector *
			Player->GetPlayerMovementData()->GetJumpZVelocity() * 0.8f;
		Player->LaunchCharacter(JumpVector, true, true);
	}
}
