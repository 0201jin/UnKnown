// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "MyLevelScriptActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NextGate.generated.h"

UCLASS()
class UNKNOWN_API ANextGate : public AActor
{
	GENERATED_BODY()

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:	
	// Sets default values for this actor's properties
	ANextGate();

protected:
	UStaticMeshComponent * Mesh;
	UBoxComponent * Box;
	AMyLevelScriptActor * level;
};
