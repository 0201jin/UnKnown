// Fill out your copyright notice in the Description page of Project Settings.

#include "Tariarri.h"
#include "Monster/MonsterController.h"

#define WALK_SPEED	100
#define RUN_SPEED	200
#define ATTACK_RANGE 50

// Sets default values
ATariarri::ATariarri()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bStartWithTickEnabled = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DashMo(TEXT("AnimMontage'/Game/Modeling/Monster/Stage1/Ratiarii/Retiarri_Dash_Montage.Retiarri_Dash_Montage'"));
	DashAnimation = DashMo.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMo(TEXT("AnimMontage'/Game/Modeling/Monster/Stage1/Ratiarii/Retiarri_Attack_Montage.Retiarri_Attack_Montage'"));
	AttackAnimation = AttackMo.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SpecialAttackMo(TEXT("AnimMontage'/Game/Modeling/Monster/Stage1/Ratiarii/Retiarri_Special_Attack_Montage.Retiarri_Special_Attack_Montage'"));
	SpecialAttackAnimation = SpecialAttackMo.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> DeadFX(TEXT("ParticleSystem'/Game/Effect/Monster/Stage1/Dead.Dead'"));
	DeadEffect = DeadFX.Object;

	BaseEyeHeight = 8.0f;
	CrouchedEyeHeight = 8.0f;

	RootComponent->RelativeScale3D = FVector(0.3f, 0.3f, 0.3f);

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

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletonMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->RelativeLocation = FVector(0.0f, 0.0f, -90);
	SkeletalMesh->RelativeRotation = FRotator(0, -90, 0);
	SkeletalMesh->RelativeScale3D = FVector(2, 2, 2);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skeleton(TEXT("SkeletalMesh'/Game/Modeling/Monster/Stage1/Ratiarii/Retiarri.Retiarri'"));
	static ConstructorHelpers::FObjectFinder<UClass> AnimClass(TEXT("AnimBlueprint'/Game/Modeling/Monster/Stage1/Ratiarii/NewAnimBlueprint.NewAnimBlueprint_C'"));
	SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkeletalMesh->AnimClass = AnimClass.Object;
	SkeletalMesh->SetSkeletalMesh(Skeleton.Object);
	SkeletalMesh->bGenerateOverlapEvents = true;

	WeaponCollision = NewObject<UBoxComponent>(this, TEXT("CollisionBox"));
	WeaponCollision->AttachTo(SkeletalMesh, TEXT("Bone001"));
	WeaponCollision->bGenerateOverlapEvents = true;
	WeaponCollision->RelativeLocation = FVector(87.228, -0.632, 0);
	WeaponCollision->RelativeScale3D = FVector(0.35, 0.55, 0.1);
	WeaponCollision->RelativeRotation = FRotator(0, -90, 0);
	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &ATariarri::OnOverlapBegin);

	AiPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));

	Sight->PeripheralVisionAngleDegrees = 120;
	Sight->SightRadius = 400.0f;
	Sight->LoseSightRadius = 475.0f;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;
	Sight->DetectionByAffiliation.bDetectEnemies = true;

	AiPerception->ConfigureSense(*Sight);
	AiPerception->OnPerceptionUpdated.AddDynamic(this, &ATariarri::PerceptionUpdated);
	AiPerception->bNeverNeedsRenderUpdate = true;
}

// Called when the game starts or when spawned
void ATariarri::BeginPlay()
{
	Super::BeginPlay();

	Health = 300;

	bAttackCool = true;
	bDashCool = true;
	bSpecialAttackCool = true;
	bCool = true;
	bColli = false;

	AiPerception->UpdatePerceptionWhitelist(Sight->GetSenseID(), false);

	GetCharacterMovement()->SetComponentTickEnabled(false);

	SkeletalMesh->bPauseAnims = true;
}

