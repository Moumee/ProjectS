

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/SuraPlayerEnums.h"
#include "ActorComponents/WeaponSystem/WeaponStateType.h"
#include "Animation/AnimInstance.h"
#include "SuraPlayerAnimInstance_Weapon.generated.h"

class USuraPlayerBaseState;
class ASuraCharacterPlayerWeapon;
class UACWeapon;
/**
 * 
 */
UCLASS()
class SURAS_API USuraPlayerAnimInstance_Weapon : public UAnimInstance
{
	GENERATED_BODY()
public:
	//SuraPlayerAnimInstance_Weapon();
	//~SuraPlayerAnimInstance_Weapon();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	ASuraCharacterPlayerWeapon* Character;


	//-------------------------------------------------
	// <승환님 Player logic 관련>

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float RunSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bCrouchTriggered;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Pitch;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	USuraPlayerBaseState* CurrentState;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	EPlayerState CurrentStateType;


	//-------------------------------------------------
	// <이재형 Weapon system 관련>

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	EWeaponStateType CurrentWeaponStateType;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	bool bIsZoomIn;

	//TODO: 사용안할 듯. 삭제요망
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FVector ScreenCenterWorldLocation;

	//TODO: 사용안할 듯. 삭제요망
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FVector TargetRightHandWorldLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FTransform AimSocketRelativeTransform;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FTransform AimPointRelativeTransform;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FVector AimPointLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FRotator AimPointRotation;

	//TODO: Logic 바꿔야 함
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	UACWeapon* CurrentWeapon;

	//--------------------------

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FVector AimPointRelativeLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FRotator AimPointRelativeRotation;

	//---------------------------------------

	//UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	//FTransform DefalutCameraRelativeTransform;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FVector HandTargetRelativeLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FRotator HandTargetRelativeRotation;



public:
	void UpdateWeapon();

	void SetAimSocket();

	void SetAimPoint();

	//---------------------------

	void SetTargetRightHandTransform();

	void LogTransform(const FTransform& Transform, const FString& TransformName = TEXT("Transform"));
};
