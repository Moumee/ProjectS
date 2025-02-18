// InventoryManager.cpp
#include "ActorComponents/UISystem/ACInventoryManager.h"
#include "ActorComponents/UISystem/ACUIMangerComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


UACInventoryManager::UACInventoryManager()
{
	
}

// void UACInventoryManager::UnlockWeapon(FName WeaponImageName, FName WeaponTextName)
// {	// 재형님 쪽 코드 배열 값 받아와서 인자로 쓰면 될 듯
// 	
// 	// 테스트용으로 그냥 바로 해당 버튼 바인딩해서 unlock처리
// 	if (!UIManagerComponent)  // UIComponent가 nullptr이면 찾기
// 	{
// 		AActor* Owner = GetOwner();
// 		if (Owner)
// 		{
// 			UIManagerComponent = Owner->FindComponentByClass<UACUIMangerComponent>();
// 		}
// 	}
//
// 	if (!UIManagerComponent)  // UIComponent가 여전히 nullptr이면 로그 출력 후 종료
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("UIComponent is nullptr! Cannot access UI."));
// 		return;
// 	}
// 	
// 	// UIComponent를 통해 InventoryWidget 가져오기
// 	UInventoryWidget* InventoryWidget = Cast<UInventoryWidget>(UIManagerComponent->GetWidget(EUIType::Inventory));
// 	
// 	if (InventoryWidget)
// 	{
// 		UWidget* WeaponImageSlot = InventoryWidget->GetWidgetFromName(WeaponImageName);
// 		UWidget* WeaponTextSlot = InventoryWidget->GetWidgetFromName(WeaponTextName);
// 		
// 		if (WeaponImageSlot)
// 		{
// 			// 총기 이미지 활성화
// 			UImage* WeaponImage = Cast<UImage>(WeaponImageSlot);
// 			if (WeaponImage)
// 			{
// 				WeaponImage->SetColorAndOpacity(FLinearColor(1,1,1,1));
// 			}
// 		}
// 		if (WeaponTextSlot)
// 		{
// 			// 총기 이름 텍스트 활성화
// 			UTextBlock* WeaponText = Cast<UTextBlock>(WeaponTextSlot);
// 			if (WeaponText)
// 			{
// 				WeaponText->SetColorAndOpacity(FLinearColor(1,1,1,1));
// 				WeaponText->SetText(FText::FromString(WeaponText->GetName())); // dt_weapon에서 설정한 이름을 변수로서 가져와야함
// 			}
// 		}
// 	}
// 	
// }


