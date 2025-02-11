// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "EnemyHealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UEnemyHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UProgressBar* HealthBar = nullptr;

// protected:
	// Called when the game starts
	// virtual bool Initialize() override;
	
public:
	void SetHealthBarPercent(float const value);
};
