


#include "WeaponSystemComponent.h"

#include "ActorComponents/WeaponSystem/SuraCharacterPlayerWeapon.h" //TODO: Player Class 수정하기
#include "ActorComponents/WeaponSystem/SuraWeaponPickUp.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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
}

#pragma region PlayerReference
void UWeaponSystemComponent::InitializePlayerReference()
{
	ASuraCharacterPlayerWeapon* NewPlayerOwner = Cast<ASuraCharacterPlayerWeapon>(GetOwner());
	if (IsValid(NewPlayerOwner))
	{
		PlayerOwner = NewPlayerOwner;
		PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
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

	return false;
}

void UWeaponSystemComponent::PickUpWeapon()
{



}
#pragma endregion


