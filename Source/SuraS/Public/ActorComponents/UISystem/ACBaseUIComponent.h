// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/InventoryWidget.h"

#include "ACBaseUIComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API UACBaseUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UACBaseUIComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// UI 위젯을 저장할 변수
	UInventoryWidget* InventoryWidget;

public:
	// UI를 띄우는 함수
	void ShowUI();

	// UI를 숨기는 함수
	void HideUI();

		
};
