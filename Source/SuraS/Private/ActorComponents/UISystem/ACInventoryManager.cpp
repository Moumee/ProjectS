// InventoryManager.cpp
#include "ActorComponents/UISystem/ACInventoryManager.h"
#include "ActorComponents/UISystem/ACBaseUIComponent.h"
#include "Components/Image.h"

UACInventoryManager::UACInventoryManager()
{
	// // 3개의 무기 타입 (라이플, 스나이퍼, 권총 - 추후 추가) /  각 타입당 5개의 슬롯(임시)
	// WeaponStates.SetNum(3);
	// for (int32 i = 0; i < 3; i++)
	// {
	// 	WeaponStates[i].SetNum(5, EAllowShrinking::No);
	// }
	//
	// // 기본 지급 무기 설정 ( 각 종류별 첫 번째 무기는 게임 시작 부터 unlock)
	// WeaponStates[0][0] = true;
	// WeaponStates[1][0] = true;
	// WeaponStates[2][0] = true;
}

void UACInventoryManager::OnWeaponCollected(const int32 WeaponType, const int32 WeaponIndex)
{
	// if (WeaponType >= 0 && WeaponType < 3 && WeaponIndex >= 0 && WeaponIndex < 5)
	// {
	// 	// 해당 무기 언락 처리
	// 	WeaponStates[WeaponType][WeaponIndex] = true;
	// 	
	// 	// UI 업데이트
	// 	if (InventoryWidget)
	// 	{
	// 		InventoryWidget->UpdateWeaponSlot(WeaponType, WeaponIndex, true);
	// 	}
	// }
}

void UACInventoryManager::UnlockWeapon()
{
	// // 무기 언락 로직 (여기서는 간단하게 첫 번째 슬롯 언락 예시)
	// WeaponStates[0][1] = true;
	//
	// // UI 업데이트 (예시로 첫 번째 슬롯을 업데이트)
	// if (InventoryWidget)
	// {
	// 	InventoryWidget->UpdateWeaponSlot(0, 1, true);  // 예시로 UI 업데이트
	// }

	// 테스트용으로 그냥 바로 해당 버튼 바인딩해서 unlock처리
	
}
	

