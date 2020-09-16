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
#include "GameFramework/Character.h"
#include "ShieldMonster.generated.h"

UCLASS()
class UNKNOWN_API AShieldMonster : public AMonsterMgr
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UAIPerceptionComponent * AiPerception;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent * Weapon;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent * Shield;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UBoxComponent * WeaponCollision;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Sets default values for this character's properties
	AShieldMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	void AttackColiReset();

	UFUNCTION()
		void PerceptionUpdated(const TArray<AActor*>& Actors);

protected:
	USkeletalMeshComponent * SkeletalMesh;
	UAISenseConfig_Sight * Sight;

	class UAnimMontage * AttackAnimation;
	class UAnimMontage * AttackAnimation1;

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
	FTimerHandle TimerAttackColi;

	int Action;
	int SleepTime;
	int SleepMan;
	int Number;

	int Health;

	bool AttackCool;
	bool AttackColi;

	int DAMAGE;
};
