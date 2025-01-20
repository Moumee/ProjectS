// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerFallingState.h"

#include "Camera/CameraComponent.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerDashingState.h"
#include "Characters/Player/SuraPlayerMantlingState.h"
#include "Characters/Player/SuraPlayerRunningState.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

USuraPlayerFallingState::USuraPlayerFallingState()
{
	StateDisplayName = "Falling";
}

void USuraPlayerFallingState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);
}

void USuraPlayerFallingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);

	// FHitResult WallHitResult;
	// FCollisionQueryParams WallParams;
	// WallParams.AddIgnoredActor(Player);
	//
	// const FVector WallDetectStart = Player->GetActorLocation();
	// const FVector WallDetectEnd = WallDetectStart + Player->GetActorForwardVector() * 50.f;
	// const bool bWallHit = GetWorld()->SweepSingleByChannel(WallHitResult, WallDetectStart, WallDetectEnd, FQuat::Identity,
	// 	ECC_GameTraceChannel1, FCollisionShape::MakeCapsule(Player->GetCapsuleComponent()->GetScaledCapsuleRadius(),
	// 		Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()), WallParams);
	//
	// if (Player->IsDebugMode())
	// {
	// 	DrawDebugSphere(GetWorld(), WallHitResult.Location, 20, 20, FColor::Red, false, 1.f);
	// }
	//
	// if (bWallHit && Player->GetCharacterMovement()->IsFalling() && Player->ForwardAxisInputValue > 0.f)
	// {
	// 	FHitResult LedgeHitResult;
	// 	FCollisionQueryParams LedgeParams;
	// 	LedgeParams.AddIgnoredActor(Player);
	// 	
	// 	FVector LedgeDetectStart = Player->GetCamera()->GetComponentLocation() + FVector::UpVector * 200.f;
	// 	FVector LedgeDetectEnd = FVector(LedgeDetectStart.X, LedgeDetectStart.Y, WallHitResult.ImpactPoint.Z);
	//
	// 	bool bLedgeHit = GetWorld()->SweepSingleByChannel(LedgeHitResult, LedgeDetectStart, LedgeDetectEnd, FQuat::Identity,
	// 		ECC_GameTraceChannel2, FCollisionShape::MakeSphere(20.f), LedgeParams);
	//
	// 	if (Player->IsDebugMode())
	// 	{
	// 		DrawDebugSphere(GetWorld(), LedgeHitResult.ImpactPoint, 20.f, 10, FColor::Green, false, 0.5f, 0, 1.f);
	// 	}
	//
	// 	if (bLedgeHit && Player->GetCharacterMovement()->IsWalkable(LedgeHitResult))
	// 	{
	// 		Player->MantleHitResult = LedgeHitResult;
	// 		Player->ChangeState(Player->MantlingState);
	// 		return;
	// 		// if (LedgeHitResult.ImpactPoint.Z < Player->GetActorLocation().Z)
	// 		// {
	// 		// 	
	// 		// 	
	// 		// }
	// 		// else
	// 		// {
	// 		// 	Player->ChangeState(Player->HangingState);
	// 		// 	return;
	// 		// }
	// 	}
	// 	
	// }
	

	if (Player->bDashTriggered)
	{
		Player->ChangeState(Player->DashingState);
		return;
	}

	if (Player->bRunTriggered)
	{
		Player->DesiredGroundState = Player->RunningState;
	}

	if (Player->bLandedTriggered)
	{
		Player->ChangeState(Player->DesiredGroundState);
		return;
	}

	
}

void USuraPlayerFallingState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);
}

void USuraPlayerFallingState::Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Move(Player, InputVector);

	Player->AddMovementInput(Player->GetActorForwardVector(), InputVector.Y);
	Player->AddMovementInput(Player->GetActorRightVector(), InputVector.X);
	
}

void USuraPlayerFallingState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Look(Player, InputVector);
	
	Player->AddControllerPitchInput(InputVector.Y);
	Player->AddControllerYawInput(InputVector.X);
}

void USuraPlayerFallingState::StartJumping(ASuraCharacterPlayer* Player)
{
	Super::StartJumping(Player);
	if (Player->JumpsLeft > 0 && Player->JumpsLeft != Player->MaxJumps)
	{
		Player->DoubleJump();
	}
}

void USuraPlayerFallingState::Landed(ASuraCharacterPlayer* Player, const FHitResult& HitResult)
{
	Super::Landed(Player, HitResult);
}
