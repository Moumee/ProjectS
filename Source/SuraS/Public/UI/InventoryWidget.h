// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUIWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h" // UButton 포함

#include "InventoryWidget.generated.h"

// 위젯 클래스 전방선언
class UTextBlock;
class FReply;
class UWidgetSwitcher;
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
class SURAS_API UInventoryWidget : public UBaseUIWidget	
{
	GENERATED_BODY()

protected:
	// Back 버튼 변수 (디자이너에서 설정된 버튼)
	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;
		
	/**  탭관련  **/
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TabWeapon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TabChip;
	
	EInventoryTab CurrentTab;

	// Widget Switcher
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* ContentSwitcher;

	/** 애니메이션 **/
	// BtnHover 애니메이션
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* BtnHover;

	/** 사운드 **/
	// 탭 전환 효과음
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* TabSwitchSound;
	

private:
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
	void SwitchToWeaponTab();
	void SwitchToChipTab();
	

	// 탭 콘텐츠를 보여주는 함수
	void ShowTabContent() const;

	//BackButton의 Hover 이벤트 구현
	UFUNCTION()
	void OnBackButtonHover();
	UFUNCTION()
	void OnBackButtonUnHover();
	
	
};

