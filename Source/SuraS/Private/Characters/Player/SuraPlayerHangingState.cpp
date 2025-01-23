// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerHangingState.h"

#include "Camera/CameraComponent.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerMantlingState.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


USuraPlayerHangingState::USuraPlayerHangingState()
{
	ElapsedTime = 0.f;
	StateDisplayName = "Hanging";
	StateType = EPlayerState::Hanging;
	bShouldMoveToHangPosition = false;
}

void USuraPlayerHangingState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);

	ForwardPressElapsedTime = 0.f;
	StartPosition = Player->GetActorLocation();
	Player->GetCharacterMovement()->StopMovementImmediately();
	ElapsedTime = 0.f;
	bShouldMoveToHangPosition = true;
	Player->GetCharacterMovement()->GravityScale = 0.f;
	HangPosition = FVector(Player->WallHitResult.ImpactPoint.X, Player->WallHitResult.ImpactPoint.Y, Player->MantleHitResult.ImpactPoint.Z - 30.f) +
		Player->WallHitResult.ImpactNormal * 60.f;
}

void USuraPlayerHangingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);
	
	// float NewCapsuleHeight = FMath::FInterpTo(Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
	// Player->GetDefaultCapsuleHalfHeight(), DeltaTime, 5.f);
	// Player->GetCapsuleComponent()->SetCapsuleHalfHeight(NewCapsuleHeight);
	//
	// FVector CurrentCameraLocation = Player->GetCamera()->GetRelativeLocation();
	// float NewCameraZ = FMath::FInterpTo(Player->GetCamera()->GetRelativeLocation().Z,
	// 	Player->GetDefaultCameraLocation().Z, DeltaTime, 5.f);
	// Player->GetCamera()->SetRelativeLocation(FVector(CurrentCameraLocation.X, CurrentCameraLocation.X, NewCameraZ));

	

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

	if (!bShouldMoveToHangPosition || ElapsedTime > 0.1f)
	{
		if (Player->ForwardAxisInputValue > 0.f)
		{
			ForwardPressElapsedTime += DeltaTime;
		}
		else
		{
			ForwardPressElapsedTime = 0.f;
		}
		
		if (ForwardPressElapsedTime > 0.2f)
		{
			Player->ChangeState(Player->MantlingState);
			return;
		}
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
