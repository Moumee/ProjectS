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

// UBaseUIWidget* UACBaseUIComponent::GetOrCreateWidget(EUIType UIType)
// {
// 	// UIWidgetClasses에서 UIType에 맞는 위젯 클래스를 가져와서 생성
// 	
// 	if (UIWidgetClasses.Contains(UIType))
// 	{
// 		UBaseUIWidget* NewWidget = CreateWidget<UBaseUIWidget>(GetWorld(), UIWidgetClasses[UIType]);
// 		if (NewWidget)
// 		{
// 			UIWidgets.Add(UIType, NewWidget);
// 		}
// 		return NewWidget;
// 	}
// 	
// 	return nullptr;
// }

void UACBaseUIComponent::OpenUI(EUIType UIType)
{
	UBaseUIWidget* TargetWidget = GetWidget(UIType);
	if (!TargetWidget) return; // 위젯 생성에 실패한 경우 처리

	if (!TargetWidget->IsInViewport())
	{
		TargetWidget->OpenUI();
	}
}

UBaseUIWidget* UACBaseUIComponent::GetWidget(EUIType UIType)
{
	// UIType에 해당하는 위젯이 이미 생성되어 있는지 확인
	if (!UIWidgets.Contains(UIType))
	{
		// 위젯이 없다면 새로 생성
		UBaseUIWidget* NewWidget = CreateWidget<UBaseUIWidget>(GetWorld(), UIWidgetClasses[UIType]);
        
		// 새로 생성된 위젯이 있으면 UIWidgets에 저장
		if (NewWidget)
		{
			UIWidgets.Add(UIType, NewWidget);
			return NewWidget;
		}
        
		// 위젯 생성 실패 시 nullptr 반환
		return nullptr;
	}

	// 이미 생성된 위젯이 있으면 반환
	return UIWidgets[UIType];
}



