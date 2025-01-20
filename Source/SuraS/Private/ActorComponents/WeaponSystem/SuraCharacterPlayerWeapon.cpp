// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/SuraCharacterPlayerWeapon.h"
#include "ActorComponents/WeaponSystem/SuraWeaponPickUp.h"
#include "WeaponSystemComponent.h" // TODO: 파일 경로 수정

#include "EnhancedInputComponent.h"
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
#include "Components/CapsuleComponent.h"
//#include "GameFramework/CharacterMovementComponent.h"


ASuraCharacterPlayerWeapon::ASuraCharacterPlayerWeapon()
{

}

void ASuraCharacterPlayerWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapped!!!"));

	//ASuraWeaponPickUp* PickUpActor = Cast<ASuraWeaponPickUp>(OtherActor);
	//if (PickUpActor)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Overlapped!!!"));
	//}


}

void ASuraCharacterPlayerWeapon::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASuraCharacterPlayerWeapon::OnOverlapBegin);

	//WeaponSystem
	WeaponSystem = CreateDefaultSubobject<UWeaponSystemComponent>(TEXT("WeaponSystem"));
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

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ASuraCharacterPlayerWeapon::Move);
	}
}

void ASuraCharacterPlayerWeapon::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
}
