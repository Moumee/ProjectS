// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/SuraCharacterPlayerWeapon.h"

//#include "EnhancedInputComponent.h"
//#include "EnhancedInputSubsystems.h"
//#include "ActorComponents/ACPlayerMovmentData.h"
//#include "ActorComponents/WallRun/ACPlayerWallRun.h"
//#include "Camera/CameraComponent.h"
//#include "Characters/Player/SuraPlayerBaseState.h"
//#include "Characters/Player/SuraPlayerDashImpulseState.h"
//#include "Characters/Player/SuraPlayerDashMovementState.h"
//#include "Characters/Player/SuraPlayerFallingState.h"
//#include "Characters/Player/SuraPlayerJumpingState.h"
//#include "Characters/Player/SuraPlayerRunningState.h"
//#include "Characters/Player/SuraPlayerWalkingState.h"
//#include "Components/CapsuleComponent.h"
//#include "GameFramework/CharacterMovementComponent.h"


ASuraCharacterPlayerWeapon::ASuraCharacterPlayerWeapon()
{

}

void ASuraCharacterPlayerWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASuraCharacterPlayerWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ASuraCharacterPlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASuraCharacterPlayerWeapon::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASuraCharacterPlayerWeapon::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
}
