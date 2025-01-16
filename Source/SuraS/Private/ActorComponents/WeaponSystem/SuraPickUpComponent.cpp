

#include "ActorComponents/WeaponSystem/SuraPickUpComponent.h"


// Sets default values for this component's properties
USuraPickUpComponent::USuraPickUpComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	// PrimaryComponentTick.bCanEverTick = true;

	// ...

	// Setup the Sphere Collision
	SphereRadius = 32.f;


	//TODO: Enable Simulate Physics (Optional)
}


// Called when the game starts
void USuraPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &USuraPickUpComponent::OnSphereBeginOverlap);
}

void USuraPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	ASuraCharacterPlayerWeapon* Character = Cast<ASuraCharacterPlayerWeapon>(OtherActor);
	if (Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);

		UE_LOG(LogTemp, Warning, TEXT("Weapon Collided"));
	}
}


//// Called every frame
//void USuraPickUpComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}


