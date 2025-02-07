

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ActorComponents/WeaponSystem/WeaponName.h"
#include "InteractInterface.h"

#include "SuraWeaponPickUp.generated.h"

class UACWeapon;
class USuraPickUpComponent;
class ASuraCharacterBase;
class ASuraCharacterPlayerWeapon;

UCLASS()
class SURAS_API ASuraWeaponPickUp : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASuraWeaponPickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UFUNCTION()
	void AttachToCharacter(ASuraCharacterPlayerWeapon* Character);

	UFUNCTION()
	UACWeapon* SpawnWeapon(ASuraCharacterPlayerWeapon* Character);

	void DestroyWeaponPickUp();

	//---------------------------------------------------------------------------------
	// <Interface>
public:

	virtual void Interact(ACharacter* Character) override;


protected:

	/** Weapon class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<UACWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponName WeaponName;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	//UACWeapon* Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	USuraPickUpComponent* PickUpComponent;
	
public:
	//UACWeapon* GetWeapon() const { return Weapon; }

	EWeaponName GetWeaponName() const { return WeaponName; }

};
