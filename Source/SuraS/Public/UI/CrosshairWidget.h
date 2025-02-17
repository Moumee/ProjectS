// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUIWidget.h"
#include "CrosshairWidget.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UCrosshairWidget : public UBaseUIWidget
{
	GENERATED_BODY()

public:
	// 크로스헤어 크기를 조절할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
	float CrosshairSpread;

	// 크로스헤어 선을 조절할 UI 위젯 참조 변수
	UPROPERTY(meta = (BindWidget))
	class UImage* TopCrosshair;

	UPROPERTY(meta = (BindWidget))
	UImage* BottomCrosshair;

	UPROPERTY(meta = (BindWidget))
	UImage* LeftCrosshair;

	UPROPERTY(meta = (BindWidget))
	UImage* RightCrosshair;

	// 애니메이션 함수
	UFUNCTION(BlueprintCallable, Category = "Crosshair")
	void UpdateCrosshairSpread(float DeltaTime, bool bIsMoving, bool bIsFiring);

private:
	float DefaultSpread = 10.0f;
	float MovementSpread = 30.0f;
	float FireSpread = 50.0f;
	float SpreadInterpSpeed = 10.0f;
};
