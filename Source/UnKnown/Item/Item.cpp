// Fill out your copyright notice in the Description page of Project Settings.
#include "Item.h"
#include "Engine.h"
#include "Player/Hero.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bStartWithTickEnabled = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Mat(TEXT("Material'/Game/Modeling/ItemMat.ItemMat'"));
	
	Mater = Mat.Object;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Body;

	Body->SetStaticMesh(Mesh.Object);
	Body->SetRelativeScale3D(FVector(0.2, 0.2, 0.2));
	Body->SetRelativeRotation(FRotator(50, 0, 0));
	Body->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Body->SetCollisionProfileName("IgnoreOnlyPawn");

	Collision = NewObject<USphereComponent>(this, TEXT("CollisionBox"));
	Collision->AttachTo(Body);
	Collision->bGenerateOverlapEvents = false;
	Collision->RelativeScale3D = FVector(1.5, 1.5, 1.5);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);

	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletMovement"));

	float b = (rand() % 100) * 0.01 - 0.5;
	float c = (rand() % 100) * 0.01 - 0.5;
	float d = (rand() % 100) * 0.01 - 0.5;

	UE_LOG(LogTemp, Warning, TEXT("%f %f %f"), b,c,d);

	BulletMovement->UpdatedComponent = RootComponent;
	BulletMovement->MaxSpeed = 0.f;
	BulletMovement->InitialSpeed = 300;
	BulletMovement->bRotationFollowsVelocity = false;
	BulletMovement->bShouldBounce = true;
	BulletMovement->Bounciness = 0.0001f;
	BulletMovement->Velocity = FVector(b, c, d);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::SetItemSetting(UObject * _Image, float _Weight, AMyLevelScriptActor * _Level, FString _Name, FString _Explain, bool _Consumables, int _Parts, int _Code)
{
	Consumables = _Consumables;
	Level = _Level;
	Image = _Image;
	Weight = _Weight;
	Name = _Name;
	Explain = _Explain;
	Parts = _Parts;
	ItemCode = _Code;

	UTexture * Tex = Cast<UTexture>(_Image);

	UMaterialInstanceDynamic * Instance = UMaterialInstanceDynamic::Create(Mater, this);
	Instance->SetTextureParameterValue("Tex", Tex);
	Body->SetMaterial(0, Instance);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AItem::Overlap, 1.0f, false, 1.0f);
	GetWorldTimerManager().SetTimer(DestoryHandle, this, &AItem::DestroyF, 180.0f, false, 180.0f);
}

void AItem::Overlap()
{
	Collision->bGenerateOverlapEvents = true;
}

void AItem::DestroyF()
{
	Destroy();
}

void AItem::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AHero * hero = Cast<AHero>(OtherActor);

	if (hero && hero->GetRootComponent() == OtherComp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, Name);
		hero->ItemUpdate(Weight, Image, Name, Explain, this, Consumables, Parts, ItemCode);
	}
}
