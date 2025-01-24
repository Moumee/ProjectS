// // Fill out your copyright notice in the Description page of Project Settings.
//
//
// #include "Characters/Player/SuraPlayerWallRunningState.h"
//
// #include "ActorComponents/ACWallRun.h"
// #include "Characters/Player/SuraCharacterPlayer.h"
// #include "Characters/Player/SuraPlayerJumpingState.h"
//
// USuraPlayerWallRunningState::USuraPlayerWallRunningState()
// {
// 	StateDisplayName = "Wall Running";
// 	StateType = EPlayerState::WallRunning;
// }
//
//
// void USuraPlayerWallRunningState::EnterState(ASuraCharacterPlayer* Player)
// {
// 	Super::EnterState(Player);
//
// 	Player->JumpsLeft = Player->MaxJumps;
// 	// Player->GetWallRunComponent()->StartWallRunning();
// }
//
// void USuraPlayerWallRunningState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
// {
// 	Super::UpdateState(Player, DeltaTime);
//
// 	// if (Player->GetWallRunComponent()->ShouldWallRun())
// 	// {
// 	// 	Player->GetWallRunComponent()->UpdateWallRun();
// 	// }
//
// 	if (Player->bJumpTriggered)
// 	{
// 		Player->ChangeState(Player->JumpingState);
// 		return;
// 	}
// }
//
// void USuraPlayerWallRunningState::ExitState(ASuraCharacterPlayer* Player)
// {
// 	Super::ExitState(Player);
// }
//
// void USuraPlayerWallRunningState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
// {
// 	Super::Look(Player, InputVector);
// 	Player->AddControllerYawInput(InputVector.X);
// 	Player->AddControllerPitchInput(InputVector.Y);
// }
//
// void USuraPlayerWallRunningState::Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
// {
// 	Super::Move(Player, InputVector);
// 	Player->AddMovementInput(Player->GetActorForwardVector(), InputVector.Y);
// 	Player->AddMovementInput(Player->GetActorRightVector(), InputVector.X);
// }
//
// void USuraPlayerWallRunningState::StartJumping(ASuraCharacterPlayer* Player)
// {
// 	Super::StartJumping(Player);
// 	if (Player->JumpsLeft > 0)
// 	{
// 		Player->JumpsLeft--;
// 		FVector LaunchVelocity = Player->GetWallRunComponent()->FindJumpLaunchVelocity();
// 		Player->LaunchCharacter(LaunchVelocity, false, true);
// 	}
// }
