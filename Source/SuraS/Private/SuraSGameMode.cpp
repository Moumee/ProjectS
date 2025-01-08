// Copyright Epic Games, Inc. All Rights Reserved.

#include "SuraSGameMode.h"
#include "SuraSCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASuraSGameMode::ASuraSGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
