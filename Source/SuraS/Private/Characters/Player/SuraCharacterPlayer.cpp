// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraCharacterPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ActorComponents/ACPlayerMovmentData.h"
#include "ActorComponents/WallRun/ACPlayerWallRun.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


ASuraCharacterPlayer::ASuraCharacterPlayer()
{
	// Wall-run component
	WallRunComponent = CreateDefaultSubobject<UACPlayerWallRun>(TEXT("WallRunComponent"));
	AddOwnedComponent(WallRunComponent);

	PlayerMovementData = CreateDefaultSubobject<UACPlayerMovementData>("Player Attributes");

	// Explicitly initialize the starting states
	CurrentMovementState = EMovementState::Walking;
	CurrentActionState = EActionState::None;

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

	// Bind Functions to State Changed Delegate
	OnMovementStateChanged.AddDynamic(this, &ASuraCharacterPlayer::SetBaseMovementSpeed);

	BaseMovementSpeed = GetPlayerMovementData()->GetWalkSpeed();
	GetCharacterMovement()->AirControl = GetPlayerMovementData()->GetAirControl();
}

void ASuraCharacterPlayer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

}

void ASuraCharacterPlayer::SetMovementState(const EMovementState NewMovementState)
{
	if (CurrentMovementState != NewMovementState)
	{
		CurrentMovementState = NewMovementState;

		OnMovementStateChanged.Broadcast(NewMovementState);
	}
}

void ASuraCharacterPlayer::SetActionState(const EActionState NewActionState)
{
	if (CurrentActionState != NewActionState)
	{
		CurrentActionState = NewActionState;

		OnActionStateChanged.Broadcast(NewActionState);
	}
}

void ASuraCharacterPlayer::SetBaseMovementSpeed(EMovementState NewMovementState)
{
	if (!GetPlayerMovementData()) return;
	
	switch (NewMovementState)
	{
	case EMovementState::Walking:
		BaseMovementSpeed = GetPlayerMovementData()->GetWalkSpeed();
		break;
	case EMovementState::Running:
		BaseMovementSpeed = GetPlayerMovementData()->GetRunSpeed();
		break;
	case EMovementState::Crouching:
		BaseMovementSpeed = GetPlayerMovementData()->GetCrouchSpeed();
		break;
	case EMovementState::Dashing:
		BaseMovementSpeed = GetPlayerMovementData()->GetDashSpeed();
		break;
	default:
		BaseMovementSpeed = GetPlayerMovementData()->GetWalkSpeed();
		break;
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

			GEngine->AddOnScreenDebugMessage(101, 0.0f, FColor::Green,
			                                 FString::Printf(TEXT("Player Action State : %s"), *UEnum::GetDisplayValueAsText(CurrentActionState).ToString()));

			GEngine->AddOnScreenDebugMessage(100, 0.0f, FColor::Green,
											 FString::Printf(TEXT("Player Movement State : %s"), *UEnum::GetDisplayValueAsText(CurrentMovementState).ToString()));

			GEngine->AddOnScreenDebugMessage(99, 0.f, FColor::Green,
				FString::Printf(TEXT("Input Axis Value : ( %f, %f )"), ForwardAxisInputValue, RightAxisInputValue));

			GEngine->AddOnScreenDebugMessage(98, 0.f, FColor::Green,
				FString::Printf(TEXT("Slope Speed Delta : %f"), SlopeSpeedDelta));

			
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
	FVector2D MovementVector = InputValue.Get<FVector2D>();
	ForwardAxisInputValue = MovementVector.Y;
	RightAxisInputValue = MovementVector.X;

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}

	if (CurrentMovementState == EMovementState::Running)
	{
		if (ForwardAxisInputValue <= 0.f)
		{
			SetMovementState(EMovementState::Walking);
		}
	}
}

void ASuraCharacterPlayer::StopMoving()
{
	ForwardAxisInputValue = 0.f;
	RightAxisInputValue = 0.f;
	if (CurrentMovementState == EMovementState::Running)
	{
		SetMovementState(EMovementState::Walking);
	}
	
}

