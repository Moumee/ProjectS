// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/SuraCharacterPlayerWeapon.h"
#include "ActorComponents/WeaponSystem/SuraWeaponPickUp.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h" // TODO: 파일 경로 수정

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
	//// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	//ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	//ArmMesh->SetOnlyOwnerSee(true);
	//ArmMesh->SetupAttachment(GetCapsuleComponent());
	//ArmMesh->bCastDynamicShadow = false;
	//ArmMesh->CastShadow = false;
	//ArmMesh->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);

	GetArmMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//WeaponSystem
	WeaponSystem = CreateDefaultSubobject<UWeaponSystemComponent>(TEXT("WeaponSystem"));
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

void ASuraCharacterPlayerWeapon::UpdateLookInputVector2D(const FInputActionValue& InputValue)
{
	PlayerLookInputVector2D = InputValue.Get<FVector2D>();

	//UE_LOG(LogTemp, Warning, TEXT("(Player Input Value) X: %f, Y: %f"), PlayerLookInputVector2D.X, PlayerLookInputVector2D.Y);
}

void ASuraCharacterPlayerWeapon::SetLookInputVector2DZero()
{
	PlayerLookInputVector2D = FVector2D::ZeroVector;
}

void ASuraCharacterPlayerWeapon::InterpCameraFOV(float DeltaTime)
{
	if (WeaponSystem)
	{
		if (WeaponSystem->IsWeaponModifyingCamFov())
		{
			return;
		}
	}
	Super::InterpCameraFOV(DeltaTime);
	//UE_LOG(LogTemp, Error, TEXT("Function Hiding!!!")); // 작동안함
}

void ASuraCharacterPlayerWeapon::StartCamShake(TSubclassOf<UCameraShakeBase> CamShake)
{
	if (WeaponSystem)
	{
		if (WeaponSystem->IsZoomIn())
		{
			return;
		}
	}
	//Super::StartCamShake(CamShake);
}

void ASuraCharacterPlayerWeapon::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASuraCharacterPlayerWeapon::OnOverlapBegin);
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
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASuraCharacterPlayerWeapon::UpdateLookInputVector2D);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::None, this, &ASuraCharacterPlayerWeapon::SetLookInputVector2DZero);
	}
}

void ASuraCharacterPlayerWeapon::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
}
