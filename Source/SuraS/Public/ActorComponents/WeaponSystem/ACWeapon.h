

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"

#include "ActorComponents/WeaponSystem/WeaponName.h"
#include "ActorComponents/WeaponSystem/WeaponType.h"
#include "ActorComponents/WeaponSystem/WeaponFireMode.h"
#include "ActorComponents/WeaponSystem/WeaponCamSettingValue.h"
#include "ActorComponents/WeaponSystem/WeaponInterface.h"
#include "ActorComponents/WeaponSystem/WeaponRecoilStruct.h"
#include "ActorComponents/WeaponSystem/ProjectileSpreadValue.h"

#include "Engine/DataTable.h"
#include "WeaponData.h"

#include "ACWeapon.generated.h"

class ASuraCharacterPlayerWeapon;
class USuraWeaponBaseState;
class USuraWeaponIdleState;
class USuraWeaponFiringState;
class USuraWeaponUnequippedState;
class USuraWeaponReloadingState;
class USuraWeaponSwitchingState;
class USuraWeaponTargetingState;
class USuraWeaponChargingState;
class UWeaponCameraShakeBase;

class UNiagaraSystem;
class UWidgetComponent;
class UAmmoCounterWidget;
class UWeaponAimUIWidget;

class UInputAction;
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
	//TODO: FireMapping�� Character���� ó���ϴ� ���� ������ �����غ�����
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
	UInputAction* HoldAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ChargeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;
	
	TArray<FInputBindingHandle*> InputActionBindingHandles;

	UACWeapon();

	void InitializeWeapon(ASuraCharacterPlayerWeapon* NewCharacter);
	void InitializeCamera(ASuraCharacterPlayerWeapon* NewCharacter);
	void InitializeUI();

	void LoadWeaponData(FName WeaponID);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool AttachWeaponToPlayer(ASuraCharacterPlayerWeapon* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void DetachWeaponFromPlayer();

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void FireSingleProjectile(bool bShouldConsumeAmmo = true, float AdditionalDamage = 0.f, float AdditionalRecoilAmountPitch = 0.f, float AdditionalRecoilAmountYaw = 0.f, float AdditionalProjectileRadius = 0.f, bool bIsHoming = false, AActor* HomingTarget = nullptr);
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
protected: //TODO: public���� �����ϱ�
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

	UPROPERTY(VisibleAnywhere)
	USuraWeaponTargetingState* TargetingState;

	UPROPERTY(VisibleAnywhere)
	USuraWeaponChargingState* ChargingState;

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

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "AimUIWidget")
	TSubclassOf<UWeaponAimUIWidget> AimUIWidgetClass;

	UPROPERTY()
	UWeaponAimUIWidget* AimUIWidget;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Weapon|AmmoCounterWidget")
	//TSubclassOf<UUserWidget> AmmoCounterWidgetClass;
	TSubclassOf<UAmmoCounterWidget> AmmoCounterWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|AmmoCounterWidget")
	UWidgetComponent* AmmoCounterWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|AmmoCounterWidget")
	UAmmoCounterWidget* AmmoCounterWidget;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Weapon|TargetMarkerWidget")
	TSubclassOf<UUserWidget> TargetMarkerWidgetClass;

	//-----------------------------------------------------------
	//TODO: for 3D UI Test
	UPROPERTY()
	UTextureRenderTarget2D* RenderTarget;

	//TODO: 3D UI Test
	UPROPERTY()
	UMaterialInstanceDynamic* WidgetMaterialInstance;

	void Create3DUI();
public:
	void ActivateCrosshairWidget(bool bflag);
	void ActivateAmmoCounterWidget(bool bflag);
protected:
	void SetUpAimUIDelegateBinding(ASuraProjectile* Projectile);
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
	void StartSingleShot(float AdditionalDamage = 0.f, float AdditionalRecoilAmountPitch = 0.f, float AdditionalRecoilAmountYaw = 0.f, float AdditionalProjectileRadius = 0.f);
	void StopSingleShot();
#pragma endregion

#pragma region FireMode/BurstShot
protected:
	FTimerHandle BurstShotTimer;

	UPROPERTY(EditAnywhere)
	float BurstShotFireRate = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

#pragma region FireMode/Targeting
protected:
	UPROPERTY()
	TArray<UUserWidget*> TargetMarkerWidgets;
	UPROPERTY()
	TSet<AActor*> Targets;

	UPROPERTY()
	TMap<AActor*, UUserWidget*> MapTargetActorToWidget;

	FTimerHandle TargetDetectionTimer;

	int32 MaxTargetNum = 10;

	float MaxTargetDetectionRadius = 3000.f;
	float MaxTargetDetectionAngle = 80.f;

	float CurrentTargetDetectionRadius = 0.f;
	float CurrentTargetDetectionAngle = 0.f;

	float MaxTargetDetectionTime = 8.f;
	float TimeToReachMaxTargetDetectionRange = 2.5f;
	float ElapsedTimeAfterTargetingStarted = 0.f;
