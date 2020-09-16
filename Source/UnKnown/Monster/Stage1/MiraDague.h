// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MyLevelScriptActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MiraDague.generated.h"

UCLASS()
class UNKNOWN_API AMiraDague : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UProjectileMovementComponent* BulletMovement;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UBoxComponent * Collision;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Sets default values for this actor's properties
	AMiraDague();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void DestroyF();
	void Colli();

protected:
	UStaticMeshComponent * Body;
	
	FTimerHandle DestoryHandle;
	FTimerHandle ColliHandle;
};
