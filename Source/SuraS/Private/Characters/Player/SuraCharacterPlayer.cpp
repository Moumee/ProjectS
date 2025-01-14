// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraCharacterPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ActorComponents/ACPlayerMovmentData.h"
#include "ActorComponents/WallRun/ACPlayerWallRun.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/SuraPlayerBaseState.h"
#include "Characters/Player/SuraPlayerDashImpulseState.h"
#include "Characters/Player/SuraPlayerDashMovementState.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerJumpingState.h"
#include "Characters/Player/SuraPlayerRunningState.h"
#include "Characters/Player/SuraPlayerWalkingState.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


ASuraCharacterPlayer::ASuraCharacterPlayer()
{
	// Wall-run component
	WallRunComponent = CreateDefaultSubobject<UACPlayerWallRun>(TEXT("WallRunComponent"));
	AddOwnedComponent(WallRunComponent);

	PlayerMovementData = CreateDefaultSubobject<UACPlayerMovementData>("Player Attributes");

	// Make character rotate with the controller
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Create and initialize first person camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());
	Camera->SetRelativeLocation(FVector(0.f, 0.f, 58.f));
	Camera->bUsePawnControlRotation = true;

	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm Mesh"));
	ArmMesh->SetupAttachment(Camera);

	LegMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Leg Mesh"));
	LegMesh->SetupAttachment(GetRootComponent());


	// Enable capsule hit events for wall detection
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

	// Initialize JumpsLeft
	JumpsLeft = MaxJumps;

}

void ASuraCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	DefaultGroundFriction = GetCharacterMovement()->GroundFriction;
	DefaultBrakingFrictionFactor = GetCharacterMovement()->BrakingFrictionFactor;
	DefaultBrakingDecelerationWalking = GetCharacterMovement()->BrakingDecelerationWalking;

	BaseMovementSpeed = GetPlayerMovementData()->GetWalkSpeed();
	GetCharacterMovement()->AirControl = GetPlayerMovementData()->GetAirControl();

	WalkingState = NewObject<USuraPlayerWalkingState>(this, USuraPlayerWalkingState::StaticClass());
	RunningState = NewObject<USuraPlayerRunningState>(this, USuraPlayerRunningState::StaticClass());
	JumpingState = NewObject<USuraPlayerJumpingState>(this, USuraPlayerJumpingState::StaticClass());
	FallingState = NewObject<USuraPlayerFallingState>(this, USuraPlayerFallingState::StaticClass());
	DashImpulseState = NewObject<USuraPlayerDashImpulseState>(this, USuraPlayerDashImpulseState::StaticClass());
	DashMovementState = NewObject<USuraPlayerDashMovementState>(this, USuraPlayerDashMovementState::StaticClass());

	ChangeState(WalkingState);

}

void ASuraCharacterPlayer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

}

void ASuraCharacterPlayer::ResetTriggeredBooleans()
{
	bJumpTriggered = false;
	bRunTriggered = false;
	bDashTriggered = false;
	bCrouchTriggered = false;
	bLandedTriggered = false;
}

void ASuraCharacterPlayer::SetBaseMovementSpeed(const float MovementSpeed)
{
	BaseMovementSpeed = MovementSpeed;
}

void ASuraCharacterPlayer::ChangeState(USuraPlayerBaseState* NewState)
{
	if (!NewState || NewState == CurrentState) return;

	if (CurrentState)
	{
		CurrentState->ExitState(this);
	}
	CurrentState = NewState;
	CurrentState->EnterState(this);
}

bool ASuraCharacterPlayer::IsFallingDown() const
{
	
	return GetCharacterMovement()->IsFalling() && GetCharacterMovement()->Velocity.Z < 0.f;
}

bool ASuraCharacterPlayer::HasMovementInput() const
{
	return ForwardAxisInputValue != 0.f || RightAxisInputValue != 0.f;
}

void ASuraCharacterPlayer::PrimaryJump()
{
	if (JumpsLeft > 0)
	{
		bJumpTriggered = true;
		JumpsLeft--;
		FVector JumpVector = FVector::UpVector * GetPlayerMovementData()->GetJumpZVelocity();
		LaunchCharacter(JumpVector, false, true);
	}
}

