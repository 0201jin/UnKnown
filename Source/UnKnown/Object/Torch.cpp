// Fill out your copyright notice in the Description page of Project Settings.

#include "Torch.h"


// Sets default values
ATorch::ATorch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Skeleton(TEXT("StaticMesh'/Game/Modeling/Torch/Torch.Torch'"));
	StaticMesh->SetStaticMesh(Skeleton.Object);
	StaticMesh->CastShadow = false;
	//StaticMesh->Mobility = EComponentMobility::Static;
	StaticMesh->RelativeScale3D = FVector(0.5f, 0.5f, 0.5f);
	StaticMesh->SetCollisionProfileName("OverlapAll");

	FireParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	FireParticle->SetupAttachment(StaticMesh);
	ConstructorHelpers::FObjectFinder<UParticleSystem> Fire(TEXT("ParticleSystem'/Game/Effect/Particle/Torch.Torch'"));
	FireParticle->RelativeLocation = FVector(15, 0, 24.88);
	FireParticle->SetTemplate(Fire.Object);

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetupAttachment(FireParticle);
	PointLight->RelativeLocation = FVector(1, 0, 5.6);
	PointLight->Intensity = Bright;
	PointLight->AttenuationRadius = 100;
	PointLight->LightColor = FColor(255, 169.673685, 73.297455);
}

void ATorch::ResetActor()
{
	FireParticle->Activate(true);
}

// Called when the game starts or when spawned
void ATorch::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATorch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

