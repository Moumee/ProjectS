#include "UI/InventoryWidget.h"

#include "ActorComponents/UISystem/ACInventoryManager.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 키보드 입력을 받기 위해 IsFocusable를 true로 설정
    this->SetIsFocusable(true);
    
    // 기본값 설정
    CurrentTab = EInventoryTab::Weapon;
    SetActiveTab(CurrentTab);


#pragma region Weapon

    //const int32 MaxWeaponTypes = 3;  // 예: Rifle, Sniper, Pistol
    //const int32 MaxWeaponSlots = 5;  // 각 무기 타입에 대해 5개의 슬롯

    // Weapon 이미지 및 이름 배열 초기화
    // WeaponImages.SetNum(MaxWeaponTypes);
    // WeaponNames.SetNum(MaxWeaponSlots);

    // for (int32 i = 0; i < MaxWeaponTypes; ++i)
    // {
    //     // WeaponImages[i].SetNum(MaxWeaponSlots);
    //     // WeaponNames[i].SetNum(MaxWeaponSlots);
    // }
    
    // 무기 타입별 슬롯에 해당하는 UI 위젯 할당
    // 구현해야함

    // 공통 Lock 이미지 (모든 잠긴 무기 위에 띄울 자물쇠)
    //LockImage = Cast<UImage>(GetWidgetFromName(TEXT("Lock_Overlay")));

    // 기본적으로 모든 무기 슬롯은 잠겨있다고 가정
    // for (int32 i = 0; i < MaxWeaponTypes; ++i)
    // {
    //     for (int32 j = 0; j < MaxWeaponSlots; ++j)
    //     {
    //         if (WeaponStates[i][j] == false)
    //         {
    //             // 잠금 이미지 보이기
    //             if (WeaponImages[i][j])
    //             {
    //                 WeaponImages[i][j]->SetVisibility(ESlateVisibility::Visible);
    //             }
    //         }
    //     }
    // }
    
#pragma endregion Weapon
 
    
    
}

FReply UInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    // 키 입력 처리: Q 또는 E 키에 대한 처리를 추가
    if (InKeyEvent.GetKey() == EKeys::Q)
    {
        SwitchToWeaponTab();  // Weapon 탭으로 돌아가기
        return FReply::Handled();  // 이벤트 처리 완료
    }
    else if (InKeyEvent.GetKey() == EKeys::E)
    {
        SwitchToChipTab();  // Chip 탭으로 전환
        return FReply::Handled();  // 이벤트 처리 완료
    }

    // 키 입력 처리: I와 Tab 키를 눌러서 인벤토리 닫기
    if (InKeyEvent.GetKey() == EKeys::I || InKeyEvent.GetKey() == EKeys::Tab)
    {
        UInventoryWidget::CloseUI();
        return FReply::Handled();
    }

    if (InKeyEvent.GetKey() == EKeys::T)  // T 키를 눌렀을 때
    {
        // 무기 언락 처리
        if (UACInventoryManager* InventoryManager = GetOwningPlayerPawn()->FindComponentByClass<UACInventoryManager>())
        {
            InventoryManager->UnlockWeapon("WeaponImage1_0","WeaponText1_0", "LockBackground1_0", "LockImage1_0");  // 예시로 UnlockWeapon 함수 호출
            GEngine->AddOnScreenDebugMessage(-1, 2.f , FColor::Red, TEXT("Weapon unlocked"));
        }
    
        return FReply::Handled();
    }

    return FReply::Handled();  // Tab 키 기본 동작 방지
}


void UInventoryWidget::SetActiveTab(EInventoryTab NewTab)
{
    CurrentTab = NewTab;

    // 탭 강조 (불투명도 설정)
    if (TabWeapon && TabChip)
    {
        TabWeapon->SetOpacity(CurrentTab == EInventoryTab::Weapon ? 1.0f : 0.5f);
        TabChip->SetOpacity(CurrentTab == EInventoryTab::Chip ? 1.0f : 0.5f);
    }

    // 활성화된 탭에 해당하는 콘텐츠를 표시
    ShowTabContent();
    
}

void UInventoryWidget::SwitchToWeaponTab()
{
    if (CurrentTab == EInventoryTab::Chip)
    {
        SetActiveTab(EInventoryTab::Weapon);
    }
}

void UInventoryWidget::SwitchToChipTab()
{
    if (CurrentTab == EInventoryTab::Weapon)
    {
        SetActiveTab(EInventoryTab::Chip);
    }
}

void UInventoryWidget::ShowTabContent() const
{
    // 현재 탭에 따라 Widget Switcher의 Index를 전환
    if (ContentSwitcher)
    {
        int32 Index = (CurrentTab == EInventoryTab::Weapon) ? 0 : 1;
        ContentSwitcher->SetActiveWidgetIndex(Index);
        
        // 콘텐츠가 바뀔 때 효과음 추가
        if (USoundBase* SwitchSound = TabSwitchSound)
        {
            UGameplayStatics::PlaySound2D(this, SwitchSound);
        }
    }
   
}

void UInventoryWidget::OpenUI()
{
    Super::OpenUI();
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, "OpenInventory");
}

void UInventoryWidget::CloseUI()
{
    Super::CloseUI();
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, "CloseInventory");
}

void UInventoryWidget::UpdateWeaponSlot(int32 WeaponType, int32 WeaponIndex, bool bIsUnlocked)
{
    
}

