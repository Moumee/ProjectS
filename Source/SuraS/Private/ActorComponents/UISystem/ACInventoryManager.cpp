// InventoryManager.cpp
#include "ActorComponents/UISystem/ACInventoryManager.h"
#include "ActorComponents/UISystem/ACUIMangerComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


UACInventoryManager::UACInventoryManager()
{
	
}

void UACInventoryManager::OnWeaponCollected(const int32 WeaponType, const int32 WeaponIndex)
{
	
}

void UACInventoryManager::UnlockWeapon(const FName WeaponImageName, const FName WeaponTextName, const FName LockBackgroundName, const FName LockImageName)
{	// 재형님 쪽 코드 배열 값 받아와서 인자로 쓰면 될 듯

	
	// 테스트용으로 그냥 바로 해당 버튼 바인딩해서 unlock처리
	if (!BaseUIComponent)  // UIComponent가 nullptr이면 찾기
	{
		AActor* Owner = GetOwner();
		if (Owner)
		{
			BaseUIComponent = Owner->FindComponentByClass<UACUIMangerComponent>();
		}
	}

	if (!BaseUIComponent)  // UIComponent가 여전히 nullptr이면 로그 출력 후 종료
	{
		UE_LOG(LogTemp, Warning, TEXT("UIComponent is nullptr! Cannot access UI."));
		return;
	}
	
	// UIComponent를 통해 InventoryWidget 가져오기
	UInventoryWidget* InventoryWidget = Cast<UInventoryWidget>(BaseUIComponent->GetWidget(EUIType::Inventory));
	
	if (InventoryWidget)
	{
		UWidget* WeaponImageSlot = InventoryWidget->GetWidgetFromName(WeaponImageName);
		UWidget* WeaponTextSlot = InventoryWidget->GetWidgetFromName(WeaponTextName);
		UWidget* LockBackgroundSlot = InventoryWidget->GetWidgetFromName(LockBackgroundName);
		UWidget* LockImageSlot = InventoryWidget->GetWidgetFromName(LockImageName);
		
		if (WeaponImageSlot)
		{
			// 총기 이미지 활성화
			UImage* WeaponImage = Cast<UImage>(WeaponImageSlot);
			if (WeaponImage)
			{
				//WeaponImage->SetVisibility(ESlateVisibility::Visible);
				WeaponImage->SetColorAndOpacity(FLinearColor(1,1,1,1));
			}
		}
		if (WeaponTextSlot)
		{
			// 총기 이름 텍스트 활성화
			UTextBlock* WeaponText = Cast<UTextBlock>(WeaponTextSlot);
			if (WeaponText)
			{
				//WeaponText->SetVisibility(ESlateVisibility::Visible);
				WeaponText->SetColorAndOpacity(FLinearColor(1,1,1,1));
				WeaponText->SetText(FText::FromString(TEXT("SNIPER1"))); 
				//WeaponText->SetText(FText::FromString(WeaponText->GetName())); // 추후에는 UMG에서 슬롯명 지정하고 해당 슬롯명을 인자로 주면 됨
			}
		}
		if (LockBackgroundSlot)
		{
			UImage* LockBackground = Cast<UImage>(LockBackgroundSlot);
			if (LockBackground)
			{
				LockBackground->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		if (LockImageSlot)
		{
			UImage* LockImage = Cast<UImage>(LockImageSlot);
			if (LockImage)
			{
				LockImage->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
	
}
