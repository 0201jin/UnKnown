// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "Player/Hero.h"
#include "Monster/MonsterMgr.h"
#include "MyLevelScriptActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Ghost.generated.h"

UCLASS()
class UNKNOWN_API AGhost : public AMonsterMgr
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UBoxComponent * WeaponCollision;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USphereComponent * Sphere;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void SphereOnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Sets default values for this pawn's properties
	AGhost();

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

	void SetMonNum(int number, AMyLevelScriptActor * cla, int Stage);

protected:
	void RandomAction();

protected:
	USkeletalMeshComponent * SkeletalMesh;
	
	AHero * Hero;

	FVector RoomLocation;
	FVector RoomSize;
	FVector Destination;

	AAIController* AiController;

	int RoomCount;
	int index;
	int Number;
	int Action;
	int SleepTime;
	int SleepMan;

	GRoom * RoomArray[256];

	int DAMAGE;
};
