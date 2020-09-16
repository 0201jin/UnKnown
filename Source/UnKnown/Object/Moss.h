// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "Object/ObjectMgr.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Moss.generated.h"

UCLASS()
class UNKNOWN_API AMoss : public AObjectMgr
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetNum(UStaticMesh * _MossMesh);
	void SetHomeVector(FVector _Location);
	void GoHome();
	void ResetActor();

protected:
	UStaticMeshComponent * StaticMesh;
	
	FVector HomeLocation;
};