// Called every frame
void ATariarri::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AiController)
	{
		if (Action == 0)
		{
			bColli = false;
			if (SleepMan >= SleepTime)
			{
				RandomAction();
			}
			SleepMan++;
		}

		else if (Action == 1)
		{
			bColli = false;
			AiController->MoveToLocation(Destination);

			if (GetActorLocation().X + 20 > Destination.X &&
				GetActorLocation().X - 20 < Destination.X &&
				GetActorLocation().Y + 20 > Destination.Y &&
				GetActorLocation().Y - 20 < Destination.Y)
			{
				RandomAction();
			}
		}

		else if (Action == 2 && bCool)
		{
			if (PlayerHero && !PlayerHero->GetAlive())
			{
				Action = 0;
				return;
			}

			FVector Destina = PlayerHero->GetActorLocation();

			AiController->MoveToLocation(Destina);

			if (PlayerHero->GetDistanceTo(this) < ATTACK_RANGE &&
				bAttackCool)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, TEXT("ATTACK"));
				bAttackCool = false;
				bCool = false;
				bColli = true;
				Attack();
				GetWorldTimerManager().SetTimer(CoolTimer, this, &ATariarri::bSetCool, 1.0f, false, 1.0f);
				GetWorldTimerManager().SetTimer(AttackTimer, this, &ATariarri::bSetAttack, 2.0f, false, 2.0f);
				return;
			}

			if (PlayerHero->GetDistanceTo(this) > ATTACK_RANGE &&
				PlayerHero->GetDistanceTo(this) < ATTACK_RANGE + 100 &&
				bDashCool)
			{
				bDashCool = false;
				bCool = false;
				bColli = true;
				Dash();
				GetWorldTimerManager().SetTimer(CoolTimer, this, &ATariarri::bSetCool, 1.0f, false, 1.0f);
				GetWorldTimerManager().SetTimer(DashTimer, this, &ATariarri::bSetDash, 5.0f, false, 5.0f);
				return;
			}

			if (PlayerHero->GetDistanceTo(this) < ATTACK_RANGE &&
				bSpecialAttackCool)
			{
				bSpecialAttackCool = false;
				bCool = false;
				bColli = true;
				SpecialAttack();
				GetWorldTimerManager().SetTimer(CoolTimer, this, &ATariarri::bSetCool, 1.0f, false, 1.0f);
				GetWorldTimerManager().SetTimer(Special_AttackTimer, this, &ATariarri::bSetSpecialAttack, 10.0f, false, 10.0f);
				return;
			}
		}
	}
	else 
	{
		AiController = Cast<AAIController>(GetController());
	}
}

void ATariarri::PutDamage(int damage, AHero * hero)
{
	if (AttackedHero == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Attack"));
		Health -= damage;

		PlayerHero = hero;
		AttackedHero = hero;

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

		GetWorldTimerManager().SetTimer(TimerHandle, this, &ATariarri::ResetAttackedHero, 0.05f, false, 0.05f);

		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Tariarri : Damage"));
	}
}

void ATariarri::PutDamage(int damage)
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

void ATariarri::SetMonNum(int number, AMyLevelScriptActor * cla, int Stage)
{
	Number = number;
	level = cla;
	DAMAGE = Stage + 20;
}

void ATariarri::SetPause()
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

void ATariarri::SetUnPause()
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

void ATariarri::Attack()
{
	FVector Destina = GetActorLocation();

	AiController->MoveToLocation(Destina);

	SkeletalMesh->GetAnimInstance()->Montage_Play(AttackAnimation);
}

void ATariarri::Dash()
{
	GetWorldTimerManager().SetTimer(Dash_TrackingTimer, this, &ATariarri::Tracking, 0.1f, true, 0.0f);
	GetWorldTimerManager().SetTimer(Dash_TrackingEndTimer, this, &ATariarri::EndTracking, 1.0, false, 1.0f);

	SkeletalMesh->GetAnimInstance()->Montage_Play(DashAnimation);
}

void ATariarri::SpecialAttack()
{
	FVector Destina = GetActorLocation();

	AiController->MoveToLocation(Destina);

	SkeletalMesh->GetAnimInstance()->Montage_Play(SpecialAttackAnimation);
}

void ATariarri::bSetAttack()
{
	bAttackCool = true;
}

void ATariarri::bSetDash()
{
	bDashCool = true;
}

void ATariarri::bSetSpecialAttack()
{
	bSpecialAttackCool = true;
}

void ATariarri::bSetCool()
{
	bCool = true;
}

void ATariarri::Tracking()
{
	FVector Destina = GetActorLocation() + (GetActorForwardVector() * 50);
	AiController->MoveToLocation(Destina);
}

void ATariarri::EndTracking()
{
	FVector Destina = GetActorLocation();

	AiController->MoveToLocation(Destina);

	GetWorldTimerManager().ClearTimer(Dash_TrackingTimer);
}

void ATariarri::RandomAction()
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

void ATariarri::ResetAttackedHero()
{
	AttackedHero = nullptr;
}

void ATariarri::PerceptionUpdated(const TArray<AActor*>& Actors)
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
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Tariarri : BAL GYEON"));
					Action = 2;
				}
				else
				{
					//Player out of sight
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Tariarri : SI BULL"));
					Action = 0;
				}
			}
		}
	}
}

void ATariarri::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AHero * _hero = Cast<AHero>(OtherActor);

	if (_hero && _hero->GetRootComponent() == OtherComp && bColli)
	{
		_hero->PutDamage(DAMAGE);
		bColli = false;
	}
}