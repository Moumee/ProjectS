

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponFireMode.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EWeaponFireMode : uint8
{
	WeaponFireMode_Pistol UMETA(DisplayName = "Pistol"),
	WeaponFireMode_Rifle UMETA(DisplayName = "Rifle")
};

UCLASS()
class SURAS_API AWeaponFireMode : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponFireMode();

	//protected:
	//	// Called when the game starts or when spawned
	//	virtual void BeginPlay() override;
	//
	//public:
	//	// Called every frame
	//	virtual void Tick(float DeltaTime) override;
};
