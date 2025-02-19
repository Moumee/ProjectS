// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/UISystem/ACCrosshairManager.h"

#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Components/Overlay.h"

// Sets default values for this component's properties
UACCrosshairManager::UACCrosshairManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bIsTargeting = false;
	bIsHeadShot = false;

	// crosshair widget class 초기화
	// InitializeCrosshairWidgetClass();

	// crosshair widget 초기화
	// InitializeCrosshairWidget();

	// Overlay 위젯 초기화 및 가져오기
	// if (CrosshairWidget)
	// {
	// 	NormalOverlay = Cast<UOverlay>(CrosshairWidget->GetWidgetFromName(TEXT("NormalDamageOverlay")));
	// 	CriticalOverlay = Cast<UOverlay>(CrosshairWidget->GetWidgetFromName(TEXT("CriticalDamageOverlay")));
	// }
	
}


// Called when the game starts
void UACCrosshairManager::BeginPlay()
{
	Super::BeginPlay();

	// begin play 호출되는지 디버그 메세지
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Crosshair Begin play called");
	
	// 플레이어 컨트롤러 가져오기
	PlayerController = GetWorld()->GetFirstPlayerController();

}


// Called every frame
void UACCrosshairManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 크로스헤어 상태 업데이트
	// UpdateCrosshairState();

	PerformLineTrace();

	// crosshair state debug message
	// if (bIsTargeting)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, TEXT("Targeting: TRUE"));
	// }
	// else
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, TEXT("Targeting: FALSE"));
	// }
	//
	// if (bIsHeadShot)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, TEXT("Headshot: TRUE"));
	// }
	// else
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, TEXT("Headshot: FALSE"));
	// }
}

void UACCrosshairManager::InitializeCrosshairWidget()
{
	// Crosshair을 초기화하고 화면에 띄우기

	if (CrosshairWidgetClass)
	{
		CrosshairWidget = CreateWidget<UCrosshairWidget>(GetWorld(), CrosshairWidgetClass);
		if (CrosshairWidget)
		{
			CrosshairWidget->AddToViewport();
		}
	}

	
}

void UACCrosshairManager::InitializeCrosshairWidgetClass()
{
	// FClassFinder는 특정 자산을 찾는 데 사용되는 클래스입니다.
	// "/Game/Path/To/Your/Blueprints/wbp_crosshair"는 wbp_crosshair 블루프린트의 경로입니다.
	static ConstructorHelpers::FClassFinder<UCrosshairWidget> WBPCrosshair(TEXT("WidgetBlueprint'/Game/UI/Crosshair/WBP_Crosshair.WBP_Crosshair_C'"));
    
	// Succeeded() 메서드는 경로에서 자산을 정상적으로 찾았는지 확인하는 방법입니다.
	if (WBPCrosshair.Succeeded())
	{
		// 자산을 찾으면 CrosshairWidgetClass에 해당 클래스를 할당합니다.
		CrosshairWidgetClass = WBPCrosshair.Class;
	}
	else
	{
		// 만약 자산을 찾지 못하면 로그에 경고를 출력합니다.
		UE_LOG(LogTemp, Warning, TEXT("CrosshairWidgetClass failed to load!"));
	}
}

void UACCrosshairManager::UpdateCrosshairState() const
{
	if (bIsTargeting)
	{
		//적 조준 상태 (헤드샷)
		if (bIsHeadShot)
		{
			// 헤드샷 상태에서 크리티컬 오버레이만 보이도록
			CriticalOverlay->SetVisibility(ESlateVisibility::Visible);
			NormalOverlay->SetVisibility(ESlateVisibility::Collapsed);
		}
		else 
		{
			// 적 조준 상태(일반샷)
			NormalOverlay->SetVisibility(ESlateVisibility::Visible);
			CriticalOverlay->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else // 적 비조준 상태
	{
		NormalOverlay->SetVisibility(ESlateVisibility::Collapsed);
		CriticalOverlay->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}


void UACCrosshairManager::ToggleCrosshairVisibility(bool bIsVisible) const
{
	if (CrosshairWidget)
	{
		CrosshairWidget->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}


void UACCrosshairManager::PerformLineTrace()
{
	if (!PlayerController) return;

	FVector Start;
	FRotator CameraRotation;

	// 카메라 위치 및 방향 가져오기	
	GetCameraViewPoint(Start, CameraRotation);
	
	FVector End = Start + CameraRotation.Vector() * 5000.0f; // 레이 트레이싱 거리 설정

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner()); // 플레이어 캐릭터 자신 무시

	// TraceComplex를 활성화하여 더 정밀한 충돌 처리
	QueryParams.bTraceComplex = true;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult, Start, End, ECC_Visibility, QueryParams);

	// 레이 트레이스를 시각적으로 확인할 수 있게 함
	// DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 1.0f);

	// 히트한 대상을 체크
	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		FName HitBone = HitResult.BoneName;

		// 적을 조준한 경우
		ASuraCharacterEnemyBase* Enemy = Cast<ASuraCharacterEnemyBase>(HitActor);
		if (Enemy)
		{
			bIsTargeting = true;

			// 맞춘 부위 판별
			CheckHitLocation(HitActor, HitBone);
		}
		else
		{
			bIsTargeting = false;
			bIsHeadShot = false;
		}
	}
		
}

void UACCrosshairManager::GetCameraViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	if (PlayerController)
	{
		PlayerController->GetPlayerViewPoint(OutLocation, OutRotation);
	}
}

void UACCrosshairManager::CheckHitLocation(AActor* HitActor, FName HitBone)
{
	if (HitActor && HitActor->IsA(ASuraCharacterEnemyBase::StaticClass())) //적일 때만
	{
		if (HitBone == "head")
		{
			bIsHeadShot = true;
		}
		else
		{
			bIsHeadShot = false;
		}
	}
	else
	{
		bIsHeadShot = false;
	}
}








