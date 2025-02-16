// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerWallRunningState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerJumpingState.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


USuraPlayerWallRunningState::USuraPlayerWallRunningState()
{
	StateDisplayName = "Wall Running";
	StateType = EPlayerState::WallRunning;
}


void USuraPlayerWallRunningState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);

	PlayerController = Player->GetController<APlayerController>();

	MaxDuration = Player->GetPlayerMovementData()->GetWallRunningMaxDuration();
	
	
	ElapsedTime = 0.f;
	bShouldTilt = true;
	bShouldRotateCamera = false;
	bFrontWallFound = false;
	Player->GetCharacterMovement()->GravityScale = 0.f;
	Player->GetCharacterMovement()->AirControl = 1.f;
	Player->JumpsLeft = Player->MaxJumps;
	WallRunSide = Player->WallRunSide;
	// Need to make variable for minimum wall run velocity!! Currently set to WalkSpeed for testing purposes.
	StateEnterVelocity = FMath::Clamp(Player->GetCharacterMovement()->Velocity.Size(), Player->GetPlayerMovementData()->GetRunSpeed()
		, Player->GetPlayerMovementData()->GetMaxWallRunSpeed());
	TargetRoll = Player->WallRunSide == EWallSide::Left ? 15.f : -15.f;
}



void USuraPlayerWallRunningState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);

	if (Player->WallRunCamShake && PlayerController)
	{
		PlayerController->ClientStartCameraShake(Player->WallRunCamShake);
	}

	if (ElapsedTime < MaxDuration)
	{
		ElapsedTime += DeltaTime;
	}
	else
	{
		Player->ChangeState(Player->FallingState);
		return;
	}

	Player->InterpCapsuleHeight(1.f, DeltaTime);

	SetPlayerWallOffsetLocation(Player, DeltaTime);

	if (Player->ForwardAxisInputValue <= 0.f)
	{
		Player->ChangeState(Player->FallingState);
		return;
	}

	if (bShouldTilt)
	{
		FRotator CurrentControlRotation = Player->GetControlRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentControlRotation,
			FRotator(CurrentControlRotation.Pitch, CurrentControlRotation.Yaw, TargetRoll), DeltaTime, 10.f);
		Player->GetController()->SetControlRotation(NewRotation);
		if (FMath::Abs(Player->GetControlRotation().Roll - TargetRoll) < 0.1f)
		{
			FRotator CurrentRotation = Player->GetControlRotation();
			Player->GetController()->SetControlRotation(FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw, TargetRoll));
			bShouldTilt = false;
		}
	}

	
	if (Player->bJumpTriggered)
	{
		Player->ChangeState(Player->JumpingState);
		return;
	}

	float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Player->WallRunDirection, Player->GetActorForwardVector())));
	if (Angle > 120.f)
	{
		Player->ChangeState(Player->FallingState);
		return;
	}

	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Player);

	if (!bFrontWallFound)
	{
		bool bFrontWallHit = GetWorld()->LineTraceSingleByChannel(
		FrontWallHit,
		Player->GetActorLocation(),
		Player->GetActorLocation() + Player->WallRunDirection * 75.f,
		ECC_Visibility,
		Params);

		if (bFrontWallHit)
		{
			if (FrontWallHit.ImpactNormal.Z > -0.05f && FrontWallHit.ImpactNormal.Z < Player->GetCharacterMovement()->GetWalkableFloorZ())
			{
				bFrontWallFound = true;
			}
		}
	}

	if (bFrontWallFound && FrontWallHit.IsValidBlockingHit())
	{
		
		if (WallRunSide == EWallSide::Left)
		{
			Player->WallRunDirection = FVector::CrossProduct(FrontWallHit.ImpactNormal, FVector::UpVector).GetSafeNormal();
			float ForwardAndWallAngle = FMath::RadiansToDegrees(
				FMath::Acos(FVector::DotProduct(Player->WallRunDirection, Player->GetActorForwardVector())));
			
			if (FVector::CrossProduct(Player->WallRunDirection, Player->GetActorForwardVector()).Z < 0.f &&
				ForwardAndWallAngle > 60.f)
			{
				Player->ChangeState(Player->FallingState);
				return;
			}
		}
		else if (WallRunSide == EWallSide::Right)
		{
			Player->WallRunDirection = FVector::CrossProduct(FrontWallHit.ImpactNormal, FVector::DownVector).GetSafeNormal();
			float ForwardAndWallAngle = FMath::RadiansToDegrees(
				FMath::Acos(FVector::DotProduct(Player->WallRunDirection, Player->GetActorForwardVector())));
			if (FVector::CrossProduct(Player->WallRunDirection, Player->GetActorForwardVector()).Z > 0.f &&
				ForwardAndWallAngle > 60.f)
			{
				Player->ChangeState(Player->FallingState);
				return;
			}
		}
		bFrontWallFound = false;
	}
	
	FVector LineTraceEnd = FVector::ZeroVector;
	if (Player->WallRunSide == EWallSide::Left)
	{
		LineTraceEnd = Player->GetActorLocation() +
			FVector::CrossProduct(Player->WallRunDirection, FVector::UpVector).GetSafeNormal() * 200.f;
	}
	else if (Player->WallRunSide == EWallSide::Right)
	{
		LineTraceEnd = Player->GetActorLocation() +
			FVector::CrossProduct(Player->WallRunDirection, FVector::DownVector).GetSafeNormal() * 200.f;
	}

	bool bWallHit = GetWorld()->LineTraceSingleByChannel(
		WallHit,
		Player->GetActorLocation(),
		LineTraceEnd,
		ECC_Visibility,
		Params);

	if (bWallHit && WallHit.IsValidBlockingHit())
	{
		FVector WallNormalXY = FVector(WallHit.ImpactNormal.X, WallHit.ImpactNormal.Y, 0.f);
		
		FVector NewWallRunDirection = FVector::ZeroVector;
		if (Player->WallRunSide == EWallSide::Left)
		{
			NewWallRunDirection = FVector::CrossProduct(WallNormalXY, FVector::UpVector).GetSafeNormal();
		}
		else if (Player->WallRunSide == EWallSide::Right)
		{
			NewWallRunDirection = FVector::CrossProduct(WallNormalXY, FVector::DownVector).GetSafeNormal();
		}
		Player->WallRunDirection = NewWallRunDirection;
		FVector NewVelocity = FVector(NewWallRunDirection.X, NewWallRunDirection.Y, 0.f) * StateEnterVelocity;
		if (ElapsedTime < 0.75f)
		{
			NewVelocity += FVector::UpVector * 75.f;
		}
		Player->GetCharacterMovement()->Velocity = NewVelocity;
	}
	else
	{
		Player->ChangeState(Player->FallingState);
		return;
	}

	// if (bShouldRotateCamera)
	// {
	// 	FRotator CurrentRotation = Player->GetControlRotation();
	// 	float TargetYaw = Player->WallRunDirection.Rotation().Yaw;
	// 	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, FRotator(CurrentRotation.Pitch, TargetYaw, CurrentRotation.Roll),
	// 		DeltaTime, 20.f);
	// 	Player->GetController()->SetControlRotation(NewRotation);
	//
	// 	if (FMath::IsNearlyEqual(Player->GetActorRotation().Yaw, TargetYaw, 1.f))
	// 	{
	// 		Player->GetController()->SetControlRotation(FRotator(CurrentRotation.Pitch, TargetYaw, CurrentRotation.Roll));
	// 		bShouldRotateCamera = false;
	// 	}
	// }

	
	
	
}

