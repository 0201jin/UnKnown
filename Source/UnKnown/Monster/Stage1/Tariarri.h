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
#include "Tariarri.generated.h"

UCLASS()
class UNKNOWN_API ATariarri : public AMonsterMgr
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UAIPerceptionComponent * AiPerception;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UBoxComponent * WeaponCollision;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Sets default values for this pawn's properties
	ATariarri();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void PerceptionUpdated(const TArray<AActor*>& Actors);

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
	void Attack();
	void Dash();
	void SpecialAttack();

	void bSetAttack();
	void bSetDash();
	void bSetSpecialAttack();
	void bSetCool();

	void Tracking();
	void EndTracking();

	void RandomAction();

	void ResetAttackedHero();

protected:
	UAISenseConfig_Sight * Sight;
	USkeletalMeshComponent * SkeletalMesh;

	AAIController* AiController;

	class UAnimMontage * AttackAnimation;
	class UAnimMontage * DashAnimation;
	class UAnimMontage * SpecialAttackAnimation;

	UParticleSystem* DeadEffect;

protected:
	FTimerHandle CoolTimer;
	FTimerHandle DashTimer;
	FTimerHandle AttackTimer;
	FTimerHandle Special_AttackTimer;
	FTimerHandle Dash_TrackingTimer;
	FTimerHandle Dash_TrackingEndTimer;
	FTimerHandle TimerHandle;

	AMyLevelScriptActor * level;

	int Health;
	int Action;
	int SleepTime;
	int SleepMan;
	int Number;

	FVector Destination;
	FVector RoomLocation;
	FVector RoomSize;

	bool bAttackCool;
	bool bDashCool;
	bool bSpecialAttackCool;
	bool bCool;
	bool bColli;

	AHero * PlayerHero;
	AHero * AttackedHero;

	int  DAMAGE;
};
