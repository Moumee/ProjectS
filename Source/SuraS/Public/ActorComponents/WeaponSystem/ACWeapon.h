

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"

#include "ActorComponents/WeaponSystem/WeaponName.h"
#include "ActorComponents/WeaponSystem/WeaponType.h"
#include "ActorComponents/WeaponSystem/WeaponFireMode.h"
#include "ActorComponents/WeaponSystem/WeaponCamSettingValue.h"
#include "ActorComponents/WeaponSystem/WeaponInterface.h"

#include "Engine/DataTable.h"
#include "WeaponData.h"

#include "ACWeapon.generated.h"

//TODO: Character Class를 ASuraCharacterBase로 할지 고민해봐야함
// 근데 우선은 상속받은 클래스로 구현
class ASuraCharacterPlayerWeapon;
class USuraWeaponBaseState;
class USuraWeaponIdleState;
class USuraWeaponFiringState;
class USuraWeaponUnequippedState;
class USuraWeaponReloadingState;
class USuraWeaponSwitchingState;
class UWeaponCameraShakeBase;

class UNiagaraSystem;
class UWidgetComponent;
class UAmmoCounterWidget;

struct FInputBindingHandle;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API UACWeapon : public USkeletalMeshComponent, public IWeaponInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	UDataTable* WeaponDataTable;

	FWeaponData* WeaponData;

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
	class UInputAction* FireSingleShotAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireBurstShotAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;
	

	FInputBindingHandle* LeftMouseButtonActionBinding;
	FInputBindingHandle* RightMouseButtonActionBinding;
	FInputBindingHandle* RButtonActionBinding;

	TArray<FInputBindingHandle*> InputActionBindingHandles;

	UACWeapon();

	void InitializeWeapon(ASuraCharacterPlayerWeapon* NewCharacter);
	void InitializeCamera(ASuraCharacterPlayerWeapon* NewCharacter);
	void InitializeUI();

	void LoadWeaponData(FName WeaponID);

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool AttachWeaponToPlayer(ASuraCharacterPlayerWeapon* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void DetachWeaponFromPlayer();

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void FireSingleProjectile();
	void FireMultiProjectile();
	void SpawnProjectile();

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
protected: //TODO: public으로 수정하기
	UPROPERTY(VisibleAnywhere)
	USuraWeaponBaseState* CurrentState;

	UPROPERTY(VisibleAnywhere)
	USuraWeaponIdleState* IdleState;

	UPROPERTY(VisibleAnywhere)
	USuraWeaponFiringState* FiringState;

	UPROPERTY(VisibleAnywhere)
	USuraWeaponUnequippedState* UnequippedState;

	UPROPERTY(VisibleAnywhere)
	USuraWeaponReloadingState* ReloadingState;

	UPROPERTY(VisibleAnywhere)
	USuraWeaponSwitchingState* SwitchingState;

public:
	UFUNCTION()
	USuraWeaponBaseState* GetCurrentState() const { return CurrentState; }

	void ChangeState(USuraWeaponBaseState* NewState);
#pragma endregion

protected:
	/** The Character holding this weapon*/
	ASuraCharacterPlayerWeapon* Character;
	
	APlayerController* CharacterController;
		
#pragma region Animation
protected:
	void StartFireAnimation(UAnimMontage* CharacterFireAnimation, UAnimMontage* WeaponFireAnimation);
	void StartAnimation(UAnimMontage* CharacterAnimation, UAnimMontage* WeaponAnimation, float CharacterAnimPlayRate, float WeaponAnimPlayRate);
	void CancelAnimation(UAnimMontage* CharacterAnimation, UAnimMontage* WeaponAnimation);
#pragma endregion

#pragma region Animation/Character
protected:
	UPROPERTY()
	UAnimInstance* CharacterAnimInstance;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Character")
	UAnimMontage* AM_Fire_Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Character")
	UAnimMontage* AM_Reload_Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Character")
	UAnimMontage* AM_Equip_Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Character")
	UAnimMontage* AM_Unequip_Character;
#pragma endregion

#pragma region Animation/Weapon
protected:
	UPROPERTY()
	UAnimInstance* WeaponAnimInstance;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Weapon")
	UAnimMontage* AM_Fire_Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Weapon")
	UAnimMontage* AM_Reload_Weapon;
#pragma endregion

#pragma region Niagara
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* MuzzleFireEffect;
public:
	void SpawnMuzzleFireEffect(FVector SpawnLocation, FRotator SpawnRotation);
#pragma endregion

#pragma region Aim
protected:
	FTransform AimSocketRelativeTransform;

	bool bIsZoomIn = false;

	FVector RightHandToAimSocketOffset;

protected:
	float LineTraceMaxDistance = 10000.f;

	UPROPERTY(EditAnywhere)
	float SphereTraceRadius = 20.f;

	FVector ScreenCenterWorldPosition;
	FVector ScreenCenterWorldDirection;

	FVector TargetLocationOfProjectile;

public:
	bool PerformLineTrace(FVector StartLocation, FVector LineDirection, float MaxDistance, FVector& HitLocation);
	bool PerformSphereTrace(FVector StartLocation, FVector TraceDirection, float MaxDistance, float SphereRadius, FVector& HitLocation);

	FVector CalculateScreenCenterWorldPositionAndDirection(FVector& OutWorldPosition, FVector& OutWorldDirection) const;




	void SetAimSocketTransform();
	void SetAimSocketRelativeTransform();
	FTransform GetAimSocketRelativeTransform();
#pragma endregion

#pragma region Equip/Unequip
protected:
	UPROPERTY(EditAnywhere)
	float WeaponSwitchingRate = 1.f;
	FTimerHandle SwitchingTimer;
public:
	void SwitchWeapon(ASuraCharacterPlayerWeapon* TargetCharacter, bool bEquip);
	void EndWeaponSwitch(ASuraCharacterPlayerWeapon* TargetCharacter, bool bEquip);
	void EquipWeapon(ASuraCharacterPlayerWeapon* TargetCharacter);
	void UnequipWeapon(ASuraCharacterPlayerWeapon* TargetCharacter);
	void SetInputActionBinding();
	void ResetInputActionBinding();
#pragma endregion

#pragma region Reload
protected:
	UPROPERTY(EditAnywhere)
	bool bCanAutoReload = true;
	//TODO: Auto Reload true일 때, 각각의 fire가 끝나면, Check ammo를 통해 남은 탄환수를 확인하고 0이면 자동으로 reloading으로 넘어가기

	UPROPERTY(EditAnywhere)
	float ReloadingTime = 2.5f;

	UPROPERTY(EditAnywhere)
	int32 MaxAmmo = 20.f;

	UPROPERTY(EditAnywhere)
	int32 NumOfLeftAmmo;

	FTimerHandle ReloadingTimer;
protected:
	void HandleReload();
	void CancelReload();
public:
	void StartReload();
protected:
	void StopReload();

	void ConsumeAmmo();
	void ReloadAmmo();
	bool HasAmmo();
public:
	void AutoReload();
	virtual void ReloadingEnd() override; //Legacy
#pragma endregion

#pragma region UI
protected:
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Weapon|CrosshairWidget")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	UPROPERTY()
	UUserWidget* CrosshairWidget;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Weapon|AmmoCounterWidget")
	//TSubclassOf<UUserWidget> AmmoCounterWidgetClass;
	TSubclassOf<UAmmoCounterWidget> AmmoCounterWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|AmmoCounterWidget")
	UWidgetComponent* AmmoCounterWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|AmmoCounterWidget")
	UAmmoCounterWidget* AmmoCounterWidget;
	//UUserWidget* AmmoCounterWidget;

	//-----------------------------------------------------------
	UPROPERTY()
	UTextureRenderTarget2D* RenderTarget;

	// 3D UI의 머티리얼 인스턴스
	UPROPERTY()
	UMaterialInstanceDynamic* WidgetMaterialInstance;

	void Create3DUI();
public:
	void ActivateCrosshairWidget(bool bflag);
	void ActivateAmmoCounterWidget(bool bflag);
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
protected:
	void HandleSingleFire();
	void HandleBurstFire();
	void HandleFullAutoFire();
#pragma endregion

#pragma region FireMode/SingleShot
protected:
	FTimerHandle SingleShotTimer;

	UPROPERTY(EditAnywhere)
	float SingleShotDelay = 1.f;
public:
	void StartSingleShot();
	void StopSingleShot();
#pragma endregion

#pragma region FireMode/BurstShot
protected:
	FTimerHandle BurstShotTimer;

	UPROPERTY(EditAnywhere)
	float BurstShotFireRate = 0.1f;

	UPROPERTY(EditAnywhere)
	int32 BurstShotCount = 3;

	int32 BurstShotFired = 0;

protected:
	void StartBurstFire(bool bMultiProjectile = false);
	void StopBurstFire();
#pragma endregion

#pragma region FireMode/FullAuto

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|FireMode")
	float FullAutoShotFireRate = 0.1f;

	FTimerHandle FullAutoShotTimer;

protected:
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
public:
	void AddRecoilValue();
	void ApplyRecoil(float DeltaTime);
	void RecoverRecoil(float DeltaTime);
	void UpdateRecoil(float DeltaTime);
#pragma endregion

#pragma region Projectile/Spread
protected:
	UPROPERTY(EditAnywhere)
	int32 PelletsNum = 9;

	UPROPERTY(EditAnywhere)
	float MaxAngleOfProjectileSpread = 15.f;
#pragma endregion

#pragma region Camera
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponCamSettingValue CamSetting_Default = FWeaponCamSettingValue(
		90.f,
		{ 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 70.f },
		1.f, 1.f, 15.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponCamSettingValue CamSetting_ZoomIn = FWeaponCamSettingValue(
		90.f,
		{ 0.f, 0.f, 0.f },
		{ 50.f, 0.f, 70.f },
		1.f, 1.f, 15.f);

	FTimerHandle CamSettingTimer;
public:
	void StartCameraSettingChange(FWeaponCamSettingValue* CamSetting);

	void UpdateCameraSetting(float DeltaTime, FWeaponCamSettingValue* CamSetting);

	void StopCameraSettingChange();
#pragma endregion

#pragma region CameraShake
protected:
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Weapon|CameraShake")
	TSubclassOf<UWeaponCameraShakeBase> CameraShakeClass;
public:
	void ApplyCameraShake();
#pragma endregion
};