void USuraPlayerWallRunningState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);
	Player->GetCharacterMovement()->GravityScale = Player->DefaultGravityScale;
	Player->GetCharacterMovement()->AirControl = Player->GetPlayerMovementData()->GetAirControl();
	Player->WallRunSide = EWallSide::None;
	ElapsedTime = 0.f;
	bFrontWallFound = false;
	bShouldRotateCamera = false;
	Player->bShouldRestoreCameraTilt = true;
}

void USuraPlayerWallRunningState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Look(Player, InputVector);
	Player->AddControllerYawInput(InputVector.X);
	Player->AddControllerPitchInput(InputVector.Y);
}

void USuraPlayerWallRunningState::Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Move(Player, InputVector);
}

void USuraPlayerWallRunningState::StartJumping(ASuraCharacterPlayer* Player)
{
	Super::StartJumping(Player);
	
	if (Player->JumpsLeft > 0)
	{
		FVector LaunchDirectionXY = FVector::ZeroVector;
		Player->JumpsLeft--;
		if (Player->WallRunSide == EWallSide::Left)
		{
			LaunchDirectionXY = FVector::CrossProduct(Player->WallRunDirection, FVector::DownVector).GetSafeNormal();
		}
		else if (Player->WallRunSide == EWallSide::Right)
		{
			LaunchDirectionXY = FVector::CrossProduct(Player->WallRunDirection, FVector::UpVector).GetSafeNormal();
		}
		FVector LaunchVector = LaunchDirectionXY * 500.f +
			FVector::UpVector * Player->GetPlayerMovementData()->GetPrimaryJumpZSpeed();
		Player->LaunchCharacter(LaunchVector, false, true);
	}
}

void USuraPlayerWallRunningState::SetPlayerWallOffsetLocation(ASuraCharacterPlayer* Player, float DeltaTime)
{
	if (WallHit.ImpactNormal != FVector::ZeroVector)
	{
		FVector NewLocation = FMath::VInterpTo(Player->GetActorLocation(),
											   Player->GetActorLocation() + WallHit.ImpactNormal * 30.f, DeltaTime, 15.f);
	}
}