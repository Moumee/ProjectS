// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoCounterWidget.generated.h"

UCLASS()
class SURAS_API UAmmoCounterWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//UPROPERTY(EditAnyWhere, meta = (BindWidget))
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* AmmoCount;

	void UpdateAmmoCount(int32 NewAmmoCount);
};
