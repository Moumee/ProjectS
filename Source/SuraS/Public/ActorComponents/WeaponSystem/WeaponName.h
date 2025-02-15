

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponName.generated.h"

/**
 *
 */
UENUM(BlueprintType)
enum class EWeaponName : uint8
{
	WeaponName_Pistol UMETA(DisplayName = "Pistol"),
	WeaponName_Rifle UMETA(DisplayName = "Rifle"),
	WeaponName_ShotGun UMETA(DisplayName = "ShotGun"),
	WeaponName_MissileLauncher UMETA(DisplayName = "MissileLauncher")
};

UCLASS()
class SURAS_API AWeaponName : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponName();

	//protected:
	//	// Called when the game starts or when spawned
	//	virtual void BeginPlay() override;
	//
	//public:
	//	// Called every frame
	//	virtual void Tick(float DeltaTime) override;
};
