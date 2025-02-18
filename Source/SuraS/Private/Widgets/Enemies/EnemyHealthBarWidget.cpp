// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Enemies/EnemyHealthBarWidget.h"

void UEnemyHealthBarWidget::SetHealthBarPercent(float const Value)
{
	HealthBar->SetPercent(Value);
}

void UEnemyHealthBarWidget::ResizeHealthBar(FVector2D NewSize)
{
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(HealthBar->Slot);
	CanvasSlot->SetSize(NewSize);
}

void UEnemyHealthBarWidget::HideInGame(bool bHidden)
{
	if (bHidden)
		SetVisibility(ESlateVisibility::Hidden);
	else
		SetVisibility(ESlateVisibility::Visible);
}

FVector2D UEnemyHealthBarWidget::GetHealthBarSize()
{
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(HealthBar->Slot);
	return CanvasSlot->GetSize();
}

void UEnemyHealthBarWidget::PlayFadeAnimtion()
{
	PlayAnimation(FadeInOutAnimation);
}