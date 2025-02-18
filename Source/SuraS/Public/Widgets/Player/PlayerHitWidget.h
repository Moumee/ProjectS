// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "PlayerHitWidget.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UPlayerHitWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UImage* HitEffect = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess = "true"))
	class UWidgetAnimation* FadeInOutAnimation;

public:
	void PlayFadeAnimtion();
};
