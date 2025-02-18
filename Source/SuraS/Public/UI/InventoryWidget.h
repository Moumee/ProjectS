// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUIWidget.h"

#include "InventoryWidget.generated.h"


// 위젯 클래스 전방선언
class UTextBlock;
class FReply;
class UWidgetSwitcher;
class UImage;
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
	/**  탭관련  **/
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TabWeapon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TabChip;
	
	EInventoryTab CurrentTab;

	// Widget Switcher
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* ContentSwitcher;

	/** 사운드 **/
	// 탭 전환 효과음
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* TabSwitchSound;
	
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

	// UI 열고 닫기
	virtual void OpenUI() override;
	
	virtual void CloseUI() override;

#pragma region Weapon
	
	// WBP_Invnetory 초기화
	void InitializeInventory();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	UDataTable* DTWeapon;  // 블루프린트에서 데이터 테이블 할당

	void UnlockWeapon(FName WeaponName);


	/** 총기 umg 바인딩 **/
	UPROPERTY(meta = (BindWidget))
	UImage* Rifle;

	UPROPERTY(meta = (BindWidget))
	UImage* ShotGun;

	UPROPERTY(meta = (BindWidget))
	UImage* MissileLauncher;

	UPROPERTY(meta = (BindWidget))
	UImage* RailGun;

	TMap<FString, UImage*> WeaponImages;

#pragma endregion Weapon

private:
	// 3 x 5 무기 UI 배열
	//TArray<TArray<UImage*>> WeaponImages;
	//TArray<TArray<UTextBlock*>> WeaponNames;

	// lock 이미지
	UPROPERTY()
	UImage* LockImage;
	
	UPROPERTY()
	UImage* LockBackGroundImage;
	
};

