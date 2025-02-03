
#include "ActorComponents/WeaponSystem/ACWeapon.h"
//#include "Characters/Player/SuraCharacterPlayer.h"
#include "ActorComponents/WeaponSystem/SuraCharacterPlayerWeapon.h"

#include "ActorComponents/WeaponSystem/SuraProjectile.h"

//TODO: file path 수정하기
#include "WeaponInterface.h"
#include "WeaponSystemComponent.h"

#include "SuraWeaponBaseState.h"
#include "SuraWeaponIdleState.h"
#include "SuraWeaponFiringState.h"
#include "SuraWeaponUnequippedState.h"

#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h" //TODO: 무엇을 위한 header인지 알아봐야함
#include "Engine/World.h"

#include "Camera/CameraComponent.h"
//#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"

// Sets default values for this component's properties
UACWeapon::UACWeapon()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true; //TODO: Tick 활용하면 활성화해야함.

	// ...

	//TODO: Muzzle Offset은 weapon Type에 따라서 다를듯
	// 현재는 기본적으로 Weapon Mesh의"Muzzle" Socket의 위치를 이용하고 있어서 MuzzleOffset는 사용 안하는 중임
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);

	// Fire Mode
	//TODO: 일단은 기본 Single로 하는데, WeaponName에 따라서 생성자에서 지정해주는 식으로 수정하기
	WeaponName = EWeaponName::WeaponName_Rifle;
	WeaponType = EWeaponType::WeaponType_Rifle;
	WeaponFireMode = EWeaponFireMode::WeaponFireMode_Single;
	
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//SetCollisionObjectType(ECC_GameTraceChannel3); //Weapon
	//SetCollisionResponseToAllChannels(ECR_Ignore);
}

void UACWeapon::InitializeWeapon(ASuraCharacterPlayerWeapon* NewCharacter)
{
	Character = NewCharacter;

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		//TODO: 아래 코드에 대해서 알아봐야함. Multi Play를 위한 것인가?
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		//TODO: Weapon Type에 따라서 Bind되는 Action이 다르도록 설계하기
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			LeftMouseButtonActionBinding = &EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UACWeapon::Fire);

			// Zoom Toggle
			RightMouseButtonActionBinding = &EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &UACWeapon::ZoomToggle);
		}
	}

	// Set Aim Socket Relative Transform
	SetAimSocketRelativeTransform();
}

// Called when the game starts
void UACWeapon::BeginPlay()
{
	Super::BeginPlay();

	// ...

	//TODO: WeaponType에 따라서 생성되는 State 객체들이 달라지도록 설정하기
	IdleState = NewObject<USuraWeaponIdleState>(this, USuraWeaponIdleState::StaticClass());
	FiringState = NewObject<USuraWeaponFiringState>(this, USuraWeaponFiringState::StaticClass());
	UnequippedState = NewObject<USuraWeaponUnequippedState>(this, USuraWeaponUnequippedState::StaticClass());

	ChangeState(UnequippedState);


	//SetCollisionObjectType(ECC_EngineTraceChannel2);

	//SetCollisionProfileName(FName("Weapon"));
	//SetCollisionResponseToAllChannels(ECR_Ignore);
}

void UACWeapon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	//TODO: UpdateState() 활성화시키기
}

void UACWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

