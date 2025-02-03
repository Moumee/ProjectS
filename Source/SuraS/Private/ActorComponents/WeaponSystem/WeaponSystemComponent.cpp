


#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"

#include "ActorComponents/WeaponSystem/SuraCharacterPlayerWeapon.h" //TODO: Player Class 수정하기
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
				//TODO : Weapon Switching하는 logic 필요함

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
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Visibility)); //TODO: Collision Channel 설정하기

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

					// TODO: Overlap 된 weapon에 UI on/off 기능 추가하기
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
		//TODO: 바로 Attach 하는 것이 아니라 inventory에 총기를 저장하기
		// OverlappedWeapon->AttachToCharacter(PlayerOwner);
		ObtainNewWeapon(OverlappedWeapon);
	}
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
		CurrentWeapon->EquipWeapon(PlayerOwner);
	}

	return true;
}

#pragma endregion

void UWeaponSystemComponent::ZoomIn(bool bZoomIn)
{
	bIsZoomIn = bZoomIn;

	UE_LOG(LogTemp, Warning, TEXT("ZOOM Toggled!!!"));
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

	// 화면 크기 가져오기
	FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();

	// 화면 중앙 좌표 계산
	FVector2D ScreenCenter(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);

	// 화면 중심의 월드 위치와 방향 가져오기
	FVector WorldPosition, WorldDirection;
	if (PlayerController->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldPosition, WorldDirection))
	{
		OutWorldPosition = WorldPosition;
		OutWorldDirection = WorldDirection;
		return WorldPosition + (WorldDirection * 15.0f); // TODO: 임의 거리를 사용했음. 수치 조절 가능하도록 구현하기
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


//TODO: 아래 함수 완성하기
//FVector UWeaponSystemComponent::ConvertTargetToBoneSpace(const FVector& TargetWorldLocation, const FName& BoneName) const
//{
//	// 캐릭터의 스켈레탈 메시 컴포넌트 가져오기
//	USkeletalMeshComponent* SkeletalMesh = PlayerOwner->GetMesh();
//	if (!SkeletalMesh)
//	{
//		return FVector::ZeroVector;
//	}
//
//	// 월드 위치를 본 스페이스로 변환
//	//TODO: ConvertWorldLocationToBoneSpace 함수가 존재하지 않음
//	SkeletalMesh->TransformToBoneSpace(BoneName, );
//
//	FVector BoneSpaceLocation = SkeletalMesh->ConvertWorldLocationToBoneSpace(BoneName, TargetWorldPosition, EBoneSpaces::WorldSpace);
//	return BoneSpaceLocation;
//}
#pragma endregion


#pragma region SwitchWeapon

void UWeaponSystemComponent::SwitchToPreviousWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Wheel Down"));

	if (WeaponInventory.Num() > 1)
	{
		CurrentWeaponIndex--;
		if (CurrentWeaponIndex < 0)
		{
			CurrentWeaponIndex = WeaponInventory.Num() + CurrentWeaponIndex;
		}
		EquipWeapon(CurrentWeaponIndex);
	}
}

void UWeaponSystemComponent::SwitchToNextWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Wheel Up"));

	if (WeaponInventory.Num() > 1)
	{
		CurrentWeaponIndex = (CurrentWeaponIndex + 1) % WeaponInventory.Num();
		EquipWeapon(CurrentWeaponIndex);
	}
}

void UWeaponSystemComponent::EquipWeapon(int32 WeaponIndex)
{
	if (WeaponInventory.IsValidIndex(WeaponIndex))
	{
		if (IsValid(CurrentWeapon))
		{
			CurrentWeapon->UnequipWeapon(PlayerOwner);
		}

		WeaponInventory[WeaponIndex]->EquipWeapon(PlayerOwner);
		CurrentWeapon = WeaponInventory[WeaponIndex];
	}
}

#pragma endregion