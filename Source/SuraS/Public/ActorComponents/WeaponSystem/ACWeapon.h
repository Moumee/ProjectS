

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"

#include "ActorComponents/WeaponSystem/WeaponName.h"
#include "ActorComponents/WeaponSystem/WeaponType.h"
#include "ActorComponents/WeaponSystem/WeaponFireMode.h"

#include "ACWeapon.generated.h"

//TODO: Character Class를 ASuraCharacterBase로 할지 고민해봐야함
// 근데 우선은 상속받은 클래스로 구현
//class ASuraCharacterPlayer
class ASuraCharacterPlayerWeapon;
class USuraWeaponBaseState;
class USuraWeaponIdleState;
class USuraWeaponFiringState;
class USuraWeaponUnequippedState;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API UACWeapon : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ASuraProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	//TODO: FireMapping을 Character에서 처리하는 것이 나은지 고민해봐야함
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ZoomAction;

	struct FInputBindingHandle* LeftMouseButtonActionBinding;

	struct FInputBindingHandle* RightMouseButtonActionBinding;

	/** Sets default values for this component's properties */
	UACWeapon();

	void InitializeWeapon(ASuraCharacterPlayerWeapon* NewCharacter);

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool AttachWeaponToPlayer(ASuraCharacterPlayerWeapon* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void DetachWeaponFromPlayer();

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ZoomToggle();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ZoomIn();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ZoomOut();


protected:
	// Called when the game starts
	UFUNCTION()
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#pragma region WeaponState

protected:
	UPROPERTY(VisibleAnywhere)
	USuraWeaponBaseState* CurrentState;

	UPROPERTY(VisibleAnywhere)
	USuraWeaponIdleState* IdleState;

	UPROPERTY(VisibleAnywhere)
	USuraWeaponFiringState* FiringState;

	UPROPERTY(VisibleAnywhere)
	USuraWeaponUnequippedState* UnequippedState;

public:
	void ChangeState(USuraWeaponBaseState* NewState);

#pragma endregion

protected:
	/** The Character holding this weapon*/
	ASuraCharacterPlayerWeapon* Character;
	

#pragma region Aim

protected:

	FTransform AimSocketRelativeTransform;

	bool bIsZoomIn = false;

	FVector RightHandToAimSocketOffset;

protected:
	float LineTraceMaxDistance = 10000.f;

	FVector TargetLocationOfProjectile;

public:
	bool PerformLineTrace(FVector StartLocation, FVector LineDirection, float MaxDistance, FVector& HitLocation);

	void PerformMultiLineTrace(FVector StartLocation, FVector EndLocation);

	void SetAimSocketTransform();

	void SetAimSocketRelativeTransform();

	FTransform GetAimSocketRelativeTransform();


#pragma endregion


#pragma region Equip/Unequip

public:
	void EquipWeapon(ASuraCharacterPlayerWeapon* TargetCharacter);

	void UnequipWeapon(ASuraCharacterPlayerWeapon* TargetCharacter);


#pragma endregion

#pragma region UI

protected:
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Weapon|CrosshairWidget")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	UPROPERTY()
	UUserWidget* CrosshairWidget;

public:

	void ActivateCrosshairWidget(bool bflag);

#pragma endregion

#pragma region WeaponType

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponName WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;

public:
	EWeaponName GetWeaponName() const { return WeaponName; }

#pragma endregion

#pragma region FireMode

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponFireMode WeaponFireMode;

	//<Burst Mode>
	FTimerHandle BurstShotTimer;

	float BurstShotFireRate = 0.05f; //TODO: 일단 임시 설정

	const int32 BurstShotCount = 5;

	int32 BurstShotFired = 0;

protected: //내부 로직
	void HandleSingleFire();

	void HandleBurstFire();

	void HandleFullAutoFire();

	// <Busrt Shot>
	void StartBurstFire();

	void StopBurstFire();

#pragma endregion

#pragma region FireMode/FullAuto

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|FireMode")
	float FullAutoShotFireRate = 0.1f;

	FTimerHandle FullAutoShotTimer;

public:

	void StartFullAutoShot();

	void StopFullAutoShot();

#pragma endregion


#pragma region Recoil

protected:
	
	bool bIsRecoiling = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsRecoilRecoverAffectedByPlayerInput = false;

	UPROPERTY(EditAnywhere)
	float RecoilAmountPitch = 1.5f;

	UPROPERTY(EditAnywhere)
	float RecoilRangeMinPitch = 0.8f;

	UPROPERTY(EditAnywhere)
	float RecoilRangeMaxPitch = 1.2f;

	UPROPERTY(EditAnywhere)
	float RecoilAmountYaw = 0.8f;

	UPROPERTY(EditAnywhere)
	float RecoilRangeMinYaw = -1.f;

	UPROPERTY(EditAnywhere)
	float RecoilRangeMaxYaw = 1.f;

	UPROPERTY(EditAnywhere)
	float RecoilSpeed = 4.f;
	UPROPERTY(EditAnywhere)
	float RecoilRecoverSpeed = 3.5f;

	FTimerHandle RecoilRecoverTimer;

	float TotalTargetRecoilValuePitch = 0.f;
	float TotalTargetRecoilValueYaw = 0.f;

	float CulmulatedRecoilValuePitch = 0.f;
	float CulmulatedRecoilValueYaw = 0.f;

	float RecoveredRecoilValuePitch = 0.f;
	float RecoveredRecoilValueYaw = 0.f;

	//-----------------------------------
	//FVector2D PlayerLookInputVector2D = { 0.f, 0.f };

public:

	void AddRecoilValue();

	void ApplyRecoil(float DeltaTime);

	void RecoverRecoil(float DeltaTime);

	void UpdateRecoil(float DeltaTime);



#pragma endregion




};