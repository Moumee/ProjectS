// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/UISystem/ACBaseUIComponent.h"
#include "EnhancedInputComponent.h"


// Sets default values for this component's properties
UACBaseUIComponent::UACBaseUIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
}


// Called when the game starts
void UACBaseUIComponent::BeginPlay()
{
	Super::BeginPlay();
	SetupInput();
}

void UACBaseUIComponent::SetupInput()
{
	if (APlayerController* PC = Cast<APlayerController>(GetOwner()->GetInstigatorController()))
	{
		if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PC->InputComponent))
		{
			EnhancedInput->BindAction(OpenInventoryAction, ETriggerEvent::Started, this, &UACBaseUIComponent::OpenUI, EUIType::Inventory);
			//EnhancedInput->BindAction(OpenPauseMenuAction, ETriggerEvent::Started, this, &UACBaseUIComponent::ToggleUI, EUIType::PauseMenu);
		}
	}
}

UBaseUIWidget* UACBaseUIComponent::GetOrCreateWidget(EUIType UIType)
{
	// if (UIWidgets.Contains(UIType))
	// {
	// 	return UIWidgets[UIType];
	// }
	//
	// if (UIWidgetClasses.Contains(UIType))
	// {
	// 	UBaseUIWidget* NewWidget = CreateWidget<UBaseUIWidget>(GetWorld(), UIWidgetClasses[UIType]);
	// 	if (NewWidget)
	// 	{
	// 		UIWidgets.Add(UIType, NewWidget);
	// 	}
	// 	return NewWidget;
	// }
	// UIWidgetClasses에서 UIType에 맞는 위젯 클래스를 가져와서 생성
	
	if (UIWidgetClasses.Contains(UIType))
	{
		UBaseUIWidget* NewWidget = CreateWidget<UBaseUIWidget>(GetWorld(), UIWidgetClasses[UIType]);
		if (NewWidget)
		{
			UIWidgets.Add(UIType, NewWidget);
		}
		return NewWidget;
	}
	
	return nullptr;
}

void UACBaseUIComponent::OpenUI(EUIType UIType)
{
	UBaseUIWidget* TargetWidget = GetOrCreateWidget(UIType);
	if (!TargetWidget) return;

	if (!TargetWidget->IsInViewport())
	{
		TargetWidget->OpenUI();
	}
}






