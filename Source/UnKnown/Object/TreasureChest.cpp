// Fill out your copyright notice in the Description page of Project Settings.

#include "TreasureChest.h"
#include "Engine.h"

// Sets default values
ATreasureChest::ATreasureChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Skeleton(TEXT("StaticMesh'/Game/Modeling/TreasureChest/itemcrete.itemcrete'"));
	StaticMesh->SetStaticMesh(Skeleton.Object);
	StaticMesh->CastShadow = false;
	StaticMesh->RelativeScale3D = FVector(0.5f, 0.5f, 0.5f);
	StaticMesh->bGenerateOverlapEvents = true;
	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ATreasureChest::OnOverlapBegin);
}

void ATreasureChest::ResetActor()
{
}

// Called when the game starts or when spawned
void ATreasureChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATreasureChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATreasureChest::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	level = Cast<AMyLevelScriptActor>(GetWorld()->GetLevelScriptActor());
	AHero * hero = Cast<AHero>(OtherActor);
	if (hero && hero->GetWeaponBox() == OtherComp && hero->GetSwordColli())
	{
		UE_LOG(LogTemp, Warning, TEXT("Hero"));
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);

		int Count = (rand() % 5) + 1;

		for (int a = 0; a < Count; a++)
		{
			int r = level->RandomItemCode();
			level->SpawnItem(FVector(GetActorLocation().X, GetActorLocation().Y, 50), r);
		}	
	}
}