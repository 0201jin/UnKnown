// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillCircleCollision.h"
#include "Engine.h"
#include "Monster/MonsterMgr.h"

// Sets default values
ASkillCircleCollision::ASkillCircleCollision()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("StaticMesh'/Game/Modeling/CircleCollision.CircleCollision'"));

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Body;

	Body->SetStaticMesh(Mesh.Object);
	Body->SetRelativeScale3D(FVector(0.0, 0.0, 0.0));
	Body->SetRelativeRotation(FRotator(0, 0, 0));
	Body->bGenerateOverlapEvents = true;
	Body->SetCollisionProfileName("IgnoreOnlyPawn");
	Body->OnComponentBeginOverlap.AddDynamic(this, &ASkillCircleCollision::OnOverlapBegin);
}

void ASkillCircleCollision::SetSetting(int _Damage, AHero * _hero, FVector _Scale, float _KillTime)
{
	Damage = _Damage;
	hero = _hero;
	Body->SetRelativeScale3D(_Scale);

	GetWorldTimerManager().SetTimer(KillTimer, this, &ASkillCircleCollision::DestroyThis, _KillTime, false, _KillTime);
}

// Called when the game starts or when spawned
void ASkillCircleCollision::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASkillCircleCollision::DestroyThis()
{
	Destroy();
}

// Called every frame
void ASkillCircleCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkillCircleCollision::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AMonsterMgr * mon = Cast<AMonsterMgr>(OtherActor);
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, OtherActor->GetName());
	if (mon && hero)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Skill"));
		mon->PutDamage(Damage, hero);

		FVector ff = mon->GetActorForwardVector() * -50;
		mon->AddActorWorldOffset(FVector(ff.X, ff.Y, 0), true);
	}
}