// Fill out your copyright notice in the Description page of Project Settings.

#include "MushRoom.h"
#include "Monster/MonsterController.h"

#define WALK_SPEED 50
#define RUN_SPEED 150

// Sets default values
AMushRoom::AMushRoom()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bStartWithTickEnabled = false;

	BaseEyeHeight = 8.0f;
	CrouchedEyeHeight = 8.0f;

	GetCharacterMovement()->JumpZVelocity = 0.0f;
	GetCharacterMovement()->MaxStepHeight = 0.0f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 350.0f; //300
	GetCharacterMovement()->MaxWalkSpeed = WALK_SPEED;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 170.0f;
	GetCharacterMovement()->bIgnoreBaseRotation = true;
	GetCharacterMovement()->MaxSwimSpeed = 170.0f;
	GetCharacterMovement()->AirControl = 0.0f;

	RootComponent->RelativeScale3D = FVector(0.3f, 0.3f, 0.3f);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMo(TEXT("AnimMontage'/Game/Modeling/Monster/Stage1/MushRoom/MushroomAttack_Montage.MushroomAttack_Montage'"));
	AttackAnimation = AttackMo.Object;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletonMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->RelativeLocation = FVector(0.0f, 0.0f, -90);
	SkeletalMesh->RelativeRotation = FRotator(0, -90, 0);
	SkeletalMesh->RelativeScale3D = FVector(0.5f, 0.5f, 0.5f);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skeleton(TEXT("SkeletalMesh'/Game/Modeling/Monster/Stage1/MushRoom/MushRoom.MushRoom'"));
	static ConstructorHelpers::FObjectFinder<UClass> AnimClass(TEXT("AnimBlueprint'/Game/Modeling/Monster/Stage1/MushRoom/NewAnimBlueprint.NewAnimBlueprint_C'"));
	SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkeletalMesh->AnimClass = AnimClass.Object;
	SkeletalMesh->SetSkeletalMesh(Skeleton.Object);
	SkeletalMesh->bGenerateOverlapEvents = true;
	SkeletalMesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;

	WeaponCollision = NewObject<UBoxComponent>(this, TEXT("CollisionBox"));
	WeaponCollision->AttachTo(SkeletalMesh, TEXT("Bip001-HeadNub"));
	WeaponCollision->bGenerateOverlapEvents = true;
	WeaponCollision->RelativeLocation = FVector(-50, 0, 0);
	WeaponCollision->RelativeScale3D = FVector(3, 3, 3);
	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AMushRoom::OnOverlapBegin);

	AiPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));

	Sight->PeripheralVisionAngleDegrees = 120;
	Sight->SightRadius = 400.0f;
	Sight->LoseSightRadius = 475.0f;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;
	Sight->DetectionByAffiliation.bDetectEnemies = true;

	AiPerception->ConfigureSense(*Sight);
	AiPerception->OnPerceptionUpdated.AddDynamic(this, &AMushRoom::PerceptionUpdated);
	AiPerception->bNeverNeedsRenderUpdate = true;
}

// Called when the game starts or when spawned
void AMushRoom::BeginPlay()
{
	Super::BeginPlay();

	Health = 100;

	AttackCool = true;

	AiPerception->UpdatePerceptionWhitelist(Sight->GetSenseID(), false);

	GetCharacterMovement()->SetComponentTickEnabled(false);

	SkeletalMesh->bPauseAnims = true;
}

// Called every frame
void AMushRoom::Tick(float DeltaTime)
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
		}

		else if (Action == 3)
		{
			AiController->MoveToLocation(GetActorLocation());
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

		else if (Action == 2 && AttackCool)
		{
			if (PlayerHero && !PlayerHero->GetAlive())
			{
				Action = 0;
				return;
			}

			FVector Destina = PlayerHero->GetActorLocation();

			AiController->MoveToLocation(Destina);

			if (GetActorLocation().X + 50 > Destina.X &&
				GetActorLocation().X - 50 < Destina.X &&
				GetActorLocation().Y + 50 > Destina.Y &&
				GetActorLocation().Y - 50 < Destina.Y)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, TEXT("ATTACK"));
				AttackCool = false;

				SkeletalMesh->GetAnimInstance()->Montage_Play(AttackAnimation);

				GetWorldTimerManager().SetTimer(TimerAttackCool, this, &AMushRoom::AttackCoolDown, 1.25, false, 1.25);
				GetWorldTimerManager().SetTimer(TimerAttackColi, this, &AMushRoom::AttackColiReset, 0.5, false, 0.5);
			}
		}
	}
	else
	{
		AiController = Cast<AAIController>(GetController());
	}
}

