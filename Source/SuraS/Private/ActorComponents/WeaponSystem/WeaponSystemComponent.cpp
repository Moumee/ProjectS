


#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"

#include "ActorComponents/WeaponSystem/SuraCharacterPlayerWeapon.h" //TODO: Player Class ï¿½ï¿½ï¿½ï¿½ï¿½Ï±ï¿½
#include "ActorComponents/WeaponSystem/SuraWeaponPickUp.h"
#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/WeaponName.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values for this component's properties
UWeaponSystemComponent::UWeaponSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeaponSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	InitializePlayerReference();
}


// Called every frame
void UWeaponSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	SearchWeapon();

	//CalculateScreenCenterWorldPositionAndDirection(ScreenCenterWorldLocation, ScreenCenterWorldDirection);
	CalculateTargetRightHandPosition();
}

#pragma region PlayerReference
void UWeaponSystemComponent::InitializePlayerReference()
{
	ASuraCharacterPlayerWeapon* NewPlayerOwner = Cast<ASuraCharacterPlayerWeapon>(GetOwner());
	if (IsValid(NewPlayerOwner))
	{
		PlayerOwner = NewPlayerOwner;
		PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		//PlayerController = Cast<APlayerController>(PlayerOwner->GetController())

		// Set up action bindings
		if (PlayerController)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
				Subsystem->AddMappingContext(WeaponSystemMappingContext, 1);
			}

			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				//TODO : Weapon Switchingï¿½Ï´ï¿½ logic ï¿½Ê¿ï¿½ï¿½ï¿½

				// Inereact With WeaponPickUp
				EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &UWeaponSystemComponent::PickUpWeapon);
	
				// Switch Weapon
				EnhancedInputComponent->BindAction(SwitchWeaponUpAction, ETriggerEvent::Started, this, &UWeaponSystemComponent::SwitchToNextWeapon);
				EnhancedInputComponent->BindAction(SwitchWeaponDownAction, ETriggerEvent::Started, this, &UWeaponSystemComponent::SwitchToPreviousWeapon);
			}
		}
	}
}
#pragma endregion

#pragma region SearchWeapon
bool UWeaponSystemComponent::SearchWeapon()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Visibility)); //TODO: Collision Channel ï¿½ï¿½ï¿½ï¿½ï¿½Ï±ï¿½

	TArray<AActor*> ignoreActors;
	ignoreActors.Init(PlayerOwner, 1);

	FVector sphereSpwanLocation = PlayerOwner->GetActorLocation();

	TArray<AActor*> overlappedActors;

	bool bIsWeaponInViewPort = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), sphereSpwanLocation, SearchWeaponRadius, traceObjectTypes, nullptr, ignoreActors, overlappedActors);

	float MinDistanceToWeapon = SearchWeaponRadius;
	ASuraWeaponPickUp* NearestWeapon = nullptr;

	for (AActor* overlappedActor : overlappedActors)
	{
		ASuraWeaponPickUp* WeaponObject = Cast<ASuraWeaponPickUp>(overlappedActor);
		if (WeaponObject != nullptr)
		{
			if (IsInViewport(GetScreenPositionOfWorldLocation(WeaponObject->GetActorLocation()).Get<0>(), SearchWeaponViewportRatio_Width, SearchWeaponViewportRatio_Height))
			{
				float DistanceToWeapon = PlayerOwner->GetDistanceTo(WeaponObject);
				if (DistanceToWeapon < MinDistanceToWeapon)
				{
					MinDistanceToWeapon = DistanceToWeapon;
					NearestWeapon = WeaponObject;

					// TODO: Overlap ï¿½ï¿½ weaponï¿½ï¿½ UI on/off ï¿½ï¿½ï¿½ ï¿½ß°ï¿½ï¿½Ï±ï¿½
				}
			}
		}
	}

	OverlappedWeapon = NearestWeapon;

	return bIsWeaponInViewPort;
}

TTuple<FVector2D, bool> UWeaponSystemComponent::GetScreenPositionOfWorldLocation(const FVector& SearchLocation) const
{
	FVector2D ScreenLocation = FVector2D::ZeroVector;
	bool bResult = UGameplayStatics::ProjectWorldToScreen(PlayerController, SearchLocation, ScreenLocation);

	return MakeTuple(ScreenLocation, bResult);
}

