// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SuraCharacterBase.h"
#include "ActorComponents/ACAttributes.h"

// Sets default values
ASuraCharacterBase::ASuraCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Attrbutes component
	AttributesComponent = CreateDefaultSubobject<UACAttributes>(TEXT("AttributesComponent"));
	AddOwnedComponent(AttributesComponent);
}

// Called when the game starts or when spawned
void ASuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASuraCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASuraCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

