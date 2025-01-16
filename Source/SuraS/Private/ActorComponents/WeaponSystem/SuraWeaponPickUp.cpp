
#include "ActorComponents/WeaponSystem/SuraWeaponPickUp.h"

#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/SuraPickUpComponent.h"

#include "Characters/SuraCharacterBase.h"
#include "ActorComponents/WeaponSystem/SuraCharacterPlayerWeapon.h"


// Sets default values
ASuraWeaponPickUp::ASuraWeaponPickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<UACWeapon>(TEXT("Weapon"));
	Weapon->SetupAttachment(RootComponent);

	PickUpComponent = CreateDefaultSubobject<USuraPickUpComponent>(TEXT("PickUpComponent"));
	PickUpComponent->SetupAttachment(Weapon);
}

// Called when the game starts or when spawned
void ASuraWeaponPickUp::BeginPlay()
{
	Super::BeginPlay();
	
	FAttachmentTransformRules AttachRule(EAttachmentRule::SnapToTarget, true);
	PickUpComponent->AttachToComponent(Weapon, AttachRule);

	if (PickUpComponent)
	{
		PickUpComponent->OnPickUp.AddDynamic(this, &ASuraWeaponPickUp::AttachToCharacter);

		//PickUpComponent->OnHealthChanged.AddDynamic(this, &AMyHUD::UpdateHealthBar);
	}
}

// Called every frame
void ASuraWeaponPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASuraWeaponPickUp::AttachToCharacter(ASuraCharacterPlayerWeapon* Character)
{
	Weapon->AttachWeapon(Character);

	UE_LOG(LogTemp, Warning, TEXT("Weapon Collided"));
}

void PickUpComponent()
{
}