bool UWeaponSystemComponent::IsInViewport(FVector2D ActorScreenPosition, float ScreenRatio_Width, float ScreenRatio_Height) const
{
	FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();

	bool bIsInWidth = true;
	bool bIsInHeight = true;

	// Check Width
	if (ScreenRatio_Width == 0.0f || UKismetMathLibrary::Abs(ScreenRatio_Width) > 1.0f || (ScreenRatio_Width == (1.0f - ScreenRatio_Width)))
	{
		if (ActorScreenPosition.X >= 0.0f && ActorScreenPosition.X <= ViewportSize.X)
		{
			bIsInWidth = true;
		}
		else
		{
			bIsInWidth = false;
		}
	}
	else
	{
		float LargeScreenRatio_Width;
		float SmallScreenRatio_Width;

		if (ScreenRatio_Width < (1.0f - ScreenRatio_Width))
		{
			LargeScreenRatio_Width = 1.0f - ScreenRatio_Width;
			SmallScreenRatio_Width = ScreenRatio_Width;
		}
		else
		{
			LargeScreenRatio_Width = ScreenRatio_Width;
			SmallScreenRatio_Width = 1.0f - ScreenRatio_Width;
		}

		if (ActorScreenPosition.X >= ViewportSize.X * SmallScreenRatio_Width && ActorScreenPosition.X <= ViewportSize.X * LargeScreenRatio_Width)
		{
			bIsInWidth = true;
		}
		else
		{
			bIsInWidth = false;
		}
	}

	// Check Height
	if (ScreenRatio_Height == 0.0f || UKismetMathLibrary::Abs(ScreenRatio_Height) > 1.0f || (ScreenRatio_Height == (1.0f - ScreenRatio_Height)))
	{
		if (ActorScreenPosition.Y >= 0.0f && ActorScreenPosition.Y <= ViewportSize.Y)
		{
			bIsInHeight = true;
		}
		else
		{
			bIsInHeight = false;
		}
	}
	else
	{
		float LargeScreenRatio_Height;
		float SmallScreenRatio_Height;

		if (ScreenRatio_Height < (1.0f - ScreenRatio_Height))
		{
			LargeScreenRatio_Height = 1.0f - ScreenRatio_Height;
			SmallScreenRatio_Height = ScreenRatio_Height;
		}
		else
		{
			LargeScreenRatio_Height = ScreenRatio_Height;
			SmallScreenRatio_Height = 1.0f - ScreenRatio_Height;
		}

		if (ActorScreenPosition.Y >= ViewportSize.Y * SmallScreenRatio_Height && ActorScreenPosition.Y <= ViewportSize.Y * LargeScreenRatio_Height)
		{
			bIsInHeight = true;
		}
		else
		{
			bIsInHeight = false;
		}
	}

	// Return
	if (bIsInWidth && bIsInHeight)
	{
		return true;
	}
	else
	{
		return false;
	}
}

#pragma region Interaction

void UWeaponSystemComponent::PickUpWeapon()
{
	if (OverlappedWeapon != nullptr)
	{
		//TODO: ï¿½Ù·ï¿½ Attach ï¿½Ï´ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Æ´Ï¶ï¿½ inventoryï¿½ï¿½ ï¿½Ñ±â¸¦ ï¿½ï¿½ï¿½ï¿½ï¿½Ï±ï¿½
		// OverlappedWeapon->AttachToCharacter(PlayerOwner);
		ObtainNewWeapon(OverlappedWeapon);
	}

	/** ì—¬ê¸°ë‹¤ unlock ë©”ì„œë“œ ì—°ê²° **/
}

bool UWeaponSystemComponent::ObtainNewWeapon(ASuraWeaponPickUp* NewWeaponPickUp)
{
	for (const UACWeapon* WeaponInPossession : WeaponInventory)
	{
		if (WeaponInPossession->GetWeaponName() == NewWeaponPickUp->GetWeaponName())
		{
			return false;
		}
	}

	UACWeapon* NewWeapon = NewWeaponPickUp->SpawnWeapon(PlayerOwner);
	NewWeaponPickUp->DestroyWeaponPickUp();

	WeaponInventory.AddUnique(NewWeapon);

	if (CurrentWeapon == nullptr)
	{
		CurrentWeapon = NewWeapon;
		//CurrentWeapon->EquipWeapon(PlayerOwner);
		CurrentWeapon->SwitchWeapon(PlayerOwner, true);
	}

	return true;
}

#pragma endregion

void UWeaponSystemComponent::ZoomIn(bool bZoomIn)
{
	bIsZoomIn = bZoomIn;

	UE_LOG(LogTemp, Warning, TEXT("ZOOM Toggled!!!"));
}

bool UWeaponSystemComponent::IsWeaponModifyingCamFov()
{
	if (CurrentWeapon)
	{
		if (CurrentWeapon->IsModifyingPlayerCamFov() || bIsZoomIn)
		{
			return true;
		}
	}
	return false;
}

#pragma endregion

#pragma region Aiming

void UWeaponSystemComponent::SetRightHandToAimSocketOffset(FVector offset)
{
	RightHandToAimSocketOffset = offset;
}

