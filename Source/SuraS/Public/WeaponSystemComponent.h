

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponSystemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API UWeaponSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma region PlayerReference
private:
	void InitializePlayerReference();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WeaponSystem|PlayerReference", Meta = (AllowPrivateAccess = "true"))
	class ASuraCharacterPlayerWeapon* PlayerOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WeaponSystem|PlayerReference", Meta = (AllowPrivateAccess = "true"))
	class APlayerController* PlayerController;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TargetingSystem|PlayerReference", Meta = (AllowPrivateAccess = "true"))
	//class APlayerCameraManager* PlayerCameraManager;

#pragma endregion

#pragma region SearchWeapon

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSystem")
	float SearchWeaponRadius = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSystem")
	float SearchWeaponViewportRatio_Width = 0.7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSystem")
	float SearchWeaponViewportRatio_Height = 0.7;

	UPROPERTY()
	class ASuraWeaponPickUp* OverlappedWeapon;

public:
	bool SearchWeapon();
	
	TTuple<FVector2D, bool> GetScreenPositionOfWorldLocation(const FVector& SearchLocation) const;

	bool IsInViewport(FVector2D ActorScreenPosition, float ScreenRatio_Width = 0.0f, float ScreenRatio_Height = 0.0f) const;


#pragma endregion

#pragma region Interaction

public:
	void PickUpWeapon();

#pragma endregion

};
