// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Player/Hero.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillCircleCollision.generated.h"

UCLASS()
class UNKNOWN_API ASkillCircleCollision : public AActor
{
	GENERATED_BODY()
	
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Sets default values for this actor's properties
	ASkillCircleCollision();

	void SetSetting(int _Damage, AHero * _hero, FVector _Scale, float _KillTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DestroyThis();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UStaticMeshComponent * Body;

	FTimerHandle KillTimer;
	
	int Damage;
	AHero * hero;
};
