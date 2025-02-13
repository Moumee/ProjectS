// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CrosshairWidget.h"

void UCrosshairWidget::UpdateCrosshairSpread(float DeltaTime, bool bIsMoving, bool bIsFiring)
{
	float TargetSpread = DefaultSpread;

	if (bIsFiring)
	{
		TargetSpread = FireSpread; // 발사 시 크로스헤어가 크게 확장됨
	}
	else if (bIsMoving)
	{
		TargetSpread = MovementSpread; // 이동 시 크로스헤어 확장
	}

	// 보간
	CrosshairSpread = FMath::FInterpTo(CrosshairSpread, TargetSpread, DeltaTime, SpreadInterpSpeed);
}
