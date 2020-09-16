// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "Player/Hero.h"
#include "Monster/MonsterMgr.h"
#include "MyLevelScriptActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MiraMonster.generated.h"

UCLASS()
class UNKNOWN_API AMiraMonster : public AMonsterMgr
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UAIPerceptionComponent * AiPerception;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent * Weapon;

public:
	// Sets default values for this pawn's properties
	AMiraMonster();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetRoomMaxMin(FVector _Location, FVector _Size) { RoomLocation = _Location; RoomSize = _Size; }

	void PutDamage(int damage, AHero * hero);

	void PutDamage(int damage);

	void SetMonNum(int number, AMyLevelScriptActor * cla, int Stage);

	void SetPause();

	void SetUnPause();

protected:
	void RandomAction();

	void ResetAttackedHero();
	void ResetStun();

	void AttackCoolDown();
	void Attack();
	void ShowWeapon();
	UFUNCTION()
		void PerceptionUpdated(const TArray<AActor*>& Actors);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	USkeletalMeshComponent * SkeletalMesh;
	UAISenseConfig_Sight * Sight;

	class UAnimMontage * AttackAnimation;

	UParticleSystem* DeadEffect;

	FVector RoomLocation;
	FVector RoomSize;
	FVector Destination;

	AHero * PlayerHero;
	AHero * AttackedHero;
	AHero * AttackHero;
	AAIController* AiController;
	AMyLevelScriptActor * level;

	FTimerHandle TimerHandle;
	FTimerHandle TimerStun;
	FTimerHandle TimerAttackCool;
	FTimerHandle TimerAttack;
	FTimerHandle TimerShowWeapon;

	int Action;
	int SleepTime;
	int SleepMan;
	int Number;

	int Health;

	bool AttackCool;
	
	int DAMAGE;
};
