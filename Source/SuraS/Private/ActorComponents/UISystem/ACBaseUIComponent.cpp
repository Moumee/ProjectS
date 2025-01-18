// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/UISystem/ACBaseUIComponent.h"

// Sets default values for this component's properties
UACBaseUIComponent::UACBaseUIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UACBaseUIComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UACBaseUIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UACBaseUIComponent::ShowUI()
{
	// UinventoryWidget을 인스턴스화하고 화면에 표시
	if (!InventoryWidget)
	{
		// UI 위젯 인스턴스화
		InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), UInventoryWidget::StaticClass());
	}

	if (InventoryWidget)
	{
		InventoryWidget->AddToViewport(); // 화면에 UI 추가
	}
}

void UACBaseUIComponent::HideUI()
{
	if (InventoryWidget)
	{
		InventoryWidget->RemoveFromParent(); // UI를 화면에서 제거
	}
}


