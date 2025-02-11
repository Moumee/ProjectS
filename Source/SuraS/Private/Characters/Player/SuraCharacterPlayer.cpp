// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraCharacterPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ActorComponents/ACPlayerMovmentData.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/SuraPlayerBaseState.h"
#include "Characters/Player/SuraPlayerCrouchingState.h"
#include "Characters/Player/SuraPlayerDashingState.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerHangingState.h"
#include "Characters/Player/SuraPlayerJumpingState.h"
#include "Characters/Player/SuraPlayerMantlingState.h"
#include "Characters/Player/SuraPlayerRunningState.h"
#include "Characters/Player/SuraPlayerSlidingState.h"
#include "Characters/Player/SuraPlayerWalkingState.h"
#include "Characters/Player/SuraPlayerWallRunningState.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ActorComponents/UISystem/ACInventoryManager.h"
#include "ActorComponents/UISystem/ACBaseUIComponent.h"
#include "Extensions/UIComponent.h"


ASuraCharacterPlayer::ASuraCharacterPlayer()
{


	GetCharacterMovement()->bOrientRotationToMovement = false;

	PlayerMovementData = CreateDefaultSubobject<UACPlayerMovementData>("Player Movement Data");

	// Make character rotate with the controller
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent(), FName(TEXT("Camera")));
	Camera->bUsePawnControlRotation = true;
	Camera->SetRelativeLocation(FVector(0.f, 0.f, 70.f));

	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm Mesh"));
	ArmMesh->SetupAttachment(Camera);

	DefaultCapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	DefaultCameraLocation = Camera->GetRelativeLocation();

	// Initialize JumpsLeft
	JumpsLeft = MaxJumps;

	// inventory actor components
	UIComponent = CreateDefaultSubobject<UACBaseUIComponent>(TEXT("UI Component"));
	InventoryManager = CreateDefaultSubobject<UACInventoryManager>(TEXT("UI Manager Component"));

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

	DefaultGravityScale = PlayerMovementData->GetGravityScale();
	DefaultGroundFriction = GetCharacterMovement()->GroundFriction;
	DefaultBrakingFriction = GetCharacterMovement()->BrakingFriction;
	DefaultBrakingDecelerationFalling = GetCharacterMovement()->BrakingDecelerationFalling;
	DefaultBrakingDecelerationWalking = GetCharacterMovement()->BrakingDecelerationWalking;

	DefaultCameraFOV = GetCamera()->FieldOfView;
	
	
	DefaultGroundFriction = GetCharacterMovement()->GroundFriction;

	MaxDashes = GetPlayerMovementData()->GetDashMaxStack();
	DashesLeft = MaxDashes;

	DashCooldowns.Init(0.f, MaxDashes);

	BaseMovementSpeed = GetPlayerMovementData()->GetRunSpeed();
	GetCharacterMovement()->AirControl = GetPlayerMovementData()->GetAirControl();

	WalkingState = NewObject<USuraPlayerWalkingState>(this, USuraPlayerWalkingState::StaticClass());
	RunningState = NewObject<USuraPlayerRunningState>(this, USuraPlayerRunningState::StaticClass());
	JumpingState = NewObject<USuraPlayerJumpingState>(this, USuraPlayerJumpingState::StaticClass());
	FallingState = NewObject<USuraPlayerFallingState>(this, USuraPlayerFallingState::StaticClass());
	DashingState = NewObject<USuraPlayerDashingState>(this, USuraPlayerDashingState::StaticClass());
	CrouchingState = NewObject<USuraPlayerCrouchingState>(this, USuraPlayerCrouchingState::StaticClass());
	HangingState = NewObject<USuraPlayerHangingState>(this, USuraPlayerHangingState::StaticClass());
	MantlingState = NewObject<USuraPlayerMantlingState>(this, USuraPlayerMantlingState::StaticClass());
	WallRunningState = NewObject<USuraPlayerWallRunningState>(this, USuraPlayerWallRunningState::StaticClass());
	SlidingState = NewObject<USuraPlayerSlidingState>(this, USuraPlayerSlidingState::StaticClass());

	PreviousState = RunningState;
	PreviousGroundedState = RunningState;
	CurrentState = RunningState;

}