FVector UWeaponSystemComponent::CalculateScreenCenterWorldPositionAndDirection(FVector& OutWorldPosition, FVector& OutWorldDirection) const
{
	//APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController || !PlayerController->PlayerCameraManager)
	{
		return FVector::ZeroVector;
	}

	// È­ï¿½ï¿½ Å©ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();

	// È­ï¿½ï¿½ ï¿½ß¾ï¿½ ï¿½ï¿½Ç¥ ï¿½ï¿½ï¿½
	FVector2D ScreenCenter(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);

	// È­ï¿½ï¿½ ï¿½ß½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Ä¡ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	FVector WorldPosition, WorldDirection;
	if (PlayerController->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldPosition, WorldDirection))
	{
		OutWorldPosition = WorldPosition;
		OutWorldDirection = WorldDirection;
		return WorldPosition + (WorldDirection * 15.0f); // TODO: ï¿½ï¿½ï¿½ï¿½ ï¿½Å¸ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½. ï¿½ï¿½Ä¡ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ïµï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ï±ï¿½
	}

	return FVector::ZeroVector;
}

FVector UWeaponSystemComponent::CalculateTargetRightHandPosition()
{
	const FVector AimLocation = CalculateScreenCenterWorldPositionAndDirection(ScreenCenterWorldLocation, ScreenCenterWorldDirection);

	const FRotator MeshRotation = PlayerOwner->GetMesh()->GetComponentRotation();

	const FVector TargetRightHandLocation = AimLocation - MeshRotation.RotateVector(RightHandToAimSocketOffset);

	TargetRightHandWorldLocation = TargetRightHandLocation;

	return TargetRightHandLocation;
}

FTransform UWeaponSystemComponent::GetWeaponAimSocketRelativeTransform()
{





	return FTransform();
}


//TODO: ï¿½Æ·ï¿½ ï¿½Ô¼ï¿½ ï¿½Ï¼ï¿½ï¿½Ï±ï¿½
//FVector UWeaponSystemComponent::ConvertTargetToBoneSpace(const FVector& TargetWorldLocation, const FName& BoneName) const
//{
//	// Ä³ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ì·ï¿½Å» ï¿½Þ½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ® ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
//	USkeletalMeshComponent* SkeletalMesh = PlayerOwner->GetMesh();
//	if (!SkeletalMesh)
//	{
//		return FVector::ZeroVector;
//	}
//
//	// ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Ä¡ï¿½ï¿½ ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ì½ï¿½ï¿½ï¿½ ï¿½ï¿½È¯
//	//TODO: ConvertWorldLocationToBoneSpace ï¿½Ô¼ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
//	SkeletalMesh->TransformToBoneSpace(BoneName, );
//
//	FVector BoneSpaceLocation = SkeletalMesh->ConvertWorldLocationToBoneSpace(BoneName, TargetWorldPosition, EBoneSpaces::WorldSpace);
//	return BoneSpaceLocation;
//}
#pragma endregion


#pragma region SwitchWeapon
void UWeaponSystemComponent::SwitchToPreviousWeapon()
{
	if (WeaponInventory.Num() > 1)
	{
		CurrentWeaponIndex--;
		if (CurrentWeaponIndex < 0)
		{
			CurrentWeaponIndex = WeaponInventory.Num() + CurrentWeaponIndex;
		}
		ChangeWeapon(CurrentWeaponIndex);
	}
}

void UWeaponSystemComponent::SwitchToNextWeapon()
{
	if (WeaponInventory.Num() > 1)
	{
		CurrentWeaponIndex = (CurrentWeaponIndex + 1) % WeaponInventory.Num();
		ChangeWeapon(CurrentWeaponIndex);
	}
}

void UWeaponSystemComponent::SwitchToOtherWeapon()
{
	WeaponInventory[CurrentWeaponIndex]->SwitchWeapon(PlayerOwner, true);
	CurrentWeapon = WeaponInventory[CurrentWeaponIndex];
}

void UWeaponSystemComponent::ChangeWeapon(int32 WeaponIndex)
{
	if (WeaponInventory.IsValidIndex(WeaponIndex))
	{
		if (IsValid(CurrentWeapon))
		{
			//CurrentWeapon->UnequipWeapon(PlayerOwner);
			//TODO: ¿©±â¼­ CurrentWeaponÀÇ stateÀÇ È®ÀÎÇØ¼­ ¹«±â ÀüÈ¯ ¿©ºÎ °áÁ¤ÇÏ±â
			CurrentWeapon->SwitchWeapon(PlayerOwner, false);
		}
		//WeaponInventory[WeaponIndex]->EquipWeapon(PlayerOwner);
		//CurrentWeapon = WeaponInventory[WeaponIndex];
	}
}
#pragma endregion