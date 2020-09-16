// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MyLevelScriptActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class UNKNOWN_API AItem : public AActor
{
	GENERATED_BODY()

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UProjectileMovementComponent* BulletMovement;

public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SetItemSetting(UObject * _Image, float _Weight, AMyLevelScriptActor * _Level, FString _Name, FString _Explain, bool Consumables, int _Parts, int _Code);

	UObject * GetImage()
	{
		return Image;
	}

	int GetWeight()
	{
		return Weight;
	}

private:
	void Overlap();
	void DestroyF();

protected:
	USphereComponent * Collision;

	FTimerHandle TimerHandle;
	FTimerHandle DestoryHandle;

	float Weight;
	bool Consumables;
	UObject * Image;
	UMaterial * Mater;
	FString Name;
	FString Explain;
	int Parts;
	UStaticMeshComponent * Body;
	AMyLevelScriptActor * Level;
	int ItemCode;
};
