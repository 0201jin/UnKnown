// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectMgr.generated.h"

UCLASS()
class UNKNOWN_API AObjectMgr : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectMgr();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ResetActor();
	virtual void SetNum(UStaticMesh * _MossMesh);
	virtual void SetHomeVector(FVector _Location);
	virtual void GoHome();
};
