

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	WeaponType_Pistol UMETA(DisplayName = "Pistol"),
	WeaponType_Rifle UMETA(DisplayName = "Rifle")
};

UCLASS()
class SURAS_API AWeaponType : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponType();

//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//public:
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;
};
