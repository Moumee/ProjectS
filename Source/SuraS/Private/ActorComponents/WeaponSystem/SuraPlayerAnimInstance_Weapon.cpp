


#include "ActorComponents/WeaponSystem/SuraPlayerAnimInstance_Weapon.h"

#include "ActorComponents/WeaponSystem/SuraCharacterPlayerWeapon.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"
#include "ActorComponents/WeaponSystem/ACWeapon.h"

#include "KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

//SuraPlayerAnimInstance_Weapon::SuraPlayerAnimInstance_Weapon()
//{
//}
//
//SuraPlayerAnimInstance_Weapon::~SuraPlayerAnimInstance_Weapon()
//{
//}

void USuraPlayerAnimInstance_Weapon::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ASuraCharacterPlayerWeapon>(TryGetPawnOwner());

	if (Character)
	{
		SetAimPoint();
	}

	if (Character && Character->GetWeaponSystem())
	{
		//SetAimSocket();
	}
}

void USuraPlayerAnimInstance_Weapon::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (Character)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(Character->GetCharacterMovement()->Velocity);

		Direction = UKismetAnimationLibrary::CalculateDirection(Character->GetCharacterMovement()->Velocity,
			Character->GetActorRotation());

		Pitch = UKismetMathLibrary::NormalizeAxis(Character->GetControlRotation().Pitch);

		CurrentState = Character->GetCurrentState();

		if (Character->GetCharacterMovement()->MovementMode == MOVE_Flying)
		{
			bIsInAir = true;
		}
		else
		{
			if (Character->GetCharacterMovement()->IsFalling())
			{
				bIsInAir = true;
			}
			else
			{
				bIsInAir = false;
			}
		}

		//-----------------------------------------------
		// <Related to Weapon System>

		bIsZoomIn = Character->GetWeaponSystem()->IsZoomIn();

		//ScreenCenterWorldLocation = Character->GetWeaponSystem()->GetScreenCenterWorldPosition();

		//TargetRightHandWorldLocation = Character->GetWeaponSystem()->GetTargetRightHandWorldLocation();

		SetTargetRightHandTransform();


		//TODO: Update 말고 트리거로 작동하게 해야함
		//SetAimSocket();
		//SetAimPoint();
		if (Character->GetWeaponSystem())
		{
			//SetAimSocket();
			SetAimPoint();
			UpdateWeapon();
		}


		// LogTransform(AimSocketRelativeTransform);

	}
}

void USuraPlayerAnimInstance_Weapon::UpdateWeapon()
{
	if (IsValid(Character->GetWeaponSystem())
		&& Character->GetWeaponSystem() != nullptr)
	{
		if (Character->GetWeaponSystem()->GetWeaponNum() != 0
			&& IsValid(Character->GetWeaponSystem()->GetCurrentWeapon())
			&& Character->GetWeaponSystem()->GetCurrentWeapon() != nullptr)
		{
			AimSocketRelativeTransform = Character->GetWeaponSystem()->GetCurrentWeapon()->GetAimSocketRelativeTransform();
		}


		//if (CurrentWeapon != Character->GetWeaponSystem()->GetCurrentWeapon())
		//{
		//	CurrentWeapon = Character->GetWeaponSystem()->GetCurrentWeapon();

		//	//SetAimSocket();
		//	UE_LOG(LogTemp, Warning, TEXT("Weapon Updated!!!"));
		//}

		//if (Character->GetWeaponSystem()->IsZoomIn())
		//{
		//	SetAimSocket();
		//}
	}
}

void USuraPlayerAnimInstance_Weapon::SetAimSocket()
{
	//TODO: Logic 손봐야함
	//if (IsValid(Character->GetWeaponSystem())
	//	&& Character->GetWeaponSystem() != nullptr)
	//{
	//	if (Character->GetWeaponSystem()->GetWeaponNum() != 0
	//		&& IsValid(Character->GetWeaponSystem()->GetCurrentWeapon())
	//		&& Character->GetWeaponSystem()->GetCurrentWeapon() != nullptr)
	//	{
	//		FTransform AimSocketTransform = Character->GetWeaponSystem()->GetCurrentWeapon()->GetSocketTransform(FName(TEXT("Aim")));
	//		FTransform RightHandTransform = Character->GetMesh()->GetSocketTransform(FName(TEXT("ik_hand_gun")));

	//		AimSocketRelativeTransform = AimSocketTransform.GetRelativeTransform(RightHandTransform);
	//	}
	//}
	//else
	//{
	//	FTransform AimSocketTransform = Character->GetMesh()->GetSocketTransform(FName(TEXT("ik_hand_gun")));
	//	FTransform RightHandTransform = Character->GetMesh()->GetSocketTransform(FName(TEXT("ik_hand_gun")));

	//	AimSocketRelativeTransform = AimSocketTransform.GetRelativeTransform(RightHandTransform);
	//}

	if (IsValid(Character->GetWeaponSystem())
		&& Character->GetWeaponSystem() != nullptr)
	{
		if (Character->GetWeaponSystem()->GetWeaponNum() != 0
			&& IsValid(Character->GetWeaponSystem()->GetCurrentWeapon())
			&& Character->GetWeaponSystem()->GetCurrentWeapon() != nullptr)
		{
			FTransform AimSocketTransform = Character->GetWeaponSystem()->GetCurrentWeapon()->GetSocketTransform(FName(TEXT("Aim")));
			FTransform IKHandGunTransform = Character->GetMesh()->GetSocketTransform(FName(TEXT("ik_hand_gun")));

			AimSocketRelativeTransform = AimSocketTransform.GetRelativeTransform(IKHandGunTransform);
			//AimSocketRelativeTransform = IKHandGunTransform.GetRelativeTransform(AimSocketTransform);
		}
	}
	else
	{
		FTransform AimSocketTransform = Character->GetMesh()->GetSocketTransform(FName(TEXT("ik_hand_gun")));
		FTransform IKHandGunTransform = Character->GetMesh()->GetSocketTransform(FName(TEXT("ik_hand_gun")));

		//AimSocketRelativeTransform = AimSocketTransform.GetRelativeTransform(IKHandGunTransform);
		AimSocketRelativeTransform = IKHandGunTransform.GetRelativeTransform(AimSocketTransform);
	}
}

