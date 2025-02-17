// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/InventoryWidget.h"
#include "ACUIMangerComponent.generated.h"

class UInputAction;
class UEnhancedInputComponent;
class UBaseUIWidget;
class UACInventoryManager;
class UACCrosshairManager;


UENUM(BlueprintType)
enum class EUIType : uint8
{
	None UMETA(DisplayName = "None"),
	Inventory UMETA(DisplayName = "Inventory"),
	Pause UMETA(DisplayName = "Pause")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API UACUIMangerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UACUIMangerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetupInput();

public:	
	// Called every frame
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** 특정 UI 열기/ 닫기 **/
	void OpenUI(EUIType UIType);

	UBaseUIWidget* GetWidget(EUIType UIType);

	
private:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* OpenInventoryAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* OpenPauseMenuAction;

	UPROPERTY(EditAnywhere, Category = "UI")
	TMap<EUIType, TSubclassOf<UBaseUIWidget>> UIWidgetClasses; // 위젯 블루프린트 클래스

	/** 생성된 UI 위젯 관리 **/
	UPROPERTY()
	TMap<EUIType, UBaseUIWidget*> UIWidgets;

	/** UI Manager들 **/
	UPROPERTY()
	UACInventoryManager* InventoryManager;

	UPROPERTY()
	UACCrosshairManager* CrosshairManager;

	void InitializeMangers();

	

public:
		
};
