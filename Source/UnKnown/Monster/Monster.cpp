// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster.h"
#include "Monster/MonsterController.h"

#define WALK_SPEED 50
#define RUN_SPEED 150

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMo(TEXT("AnimMontage'/Game/Modeling/Monster/Stage1/Attack_Montage.Attack_Montage'"));
	AttackAnimation = AttackMo.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMo1(TEXT("AnimMontage'/Game/Modeling/Monster/Stage1/Attack1_Montage.Attack1_Montage'"));
	AttackAnimation1 = AttackMo1.Object;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletonMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->RelativeLocation = FVector(0.0f, 0.0f, -90);
	SkeletalMesh->RelativeRotation = FRotator(0, -90, 0);
	SkeletalMesh->RelativeScale3D = FVector(2, 2, 2);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skeleton(TEXT("SkeletalMesh'/Game/Modeling/Monster/Stage1/Mesh.Mesh'"));
	static ConstructorHelpers::FObjectFinder<UClass> AnimClass(TEXT("/Game/Modeling/Monster/Stage1/NewAnimBlueprint.NewAnimBlueprint_C"));
	SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkeletalMesh->AnimClass = AnimClass.Object;
	SkeletalMesh->SetSkeletalMesh(Skeleton.Object);
	SkeletalMesh->bGenerateOverlapEvents = true;
	SkeletalMesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;

	Weapon = NewObject<UStaticMeshComponent>(this, TEXT("Weapon"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMesh(TEXT("StaticMesh'/Game/Modeling/Monster/Stage1/Mesh_Object003.Mesh_Object003'"));
	Weapon->SetStaticMesh(WeaponMesh.Object);
	Weapon->AttachTo(SkeletalMesh, TEXT("Bip003-Xtra02Nub"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Weapon->RelativeRotation = FRotator(0, 0, 180);
	Weapon->RelativeLocation = FVector(0, 13, 0);

	/*Shield = NewObject<UStaticMeshComponent>(this, TEXT("Shield"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShieldMesh(TEXT("StaticMesh'/Game/Modeling/Monster/Stage1/Mesh_Object004.Mesh_Object004'"));
	Shield->SetStaticMesh(ShieldMesh.Object);
	Shield->AttachTo(SkeletalMesh, TEXT("Bip003-L-Hand"));
	Shield->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Shield->RelativeRotation = FRotator(12.6, -32.2, -10.25);
	Shield->RelativeLocation = FVector(3.42, -0.73, 6.27);*/

	WeaponCollision = NewObject<UBoxComponent>(this, TEXT("CollisionBox"));
	WeaponCollision->AttachTo(Weapon);
	WeaponCollision->bGenerateOverlapEvents = true;
	WeaponCollision->RelativeLocation = FVector(0, 3, 0);
	WeaponCollision->RelativeScale3D = FVector(0.05, 0.6, 0.125);
	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnOverlapBegin);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> DeadFX(TEXT("ParticleSystem'/Game/Effect/Monster/Stage1/Dead.Dead'"));
	DeadEffect = DeadFX.Object;

	AiPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));

	Sight->PeripheralVisionAngleDegrees = 120;
	Sight->SightRadius = 400.0f;
	Sight->LoseSightRadius = 475.0f;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;
	Sight->DetectionByAffiliation.bDetectEnemies = true;

	AiPerception->ConfigureSense(*Sight);
	AiPerception->OnPerceptionUpdated.AddDynamic(this, &AMonster::PerceptionUpdated);
	AiPerception->bNeverNeedsRenderUpdate = true;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	Health = 100;

	AttackCool = true;

	AiPerception->UpdatePerceptionWhitelist(Sight->GetSenseID(), false);

	GetCharacterMovement()->SetComponentTickEnabled(false);

	SkeletalMesh->bPauseAnims = true;
}

// Called every frame
void AMonster::Tick(float DeltaTime)
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

			if (GetActorLocation().X + 30 > Destina.X &&
				GetActorLocation().X - 30 < Destina.X &&
				GetActorLocation().Y + 30 > Destina.Y &&
				GetActorLocation().Y - 30 < Destina.Y)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, TEXT("ATTACK"));
				AttackCool = false;
				
				int RandAnimation = (rand() % 2);

				if (RandAnimation == 0)
				{
					SkeletalMesh->GetAnimInstance()->Montage_Play(AttackAnimation);
				}

				else if (RandAnimation == 1)
				{
					SkeletalMesh->GetAnimInstance()->Montage_Play(AttackAnimation1);
				}
				
				GetWorldTimerManager().SetTimer(TimerAttackCool, this, &AMonster::AttackCoolDown, 0.8, false, 0.8);
				GetWorldTimerManager().SetTimer(TimerAttackColi, this, &AMonster::AttackColiReset, 0.35, false, 0.35);
			}
		}
	}
	else
	{
		AiController = Cast<AAIController>(GetController());
	}
}

void AMonster::PutDamage(int damage, AHero* hero)
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

		GetWorldTimerManager().SetTimer(TimerStun, this, &AMonster::ResetStun, 0.3f, false, 0.3f);

		if (Health <= 0)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeadEffect, GetActorLocation(), hero->GetActorRotation());
			int r = level->RandomItemCode();
			level->SpawnItem(FVector(GetActorLocation().X, GetActorLocation().Y, 15), r);
			level->InitMonsterArray(Number);
			Destroy();
			Action = 100;
			AiController->Destroy();
		}

		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMonster::ResetAttackedHero, 0.05f, false, 0.05f);
	}
}

void AMonster::PutDamage(int damage)
{
	Health -= damage;

	if (Health <= 0)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeadEffect, GetActorLocation(), GetActorRotation());
		int r = level->RandomItemCode();
		level->SpawnItem(FVector(GetActorLocation().X, GetActorLocation().Y, 15), r);
		level->InitMonsterArray(Number);
		Destroy();
		Action = 100;
		AiController->Destroy();
	}
}

void AMonster::SetMonNum(int number, AMyLevelScriptActor * cla, int Stage)
{
	Number = number;
	level = cla;
	DAMAGE = 5 + Stage;
}

void AMonster::SetPause()
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

void AMonster::SetUnPause()
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

void AMonster::RandomAction()
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

void AMonster::ResetAttackedHero()
{
	AttackedHero = nullptr;
}

void AMonster::ResetStun()
{
	Action = 2;

	UCapsuleComponent * RootCom = Cast<UCapsuleComponent>(RootComponent);
	if (RootCom)
	{
		RootCom->SetCollisionProfileName("Pawn");
	}
}

void AMonster::AttackCoolDown()
{
	AttackCool = true;
	AttackColi = false;
	AttackHero = nullptr;
	Action = 2;
}

void AMonster::AttackColiReset()
{
	AttackColi = true;
}

void AMonster::PerceptionUpdated(const TArray<AActor*>& Actors)
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

void AMonster::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AHero * hero = Cast<AHero>(OtherActor);

	if (!AttackCool && hero && hero->GetRootComponent() == OtherComp && AttackHero == nullptr && AttackColi)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Attack Success"));
		hero->PutDamage(DAMAGE);
		AttackHero = hero;
	}
}