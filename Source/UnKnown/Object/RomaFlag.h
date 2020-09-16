// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "Object/ObjectMgr.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RomaFlag.generated.h"

UCLASS()
class UNKNOWN_API ARomaFlag : public AObjectMgr
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARomaFlag();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UStaticMeshComponent * StaticMesh;
	
};
