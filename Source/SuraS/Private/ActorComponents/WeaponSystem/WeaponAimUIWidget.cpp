// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/WeaponAimUIWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

void UWeaponAimUIWidget::NativeConstruct()
{
    Super::NativeConstruct();

    //if (OutCircle)
    //{
    //    UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(OutCircle->Slot);
    //    if (CanvasSlot)
    //    {
    //        DefaultOutCircleSize = CanvasSlot->GetSize();
    //    }
    //}

    UE_LOG(LogTemp, Error, TEXT("UWeaponAimUIWidget::NativeConstruct()!!!"));
}

void UWeaponAimUIWidget::ResetAimUISize()
{
    if (OutCircle)
    {
        UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(OutCircle->Slot);
        if (CanvasSlot)
        {
            CanvasSlot->SetSize(DefaultOutCircleSize);
        }
    }
}

void UWeaponAimUIWidget::ApplyAimUISpread(float SpreadValue)
{
    if (OutCircle)
    {
        UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(OutCircle->Slot);
        if (CanvasSlot)
        {
            FVector2D NewSize = { DefaultOutCircleSize.X + SpreadValue * OutCircleSpreadSizeOffset, DefaultOutCircleSize.Y + SpreadValue * OutCircleSpreadSizeOffset };
            CanvasSlot->SetSize(NewSize);
        }
    }
}
