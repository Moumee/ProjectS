// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/UISystem/ACUIMangerComponent.h"
#include "EnhancedInputComponent.h"
#include "ActorComponents/UISystem/ACCrosshairManager.h"


// Sets default values for this component's properties
UACUIMangerComponent::UACUIMangerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
}


// Called when the game starts
void UACUIMangerComponent::BeginPlay()
{
	Super::BeginPlay();
	SetupInput();

	InitializeMangers();

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "UImanager begin play called");
}

void UACUIMangerComponent::SetupInput()
{
	if (APlayerController* PC = Cast<APlayerController>(GetOwner()->GetInstigatorController()))
	{
		if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PC->InputComponent))
		{
			EnhancedInput->BindAction(OpenInventoryAction, ETriggerEvent::Started, this, &UACUIMangerComponent::OpenUI, EUIType::Inventory);
			//EnhancedInput->BindAction(OpenPauseMenuAction, ETriggerEvent::Started, this, &UACBaseUIComponent::ToggleUI, EUIType::PauseMenu);
		}
	}
}

void UACUIMangerComponent::OpenUI(EUIType UIType)
{
	
	UBaseUIWidget* TargetWidget = GetWidget(UIType);
	if (!TargetWidget) return; // 위젯 생성에 실패한 경우 처리

	if (!TargetWidget->IsInViewport())
	{
		TargetWidget->OpenUI();
	}
}

UBaseUIWidget* UACUIMangerComponent::GetWidget(EUIType UIType)
{
	// UIType에 해당하는 위젯이 이미 생성되어 있는지 확인
	if (!UIWidgets.Contains(UIType))
	{
		// 위젯이 없다면 새로 생성
		UBaseUIWidget* NewWidget = CreateWidget<UBaseUIWidget>(GetWorld(), UIWidgetClasses[UIType]);
		UE_LOG(LogTemp, Warning, TEXT("Widget created"));
        
		// 새로 생성된 위젯이 있으면 UIWidgets에 저장
		if (NewWidget)
		{
			UIWidgets.Add(UIType, NewWidget);
		}
		return NewWidget;
	}

	// 이미 생성된 위젯이 있으면 반환
	return UIWidgets[UIType];
}

void UACUIMangerComponent::InitializeMangers()
{
	CrosshairManager = NewObject<UACCrosshairManager>(this, UACCrosshairManager::StaticClass());
	CrosshairManager->RegisterComponent(); // 이 시점에서 해당 manager의 begin play가 호출됨.

	

	// // 인벤토리 매니저 생성 및 등록
	// InventoryManager = NewObject<UACInventoryManager>(this, UACInventoryManager::StaticClass());
	// InventoryManager->RegisterComponent();
}



