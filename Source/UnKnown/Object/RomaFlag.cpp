// Fill out your copyright notice in the Description page of Project Settings.

#include "RomaFlag.h"


// Sets default values
ARomaFlag::ARomaFlag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Skeleton(TEXT("StaticMesh'/Game/Modeling/RomaFlag/RomaFlag.RomaFlag'"));
	StaticMesh->SetStaticMesh(Skeleton.Object);
	StaticMesh->CastShadow = false;
	//StaticMesh->Mobility = EComponentMobility::Static;
	StaticMesh->RelativeScale3D = FVector(0.5f, 0.5f, 0.5f);
	StaticMesh->SetCollisionProfileName("OverlapAll");
}

// Called when the game starts or when spawned
void ARomaFlag::BeginPlay()
{
	Super::BeginPlay();
	
}