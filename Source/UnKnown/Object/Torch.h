// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "Object/ObjectMgr.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Torch.generated.h"

UCLASS()
class UNKNOWN_API ATorch : public AObjectMgr
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Default)
		class UParticleSystemComponent * FireParticle;
	
public:	
	// Sets default values for this actor's properties
	ATorch();

	void ResetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UStaticMeshComponent * StaticMesh;
	UPointLightComponent * PointLight;

	float Bright = 2500.0f;
};