void ASuraCharacterPlayer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

}

void ASuraCharacterPlayer::ResetTriggeredBooleans()
{
	bJumpTriggered = false;
	bDashTriggered = false;
	bLandedTriggered = false;
}

void ASuraCharacterPlayer::SetBaseMovementSpeed(const float MovementSpeed)
{
	BaseMovementSpeed = MovementSpeed;
}

float ASuraCharacterPlayer::GetBaseMovementSpeed() const
{
	return BaseMovementSpeed;
}

void ASuraCharacterPlayer::ChangeState(USuraPlayerBaseState* NewState)
{
	if (!NewState || NewState == CurrentState) return;

	if (CurrentState)
	{
		CurrentState->ExitState(this);
		PreviousState = CurrentState;
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
		JumpsLeft--;
		FVector JumpVector = FVector::UpVector * GetPlayerMovementData()->GetPrimaryJumpZSpeed();
		LaunchCharacter(JumpVector, false, true);
	}
}

void ASuraCharacterPlayer::DoubleJump()
{
	if (JumpsLeft > 0 && GetCharacterMovement()->IsFalling())
	{
		JumpsLeft--;
		FVector InputDirection = GetActorForwardVector() * ForwardAxisInputValue + GetActorRightVector() * RightAxisInputValue;
		FVector LaunchVelocity = InputDirection * GetPlayerMovementData()->GetDoubleJumpXYSpeed() +
			FVector::UpVector * GetPlayerMovementData()->GetDoubleJumpZSpeed();
		LaunchCharacter(LaunchVelocity, false, true);
	}
}

float ASuraCharacterPlayer::GetDefaultCapsuleHalfHeight() const
{
	return DefaultCapsuleHalfHeight;
}

FVector ASuraCharacterPlayer::GetDefaultCameraLocation() const
{
	return DefaultCameraLocation;
}

USuraPlayerBaseState* ASuraCharacterPlayer::GetCurrentState() const
{
	return CurrentState;
}

USuraPlayerBaseState* ASuraCharacterPlayer::GetPreviousState() const
{
	return PreviousState;
}

USuraPlayerBaseState* ASuraCharacterPlayer::GetPreviousGroundedState() const
{
	return PreviousGroundedState;
}

void ASuraCharacterPlayer::SetPreviousGroundedState(USuraPlayerBaseState* InState)
{
	PreviousGroundedState = InState;
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

			GEngine->AddOnScreenDebugMessage(101, 0.f, FColor::Green,
				FString::Printf(TEXT("Dashes Left : %d / %d"), DashesLeft, MaxDashes));

			FString CooldownInfo;
			for (float Cooldown : DashCooldowns)
			{
				CooldownInfo += FString::Printf(TEXT("%.2f "), Cooldown);
			}

			GEngine->AddOnScreenDebugMessage(100, 0.f, FColor::Green,
				FString::Printf(TEXT("Dash Cooldowns: %s"), *CooldownInfo));

			GEngine->AddOnScreenDebugMessage(99, 0.f, FColor::Green,
				FString::Printf(TEXT("Input Axis Value : ( %f, %f )"), ForwardAxisInputValue, RightAxisInputValue));

			GEngine->AddOnScreenDebugMessage(98, 0.f, FColor::Green,
				FString::Printf(TEXT("Slope Speed Delta : %f"), SlopeSpeedDelta));

			GEngine->AddOnScreenDebugMessage(97, 0.f, FColor::Red,
				FString::Printf(TEXT("Current State : %s"), *CurrentState->StateDisplayName.ToString()));

			GEngine->AddOnScreenDebugMessage(96, 0.f, FColor::Green,
				FString::Printf(TEXT("Previous Grounded State : %s"), *PreviousGroundedState->StateDisplayName.ToString()));

			GEngine->AddOnScreenDebugMessage(95, 0.f, FColor::Green,
				FString::Printf(TEXT("Previous State : %s"), *PreviousState->StateDisplayName.ToString()));

			GEngine->AddOnScreenDebugMessage(94, 0.f, FColor::Green,
				FString::Printf(TEXT("Wall Run Side : %s"), *UEnum::GetDisplayValueAsText(WallRunSide).ToString()));
		}
	}
}

