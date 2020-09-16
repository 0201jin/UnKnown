// Fill out your copyright notice in the Description page of Project Settings.

#include "NextGate.h"
#include "Player/Hero.h"

// Sets default values
ANextGate::ANextGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh1(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetStaticMesh(Mesh1.Object);
	Mesh->SetRelativeScale3D(FVector(5, 5, 0.1));

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->AttachTo(Mesh);
	Box->InitBoxExtent(FVector(50, 50, 180));
	Box->SetRelativeLocation(FVector(0, 0, 90));

	Box->bGenerateOverlapEvents = true;
	Box->OnComponentBeginOverlap.AddDynamic(this, &ANextGate::OnOverlapBegin);
}

void ANextGate::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	level = Cast<AMyLevelScriptActor>(GetWorld()->GetLevelScriptActor());
	AHero * hero = Cast<AHero>(OtherActor);
	
	if (level && hero && level->GetNextFloor() && hero->GetRootComponent() == OtherComp)
	{
		level->NextFloor();

		UE_LOG(LogTemp, Warning, TEXT("OverLap")); 
	}
}
