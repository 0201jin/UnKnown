// Fill out your copyright notice in the Description page of Project Settings.

#include "MiraDague.h"
#include "Engine.h"
#include "Player/Hero.h"

// Sets default values
AMiraDague::AMiraDague()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("StaticMesh'/Game/Modeling/Monster/Stage1/RangeMira/MiraSword.MiraSword'"));

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Body;

	Body->SetStaticMesh(Mesh.Object);
	Body->SetRelativeScale3D(FVector(0.5, 0.5, 0.5));
	Body->SetRelativeRotation(FRotator(0, 0, 45));
	Body->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Body->SetCollisionProfileName("IgnoreOnlyPawn");

	Collision = NewObject<UBoxComponent>(this, TEXT("CollisionBox"));
	Collision->AttachTo(Body);
	Collision->bGenerateOverlapEvents = true;
	Collision->RelativeLocation = FVector(18, 1, 0);
	Collision->RelativeScale3D = FVector(0.1, 0.75, 0.075);
	Collision->RelativeRotation = FRotator(0, -88, 0);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AMiraDague::OnOverlapBegin);

	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletMovement"));

	BulletMovement->UpdatedComponent = RootComponent;
	BulletMovement->MaxSpeed = 0.f;
	BulletMovement->InitialSpeed = 900;
	BulletMovement->bRotationFollowsVelocity = false;
	BulletMovement->bShouldBounce = true;
	BulletMovement->Bounciness = 0.0001f;
	BulletMovement->Friction = 1.0f;
	BulletMovement->Velocity = GetActorForwardVector();
}

// Called when the game starts or when spawned
void AMiraDague::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(ColliHandle, this, &AMiraDague::Colli, 1.0f, false, 1.0f);
	GetWorldTimerManager().SetTimer(DestoryHandle, this, &AMiraDague::DestroyF, 1.5f, false, 1.5f);
}

// Called every frame
void AMiraDague::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMiraDague::DestroyF()
{
	Destroy();
}

void AMiraDague::Colli()
{
	Collision->bGenerateOverlapEvents = false;
}

void AMiraDague::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AHero * _hero = Cast<AHero>(OtherActor);

	if (_hero && _hero->GetRootComponent() == OtherComp)
	{
		_hero->PutDamage(10);
	}
}