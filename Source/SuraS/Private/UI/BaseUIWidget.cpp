// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseUIWidget.h"
#include "Kismet/GameplayStatics.h"


void UBaseUIWidget::NativeConstruct()
{
	Super::NativeConstruct();
}


// UI를 화면에 띄우는 메서드
void UBaseUIWidget::OpenUI()
{
	AddToViewport();

	// 게임 일시정지
	// UGameplayStatics::SetGamePaused(GetWorld(), true);

	// 위젯에 포커스 주기
	SetKeyboardFocus();
	
	// InputMode를 UI로 변경
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->SetInputMode(FInputModeUIOnly());
		PC->bShowMouseCursor = true;
	}
}

// UI를 화면에서 제거하는 메서드
void UBaseUIWidget::CloseUI()
{
	RemoveFromParent();
        
	// 게임 일시정지 해제
	// UGameplayStatics::SetGamePaused(GetWorld(), false);
        
	// InputMode를 게임 모드로 변경
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
	}
}


