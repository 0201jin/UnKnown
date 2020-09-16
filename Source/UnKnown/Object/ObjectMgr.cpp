// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectMgr.h"


// Sets default values
AObjectMgr::AObjectMgr()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AObjectMgr::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObjectMgr::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObjectMgr::ResetActor()
{
}

void AObjectMgr::SetNum(UStaticMesh * _MossMesh)
{
}

void AObjectMgr::SetHomeVector(FVector _Location)
{
}

void AObjectMgr::GoHome()
{
}

