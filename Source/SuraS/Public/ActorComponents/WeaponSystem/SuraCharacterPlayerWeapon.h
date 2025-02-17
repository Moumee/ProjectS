// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "SuraCharacterPlayerWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API ASuraCharacterPlayerWeapon : public ASuraCharacterPlayer
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void Landed(const FHitResult& Hit) override;

public:
	ASuraCharacterPlayerWeapon();

//-----------------------------------------------------------------
//TODO: 승환님께 아래 내용들 Player Character class에 필요하다고 말씀드리기. 
// 또 ASuraCharacterPlayer의 private member를 protected로 전환 부탁드리기

public:
	/** Returns ArmMesh subobject **/
	//USkeletalMeshComponent* GetArmMesh() const { return ArmMesh; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return Camera; }

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Interact();

protected:
#pragma region Input

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	FVector2D PlayerLookInputVector2D;

public:
	void UpdateLookInputVector2D(const FInputActionValue& InputValue);
	void SetLookInputVector2DZero();

	FVector2D GetPlayerLookInputVector() const { return PlayerLookInputVector2D; }

#pragma endregion


#pragma region WeaponSystem

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponSystem")
	class UWeaponSystemComponent* WeaponSystem;

public:
	UFUNCTION()
	FORCEINLINE class UWeaponSystemComponent* GetWeaponSystem() const
	{
		return WeaponSystem;
	}

#pragma endregion

#pragma region Mesh
	UFUNCTION()
	USkeletalMeshComponent* GetArmMesh() { return ArmMesh; }
#pragma endregion

public:
	void InterpCameraFOV(float DeltaTime); // for test



};



