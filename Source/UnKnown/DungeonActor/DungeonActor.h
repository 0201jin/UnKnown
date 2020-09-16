// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonActor.generated.h"

UCLASS()
class UNKNOWN_API ADungeonActor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ADungeonActor();

	void SetMaterial();
	void SetMatInst(int size, UTexture * tex, UTexture * normal);
	void SetWallMaetInst(UTexture * tex);

protected:
	UStaticMeshComponent * Mesh;
	UMaterial * Mater;
	UMaterial * WallMater;
};
