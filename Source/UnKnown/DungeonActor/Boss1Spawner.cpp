// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss1Spawner.h"
#include "Engine.h"

// Sets default values
ABoss1Spawner::ABoss1Spawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh1(TEXT("StaticMesh'/Game/Modeling/BossSpawner/1/Modeling.Modeling'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Mat(TEXT("Material'/Game/Modeling/BossSpawner/1/Material__4.Material__4'"));

	Mater = Mat.Object;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Mesh;

	Mesh->SetStaticMesh(Mesh1.Object);

	Mesh->SetRelativeScale3D(FVector(1, 1, 1));
	Mesh->SetRelativeRotation(FRotator(0, -90, 0));

	Collision = NewObject<UBoxComponent>(this, TEXT("CollisionBox"));
	Collision->AttachTo(RootComponent);
	Collision->SetRelativeScale3D(FVector(1.5, 1.5, 1.5));
	Collision->bGenerateOverlapEvents = true;
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABoss1Spawner::OnOverlapBegin);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABoss1Spawner::OnCollOverlapEnd);
}

// Called when the game starts or when spawned
void ABoss1Spawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoss1Spawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoss1Spawner::SetHero(ACharacter * _hero)
{
	AHero * hero = Cast<AHero>(_hero);
	UE_LOG(LogTemp, Warning, TEXT("Boss"));

	UStaticMeshComponent * RootCom = Cast<UStaticMeshComponent>(RootComponent);

	if (RootCom)
	{
		RootCom->SetCollisionProfileName("NoCollision");
	}

	ABoss1 * monster = GetWorld()->SpawnActor<ABoss1>(ABoss1::StaticClass(),
		FTransform(FRotator(0, 0, 0),
			FVector(
				GetActorLocation().X,
				GetActorLocation().Y,
				50),
			FVector(0.5, 0.5, 0.5)));

	monster->SpawnDefaultController();
	monster->SetHero(hero);

	Destroy();
}

void ABoss1Spawner::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AHero * hero = Cast<AHero>(OtherActor);
	if (hero && hero->GetActiveBox() == OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hero"));
		hero->SetActiveObject(this);
		hero->bUseActiveObject = true;
	}
}

void ABoss1Spawner::OnCollOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	AHero * hero = Cast<AHero>(OtherActor);
	if (hero && hero->GetActiveBox() == OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("UnHero"));
		hero->bUseActiveObject = false;
	}
}
