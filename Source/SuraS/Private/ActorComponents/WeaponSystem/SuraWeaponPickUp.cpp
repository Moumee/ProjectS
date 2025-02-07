
#include "ActorComponents/WeaponSystem/SuraWeaponPickUp.h"

#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/SuraPickUpComponent.h"
#include "ActorComponents/WeaponSystem/WeaponName.h"

#include "Characters/SuraCharacterBase.h"
#include "ActorComponents/WeaponSystem/SuraCharacterPlayerWeapon.h"


// Sets default values
ASuraWeaponPickUp::ASuraWeaponPickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Weapon = CreateDefaultSubobject<UACWeapon>(TEXT("Weapon"));
	//Weapon->SetupAttachment(RootComponent);

	WeaponName = EWeaponName::WeaponName_Rifle;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	// TODO: 어차피 필요 없는 것 같아서 삭제해야함
	PickUpComponent = CreateDefaultSubobject<USuraPickUpComponent>(TEXT("PickUpComponent"));
	PickUpComponent->SetupAttachment(WeaponMesh);
}

// Called when the game starts or when spawned
void ASuraWeaponPickUp::BeginPlay()
{
	Super::BeginPlay();
	
	FAttachmentTransformRules AttachRule(EAttachmentRule::SnapToTarget, true);
	//PickUpComponent->AttachToComponent(Weapon, AttachRule);
	PickUpComponent->AttachToComponent(WeaponMesh, AttachRule);

	////TODO: 방식 바꿨기 때문에 해제함
	//if (PickUpComponent)
	//{
	//	PickUpComponent->OnPickUp.AddDynamic(this, &ASuraWeaponPickUp::AttachToCharacter);
	//}
}

// Called every frame
void ASuraWeaponPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASuraWeaponPickUp::AttachToCharacter(ASuraCharacterPlayerWeapon* Character)
{
	//Weapon->AttachWeaponToPlayer(Character);

	UE_LOG(LogTemp, Warning, TEXT("Weapon has been spwaned"));
}


UACWeapon* ASuraWeaponPickUp::SpawnWeapon(ASuraCharacterPlayerWeapon* Character)
{
	UACWeapon* NewWeapon = nullptr;
	
	if (WeaponClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr && Character != nullptr)
		{
			//const FVector SpawnLocation = Character->GetActorLocation();
			//const FRotator SpawnRotation = Character->GetActorRotation();

			//FActorSpawnParameters ActorSpawnParams;
			//ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			NewWeapon = NewObject<UACWeapon>(Character, WeaponClass);
			NewWeapon->RegisterComponent();
			//NewWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));

			//NewWeapon = World->SpawnActor<UACWeapon>(WeaponClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			NewWeapon->InitializeWeapon(Character);
		}
	}

	return NewWeapon;
}

void ASuraWeaponPickUp::DestroyWeaponPickUp()
{
	Destroy();
}

void ASuraWeaponPickUp::Interact(ACharacter* Character)
{
	ASuraCharacterPlayerWeapon* CharacterPickingUp = Cast<ASuraCharacterPlayerWeapon>(Character);
	if (CharacterPickingUp)
	{
		//Weapon->AttachWeaponToPlayer(CharacterPickingUp);
		UE_LOG(LogTemp, Warning, TEXT("Weapon has been spwaned"));
	}
}


void PickUpComponent() //TODO: ???
{

}


