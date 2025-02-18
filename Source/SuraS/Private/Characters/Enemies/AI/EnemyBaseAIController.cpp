// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Player/SuraCharacterPlayer.h" // For detecting the player
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyBaseAIController::AEnemyBaseAIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void AEnemyBaseAIController::InitializeBlackBoard(float StrafeRadius, float AttackRadius, float AttackRate)
{
	GetBlackboardComponent()->SetValueAsFloat("StrafeRadius", StrafeRadius);
	GetBlackboardComponent()->SetValueAsFloat("AttackRadius", AttackRadius);
	GetBlackboardComponent()->SetValueAsFloat("AttackRate", AttackRate);
}

void AEnemyBaseAIController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);

	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(PossessedPawn))
	{
		if (UBehaviorTree* const BehaviorTree = Enemy->GetBehaviorTree())
		{
			UBlackboardComponent* Bboard;
			UseBlackboard(BehaviorTree->BlackboardAsset, Bboard);
			Blackboard = Bboard; // "Blackboard" is an already existing variable name in AAIController class

			RunBehaviorTree(BehaviorTree);

			UpdateCurrentState(EEnemyState::Idle);

			Enemy->SetUpAIController(this);
		}
	}
}

void AEnemyBaseAIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Comp")));

		SightConfig->SightRadius = 1000.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 100.f;
		SightConfig->PeripheralVisionAngleDegrees = 70.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 500.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyBaseAIController::OnTargetSighted);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

void AEnemyBaseAIController::OnTargetSighted(AActor* SeenTarget, FAIStimulus const Stimulus)
{
	if (ASuraCharacterPlayer* const Player = Cast<ASuraCharacterPlayer>(SeenTarget))
	{
		GetBlackboardComponent()->SetValueAsObject("AttackTarget", Player);
		UpdateCurrentState(EEnemyState::Attacking);
	}
}

void AEnemyBaseAIController::UpdateCurrentState(EEnemyState NewState)
{
	CurrentState = NewState;
	GetBlackboardComponent()->SetValueAsEnum("State", (uint8)NewState);
}