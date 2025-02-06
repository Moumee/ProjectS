// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Enemies/EnemyHealthBarWidget.h"

void UEnemyHealthBarWidget::SetHealthBarPercent(float const value)
{
	HealthBar->SetPercent(value);
}
