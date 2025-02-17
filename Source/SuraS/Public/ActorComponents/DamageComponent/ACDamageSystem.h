// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACDamageSystem.generated.h"

struct FDamageData;

// Event dispatcher
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamaged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API UACDamageSystem : public UActorComponent
{
	GENERATED_BODY()

	float Health = 100.f;
	float MaxHealth = 100.f;
	bool bIsInterruptible = true;
	bool bIsInvincible = false;
	bool bIsDead = false;

public:	
	// Sets default values for this component's properties
	UACDamageSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Setters
	void SetHealth(float Health);
	void SetMaxHealth(float MaxHealth);
	void SetIsInterruptible(bool bIsInterruptible);
	void SetIsInvincible(bool bIsInvincible);
	void SetIsDead(bool bIsDead);

	// Getters
	float GetHealth() const { return Health; }
	float GetMaxHealth() const { return MaxHealth; }

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnDamaged OnDamaged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeath OnDeath;

	UFUNCTION(BlueprintCallable)
	bool TakeDamage(const FDamageData& DamageData, const AActor* DamageCauser);
};