bool UACWeapon::AttachWeaponToPlayer(ASuraCharacterPlayerWeapon* TargetCharacter)
{
	Character = TargetCharacter;

	//TODO: 무기 소지 확인 방법 수정하기. 다수 무기를 소지할 수도 있기 때문에,
	// 그리고 WeaponSystemComponent에서 weapon 관련 로직을 다룰 것이기 때문에 따로 설정해야함
	// 그리고 플레이어가 한번 획득한 무기는 WeaponSystem의 WeaponInventory에 저장되어 영구적으로 사용 가능하도록 해야함
	// Check that the character is valid, and has no weapon component yet
	//if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UACWeapon>())
	//{
	//	return false;
	//}
	if (Character == nullptr)
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	// AttachToComponent(Character->GetArmMesh(), AttachmentRules, FName(TEXT("GripPoint")));
	AttachToComponent(Character->GetMesh(), AttachmentRules, FName(TEXT("Gun")));

	//TODO: 더 좋은 방법을 생각해 봐야함
	//RightHandToAimSocketOffset = this->GetSocketLocation(FName(TEXT("Aim"))) - Character->GetMesh()->GetSocketLocation(FName("Gun"));
	RightHandToAimSocketOffset = this->GetSocketLocation(FName(TEXT("Aim"))) - Character->GetMesh()->GetBoneLocation(FName(TEXT("hand_r")));
	if (Character->GetWeaponSystem() && Character->GetWeaponSystem()->GetClass()->ImplementsInterface(UWeaponInterface::StaticClass()))
	{
		Character->GetWeaponSystem()->SetRightHandToAimSocketOffset(RightHandToAimSocketOffset);
	}

	// Set AimSocketTransform
	SetAimSocketTransform();

	// Set Up Widget UI Class
	ActivateCrosshairWidget(true);

	SetVisibility(true);

	return true;
}

void UACWeapon::DetachWeaponFromPlayer()
{
	if (Character == nullptr)
	{
		return;
	}
	else
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				//Subsystem->RemoveMappingContext(FireMappingContext);
			}

			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				//EnhancedInputComponent->RemoveBinding(*LeftMouseButtonActionBinding);
				//EnhancedInputComponent->RemoveBinding(*RightMouseButtonActionBinding);
			}
		}

		DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		Character = nullptr;

		ActivateCrosshairWidget(false);

		SetVisibility(false);
	}
}

void UACWeapon::Fire()
{
	if (CurrentState != UnequippedState)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fired!!!"));

		if (Character == nullptr || Character->GetController() == nullptr)
		{
			return;
		}

		//TODO: 투사체 타입이 직사형인지 곡사형인지, 그리고 관통형인지에 따라서 Trace 방식을 달리 해야함. 우선은 단순 직사형 and 일반형(관통X)로 진행
		FVector LineTraceStartLocation = Character->GetCamera()->GetComponentLocation();
		FVector LineTraceDirection = Character->GetCamera()->GetForwardVector();
		FVector LineTraceHitLocation;

		if (PerformLineTrace(LineTraceStartLocation, LineTraceDirection, LineTraceMaxDistance, LineTraceHitLocation))
		{
			TargetLocationOfProjectile = LineTraceHitLocation;
		}
		else
		{
			TargetLocationOfProjectile = LineTraceHitLocation;
		}

		// Try and fire a projectile
		if (ProjectileClass != nullptr)
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				//// <Old Version>
				//APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
				//const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
				//// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				//const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

				// <New Version>
				const FVector SpawnLocation = this->GetSocketLocation(FName(TEXT("Muzzle")));
				const FRotator SpawnRotation = (TargetLocationOfProjectile - SpawnLocation).Rotation();

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				// Spawn the projectile at the muzzle
				ASuraProjectile* Projectile = World->SpawnActor<ASuraProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				Projectile->InitializeProjectile(Character);
			}
		}

		// Try and play the sound if specified
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
		}

		// Try and play a firing animation if specified
		if (FireAnimation != nullptr)
		{
			// Get the animation object for the arms mesh
			//UAnimInstance* AnimInstance = Character->GetArmMesh()->GetAnimInstance();
			UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
	}
}

void UACWeapon::StopFire()
{
	//TODO: 연발 타이머 초기화


}

void UACWeapon::ZoomToggle()
{
	if (CurrentState != UnequippedState)
	{
		if (bIsZoomIn)
		{
			ZoomOut();
		}
		else
		{
			ZoomIn();
		}
	}
}

void UACWeapon::ZoomIn()
{
	bIsZoomIn = true;

	if (Character->GetWeaponSystem() && Character->GetWeaponSystem()->GetClass()->ImplementsInterface(UWeaponInterface::StaticClass()))
	{
		Character->GetWeaponSystem()->ZoomIn(true);
	}
	ActivateCrosshairWidget(false);
}

