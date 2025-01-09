// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraCharacterPlayer.h"
#include "ActorComponents/ACWallRun.h"

ASuraCharacterPlayer::ASuraCharacterPlayer()
{
	// Wall-run component
	WallRunComponent = CreateDefaultSubobject<UACWallRun>(TEXT("WallRunComponent"));
	AddOwnedComponent(WallRunComponent);
}
