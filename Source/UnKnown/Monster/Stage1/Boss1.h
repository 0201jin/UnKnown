// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "Player/Hero.h"
#include "Monster/MonsterMgr.h"
#include "MyLevelScriptActor.h"
#include "Anim/BossAnim.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Boss1.generated.h"

UCLASS()
class UNKNOWN_API ABoss1 : public AMonsterMgr
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UBoxComponent * WeaponCollision;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Sets default values for this pawn's properties
	ABoss1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Dash();
	void Dash_Dash();

	void Jump();
	void Jump_Jump();
	void Jump_Landing();

	void Back();

	void Attack();
	void AttackNormal1();
	void AttackNormal2();
	void AttackNormal3();
	void MoveLocation();

	void JDT();
	void CanMove();
	void CanRot();
	void CanSA();
	void CanAttack();
	void CanNormalAttack();
	void SetTargetLo();

	void Surprise_Attack();

	void WeaponColli();
	void ClearWeaponColli();
	void ClearMove();

	void SendTired(bool Tired);

	void Begin();
	void End();
	void PlusInstance();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetHero(AHero * _hero) { hero = _hero; }
	void PutDamage(int damage, AHero * hero);
	void PauseBoss();

protected:
	UMaterial * Mater;

	USkeletalMeshComponent * SkeletalMesh;

	class UAnimMontage * AppearAnimation;
	class UAnimMontage * DieAnimation;
	
	class UAnimMontage * DashAnimation;
	class UAnimMontage * JumpAnimation;
	class UAnimMontage * BackAttackAnimation;
	class UAnimMontage * SurpriseAttackAnimation;
	class UAnimMontage * AttackAnimation;
	class UAnimMontage * AttackNormal1Animation;
	class UAnimMontage * AttackNormal2Animation;
	class UAnimMontage * AttackNormal3Animation;

	AAIController* AiController;

protected:
	AHero * hero;

	bool bJD;
	bool bCanMove;
	bool bTired;
	bool bCanRot;
	bool bSA;
	bool bCanAttack;
	bool bCanNormalAttack;
	bool bDead;

	UParticleSystem* JumpEffect;

	FTimerHandle JDTimer;
	FTimerHandle CanMoveTimer;
	FTimerHandle CanRotTimer;
	FTimerHandle CanNormalAttackTimer;
	FTimerHandle TargetTimer;
	FTimerHandle SATimer;
	FTimerHandle CanAttackTimer;

	FTimerHandle Jump_JumpTimer;
	FTimerHandle Jump_LandingTimer;

	FTimerHandle Dash_DashTimer;

	FTimerHandle MoveTimer;
	FTimerHandle ColliTimer;
	FTimerHandle ClearWeaponColliTimer;
	
	int Health;

	float ssfd;
	
	FVector TargetLo;
};
