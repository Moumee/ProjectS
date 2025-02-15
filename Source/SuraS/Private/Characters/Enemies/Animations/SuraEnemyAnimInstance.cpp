// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Animations/SuraEnemyAnimInstance.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void USuraEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Enemy = Cast<ASuraCharacterEnemyBase>(TryGetPawnOwner());

	if (Enemy)
	{
		
	}
}

void USuraEnemyAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (Enemy)
	{
		Velocity = Enemy->GetCharacterMovement()->Velocity;

		GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);

		Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, Enemy->GetActorRotation());
	}
}
