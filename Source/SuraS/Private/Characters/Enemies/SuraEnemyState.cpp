// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SuraEnemyState.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"

EEnemyState USuraEnemyState::GetState() const
{
    return EnemyState;
}

void USuraEnemyState::EnterState(ASuraCharacterEnemyBase* Enemy)
{
}

void USuraEnemyState::UpdateState(ASuraCharacterEnemyBase* Enemy)
{
}

void USuraEnemyState::ExitState(ASuraCharacterEnemyBase* Enemy)
{
}