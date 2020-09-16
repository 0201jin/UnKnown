// Fill out your copyright notice in the Description page of Project Settings.

#include "Moss.h"

// Sets default values
AMoss::AMoss()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
	StaticMesh->CastShadow = false;
	StaticMesh->RelativeScale3D = FVector(0.5f, 0.5f, 0.5f);
	//StaticMesh->SetCollisionProfileName("OverlapAll");

	SetActorHiddenInGame(true);
}

// Called when the game starts or when spawned
void AMoss::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMoss::SetNum(UStaticMesh * _MossMesh)
{
	StaticMesh->SetStaticMesh(_MossMesh);
}

void AMoss::SetHomeVector(FVector _Location)
{
	HomeLocation = _Location;
}

void AMoss::GoHome()
{
	SetActorLocation(HomeLocation);
	SetActorHiddenInGame(true);
}

void AMoss::ResetActor()
{
	RootComponent->Activate(true);
}

