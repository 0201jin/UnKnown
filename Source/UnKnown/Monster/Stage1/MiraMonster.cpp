// Fill out your copyright notice in the Description page of Project Settings.

#include "MiraMonster.h"
#include "Monster/MonsterController.h"
#include "Monster/Stage1/MiraDague.h"

#define WALK_SPEED	50
#define RUN_SPEED	50
#define ATTACK_RANGE 150

// Sets default values
AMiraMonster::AMiraMonster()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMo(TEXT("AnimMontage'/Game/Modeling/Monster/Stage1/RangeMira/RangeMira_Attack_Montage.RangeMira_Attack_Montage'"));
	AttackAnimation = AttackMo.Object;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletonMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->RelativeLocation = FVector(0.0f, 0.0f, -90);
	SkeletalMesh->RelativeRotation = FRotator(0, -90, 0);
	SkeletalMesh->RelativeScale3D = FVector(2, 2, 2);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skeleton(TEXT("SkeletalMesh'/Game/Modeling/Monster/Stage1/RangeMira/RangeMira.RangeMira'"));
	static ConstructorHelpers::FObjectFinder<UClass> AnimClass(TEXT("AnimBlueprint'/Game/Modeling/Monster/Stage1/RangeMira/NewAnimBlueprint.NewAnimBlueprint_C'"));
	SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkeletalMesh->AnimClass = AnimClass.Object;
	SkeletalMesh->SetSkeletalMesh(Skeleton.Object);
	SkeletalMesh->bGenerateOverlapEvents = true;
	SkeletalMesh->SetCollisionProfileName("OverlapAllDynamic");
	SkeletalMesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;

	Weapon = NewObject<UStaticMeshComponent>(this, TEXT("Weapon"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMesh(TEXT("StaticMesh'/Game/Modeling/Monster/Stage1/RangeMira/MiraSword.MiraSword'"));
	Weapon->SetStaticMesh(WeaponMesh.Object);
	Weapon->AttachTo(SkeletalMesh, TEXT("Bone001"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Weapon->RelativeRotation = FRotator(0, 0, 180);
	Weapon->RelativeLocation = FVector(0, 0, 0);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> DeadFX(TEXT("ParticleSystem'/Game/Effect/Monster/Stage1/Mira/Dead.Dead'"));
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
	AiPerception->OnPerceptionUpdated.AddDynamic(this, &AMiraMonster::PerceptionUpdated);
	AiPerception->bNeverNeedsRenderUpdate = true;
}

// Called when the game starts or when spawned
void AMiraMonster::BeginPlay()
{
	Super::BeginPlay();

	Health = 100;

	AttackCool = true;

	AiPerception->UpdatePerceptionWhitelist(Sight->GetSenseID(), false);

	GetCharacterMovement()->SetComponentTickEnabled(false);

	SkeletalMesh->bPauseAnims = true;
}

// Called every frame
void AMiraMonster::Tick(float DeltaTime)
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

			if (GetActorLocation().X + ATTACK_RANGE > Destina.X &&
				GetActorLocation().X - ATTACK_RANGE < Destina.X &&
				GetActorLocation().Y + ATTACK_RANGE > Destina.Y &&
				GetActorLocation().Y - ATTACK_RANGE < Destina.Y)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, TEXT("ATTACK"));
				AttackCool = false;

				FVector Destina = GetActorLocation();

				AiController->MoveToLocation(Destina);

				FVector ToT = PlayerHero->GetActorLocation() - GetActorLocation();
				FRotator DiRo = ToT.Rotation();
				DiRo.Pitch = 0.0f;
				SetActorRotation(DiRo);

				SkeletalMesh->GetAnimInstance()->Montage_Play(AttackAnimation);

				GetWorldTimerManager().SetTimer(TimerAttack, this, &AMiraMonster::Attack, 0.465, false, 0.465);
				GetWorldTimerManager().SetTimer(TimerShowWeapon, this, &AMiraMonster::ShowWeapon, 1.0, false, 1.0);
				GetWorldTimerManager().SetTimer(TimerAttackCool, this, &AMiraMonster::AttackCoolDown, 2.0, false, 2.0);
			}
		}
	}
	else
	{
		AiController = Cast<AAIController>(GetController());
	}
}

void AMiraMonster::PutDamage(int damage, AHero * hero)
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

		GetWorldTimerManager().SetTimer(TimerStun, this, &AMiraMonster::ResetStun, 0.3f, false, 0.3f);

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

		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMiraMonster::ResetAttackedHero, 0.05f, false, 0.05f);
	}
}

void AMiraMonster::PutDamage(int damage)
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

void AMiraMonster::SetMonNum(int number, AMyLevelScriptActor * cla, int Stage)
{
	Number = number;
	level = cla;
	DAMAGE = 5 + Stage;
}

void AMiraMonster::SetPause()
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

void AMiraMonster::SetUnPause()
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

void AMiraMonster::RandomAction()
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

void AMiraMonster::ResetAttackedHero()
{
	AttackedHero = nullptr;
}

void AMiraMonster::ResetStun()
{
	Action = 2;

	UCapsuleComponent * RootCom = Cast<UCapsuleComponent>(RootComponent);
	if (RootCom)
	{
		RootCom->SetCollisionProfileName("Pawn");
	}
}

void AMiraMonster::AttackCoolDown()
{
	AttackCool = true;
	AttackHero = nullptr;
	//Action = 2;
}

void AMiraMonster::Attack()
{
	GetWorld()->SpawnActor<AMiraDague>(AMiraDague::StaticClass(),
		FTransform(GetActorForwardVector().Rotation(),
			FVector(GetActorLocation().X, GetActorLocation().Y, 50),
			FVector(1, 1, 1)));

	Weapon->RelativeScale3D = FVector(0,0,0);
}

void AMiraMonster::ShowWeapon()
{
	Weapon->RelativeScale3D = FVector(1, 1, 1);
}

void AMiraMonster::PerceptionUpdated(const TArray<AActor*>& Actors)
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
