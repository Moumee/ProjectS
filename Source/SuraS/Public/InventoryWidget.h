// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h" // UButton 포함
#include "InventoryWidget.generated.h"


class UTextBlock;
class UScrollBox;
class FReply;
/**
 * 
 */
UENUM(BlueprintType)
enum class EInventoryTab : uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Chip UMETA(DisplayName = "Chip")
};

UCLASS()
class SURAS_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// 탭
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TabWeapon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TabChip;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* TabContentScrollBox;
	
	EInventoryTab CurrentTab;

private:
	// Back 버튼 변수 (디자이너에서 설정된 버튼)
	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	// Back 버튼 클릭 시 호출될 함수
	UFUNCTION()
	void OnBackButtonClicked();

	void CloseInventoryByInput();
	
public:
	// 위젯이 생성된 후 호출되는 함수
	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	// 탭 전환 함수
	void SetActiveTab(EInventoryTab NewTab);

	// 키 입력 함수 (Q, E)
	void SwitchToNextTab();
	void SwitchToPreviousTab();

	
	
};