void ASuraCharacterPlayer::DoubleJump()
{
	if (JumpsLeft > 0 && GetCharacterMovement()->IsFalling())
	{
		bJumpTriggered = true;
		JumpsLeft--;
		FVector InputDirection = GetActorForwardVector() * ForwardAxisInputValue + GetActorRightVector() * RightAxisInputValue;
		FVector LaunchVelocity = InputDirection * GetPlayerMovementData()->GetJumpXYVelocity() +
			FVector::UpVector * GetPlayerMovementData()->GetJumpZVelocity();
		LaunchCharacter(LaunchVelocity, false, true);
	}
}


void ASuraCharacterPlayer::PrintPlayerDebugInfo() const
{
	if (bIsDebugMode)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(106, 0.f, FColor::Green,
											 FString::Printf(TEXT("Current Player Speed : %f"), GetCharacterMovement()->Velocity.Size()));

			GEngine->AddOnScreenDebugMessage(105, 0.f, FColor::Green,
				FString::Printf(TEXT("Player Max Walk Speed : %f"), GetCharacterMovement()->MaxWalkSpeed));
			
			GEngine->AddOnScreenDebugMessage(104, 0.f, FColor::Green,
				FString::Printf(TEXT("Player Additional Speed : %f"), AdditionalMovementSpeed));

			GEngine->AddOnScreenDebugMessage(103, 0.f, FColor::Green,
				FString::Printf(TEXT("Player Base Speed : %f"), BaseMovementSpeed));

			GEngine->AddOnScreenDebugMessage(102, 0.f, FColor::Green,
				FString::Printf(TEXT("Jumps Left : %d / %d"), JumpsLeft, MaxJumps));

			GEngine->AddOnScreenDebugMessage(99, 0.f, FColor::Green,
				FString::Printf(TEXT("Input Axis Value : ( %f, %f )"), ForwardAxisInputValue, RightAxisInputValue));

			GEngine->AddOnScreenDebugMessage(98, 0.f, FColor::Green,
				FString::Printf(TEXT("Slope Speed Delta : %f"), SlopeSpeedDelta));

			GEngine->AddOnScreenDebugMessage(97, 0.f, FColor::Green,
				FString::Printf(TEXT("Current State : %s"), *CurrentState->StateDisplayName.ToString()));

			
		}
	}
}

float ASuraCharacterPlayer::FindFloorAngle() const
{
	if (!GetCharacterMovement()->IsFalling())
	{
		const FVector FloorNormal = GetCharacterMovement()->CurrentFloor.HitResult.Normal;
		const float FloorAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(FloorNormal, FVector::UpVector)));
		const FVector DirectionXY = FVector(GetVelocity().X, GetVelocity().Y, 0.f).GetSafeNormal();
		return FVector::DotProduct(FloorNormal, DirectionXY) > 0.f ? -FloorAngle : FloorAngle;
	}
	return 0.f;
	
}

void ASuraCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PrintPlayerDebugInfo();

	if (CurrentState)
	{
		CurrentState->UpdateState(this, DeltaTime);
	}

	SlopeSpeedDelta = SlopeSpeedDeltaCurve->GetFloatValue(FindFloorAngle());
	
	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed + AdditionalMovementSpeed + SlopeSpeedDelta;
}

void ASuraCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASuraCharacterPlayer::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ASuraCharacterPlayer::StopMoving);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASuraCharacterPlayer::Look);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &ASuraCharacterPlayer::StartRunning);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASuraCharacterPlayer::StartJumping);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ASuraCharacterPlayer::StartDashing);
	}
}

void ASuraCharacterPlayer::Move(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();
	
	ForwardAxisInputValue = InputVector.Y;
	RightAxisInputValue = InputVector.X;

	CurrentState->Move(this, InputVector);

}

void ASuraCharacterPlayer::StopMoving()
{
	ForwardAxisInputValue = 0.f;
	RightAxisInputValue = 0.f;

	CurrentState->StopMoving(this);
	
}

void ASuraCharacterPlayer::Look(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	CurrentState->Look(this, InputVector);
}

void ASuraCharacterPlayer::StartRunning()
{
	bRunTriggered = true;
	CurrentState->StartRunning(this);
}

void ASuraCharacterPlayer::StartJumping()
{
	bJumpTriggered = true;
	CurrentState->StartJumping(this);
}

void ASuraCharacterPlayer::StartDashing()
{
	if (bDashOnCooldown) return;
	bDashTriggered = true;
	CurrentState->StartDashing(this);
	
}

void ASuraCharacterPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	bLandedTriggered = true;
	CurrentState->Landed(this, Hit);
}



