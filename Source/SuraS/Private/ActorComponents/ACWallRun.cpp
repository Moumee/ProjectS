// // Fill out your copyright notice in the Description page of Project Settings.
//
//
// #include "ActorComponents/ACWallRun.h"
//
// #include "ActorComponents/ACPlayerMovmentData.h"
// #include "Characters/Player/SuraCharacterPlayer.h"
// #include "Characters/Player/SuraPlayerBaseState.h"
// #include "Components/CapsuleComponent.h"
// #include "GameFramework/CharacterMovementComponent.h"
//
// // Sets default values for this component's properties
// UACWallRun::UACWallRun()
// {
// 	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
// 	// off to improve performance if you don't need them.
// 	PrimaryComponentTick.bCanEverTick = true;
//
// 	// ...
// }
//
//
// // Called when the game starts
// void UACWallRun::BeginPlay()
// {
// 	Super::BeginPlay();
//
// 	Player = Cast<ASuraCharacterPlayer>(GetOwner());
// 	Player->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &UACWallRun::OnCapsuleHit);
// 	
// 	
// }
//
// bool UACWallRun::IsRunnableWall(const FVector& WallNormal) const
// {
// 	if (WallNormal.Z < -0.5f) return false;
//
// 	float WallAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(WallNormal, FVector::UpVector)));
// 	return WallAngle > Player->GetCharacterMovement()->GetWalkableFloorAngle();
// }
//
//
// // Called every frame
// void UACWallRun::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
// 	// ...
// }
//
//
//
// void UACWallRun::StartWallRunning()
// {
// 	Player->GetCharacterMovement()->AirControl = 1.f;
// 	Player->GetCharacterMovement()->GravityScale = 0.f;
// 	Player->GetCharacterMovement()->SetPlaneConstraintNormal(FVector::UpVector);
// 	bShouldWallRun = true;
// 	// Player->ResetJump();
// 	// StartCameraTilt();
// }
//
// void UACWallRun::StopWallRunning()
// {
// 	Player->GetCharacterMovement()->GravityScale = 1.f;
// 	Player->GetCharacterMovement()->AirControl = Player->GetPlayerMovementData()->GetAirControl();
// 	Player->GetCharacterMovement()->SetPlaneConstraintNormal(FVector::ZeroVector);
// 	bShouldWallRun = false;
// 	// StopCameraTilt();
// }
//
// void UACWallRun::OnCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
//                               FVector NormalImpulse, const FHitResult& Hit)
// {
// 	if (!bShouldWallRun)
// 	{
// 		if (IsRunnableWall(Hit.ImpactNormal) && Player->GetCharacterMovement()->IsFalling() &&
// 			(Player->GetCurrentState()->GetStateType() == EPlayerState::Jumping || Player->GetCurrentState()->GetStateType() == EPlayerState::Falling))
// 		{
// 			FVector OutWallRunDirection;
// 			EWallSide OutWallRunSide;
// 			FindWallRunDirectionAndSide(Hit.ImpactNormal, OutWallRunDirection, OutWallRunSide);
// 			WallRunDirection = OutWallRunDirection;
// 			WallRunSide = OutWallRunSide;
// 			if (Player->ForwardAxisInputValue > 0.f)
// 			{
// 				bShouldWallRun = true;
// 			}
// 		}
// 	}
// }
//
// FVector UACWallRun::FindJumpLaunchVelocity()
// {
// 	FVector LaunchDirection = FVector::ZeroVector;
// 	if (bShouldWallRun)
// 	{
// 		if (WallRunSide == EWallSide::Left)
// 		{
// 			LaunchDirection = FVector::CrossProduct(WallRunDirection, FVector::DownVector).GetSafeNormal();
// 		}
// 		else if (WallRunSide == EWallSide::Right)
// 		{
// 			LaunchDirection = FVector::CrossProduct(WallRunDirection, FVector::UpVector).GetSafeNormal();
// 		}
// 	}
// 	else
// 	{
// 		if (Player->GetCharacterMovement()->IsFalling())
// 		{
// 			LaunchDirection = (Player->GetActorForwardVector() * Player->ForwardAxisInputValue +
// 				Player->GetActorRightVector() * Player->RightAxisInputValue).GetSafeNormal();
// 		}
// 	}
// 	LaunchDirection *= 100.f;
// 	LaunchDirection += FVector::UpVector * 500.f;
// 	return LaunchDirection;
// }
//
// void UACWallRun::FindWallRunDirectionAndSide(const FVector& WallNormal, FVector& OutWallRunDirection, EWallSide& OutWallRunSide)
// {
// 	FVector WallNormalXY = FVector(WallNormal.X, WallNormal.Y, 0.f).GetSafeNormal();
// 	FVector RightVectorXY = FVector(Player->GetActorRightVector().X, Player->GetActorRightVector().Y, 0.f).GetSafeNormal();
// 	if (FVector::DotProduct(WallNormalXY, RightVectorXY) > 0.f)
// 	{
// 		OutWallRunDirection = FVector::CrossProduct(WallNormal, FVector::UpVector).GetSafeNormal();
// 		OutWallRunSide = EWallSide::Left;
// 	}
// 	else
// 	{
// 		OutWallRunDirection = FVector::CrossProduct(WallNormal, FVector::DownVector).GetSafeNormal();
// 		OutWallRunSide = EWallSide::Right;
// 	}
// 	
// }
//
// void UACWallRun::UpdateWallRun()
// {
// 	if (Player->ForwardAxisInputValue > 0.f)
// 	{
// 		FHitResult Hit;
// 		FCollisionQueryParams Params;
// 		Params.AddIgnoredActor(Player);
// 		FVector LineTraceEnd;
// 		if (WallRunSide == EWallSide::Left)
// 		{
// 			LineTraceEnd = Player->GetActorLocation() +
// 				FVector::CrossProduct(WallRunDirection, FVector::UpVector).GetSafeNormal() * 200.f;
// 		}
// 		else if (WallRunSide == EWallSide::Right)
// 		{
// 			LineTraceEnd = Player->GetActorLocation() +
// 				FVector::CrossProduct(WallRunDirection, FVector::DownVector).GetSafeNormal() * 200.f;
// 		}
// 		bool bHit = GetWorld()->LineTraceSingleByChannel(
// 			Hit,
// 			Player->GetActorLocation(),
// 			LineTraceEnd,
// 			ECC_Visibility,
// 			Params);
//
// 		if (bHit)
// 		{
// 			FVector OutWallRunDirection;
// 			EWallSide OutWallRunSide;
// 			FindWallRunDirectionAndSide(Hit.ImpactNormal, OutWallRunDirection, OutWallRunSide);
// 			if (OutWallRunSide == WallRunSide)
// 			{
// 				WallRunDirection = OutWallRunDirection;
// 				WallRunSide = OutWallRunSide;
// 				Player->GetCharacterMovement()->Velocity = FVector(WallRunDirection.X, WallRunDirection.Y, 0.f) *
// 					Player->GetCharacterMovement()->GetMaxSpeed();
// 			}
// 			else
// 			{
// 				StopWallRunning();
// 			}
// 		}
// 		else
// 		{
// 			StopWallRunning();
// 		}
// 	}
// 	else
// 	{
// 		StopWallRunning();
// 	}
// }
//
// bool UACWallRun::ShouldWallRun()
// {
// 	return bShouldWallRun;
// }
