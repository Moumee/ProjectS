#include "InventoryWidget.h"

#include "Characters/Player/SuraCharacterPlayer.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
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
    
}

FReply UInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    // 키 입력 처리: Q 또는 E 키에 대한 처리를 추가
    if (InKeyEvent.GetKey() == EKeys::Q)
    {
        SwitchToPreviousTab();  // Weapon 탭으로 돌아가기
        return FReply::Handled();  // 이벤트 처리 완료
    }
    else if (InKeyEvent.GetKey() == EKeys::E)
    {
        SwitchToNextTab();  // Chip 탭으로 전환
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

    // 콘텐츠 슬라이드
    if (TabContentScrollBox)
    {
        float ScrollOffset = (CurrentTab == EInventoryTab::Weapon) ? 0.0f : TabContentScrollBox->GetDesiredSize().X / 2;
        TabContentScrollBox->SetScrollOffset(ScrollOffset);
    }
    
}

void UInventoryWidget::SwitchToNextTab()
{
    if (CurrentTab == EInventoryTab::Weapon)
    {
        SetActiveTab(EInventoryTab::Chip);
    }
}


void UInventoryWidget::SwitchToPreviousTab()
{
    if (CurrentTab == EInventoryTab::Chip)
    {
        SetActiveTab(EInventoryTab::Weapon);
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
