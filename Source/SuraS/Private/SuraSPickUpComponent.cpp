// Copyright Epic Games, Inc. All Rights Reserved.

#include "SuraSPickUpComponent.h"

USuraSPickUpComponent::USuraSPickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void USuraSPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &USuraSPickUpComponent::OnSphereBeginOverlap);
}

void USuraSPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	ASuraSCharacter* Character = Cast<ASuraSCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
