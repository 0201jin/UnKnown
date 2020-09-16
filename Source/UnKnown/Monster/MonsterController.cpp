// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterController.h"

AMonsterController::AMonsterController()
{
	AiPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));

	AiPerception->bNeverNeedsRenderUpdate = true;
	AiPerception->SetDominantSense(Sight->GetSenseImplementation());
}

void AMonsterController::BeginPlay()
{
	Super::BeginPlay();

	AiPerception->OnPerceptionUpdated.AddDynamic(this, &AMonsterController::PerceptionUpdated);
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), this);
}

void AMonsterController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);
	OnPossess(InPawn);
}

void AMonsterController::OnPossess(APawn * In)
{
	Sight->PeripheralVisionAngleDegrees = 120;
	Sight->SightRadius = 400.0f;
	Sight->LoseSightRadius = 475.0f;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;

	AiPerception->ConfigureSense(*Sight);
}

void AMonsterController::PerceptionUpdated(const TArray<AActor*>& Actors)
{
}
