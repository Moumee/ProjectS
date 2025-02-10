// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Animations/SuraEnemyAnimInstance.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"

void USuraEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Enemy = Cast<ASuraCharacterEnemyBase>(TryGetPawnOwner());
}

void USuraEnemyAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}
