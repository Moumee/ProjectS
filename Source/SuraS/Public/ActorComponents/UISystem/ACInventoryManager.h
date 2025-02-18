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
	UACInventoryManager();

	// UFUNCTION(BlueprintCallable, Category="Inventory")
	
	void UnlockWeapon(FName WeaponName);

protected:
	// TArray<TArray<bool>> WeaponStates; // 무기 상태 (락: false, 언락: true)

	// Inventory 위젯을 저장할 변수
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	// UInventoryWidget* InventoryWidget;
	
};