void UACWeapon::ZoomOut()
{
	bIsZoomIn = false;

	if (Character->GetWeaponSystem() && Character->GetWeaponSystem()->GetClass()->ImplementsInterface(UWeaponInterface::StaticClass()))
	{
		Character->GetWeaponSystem()->ZoomIn(false);
	}
	ActivateCrosshairWidget(true);
}

#pragma region WeaponState

void UACWeapon::ChangeState(USuraWeaponBaseState* NewState)
{
	if (!NewState || NewState == CurrentState) return;

	if (CurrentState)
	{
		CurrentState->ExitState(this);
	}
	CurrentState = NewState;
	CurrentState->EnterState(this);
}

#pragma endregion

bool UACWeapon::PerformLineTrace(FVector StartLocation, FVector LineDirection, float MaxDistance, FVector& HitLocation)
{
	FVector Start = StartLocation;
	FVector End = StartLocation + LineDirection * MaxDistance;

	FHitResult HitResult;

	FCollisionQueryParams Params;
	Params.AddIgnoredComponent(this);
	Params.AddIgnoredActor(Character);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,           // 충돌 결과 저장
		Start,               // 시작 지점
		End,                 // 끝 지점
		ECC_Visibility,      // 충돌 채널
		Params               // 쿼리 매개변수
	);


	if (bHit)
	{
		HitLocation = HitResult.ImpactPoint;
	}
	else
	{
		HitLocation = End;
	}

	return bHit;
}

void UACWeapon::SetAimSocketTransform()
{
	FTransform AimSocketTransform = GetSocketTransform(FName(TEXT("Aim")));
	FTransform IKHandGunTransform = Character->GetMesh()->GetSocketTransform(FName(TEXT("ik_hand_gun")));

	AimSocketRelativeTransform = AimSocketTransform.GetRelativeTransform(IKHandGunTransform);
}

void UACWeapon::SetAimSocketRelativeTransform()
{
	AimSocketRelativeTransform = GetSocketTransform(FName(TEXT("Aim")), ERelativeTransformSpace::RTS_Component);
}

FTransform UACWeapon::GetAimSocketRelativeTransform()
{
	return AimSocketRelativeTransform;
}

#pragma region Equip/Unequip

void UACWeapon::EquipWeapon(ASuraCharacterPlayerWeapon* TargetCharacter)
{
	// TODO: 함수 최적화 해야함
	UE_LOG(LogTemp, Warning, TEXT("Equip Weapon!!!"));
	AttachWeaponToPlayer(TargetCharacter);

	ChangeState(IdleState);
}

void UACWeapon::UnequipWeapon(ASuraCharacterPlayerWeapon* TargetCharacter)
{
	// TODO: Detach 전에 처리해야하는 것들 처리해야함
	UE_LOG(LogTemp, Warning, TEXT("Unequip Weapon!!!"));
	DetachWeaponFromPlayer();

	ChangeState(UnequippedState);
}

#pragma endregion

void UACWeapon::ActivateCrosshairWidget(bool bflag)
{
	if (bflag)
	{
		if (CrosshairWidgetClass)
		{
			CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
			if (CrosshairWidget)
			{
				CrosshairWidget->AddToViewport();
			}
		}
	}
	else
	{
		if (CrosshairWidget)
		{
			CrosshairWidget->RemoveFromViewport();
			CrosshairWidget = nullptr;
		}
	}
}


#pragma region FIreMode

void UACWeapon::HandleSingleFire()
{
}

void UACWeapon::HandleBurstFire()
{
	GetWorld()->GetTimerManager().SetTimer(BurstShotTimer, this, &UACWeapon::StartBurstFire, BurstShotFireRate, true);
}

void UACWeapon::HandleFullAutoFire()
{

}

void UACWeapon::StartBurstFire()
{
	if (BurstShotFired < BurstShotCount)
	{
		Fire();
		BurstShotFired++;
	}
	else
	{
		StopBurstFire();
	}
}

void UACWeapon::StopBurstFire()
{
	BurstShotFired = 0;
	if (GetWorld()->GetTimerManager().IsTimerActive(BurstShotTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(BurstShotTimer);
	}
}


#pragma endregion


