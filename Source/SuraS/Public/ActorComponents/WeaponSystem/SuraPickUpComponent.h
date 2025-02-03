

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Characters/SuraCharacterBase.h" //TODO: Base Character class여야 하는지 고민해봐야함
#include "ActorComponents/WeaponSystem/SuraCharacterPlayerWeapon.h"

#include "SuraPickUpComponent.generated.h"

//TODO: Delegate 말고 Interface로 구현하는게 좋을지 고민해봐야함
// Declaration of the delegate that will be called when someone picks this up
// The character picking this up is the parameter sent with the notification
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUpWeapon, ASuraCharacterPlayerWeapon*, PickUpCharacter);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API USuraPickUpComponent : public USphereComponent
{
	GENERATED_BODY()

public:	

	//TODO: Interface로 구현할지 고민해봐야함
	/** Delegate to whom anyone can subscribe to receive this event */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnPickUpWeapon OnPickUp;

	// Sets default values for this component's properties
	USuraPickUpComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Code for when something overlaps this component */
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


//public:	
//	// Called every frame
//	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	
};
