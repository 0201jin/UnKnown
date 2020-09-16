// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterController.generated.h"

/**
 * 
 */
UCLASS()
class UNKNOWN_API AMonsterController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMonsterController();

	virtual void BeginPlay() override;
	virtual void Possess(APawn* InPawn) override;
	void OnPossess(APawn* In);

	UAIPerceptionComponent *AiPerception;
	UAISenseConfig_Sight *Sight;

	UFUNCTION()
		void PerceptionUpdated(const TArray<AActor*>& Actors);
	
	
};