protected:
	void StartTargetDetection();
	void UpdateTargetDetection(float DeltaTime);
	void StopTargetDetection();

	bool SearchOverlappedActor(FVector CenterLocation, float SearchRadius, TArray<AActor*>& OverlappedActors);
	TTuple<FVector2D, bool> GetScreenPositionOfWorldLocation(const FVector& SearchLocation) const;
	bool IsInViewport(FVector2D ActorScreenPosition, float ScreenRatio_Width = 0.0f, float ScreenRatio_Height = 0.0f) const;
	float GetUnsignedAngleBetweenVectors(const FVector& VectorA, const FVector& VectorB, const FVector& Axis);
	bool CheckIfTargetIsBlockedByObstacle(AActor* target);

	UUserWidget* CreateTargetMarkerWidget(AActor* TargetActor);
public:
	void UpdateTargetMarkers();
	void ResetTargetMarkers();
protected:
	TArray<AActor*> ConfirmedTargets;
	int32 CurrentTargetIndex = 0;
	float MissileLaunchDelay = 0.2;
	FTimerHandle MissileLaunchTimer;
protected:
	void StartMissileLaunch(TArray<AActor*> TargetActors);
	void UpdateMissileLaunch();
	void StopMissileLaunch();
#pragma endregion

#pragma region FireMode/Charging
protected:
	bool bAutoFireAtMaxChargeTime = true;
	float ChargeTimeThreshold = 0.5f;
	float MaxChargeTime = 3.f;
	float ChargingAdditionalDamageBase = 100.f;

	float ChargingAdditionalRecoilAmountPitchBase = 4.f;
	float ChargingAdditionalRecoilAmountYawBase = 1.f;
	float ChargingAdditionalProjectileRadiusBase = 20.f;


	float ElapsedChargeTime = 0.f;
	FTimerHandle ChargingTimer;
protected:
	void StartCharge();
	void UpdateCharge();
	void StopCharge();
#pragma endregion

#pragma region Recoil
protected:
	bool bIsRecoiling = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponRecoilStruct DefaultRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponRecoilStruct ZoomRecoil;

	FTimerHandle RecoilRecoverTimer;

	float TotalTargetRecoilValuePitch = 0.f;
	float TotalTargetRecoilValueYaw = 0.f;

	float CulmulatedRecoilValuePitch = 0.f;
	float CulmulatedRecoilValueYaw = 0.f;

	float RecoveredRecoilValuePitch = 0.f;
	float RecoveredRecoilValueYaw = 0.f;
public:
	void AddRecoilValue(FWeaponRecoilStruct* RecoilStruct = nullptr, float AdditionalRecoilAmountPitch = 0.f, float AdditionalRecoilAmountYaw = 0.f);
	void ApplyRecoil(float DeltaTime, FWeaponRecoilStruct* RecoilStruct = nullptr);
	void RecoverRecoil(float DeltaTime, FWeaponRecoilStruct* RecoilStruct = nullptr);
	void UpdateRecoil(float DeltaTime);
#pragma endregion

#pragma region Projectile/SingleProjectileSpread
protected:
	bool bIsSpreading = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSpread")
	FProjectileSpreadValue DefaultSpread;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSpread")
	FProjectileSpreadValue ZoomSpread;

	float TotalTargetSpreadValue = 0.f;
	float CurrentSpreadVaule = 0.f;
	float SpreadRecoverTimer = 0.f;
protected:
	void AddSpreadValue(FProjectileSpreadValue* SpreadValue = nullptr);
	void ApplySpread(float DeltaTime, FProjectileSpreadValue* SpreadValue = nullptr);
	void RecoverSpread(float DeltaTime, FProjectileSpreadValue* SpreadValue = nullptr);
	void UpdateSpread(float DeltaTime);
	FVector GetRandomSpreadVector(FVector BaseDir);
#pragma endregion

#pragma region Projectile/MultiProjectileSpread
protected:
	UPROPERTY(EditAnywhere)
	int32 PelletsNum = 9;

	UPROPERTY(EditAnywhere)
	float MaxAngleOfMultiProjectileSpread = 15.f;
#pragma endregion

#pragma region Camera
protected:
	bool bIsUsingPlayerCamFov = false;
public:
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
	void ForceStopCamModification();
	bool IsModifyingPlayerCamFov() const { return bIsUsingPlayerCamFov; }
#pragma endregion

#pragma region CameraShake
protected:
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "CameraShake")
	TSubclassOf<UWeaponCameraShakeBase> DefaultCameraShakeClass;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "CameraShake")
	TSubclassOf<UWeaponCameraShakeBase> ZoomCameraShakeClass;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "CameraShake")
	TSubclassOf<UWeaponCameraShakeBase> ChargingCameraShakeClass;
public:
	void ApplyCameraShake(TSubclassOf<UWeaponCameraShakeBase> CamShakeClass = nullptr, float Scale = 1.f);
#pragma endregion
};