void USuraPlayerAnimInstance_Weapon::SetAimPoint()
{
	FTransform CameraTransform = Character->GetCamera()->GetComponentTransform();
	//FTransform HandRootTransform = Character->GetMesh()->GetSocketTransform(FName(TEXT("ik_hand_root")));
	//FTransform HandRootTransform = Character->GetMesh()->GetBoneTransform(FName(TEXT("ik_hand_root")), ERelativeTransformSpace::RTS_World);
	FTransform HandRootTransform = Character->GetMesh()->GetComponentTransform();

	FTransform CameraRelativeTransform = CameraTransform.GetRelativeTransform(HandRootTransform);
	//FTransform CameraRelativeTransform = HandRootTransform.GetRelativeTransform(CameraTransform);

	float CamOffset = 20.f; //TODO: 임시로 설정했음. 설정가능한 변수로 바꾸기

	AimPointLocation = CameraRelativeTransform.GetLocation() + CameraRelativeTransform.GetRotation().GetForwardVector() * CamOffset;
	AimPointRotation = CameraRelativeTransform.GetRotation().Rotator();
}

void USuraPlayerAnimInstance_Weapon::SetTargetRightHandTransform()
{
	if (IsValid(Character->GetWeaponSystem())
		&& Character->GetWeaponSystem() != nullptr)
	{
		if (Character->GetWeaponSystem()->GetWeaponNum() != 0
			&& IsValid(Character->GetWeaponSystem()->GetCurrentWeapon())
			&& Character->GetWeaponSystem()->GetCurrentWeapon() != nullptr)
		{
			//FTransform RootTransform = Character->GetMesh()->GetBoneTransform(FName(TEXT("ik_hand_root")), ERelativeTransformSpace::RTS_World);
			FTransform RootTransform = Character->GetMesh()->GetComponentTransform();

			FTransform CameraTransform = Character->GetCamera()->GetComponentTransform();
			FTransform WeaponAimSocketTransform = Character->GetWeaponSystem()->GetCurrentWeapon()->GetSocketTransform(FName(TEXT("Aim")));
			FTransform RightHandTransform = Character->GetMesh()->GetBoneTransform(FName(TEXT("hand_r")));

			FTransform CameraRelativeTransform = CameraTransform.GetRelativeTransform(RootTransform);
			FTransform WeaponAimSocketRelativeTransform = WeaponAimSocketTransform.GetRelativeTransform(RootTransform);
			FTransform RightHandRelativeTransform = RightHandTransform.GetRelativeTransform(RootTransform);

			float CamOffset = 50.f; //TODO: 임시로 설정했음. 설정가능한 변수로 바꾸기


			AimPointRelativeLocation = CameraRelativeTransform.GetLocation() + CameraRelativeTransform.GetRotation().GetForwardVector() * CamOffset;
			AimPointRelativeRotation = CameraRelativeTransform.GetRotation().Rotator();

			//LogTransform(CameraRelativeTransform);

			//-------------------------------------------------------------------
			//FTransform AimRelativeToCam = WeaponAimSocketRelativeTransform.GetRelativeTransform(CameraRelativeTransform);
			//FTransform RightHandRelativeToAim = RightHandRelativeTransform.GetRelativeTransform(WeaponAimSocketRelativeTransform);
	
			HandTargetRelativeLocation = AimPointRelativeLocation + (RightHandRelativeTransform.GetLocation() - WeaponAimSocketRelativeTransform.GetLocation());
			HandTargetRelativeRotation = AimPointRelativeRotation + (RightHandRelativeTransform.GetRotation().Rotator() - WeaponAimSocketRelativeTransform.GetRotation().Rotator());
		}
	}
}


void USuraPlayerAnimInstance_Weapon::LogTransform(const FTransform& Transform, const FString& TransformName)
{
	FVector Location = Transform.GetLocation();
	FRotator Rotation = Transform.Rotator();
	FVector Scale = Transform.GetScale3D();

	UE_LOG(LogTemp, Log, TEXT("%s: Location: X=%.2f, Y=%.2f, Z=%.2f | Rotation: Pitch=%.2f, Yaw=%.2f, Roll=%.2f | Scale: X=%.2f, Y=%.2f, Z=%.2f"),
		*TransformName,
		Location.X, Location.Y, Location.Z,
		Rotation.Pitch, Rotation.Yaw, Rotation.Roll,
		Scale.X, Scale.Y, Scale.Z);
}
