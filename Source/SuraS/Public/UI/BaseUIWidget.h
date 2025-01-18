// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UBaseUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 위젯의 초기화 및 공통 기능을 여기에 작성
	virtual void NativeConstruct() override;

	// 기본적인 UI 기능을 여기에 추가
	UFUNCTION(BlueprintCallable, Category = "UI")
	virtual void UpdateUI();
	
};
