// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ACAttributes.h"
#include "GameAttributes/PlayerMovementData.h"

// Sets default values for this component's properties
UACAttributes::UACAttributes()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UACAttributes::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UACAttributes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UACAttributes::InitializePlayer(ACharacter* Player)
{
	const auto PlayerMovementData = PlayerMovementDT.DataTable->FindRow<FPlayerMovementData>("Player", "");

	if (PlayerMovementData)
	{
		/*
		* Sample Initialization Code
		* 
		// Player Movement Init
		PlayerWalkSpeed = (*PlayerMovementData).WalkSpeed;
		PlayerSprintSpeed = (*PlayerMovementData).SprintSpeed;
		PlayerHoldToSprintTime = (*PlayerMovementData).HoldToSprintTime;
		PlayerJumpVelocity = (*PlayerMovementData).JumpVelocity;
		PlayerCrouchedWalkSpeed = (*PlayerMovementData).CrouchedWalkSpeed;
		PlayerCrouchSpeed = (*PlayerMovementData).CrouchSpeed;
		PlayerSlidingThreshold = (*PlayerMovementData).SlidingThreshold;
		PlayerDashDistance = (*PlayerMovementData).DashDistance;
		PlayerDashCooldown = (*PlayerMovementData).DashCooldown;

		// WallRun Component Init
		GetWallRunComponent()->WallRunSpeed = (*PlayerMovementData).WallRunSpeed;
		GetWallRunComponent()->bIsWallRunGravity = (*PlayerMovementData).bIsWallRunGravity;
		GetWallRunComponent()->TargetWallRunGravity = (*PlayerMovementData).TargetWallRunGravity;
		GetWallRunComponent()->WallRunCooldown = (*PlayerMovementData).WallRunCooldown;
		GetWallRunComponent()->WallRunJumpOffVelocity = (*PlayerMovementData).WallRunJumpOffVelocity;
		*/
	}
}

