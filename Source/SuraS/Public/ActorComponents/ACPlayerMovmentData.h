// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACPlayerMovmentData.generated.h"

struct FPlayerMovementData;
/**
 * 
 */
UCLASS()
class SURAS_API UACPlayerMovementData : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data Table", meta = (AllowPrivateAccess = "true"))
	UDataTable* PlayerMovementDT;

	FPlayerMovementData* PlayerRow;

protected:
	
	virtual void BeginPlay() override;

public:

	float GetWalkSpeed() const;

	float GetWalkRunTransitionDuration() const;
	
	float GetWalkDashTransitionDuration() const;

	float GetRunDashTransitionDuration() const;

	float GetCrouchRunTransitionDuration() const;

	float GetCrouchDashTransitionDuration() const;

	float GetWalkCrouchTransitionDuration() const;

	float GetRunSpeed() const;

	float GetCrouchSpeed() const;

	float GetJumpZVelocity() const;

	float GetJumpXYVelocity() const;

	float GetAirControl() const;

	float GetDashSpeed() const;

	float GetDashImpulseSpeed() const;

	float GetDashDuration() const;

	int GetDashMaxStack() const;

	float GetDashDistance() const;

	float GetDashCooldown() const;
	
};
