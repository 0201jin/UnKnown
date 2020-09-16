// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Player/Hero.h"
#include "Object/ActiveObjectMgr.h"
#include "Monster/Stage1/Boss1.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boss1Spawner.generated.h"

UCLASS()
class UNKNOWN_API ABoss1Spawner : public AActiveObjectMgr
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UBoxComponent * Collision;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void OnCollOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Sets default values for this actor's properties
	ABoss1Spawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetHero(ACharacter * _hero);

protected:
	UStaticMeshComponent * Mesh;
	UMaterial * Mater;
};
