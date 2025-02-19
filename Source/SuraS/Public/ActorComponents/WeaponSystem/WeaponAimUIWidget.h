// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponAimUIWidget.generated.h"

class UImage;

UCLASS()
class SURAS_API UWeaponAimUIWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
public:

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
	//float CrosshairSpread;

	UPROPERTY(meta = (BindWidget))
	UImage* Dot;

	UPROPERTY(meta = (BindWidget))
	UImage* OutCircle;

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
};
