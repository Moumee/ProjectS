// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponAimUIWidget.generated.h"

class ASuraProjectile;

class UImage;
class UOverlay;

UCLASS()
class SURAS_API UWeaponAimUIWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
public:
	UPROPERTY(meta = (BindWidget))
	UImage* Dot;

	UPROPERTY(meta = (BindWidget))
	UImage* OutCircle;

	UPROPERTY(meta = (BindWidget))
	UOverlay* NormalOverlay;

	UPROPERTY(meta = (BindWidget))
	UOverlay* CriticalOverlay;

	UPROPERTY(meta = (BindWidget))
	UImage* TopCrosshair;

	UPROPERTY(meta = (BindWidget))
	UImage* BottomCrosshair;

	UPROPERTY(meta = (BindWidget))
	UImage* LeftCrosshair;

	UPROPERTY(meta = (BindWidget))
	UImage* RightCrosshair;

	void ResetAimUISize();
	void ApplyAimUISpread(float SpreadValue = 0.f);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D DefaultOutCircleSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OutCircleSpreadSizeOffset = 10.f;


#pragma region HitIndicator
protected:
	bool bIsIndicatingNormalHit = false;
	bool bIsIndicatingCriticalHit = false;

	const float TargetNormalOverlayOpacity = 1.f;
	const float TargetCriticalOverlayOpacity = 1.f;

	float CurrentNormalOverlayOpacity = 0.f;
	float CurrentCriticalOverlayOpacity = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NormalOverlayFadeInSpeed = 15.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalOverlayFadeInSpeed = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NormalOverlayFadeOutSpeed = 15.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalOverlayFadeOutSpeed = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NormalOverlayFadeOutStartTime = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalOverlayFadeOutStartTime = 0.2f;

	float NormalOverlayFadeOutTimer = 0.f;
	float CriticalOverlayFadeOutTimer = 0.f;

public:
	void SetUpAimUIDelegateBinding(ASuraProjectile* Projectile);
protected:
	void HeadShot();
	void BodyShot();

	void FadeInCriticalOverlay(float DeltaTime);
	void FadeOutCriticalOverlay(float DeltaTime);

	void FadeInNormalOverlay(float DeltaTime);
	void FadeOutNormalOverlay(float DeltaTime);

	void SetCriticalOvelayInvisible();
	void SetNormalOvelayInvisible();

	void UpdateHitIndicator(float DeltaTime);

#pragma endregion

};
