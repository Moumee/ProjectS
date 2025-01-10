// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraCharacterPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ActorComponents/ACPlayerAttributes.h"
#include "ActorComponents/WallRun/ACPlayerWallRun.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


ASuraCharacterPlayer::ASuraCharacterPlayer()
{
	// Wall-run component
	WallRunComponent = CreateDefaultSubobject<UACPlayerWallRun>(TEXT("WallRunComponent"));
	AddOwnedComponent(WallRunComponent);

	PlayerAttributes = CreateDefaultSubobject<UACPlayerAttributes>("Player Attributes");

	// Explicitly initialize the starting movement state
	CurrentMovementState = EMovementState::Walking;
	

	// Make character rotate with the controller
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Create and initialize first person camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());
	Camera->SetRelativeLocation(FVector(0.f, 0.f, 58.f));
	Camera->bUsePawnControlRotation = true;

	// Attach the mesh to the camera so that it moves along with it
	GetMesh()->SetupAttachment(Camera);

	// Enable capsule hit events for wall detection
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

	// Initialize JumpsLeft
	JumpsLeft = MaxJumps;

	GetCharacterMovement()->AirControl = 0.7f;


	
}

void ASuraCharacterPlayer::SetMovementState(const EMovementState NewMovementState)
{
	if (CurrentMovementState != NewMovementState)
	{
		CurrentMovementState = NewMovementState;

		OnMovementStateChanged.Broadcast(NewMovementState);
	}
}

void ASuraCharacterPlayer::SetBaseMovementSpeed(EMovementState NewMovementState)
{
	if (!GetPlayerAttributes()) return;
	
	switch (NewMovementState)
	{
	case EMovementState::Walking:
		BaseMovementSpeed = GetPlayerAttributes()->GetWalkSpeed();
		break;
	case EMovementState::Running:
		BaseMovementSpeed = GetPlayerAttributes()->GetRunSpeed();
		break;
	case EMovementState::Crouching:
		BaseMovementSpeed = GetPlayerAttributes()->GetCrouchSpeed();
		break;
	case EMovementState::Dashing:
		BaseMovementSpeed = GetPlayerAttributes()->GetDashSpeed();
		break;
	default:
		BaseMovementSpeed = GetPlayerAttributes()->GetWalkSpeed();
		break;
	}
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

	BaseMovementSpeed = GetPlayerAttributes()->GetWalkSpeed();
}

void ASuraCharacterPlayer::PrintPlayerDebugInfo() const
{
	if (IsDebugMode)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(0, 0.0f, FColor::Green,
			                                 FString::Printf(TEXT("Player Movement State : %s"), *UEnum::GetDisplayValueAsText(CurrentMovementState).ToString()));

			GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Green,
			                                 FString::Printf(TEXT("Player Action State : %s"), *UEnum::GetDisplayValueAsText(CurrentActionState).ToString()));

			GEngine->AddOnScreenDebugMessage(2, 0.f, FColor::Green,
				FString::Printf(TEXT("Jumps Left : %d / %d"), JumpsLeft, MaxJumps));
			
			

			GEngine->AddOnScreenDebugMessage(3, 0.f, FColor::Green,
				FString::Printf(TEXT("Player Base Speed : %f"), BaseMovementSpeed));

			GEngine->AddOnScreenDebugMessage(4, 0.f, FColor::Green,
				FString::Printf(TEXT("Player Additional Speed : %f"), AdditionalMovementSpeed));

			GEngine->AddOnScreenDebugMessage(5, 0.f, FColor::Green,
				FString::Printf(TEXT("Player Max Walk Speed : %f"), GetCharacterMovement()->MaxWalkSpeed));

			GEngine->AddOnScreenDebugMessage(6, 0.f, FColor::Green,
											 FString::Printf(TEXT("Current Player Speed : %f"), GetCharacterMovement()->Velocity.Size()));

			

			
		}
	}
}

float ASuraCharacterPlayer::FindFloorAngle() const
{
	const FVector FloorNormal = GetCharacterMovement()->CurrentFloor.HitResult.Normal;
	const float FloorAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(FloorNormal, FVector::UpVector)));
	const FVector DirectionXY = FVector(GetVelocity().X, GetVelocity().Y, 0.f).GetSafeNormal();
	return FVector::DotProduct(FloorNormal, DirectionXY) > 0.f ? -FloorAngle : FloorAngle;
}

void ASuraCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PrintPlayerDebugInfo();

	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
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
	const FVector LaunchVelocity = LaunchDirection * GetPlayerAttributes()->GetJumpXYVelocity() +
		FVector::UpVector * GetPlayerAttributes()->GetJumpZVelocity();
	return LaunchVelocity;
}


