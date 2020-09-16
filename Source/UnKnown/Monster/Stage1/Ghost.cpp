// Fill out your copyright notice in the Description page of Project Settings.

#include "Ghost.h"
#include "Monster/MonsterController.h"

// Sets default values
AGhost::AGhost()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->JumpZVelocity = 0.0f;
	GetCharacterMovement()->MaxStepHeight = 0.0f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 350.0f; //300
	GetCharacterMovement()->MaxWalkSpeed = 170.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 170.0f;
	GetCharacterMovement()->bIgnoreBaseRotation = true;
	GetCharacterMovement()->MaxSwimSpeed = 170.0f;
	GetCharacterMovement()->AirControl = 0.0f;

	RootComponent->RelativeScale3D = FVector(0.25f, 0.25f, 0.25f);

	UCapsuleComponent * RootCom = Cast<UCapsuleComponent>(RootComponent);
	if (RootCom)
	{
		RootCom->SetCollisionProfileName("IgnoreOnlyPawn");
	}

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletonMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
	SkeletalMesh->RelativeScale3D = FVector(1.0f, 1.0f, 1.0f);
	SkeletalMesh->RelativeRotation = FRotator(0, -90, 0);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skeleton(TEXT("SkeletalMesh'/Game/Modeling/Monster/Stage1/Ghost/Ghost.Ghost'"));
	static ConstructorHelpers::FObjectFinder<UClass> AnimClass(TEXT("/Game/Modeling/Monster/Stage1/NewAnimBlueprint.NewAnimBlueprint_C"));
	SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//SkeletalMesh->AnimClass = AnimClass.Object;
	SkeletalMesh->SetSkeletalMesh(Skeleton.Object);
	SkeletalMesh->bGenerateOverlapEvents = true;
	SkeletalMesh->SetEnableGravity(false);
	SkeletalMesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->AttachTo(SkeletalMesh);
	Sphere->bGenerateOverlapEvents = true;
	Sphere->RelativeLocation = FVector(0, 0, 97);
	Sphere->RelativeScale3D = FVector(20.0, 20.0, 20.0);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AGhost::SphereOnOverlapBegin);

	WeaponCollision = NewObject<UBoxComponent>(this, TEXT("CollisionBox"));
	WeaponCollision->AttachTo(SkeletalMesh);
	WeaponCollision->bGenerateOverlapEvents = true;
	WeaponCollision->RelativeLocation = FVector(0, 0, 97);
	WeaponCollision->RelativeScale3D = FVector(1.0, 1.0, 2.0);
	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AGhost::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AGhost::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AiController)
	{
		if (Action == 0)
		{
			if (SleepMan >= SleepTime)
			{
				RandomAction();
			}
			SleepMan++;

			//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Success"));
		}

		else if (Action == 1)
		{
			AiController->MoveToLocation(Destination);

			if (GetActorLocation().X + 20 > Destination.X &&
				GetActorLocation().X - 20 < Destination.X &&
				GetActorLocation().Y + 20 > Destination.Y &&
				GetActorLocation().Y - 20 < Destination.Y)
			{
				RandomAction();
			}
		}

		else if (Action == 2)
		{
			if (Hero->GetDistanceTo(this) > 300)
			{
				RandomAction();
			}
			AiController->MoveToLocation(Hero->GetTargetLocation());
		}
	}
	else
	{
		AiController = Cast<AAIController>(GetController());
	}
}

void AGhost::SetMonNum(int number, AMyLevelScriptActor * cla, int Stage)
{
	DAMAGE = 10 + Stage;
}

void AGhost::RandomAction()
{
	Action = (rand() % 2);

	if (Action == 0)
	{
		SleepTime = (rand() % 300) + 100;

		return;
	}

	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 170.0f;

		index = (rand() % (RoomCount - 2)) + 1;

		RoomLocation.X = (float)RoomArray[index]->PSData.x + 1;
		RoomLocation.Y = (float)RoomArray[index]->PSData.y + 1;
		RoomSize.X = (float)RoomArray[index]->PSData.sx - 1;
		RoomSize.Y = (float)RoomArray[index]->PSData.sy - 1;

		int x = (rand() % (int)RoomSize.X) + RoomLocation.X;
		int y = (rand() % (int)RoomSize.Y) + RoomLocation.Y;

		Destination = FVector(x * 50, y * 50, 0);

		return;
	}
}

void AGhost::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AHero * hero = Cast<AHero>(OtherActor);

	if (hero && hero->GetRootComponent() == OtherComp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Attack Success"));
		hero->PutDamage(DAMAGE);
	}
}

void AGhost::SphereOnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AHero * hero = Cast<AHero>(OtherActor);

	if (hero && hero->GetRootComponent() == OtherComp)
	{
		Hero = hero;
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Find Hero"));
		Action = 2;
	}
}
