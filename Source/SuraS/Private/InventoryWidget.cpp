#include "InventoryWidget.h"

#include "Characters/Player/SuraCharacterPlayer.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 키보드 입력을 받기 위해 IsFocusable를 true로 설정
    this->SetIsFocusable(true);
    
    // 기본갑 설정
    CurrentTab = EInventoryTab::Weapon;
    SetActiveTab(CurrentTab);
        
    // Back 버튼에 클릭 이벤트 바인딩
    if (BackButton)
    {
        // 버튼 클릭 시 OnBackButtonClicked() 함수 호출
        BackButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnBackButtonClicked);
    }

    // BackButton Hover, UnHover 이벤트 바인딩
    if (BackButton) BackButton->OnHovered.AddDynamic(this, &UInventoryWidget::OnBackButtonHover);
    if (BackButton) BackButton->OnUnhovered.AddDynamic(this, &UInventoryWidget::OnBackButtonUnHover);
    
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

    // 키 입력 처리: I 키를 눌러서 인벤토리 닫기
    if (InKeyEvent.GetKey() == EKeys::One)
    {
        CloseInventoryByInput();
        return FReply::Handled();
    }

    // 기본 처리된 결과를 반환
    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);  // 부모 클래스의 기본 동작을 호출
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

void UInventoryWidget::OnBackButtonClicked()
{
    CloseInventoryByInput();
}

// SuraCharacterPlayer의 CloseInventory 메서드를 호출하는 함수(키입력, 마우스 입력 둘 다 공통적으로 씀)
void UInventoryWidget::CloseInventoryByInput()
{
    // UI를 제거 (인벤토리 창 닫기)
    RemoveFromParent();

    // 플레이어 캐릭터에게 알림 (게임 상태 관리 위임)
    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        if (ASuraCharacterPlayer* Player = Cast<ASuraCharacterPlayer>(PlayerController->GetPawn()))
        {
            Player->CloseInventory(); // 캐릭터에서 상태 관리
        }
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

void UInventoryWidget::OnBackButtonHover()
{
    if (BtnHover) PlayAnimation(BtnHover);
}

void UInventoryWidget::OnBackButtonUnHover()
{
    if (BtnHover) PlayAnimation(BtnHover, 0.0f, 1, EUMGSequencePlayMode::Reverse);
}




