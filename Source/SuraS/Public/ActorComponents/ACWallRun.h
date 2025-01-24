// // Fill out your copyright notice in the Description page of Project Settings.
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "Characters/Player/SuraPlayerEnums.h"
// #include "Components/ActorComponent.h"
// #include "ACWallRun.generated.h"
//
//
// class ASuraCharacterPlayer;
//
// UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
// class SURAS_API UACWallRun : public UActorComponent
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY()
// 	ASuraCharacterPlayer* Player;
//
// 	FHitResult LeftHit;
// 	FHitResult RightHit;
//
// public:	
// 	// Sets default values for this component's properties
// 	UACWallRun();
//
// protected:
// 	// Called when the game starts
// 	virtual void BeginPlay() override;
//
// 	bool IsRunnableWall(const FVector& WallNormal) const;
//
// 	bool bShouldWallRun = false;
//
// 	FVector WallRunDirection = FVector(0, 0, 0);
//
// 	EWallSide WallRunSide;
//
// public:	
// 	// Called every frame
// 	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
// 	
// 	
// 	void StartWallRunning();
// 	void StopWallRunning();
//
// 	UFUNCTION(BlueprintCallable)
// 	void OnCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
// 		FVector NormalImpulse, const FHitResult& Hit);
// 	FVector FindJumpLaunchVelocity();
//
// 	void FindWallRunDirectionAndSide(const FVector& WallNormal, FVector& OutWallRunDirection,
// 	                                 EWallSide& OutWallRunSide);
// 	void UpdateWallRun();
//
// 	bool ShouldWallRun();
// };
