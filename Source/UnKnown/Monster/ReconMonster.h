// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GenerateMap/GenerateRoom/GenerateRoom.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "Player/Hero.h"
#include "Monster/MonsterMgr.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ReconMonster.generated.h"

UCLASS()
class UNKNOWN_API AReconMonster : public AMonsterMgr
{
	GENERATED_BODY()

		UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Sets default values for this character's properties
	AReconMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetRoomInfo(GRoom * _RoomArray[256], int _RoomCount)
	{
		RoomCount = _RoomCount;
		for (int a = 0; a <= 256; a++)
		{
			if (!_RoomArray[a])
			{
				return;
			}
			RoomArray[a] = _RoomArray[a];
		}
	}

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
	void AttackColiReset();

	UFUNCTION()
		void PerceptionUpdated(const TArray<AActor*>& Actors);

protected:
	USkeletalMeshComponent * SkeletalMesh;
	UAISenseConfig_Sight * Sight;
	UAIPerceptionComponent * AiPerception;

	class UAnimMontage * AttackAnimation;
	class UAnimMontage * AttackAnimation1;

	UParticleSystem* DeadEffect;

	UStaticMeshComponent * Weapon;

	UBoxComponent * WeaponCollision;

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
	FTimerHandle TimerAttackColi;

	int RoomCount;
	int index;
	int Number;

	GRoom * RoomArray[256];

	int Action;
	int SleepTime;
	int SleepMan;

	int Health;

	bool AttackCool;
	bool AttackColi;

	int DAMAGE;
};
