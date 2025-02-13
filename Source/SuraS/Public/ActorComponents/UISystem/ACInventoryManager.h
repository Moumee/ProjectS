// ACInventoryManager.h
#pragma once

#include "CoreMinimal.h"
#include "ACUIMangerComponent.h"
#include "Components/ActorComponent.h"
#include "ACInventoryManager.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURAS_API UACInventoryManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Constructor and other functions here
	UACInventoryManager();

	// 무기 획득 시 호출되는 함수
	UFUNCTION()
	void OnWeaponCollected(int32 WeaponType ,int32 WeaponIndex);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void UnlockWeapon(FName WeaponImageName, FName WeaponTextName, FName LockBackground, FName LockImage);

protected:
	TArray<TArray<bool>> WeaponStates; // 무기 상태 (락: false, 언락: true)

	// Inventory 위젯을 저장할 변수
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	// UInventoryWidget* InventoryWidget;

	UACUIMangerComponent* BaseUIComponent;
};
