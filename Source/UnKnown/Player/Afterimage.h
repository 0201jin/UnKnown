// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "Player/Hero.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Afterimage.generated.h"

UCLASS()
class UNKNOWN_API AAfterimage : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent * Weapon;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent * ArmourMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UBoxComponent * WeaponCollision;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent * SkeletalMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UCapsuleComponent * Capsule;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Sets default values for this actor's properties
	AAfterimage();

	void Attack(AHero * _hero, int _AttackDamage, int _AttackCount);
	void WSkill(AHero * _hero, int _AttackDamage);
	void ESkill(AHero * _hero, int _AttackDamage);

	void ReinWSkill(AHero * _hero, int _AttackDamage);
	void ReinESkill(AHero * _hero, int _AttackDamage);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle WSkill_Damage_Timer;

	FTimerHandle Destroy_Timer;

	FTimerHandle ReinWSkill_Jump_Timer;
	FTimerHandle ReinWSkill_Land_Timer;

	FTimerHandle ReinESkill_Dash_Timer;
	FTimerHandle ReinESkill_End_Dash_Timer;

	FTimerHandle ReinRSkill_Circle_Timer;

	void WSkill_Damage();

	void ReinWSkill_Jump();
	void ReinWSkill_Land();

	void ReinESkill_Dash();
	void ReinESkill_End_Dash();

	void ReinRSkill_Circle();

	void TDestroy();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UMaterial * Mater;

	AHero * hero;

	int AttackDamage;

	class UAnimMontage * NormalAttack1_Animation;
	class UAnimMontage * NormalAttack2_Animation;
	class UAnimMontage * NormalAttack3_Animation;
	class UAnimMontage * W_Skill_Animation;
	class UAnimMontage * E_Skill_Animation;
	class UAnimMontage * Rein_W_Skill_Animation;
	class UAnimMontage * Rein_E_Skill_Animation;
	class UAnimMontage * Rein_R_Skill_Animation;

	UParticleSystem * W_Effect;
	
};