void ASuraCharacterPlayer::Look(const FInputActionValue& InputValue)
{
	FVector2D LookAxisVector = InputValue.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASuraCharacterPlayer::StartRunning()
{
	SetMovementState(EMovementState::Running);
	
}

void ASuraCharacterPlayer::StartJumping()
{
	if (JumpsLeft > 0)
	{
		JumpsLeft--;
		LaunchCharacter(FindJumpLaunchVelocity(), false, true);
		if (CurrentActionState == EActionState::WallRunning)
		{
			// WallRunComponent->StopWallRunning();
		}
	}
}

void ASuraCharacterPlayer::StartDashing()
{
	if (bIsDashOnCooldown) return;
	
	if (CurrentActionState == EActionState::WallRunning || CurrentActionState == EActionState::Stunned ||
		CurrentActionState == EActionState::Dead)
	{
		return;
	}

	bIsDashOnCooldown = true;
	SetMovementState(EMovementState::Dashing);

	FVector DashDirection;
	if (ForwardAxisInputValue >= 0.f && RightAxisInputValue == 0.f)
	{
		DashDirection = GetActorForwardVector();
		DashDirection = FVector(DashDirection.X, DashDirection.Y, 0.f).GetSafeNormal();
	}
	else
	{
		DashDirection = (GetActorForwardVector() * ForwardAxisInputValue + GetActorRightVector() * RightAxisInputValue);
		DashDirection = FVector(DashDirection.X, DashDirection.Y, 0.f).GetSafeNormal();
	}

	float InitialGroundFriction = GetCharacterMovement()->GroundFriction;
	float InitialBrakingFrictionFactor = GetCharacterMovement()->BrakingFrictionFactor;
	float InitialBrakingDecelerationWalking = GetCharacterMovement()->BrakingDecelerationWalking;

	bBlockInput = true;
	GetCharacterMovement()->GroundFriction = 0.f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 0.f;
	float DashImpulseSpeed = GetPlayerMovementData()->GetDashImpulseSpeed();
	float DashImpulseDuration = GetPlayerMovementData()->GetDashDistance() / DashImpulseSpeed;
	GetCharacterMovement()->Velocity = DashDirection * DashImpulseSpeed;

	FTimerHandle StopImpulseTimer;
	GetWorldTimerManager().SetTimer(StopImpulseTimer,
		[this, InitialGroundFriction, InitialBrakingFrictionFactor, InitialBrakingDecelerationWalking]()
		{
			FVector CurrentImpulseVelocity = GetCharacterMovement()->Velocity;
			FVector ResetVelocity = CurrentImpulseVelocity.GetSafeNormal() * GetCharacterMovement()->MaxWalkSpeed;
			bBlockInput = false;
			GetCharacterMovement()->Velocity = ResetVelocity;
			GetCharacterMovement()->GroundFriction = InitialGroundFriction;
			GetCharacterMovement()->BrakingFrictionFactor = InitialBrakingFrictionFactor;
			GetCharacterMovement()->BrakingDecelerationWalking = InitialBrakingDecelerationWalking;
		},
		DashImpulseDuration, false);
	
	
	
		
	GetWorldTimerManager().SetTimer(DashDurationTimer, [this](){ SetMovementState(EMovementState::Running); },
		GetPlayerMovementData()->GetDashDuration(), false);

	GetWorldTimerManager().SetTimer(DashCooldownTimer, [this](){ bIsDashOnCooldown = false; },
		GetPlayerMovementData()->GetDashCooldown(), false);
	
	
}

void ASuraCharacterPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	JumpsLeft = MaxJumps;
}

FVector ASuraCharacterPlayer::FindJumpLaunchVelocity() const
{
	FVector LaunchDirection = FVector::ZeroVector;
	if (CurrentActionState == EActionState::WallRunning)
	{
		// TODO: Get Launch Direction from the wall run component
		
	}
	else
	{
		if (GetCharacterMovement()->IsFalling())
		{
			LaunchDirection = (GetActorForwardVector() * ForwardAxisInputValue + GetActorRightVector() * RightAxisInputValue).GetSafeNormal();
		}
	}
	const FVector LaunchVelocity = LaunchDirection * GetPlayerMovementData()->GetJumpXYVelocity() +
		FVector::UpVector * GetPlayerMovementData()->GetJumpZVelocity();
	return LaunchVelocity;
}


