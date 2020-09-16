// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Player/Hero.h"
#include "Engine.h"
#include "Object/ObjectMgr.h"
#include "MyLevelScriptActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TreasureChest.generated.h"

UCLASS()
class UNKNOWN_API ATreasureChest : public AObjectMgr
{
	GENERATED_BODY()
	
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Sets default values for this actor's properties
	ATreasureChest();

	void ResetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UStaticMeshComponent * StaticMesh;
	AMyLevelScriptActor * level;
	
};
