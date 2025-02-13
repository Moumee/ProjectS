// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/CrosshairWidget.h"
#include "ACCrosshairManager.generated.h"


class UOverlay;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API UACCrosshairManager : public UActorComponent
{
	GENERATED_BODY()

public:
	
	// Sets default values for this component's properties
	UACCrosshairManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// Crosshair 상태 추적
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
	bool bIsTargeting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
	bool bIsHeadShot;

	// Crosshair 위젯 참조
	UPROPERTY()
	UCrosshairWidget* CrosshairWidget;
	
	// Crosshair Widget 클래스 (블루프린트에서 할당)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crosshair")
	TSubclassOf<UCrosshairWidget>CrosshairWidgetClass;

	// 상태별 Overlay
	UPROPERTY()
	UOverlay* NormalOverlay;

	UPROPERTY()
	UOverlay* CriticalOverlay;

public:
	// 크로스 헤어 위젯 초기화
	void InitializeCrosshairWidget();
	void InitializeCrosshairWidgetClass();

	// 크로스헤어 상태 업데이트
	void UpdateCrosshairState() const;
	
	// 크로스헤어 위젯 활성화 / 비활성화
	void ToggleCrosshairVisibility(bool bIsVisible) const;

private:
	void PerformLineTrace(); // 레이 트레이싱 수행
	void GetCameraViewPoint(FVector& OutLocation, FRotator& OutRotation) const; // 카메라 위치 가져오기
	void CheckHitLocation(AActor* HitActor, FName HitBone); // 충돌한 부위 확인

	APlayerController* PlayerController; // 플레이어 컨트롤러 참조

};