void ASuraCharacterPlayer::UpdateDashCooldowns(float DeltaTime)
{
	if (DashesLeft == MaxDashes) return;

	for (int i = 0; i < MaxDashes; i++)
	{
		if (DashCooldowns[i] > 0.f)
		{
			DashCooldowns[i] = DashCooldowns[i] - DeltaTime;
			if (DashCooldowns[i] <= KINDA_SMALL_NUMBER)
			{
				DashCooldowns[i] = 0.f;
				DashesLeft = FMath::Min(DashesLeft + 1, MaxDashes);
			}
		}
	}
}

float ASuraCharacterPlayer::FindFloorAngle() const
{
	if (!GetCharacterMovement()->IsFalling() && GetCharacterMovement()->MovementMode != MOVE_Flying)
	{
		const FVector FloorNormal = GetCharacterMovement()->CurrentFloor.HitResult.Normal;
		const float FloorAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(FloorNormal, FVector::UpVector)));
		const FVector DirectionXY = FVector(GetVelocity().X, GetVelocity().Y, 0.f).GetSafeNormal();
		return FVector::DotProduct(FloorNormal, DirectionXY) > 0.f ? -FloorAngle : FloorAngle;
	}
	return 0.f;
	
}

void ASuraCharacterPlayer::RestoreCameraTilt(float DeltaTime)
{
	if (bShouldRestoreCameraTilt)
	{
		FRotator CurrentControlRotation = GetControlRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentControlRotation,
		                                        FRotator(CurrentControlRotation.Pitch, CurrentControlRotation.Yaw, 0.f), DeltaTime, 10.f);
		GetController()->SetControlRotation(NewRotation);
		if (FMath::Abs(GetControlRotation().Roll) < 0.1f)
		{
			FRotator CurrentRotation = GetControlRotation();
			GetController()->SetControlRotation(FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw, 0.f));
			bShouldRestoreCameraTilt = false;
		}
	}
}


void ASuraCharacterPlayer::InterpCameraFOV(float DeltaTime)
{
	float Alpha = FMath::Clamp(GetCharacterMovement()->Velocity.Size() / PlayerMovementData->GetMaxCameraFOVSpeed(), 0.f, 1.f);
	float TargetFOV = FMath::Lerp(DefaultCameraFOV, PlayerMovementData->GetMaxCameraFOV(), Alpha);
	float NewCameraFOV = FMath::FInterpTo(GetCamera()->FieldOfView, TargetFOV, DeltaTime, 10.f);
	GetCamera()->SetFieldOfView(NewCameraFOV);
}

void ASuraCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	XYSpeed = FVector(GetCharacterMovement()->Velocity.X, GetCharacterMovement()->Velocity.Y, 0.f).Size();

	InterpCameraFOV(DeltaTime);

	UpdateDashCooldowns(DeltaTime);

	RestoreCameraTilt(DeltaTime);

	PrintPlayerDebugInfo();

	if (CurrentState)
	{
		CurrentState->UpdateState(this, DeltaTime);
	}
	

	SlopeSpeedDelta = FindFloorAngle() < GetCharacterMovement()->GetWalkableFloorAngle() ?
		SlopeSpeedDeltaCurve->GetFloatValue(FindFloorAngle()) : 0.f;
	
	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed + SlopeSpeedDelta;
}

void ASuraCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASuraCharacterPlayer::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ASuraCharacterPlayer::StopMoving);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASuraCharacterPlayer::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASuraCharacterPlayer::StartJumping);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ASuraCharacterPlayer::StartDashing);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ASuraCharacterPlayer::StartCrouching);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ASuraCharacterPlayer::StopCrouching);
		
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