void AMushRoom::PutDamage(int damage, AHero * hero)
{
	if (AttackedHero == nullptr)
	{
		Action = 3;
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Attack"));
		Health -= damage;

		FVector Destina = GetActorLocation();

		AiController->MoveToLocation(Destina);

		UCapsuleComponent * RootCom = Cast<UCapsuleComponent>(RootComponent);
		if (RootCom)
		{
			RootCom->SetCollisionProfileName("IgnoreOnlyPawn");
		}

		LaunchCharacter(GetActorForwardVector() * -500, true, true);

		PlayerHero = hero;
		AttackedHero = hero;

		GetWorldTimerManager().SetTimer(TimerStun, this, &AMushRoom::ResetStun, 0.3f, false, 0.3f);

		if (Health <= 0)
		{
			int r = level->RandomItemCode();
			level->SpawnItem(FVector(GetActorLocation().X, GetActorLocation().Y, 15), r);
			level->InitMonsterArray(Number);
			Destroy();
			Action = 100;
			AiController->Destroy();
		}

		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMushRoom::ResetAttackedHero, 0.05f, false, 0.05f);
	}
}

void AMushRoom::PutDamage(int damage)
{
	Health -= damage;

	if (Health <= 0)
	{
		int r = level->RandomItemCode();
		level->SpawnItem(FVector(GetActorLocation().X, GetActorLocation().Y, 15), r);
		level->InitMonsterArray(Number);
		Destroy();
		Action = 100;
		AiController->Destroy();
	}
}

void AMushRoom::SetMonNum(int number, AMyLevelScriptActor * cla, int Stage)
{
	Number = number;
	level = cla;
	DAMAGE = 5 + Stage;
}

void AMushRoom::SetPause()
{
	SetActorHiddenInGame(true);
	SkeletalMesh->bPauseAnims = true;
	GetCharacterMovement()->SetComponentTickEnabled(false);
	if (AiController)
	{
		AiController->MoveToLocation(GetActorLocation());
		AiController->SetActorTickEnabled(false);
	}
	AiPerception->UpdatePerceptionWhitelist(Sight->GetSenseID(), false);
	SetActorTickEnabled(false);
}

void AMushRoom::SetUnPause()
{
	SetActorHiddenInGame(false);
	SkeletalMesh->bPauseAnims = false;
	GetCharacterMovement()->SetComponentTickEnabled(true);
	AiPerception->UpdatePerceptionWhitelist(Sight->GetSenseID(), true);
	if (AiController)
	{
		AiController->SetActorTickEnabled(true);
	}
	SetActorTickEnabled(true);
}

void AMushRoom::RandomAction()
{
	Action = (rand() % 2);

	if (Action == 0)
	{
		SleepTime = (rand() % 1500) + 100;
	}

	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WALK_SPEED;

		int x = (rand() % (int)RoomSize.X) + RoomLocation.X;
		int y = (rand() % (int)RoomSize.Y) + RoomLocation.Y;

		Destination = FVector(x, y, 0);
	}
}

void AMushRoom::ResetAttackedHero()
{
	AttackedHero = nullptr;
}

void AMushRoom::ResetStun()
{
	Action = 2;

	UCapsuleComponent * RootCom = Cast<UCapsuleComponent>(RootComponent);
	if (RootCom)
	{
		RootCom->SetCollisionProfileName("Pawn");
	}
}

void AMushRoom::AttackCoolDown()
{
	AttackCool = true;
	AttackColi = false;
	AttackHero = nullptr;
	Action = 2;
}

void AMushRoom::AttackColiReset()
{
	AttackColi = true;
}

void AMushRoom::PerceptionUpdated(const TArray<AActor*>& Actors)
{
	if (Action != 3 && AiController)
	{
		AHero * hero = Cast<AHero>(Actors[0]);

		if (hero && hero->GetAlive())
		{
			FActorPerceptionBlueprintInfo Info;
			AiPerception->GetActorsPerception(hero, Info);

			PlayerHero = Cast<AHero>(Actors[0]);

			if (Info.LastSensedStimuli.Num() > 0)
			{
				const FAIStimulus Stimulus = Info.LastSensedStimuli[0];

				if (Stimulus.WasSuccessfullySensed())
				{
					//Player in sight
					GetCharacterMovement()->MaxWalkSpeed = RUN_SPEED;
					AiController->MoveToLocation(GetActorLocation());
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, TEXT("BAL GYEON"));
					Action = 2;
				}
				else
				{
					//Player out of sight
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, TEXT("SI BULL"));
					Action = 0;
				}
			}
		}
	}
}

void AMushRoom::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AHero * hero = Cast<AHero>(OtherActor);

	if (!AttackCool && hero && hero->GetRootComponent() == OtherComp && AttackHero == nullptr && AttackColi)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, TEXT("Attack Success"));
		hero->PutDamage(DAMAGE);
		AttackHero = hero;
		AttackColi = false;
	}
}