void ASuraCharacterPlayer::StartJumping()
{
	if (JumpsLeft <= 0) return;
	bJumpTriggered = true;
	CurrentState->StartJumping(this);
}

void ASuraCharacterPlayer::StartCrouching()
{
	bCrouchTriggered = true;
	CurrentState->StartCrouching(this);
}

void ASuraCharacterPlayer::StopCrouching()
{
	bCrouchTriggered = false;
}

void ASuraCharacterPlayer::StartDashing()
{
	if (DashesLeft <= 0) return;
	if (CurrentState == DashingState) return;
	bDashTriggered = true;
	CurrentState->StartDashing(this);
	
}

void ASuraCharacterPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	bLandedTriggered = true;
	JumpsLeft = MaxJumps;
	CurrentState->Landed(this, Hit);
}

bool ASuraCharacterPlayer::ShouldEnterWallRunning(FVector& OutWallRunDirection, EWallSide& OutWallRunSide)
{
	if (!GetCharacterMovement()->IsFalling()) return false;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	FHitResult LeftHit;
	bool bLeftHit = GetWorld()->LineTraceSingleByChannel(LeftHit, GetActorLocation(),
		GetActorLocation() + GetActorRightVector() * -45.f, ECC_Visibility, Params);
	bool bLeftWallRunnable = false;
	FHitResult RightHit;
	bool bRightHit = GetWorld()->LineTraceSingleByChannel(RightHit, GetActorLocation(),
		GetActorLocation() + GetActorRightVector() * 45.f, ECC_Visibility, Params);
	bool bRightWallRunnable = false;

	if (bLeftHit)
	{
		if (LeftHit.ImpactNormal.Z > -0.05f && LeftHit.ImpactNormal.Z < GetCharacterMovement()->GetWalkableFloorZ())
		{
			bLeftWallRunnable = true;
		}
	}

	if (bRightHit)
	{
		if (RightHit.ImpactNormal.Z > -0.05f && RightHit.ImpactNormal.Z < GetCharacterMovement()->GetWalkableFloorZ())
		{
			bRightWallRunnable = true;
		}
	}

	if (bLeftWallRunnable && bRightWallRunnable)
	{
		if (LeftHit.Distance < RightHit.Distance)
		{
			OutWallRunSide = EWallSide::Left;
			FVector WallNormalXY = FVector(LeftHit.ImpactNormal.X, LeftHit.ImpactNormal.Y, 0.f);
			OutWallRunDirection = FVector::CrossProduct(WallNormalXY, FVector::UpVector).GetSafeNormal();
		}
		else
		{
			OutWallRunSide = EWallSide::Right;
			FVector WallNormalXY = FVector(RightHit.ImpactNormal.X, RightHit.ImpactNormal.Y, 0.f);
			OutWallRunDirection = FVector::CrossProduct(WallNormalXY, FVector::DownVector).GetSafeNormal();
		}
		return true;
	}

	if (bLeftWallRunnable)
	{
		OutWallRunSide = EWallSide::Left;
		FVector WallNormalXY = FVector(LeftHit.ImpactNormal.X, LeftHit.ImpactNormal.Y, 0.f);
		OutWallRunDirection = FVector::CrossProduct(WallNormalXY, FVector::UpVector).GetSafeNormal();
		return true;
	}

	if (bRightWallRunnable)
	{
		OutWallRunSide = EWallSide::Right;
		FVector WallNormalXY = FVector(RightHit.ImpactNormal.X, RightHit.ImpactNormal.Y, 0.f);
		OutWallRunDirection = FVector::CrossProduct(WallNormalXY, FVector::DownVector).GetSafeNormal();
		return true;
	}

	return false;

	
}

void ASuraCharacterPlayer::InterpCapsuleHeight(float TargetScale, float DeltaTime)
{
	float CurrentHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	float TargetHeight = GetDefaultCapsuleHalfHeight() * TargetScale;
	float NewHeight = FMath::FInterpTo(CurrentHeight, TargetHeight, DeltaTime, 10.f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(NewHeight);
	

}




