// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss1.h"
#include "Monster/MonsterController.h"

bool bTired = false;

// Sets default values
ABoss1::ABoss1()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//애니메이션
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DashMo(TEXT("AnimMontage'/Game/Modeling/Monster/Stage1/Boss/Dash_Montage.Dash_Montage'"));
	DashAnimation = DashMo.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> JumpMo(TEXT("AnimMontage'/Game/Modeling/Monster/Stage1/Boss/Jump_Montage.Jump_Montage'"));
	JumpAnimation = JumpMo.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> BackAttackMo(TEXT("AnimMontage'/Game/Modeling/Monster/Stage1/Boss/Turn_Montage.Turn_Montage'"));
	BackAttackAnimation = BackAttackMo.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SurpriseAttackMo(TEXT("AnimMontage'/Game/Modeling/Monster/Stage1/Boss/Surprise_Attack_Montage.Surprise_Attack_Montage'"));
	SurpriseAttackAnimation = SurpriseAttackMo.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMo(TEXT("AnimMontage'/Game/Modeling/Monster/Stage1/Boss/Attack_Montage.Attack_Montage'"));
	AttackAnimation = AttackMo.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackNormal1Mo(TEXT("AnimMontage'/Game/Modeling/Monster/Stage1/Boss/Attack_Normal1_Montage.Attack_Normal1_Montage'"));
	AttackNormal1Animation = AttackNormal1Mo.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>  AttackNormal2Mo(TEXT("AnimMontage'/Game/Modeling/Monster/Stage1/Boss/Attack_Normal2_Montage.Attack_Normal2_Montage'"));
	AttackNormal2Animation = AttackNormal2Mo.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackNormal3Mo(TEXT("AnimMontage'/Game/Modeling/Monster/Stage1/Boss/Attack_Normal3_Montage.Attack_Normal3_Montage'"));
	AttackNormal3Animation = AttackNormal3Mo.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AppearMo(TEXT("AnimMontage'/Game/Modeling/Monster/Stage1/Boss/Appear_Montage.Appear_Montage'"));
	AppearAnimation = AppearMo.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DieMo(TEXT("AnimMontage'/Game/Modeling/Monster/Stage1/Boss/Dead_Montage.Dead_Montage'"));
	DieAnimation = DieMo.Object;
	
	static ConstructorHelpers::FObjectFinder<UMaterial> Mat(TEXT("Material'/Game/Modeling/Monster/Stage1/Boss/Material__4.Material__4'"));
	Mater = Mat.Object;

	//이펙트
	static ConstructorHelpers::FObjectFinder<UParticleSystem> JumpFX(TEXT("ParticleSystem'/Game/Effect/Monster/Stage1/Boss/Jump.Jump'"));
	JumpEffect = JumpFX.Object;

	GetCharacterMovement()->JumpZVelocity = 0.0f;
	GetCharacterMovement()->MaxStepHeight = 0.0f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 350.0f; //300
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	//GetCharacterMovement()->MaxWalkSpeedCrouched = 170.0f;
	GetCharacterMovement()->bIgnoreBaseRotation = true;
	GetCharacterMovement()->AirControl = 0.0f;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletonMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->RelativeLocation = FVector(0.0f, 0.0f, -90);
	SkeletalMesh->RelativeRotation = FRotator(0, -90, 0);
	SkeletalMesh->RelativeScale3D = FVector(2, 2, 2);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skeleton(TEXT("SkeletalMesh'/Game/Modeling/Monster/Stage1/Boss/Boss1.Boss1'"));
	static ConstructorHelpers::FObjectFinder<UClass> AnimClass(TEXT("AnimBlueprint'/Game/Modeling/Monster/Stage1/Boss/NewAnimBlueprint.NewAnimBlueprint_C'"));
	SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkeletalMesh->AnimClass = AnimClass.Object;
	SkeletalMesh->SetSkeletalMesh(Skeleton.Object);
	SkeletalMesh->SetCollisionProfileName("OverlapAllDynamic");
	SkeletalMesh->bGenerateOverlapEvents = true;
	SkeletalMesh->SetMaterial(0, Mater);

	WeaponCollision = NewObject<UBoxComponent>(this, TEXT("CollisionBox"));
	WeaponCollision->AttachTo(SkeletalMesh, TEXT("bone001"));
	WeaponCollision->bGenerateOverlapEvents = false;
	WeaponCollision->RelativeLocation = FVector(36, 2, 0);
	WeaponCollision->RelativeScale3D = FVector(0.1, 1.3, 0.25);
	WeaponCollision->RelativeRotation = FRotator(0, -85, 0);
	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &ABoss1::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ABoss1::BeginPlay()
{
	Super::BeginPlay();

	Health = 800;

	SkeletalMesh->GetAnimInstance()->Montage_Play(AppearAnimation);

	GetWorldTimerManager().SetTimer(JDTimer, this, &ABoss1::Begin, 3.333f, false, 3.333f);

	GetWorldTimerManager().SetTimer(SATimer, this, &ABoss1::CanSA, 5.0f, false, 5.0f);

	AMyLevelScriptActor * Level = Cast<AMyLevelScriptActor>(GetWorld()->GetLevelScriptActor());
	Level->Block_BossRoom();
}

void ABoss1::Dash() //대쉬 공격
{
	UE_LOG(LogTemp, Warning, TEXT("Boss Dash"));
	AiController->MoveToLocation(GetActorLocation());

	FVector ToT = hero->GetActorLocation() - GetActorLocation();
	FRotator DiRo = ToT.Rotation();
	DiRo.Pitch = 0.0f;
	SetActorRotation(DiRo);

	SkeletalMesh->GetAnimInstance()->Montage_Play(DashAnimation);

	FVector ToA = GetActorLocation() - hero->GetActorLocation();
	FRotator RotoA = ToA.Rotation();

	TargetLo = hero->GetActorLocation() + RotoA.Vector() * 60;

	GetWorldTimerManager().SetTimer(ColliTimer, this, &ABoss1::WeaponColli, 0.35f, false, 0.35f);
	GetWorldTimerManager().SetTimer(Dash_DashTimer, this, &ABoss1::Dash_Dash, 0.4f, false, 0.4f);
	GetWorldTimerManager().SetTimer(CanMoveTimer, this, &ABoss1::CanMove, 0.5f, false, 0.5f);
	GetWorldTimerManager().SetTimer(CanRotTimer, this, &ABoss1::CanRot, 0.5f, false, 0.5f);
	GetWorldTimerManager().SetTimer(CanAttackTimer, this, &ABoss1::CanAttack, 0.5f, false, 0.5f);
}

void ABoss1::Dash_Dash()
{
	SetActorLocation(FVector(TargetLo.X, TargetLo.Y, 52), true);
	AiController->MoveToLocation(GetActorLocation());
}

void ABoss1::Jump() //점프 공격
{
	UE_LOG(LogTemp, Warning, TEXT("Boss Jump"));
	AiController->MoveToLocation(GetActorLocation());

	SkeletalMesh->GetAnimInstance()->Montage_Play(JumpAnimation);

	GetWorldTimerManager().SetTimer(TargetTimer, this, &ABoss1::SetTargetLo, 0.4f, false, 0.4f);
	GetWorldTimerManager().SetTimer(ColliTimer, this, &ABoss1::WeaponColli, 0.668f, false, 0.668f);
	GetWorldTimerManager().SetTimer(Jump_JumpTimer, this, &ABoss1::Jump_Jump, 0.668f, false, 0.668f);
	GetWorldTimerManager().SetTimer(Jump_LandingTimer, this, &ABoss1::Jump_Landing, 0.84f, false, 0.84f);
	GetWorldTimerManager().SetTimer(CanMoveTimer, this, &ABoss1::CanMove, 2.5f, false, 2.5f);
	GetWorldTimerManager().SetTimer(CanRotTimer, this, &ABoss1::CanRot, 2.5f, false, 2.5f);
	GetWorldTimerManager().SetTimer(CanAttackTimer, this, &ABoss1::CanAttack, 2.5f, false, 2.5f);
	GetWorldTimerManager().SetTimer(ClearWeaponColliTimer, this, &ABoss1::ClearWeaponColli, 0.86f, false, 0.86f);
}

void ABoss1::Jump_Jump()
{
	AddActorLocalOffset(FVector(0, 0, 5000));
}

void ABoss1::Jump_Landing()
{
	UCapsuleComponent * RootCom = Cast<UCapsuleComponent>(RootComponent);

	if (RootCom)
	{
		RootCom->SetCollisionProfileName("IgnoreOnlyPawn");
		UE_LOG(LogTemp, Warning, TEXT("Boss IgnoreOnlyPawn"));
	}

	AiController->MoveToLocation(GetActorLocation());
	SetActorLocation(TargetLo);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), JumpEffect, FVector(GetActorLocation().X, GetActorLocation().Y, 0.051f), GetActorRotation());

	FVector ToT = hero->GetActorLocation() - GetActorLocation();
	FRotator DiRo = ToT.Rotation();
	DiRo.Pitch = 0.0f;
	SetActorRotation(DiRo);

	if (RootCom)
	{
		RootCom->SetCollisionProfileName("Pawn");
	}
}

void ABoss1::Back()
{
	AiController->MoveToLocation(GetActorLocation());
}

void ABoss1::Attack() //특수 일반 공격 
{
	UE_LOG(LogTemp, Warning, TEXT("Boss Attack"));
	AiController->MoveToLocation(GetActorLocation());

	FVector ToT = hero->GetActorLocation() - GetActorLocation();
	FRotator DiRo = ToT.Rotation();
	DiRo.Pitch = 0.0f;
	SetActorRotation(DiRo);

	SkeletalMesh->GetAnimInstance()->Montage_Play(AttackAnimation);

	WeaponCollision->bGenerateOverlapEvents = true;
	WeaponCollision->SetRelativeScale3D(FVector(0.75, 1.5, 0.75));

	GetWorldTimerManager().SetTimer(Dash_DashTimer, this, &ABoss1::MoveLocation, 0.2f, false, 0.67f);
	GetWorldTimerManager().SetTimer(MoveTimer, this, &ABoss1::MoveLocation, 0.3f, true, 0.87f);
	GetWorldTimerManager().SetTimer(TargetTimer, this, &ABoss1::ClearMove, 1.6f, false, 1.6f);
	GetWorldTimerManager().SetTimer(CanMoveTimer, this, &ABoss1::CanMove, 2.0f, false, 2.0f);
	GetWorldTimerManager().SetTimer(CanRotTimer, this, &ABoss1::CanRot, 2.0f, false, 2.0f);
	GetWorldTimerManager().SetTimer(CanAttackTimer, this, &ABoss1::CanAttack, 2.0f, false, 2.0f);
	GetWorldTimerManager().SetTimer(CanNormalAttackTimer, this, &ABoss1::CanNormalAttack, 2.0f, false, 2.0f);
}

void ABoss1::AttackNormal1() //일반 공격1 
{
	UE_LOG(LogTemp, Warning, TEXT("Boss Attack1"));
	AiController->MoveToLocation(GetActorLocation());

	FVector ToT = hero->GetActorLocation() - GetActorLocation();
	FRotator DiRo = ToT.Rotation();
	DiRo.Pitch = 0.0f;
	SetActorRotation(DiRo);

	SkeletalMesh->GetAnimInstance()->Montage_Play(AttackNormal1Animation);

	GetWorldTimerManager().SetTimer(ColliTimer, this, &ABoss1::WeaponColli, 0.4f, false, 0.4f);
	GetWorldTimerManager().SetTimer(CanAttackTimer, this, &ABoss1::CanAttack, 1.0f, false, 1.0f);
	GetWorldTimerManager().SetTimer(CanMoveTimer, this, &ABoss1::CanMove, 1.0f, false, 1.0f);
	GetWorldTimerManager().SetTimer(CanRotTimer, this, &ABoss1::CanRot, 1.0f, false, 1.0f);
	GetWorldTimerManager().SetTimer(ClearWeaponColliTimer, this, &ABoss1::ClearWeaponColli, 0.55f, false, 0.55f);
	GetWorldTimerManager().SetTimer(CanNormalAttackTimer, this, &ABoss1::CanNormalAttack, 1.5f, false, 1.5f);
}

void ABoss1::AttackNormal2() //일반 공격2 
{
	UE_LOG(LogTemp, Warning, TEXT("Boss Attack2"));
	AiController->MoveToLocation(GetActorLocation());

	FVector ToT = hero->GetActorLocation() - GetActorLocation();
	FRotator DiRo = ToT.Rotation();
	DiRo.Pitch = 0.0f;
	SetActorRotation(DiRo);

	SkeletalMesh->GetAnimInstance()->Montage_Play(AttackNormal2Animation);

	GetWorldTimerManager().SetTimer(ColliTimer, this, &ABoss1::WeaponColli, 0.4f, false, 0.4f);
	GetWorldTimerManager().SetTimer(CanMoveTimer, this, &ABoss1::CanMove, 0.833f, false, 0.833f);
	GetWorldTimerManager().SetTimer(CanRotTimer, this, &ABoss1::CanRot, 0.833f, false, 0.833f);
	GetWorldTimerManager().SetTimer(CanAttackTimer, this, &ABoss1::CanAttack, 0.833f, false, 0.833f);
	GetWorldTimerManager().SetTimer(ClearWeaponColliTimer, this, &ABoss1::ClearWeaponColli, 0.5f, false, 0.5f);
	GetWorldTimerManager().SetTimer(CanNormalAttackTimer, this, &ABoss1::CanNormalAttack, 1.5f, false, 1.5f);
}

void ABoss1::AttackNormal3() //일반 공격3
{
	UE_LOG(LogTemp, Warning, TEXT("Boss Attack3"));
	AiController->MoveToLocation(GetActorLocation());

	FVector ToT = hero->GetActorLocation() - GetActorLocation();
	FRotator DiRo = ToT.Rotation();
	DiRo.Pitch = 0.0f;
	SetActorRotation(DiRo);

	SkeletalMesh->GetAnimInstance()->Montage_Play(AttackNormal3Animation);

	GetWorldTimerManager().SetTimer(ColliTimer, this, &ABoss1::WeaponColli, 0.3f, false, 0.3f);
	GetWorldTimerManager().SetTimer(CanMoveTimer, this, &ABoss1::CanMove, 0.667f, false, 0.667f);
	GetWorldTimerManager().SetTimer(CanRotTimer, this, &ABoss1::CanRot, 0.667f, false, 0.667f);
	GetWorldTimerManager().SetTimer(CanAttackTimer, this, &ABoss1::CanAttack, 0.667f, false, 0.667f);
	GetWorldTimerManager().SetTimer(ClearWeaponColliTimer, this, &ABoss1::ClearWeaponColli, 0.4f, false, 0.4f);
	GetWorldTimerManager().SetTimer(CanNormalAttackTimer, this, &ABoss1::CanNormalAttack, 1.5f, false, 1.5f);
}

void ABoss1::MoveLocation()
{
	TargetLo = GetActorForwardVector() * 60 + GetActorLocation();
	SetActorLocation(TargetLo, true);
	//AiController->MoveToLocation(TargetLo);
}

void ABoss1::JDT()
{
	bJD = true;
}

void ABoss1::CanMove()
{
	bCanMove = true;

	WeaponCollision->bGenerateOverlapEvents = false;
	WeaponCollision->SetRelativeScale3D(FVector(0.1, 1.3, 0.25));
}

void ABoss1::CanRot()
{
	bCanRot = true;

	WeaponCollision->bGenerateOverlapEvents = false;
	WeaponCollision->SetRelativeScale3D(FVector(0.1, 1.3, 0.25));
}

void ABoss1::CanSA()
{
	bSA = true;
}

void ABoss1::CanAttack()
{
	bCanAttack = true;
}

void ABoss1::CanNormalAttack()
{
	bCanNormalAttack = true;
}

void ABoss1::SetTargetLo()
{
	TargetLo = hero->GetActorLocation();

	FVector ToT = hero->GetActorLocation() - GetActorLocation();
	FRotator DiRo = ToT.Rotation();
	DiRo.Pitch = 0.0f;
	SetActorRotation(DiRo);
}

void ABoss1::Surprise_Attack() //기습 공격 
{
	UE_LOG(LogTemp, Warning, TEXT("Surprise_Attack"));
	AiController->MoveToLocation(GetActorLocation());

	SkeletalMesh->GetAnimInstance()->Montage_Play(SurpriseAttackAnimation);
	GetWorldTimerManager().SetTimer(TargetTimer, this, &ABoss1::SetTargetLo, 0.4f, false, 0.4f);
	GetWorldTimerManager().SetTimer(ColliTimer, this, &ABoss1::WeaponColli, 0.5f, false, 0.5f);
	GetWorldTimerManager().SetTimer(Dash_DashTimer, this, &ABoss1::Dash_Dash, 0.52f, false, 0.52f);
	GetWorldTimerManager().SetTimer(CanMoveTimer, this, &ABoss1::CanMove, 0.8f, false, 0.8f);
	GetWorldTimerManager().SetTimer(CanRotTimer, this, &ABoss1::CanRot, 0.8f, false, 0.8f);
	GetWorldTimerManager().SetTimer(CanAttackTimer, this, &ABoss1::CanAttack, 0.8f, false, 0.8f);
}

void ABoss1::WeaponColli()
{
	WeaponCollision->bGenerateOverlapEvents = true;
	WeaponCollision->SetRelativeScale3D(FVector(0.5, 1.3, 0.5));
}

void ABoss1::ClearWeaponColli()
{
	WeaponCollision->bGenerateOverlapEvents = false;
}

void ABoss1::ClearMove()
{
	GetWorldTimerManager().ClearTimer(MoveTimer);
}

void ABoss1::SendTired(bool Tired)
{
	UBossAnim * Anim = Cast<UBossAnim>(SkeletalMesh->GetAnimInstance());
	Anim->GetBossTired(Tired);
}

void ABoss1::Begin()
{
	bJD = true;
	bCanMove = true;
	bCanRot = true;
	//bSA = false;
	bCanAttack = true;
	bCanNormalAttack = true;
}

void ABoss1::End()
{
	Destroy();
	AMyLevelScriptActor * Level = Cast<AMyLevelScriptActor>(GetWorld()->GetLevelScriptActor());
	Level->ShowGate();
}

void ABoss1::PlusInstance()
{
	ssfd += 0.0065;

	UMaterialInstanceDynamic * Instance = UMaterialInstanceDynamic::Create(Mater, nullptr);

	Instance->SetScalarParameterValue(TEXT("Burn"), ssfd);
	SkeletalMesh->SetMaterial(0, Instance);

	if (ssfd >= 1.0)
	{
		GetWorldTimerManager().ClearTimer(JDTimer);	
	}

	UE_LOG(LogTemp, Warning, TEXT("%f"), ssfd);
}

// Called every frame
void ABoss1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AiController && Health > 0)
	{
		if (hero->GetDistanceTo(this) >= 180 && bJD && bCanAttack) //특수 공격 조건
		{
			int ran = (rand() % 2);

			if (ran == 0)
			{
				Dash(); //대쉬 공격 
			}
			else
			{
				Jump(); //점프 공격
			}
			GetWorldTimerManager().SetTimer(JDTimer, this, &ABoss1::JDT, 5.0f, false, 5.0f);
			bJD = false;
			bCanMove = false;
			bCanRot = false;
			bCanAttack = false;
		}

		else if (hero->GetDistanceTo(this) < 40 && bCanAttack && bCanNormalAttack) //일반공격 조건 
		{
			int ran = (rand() % 7);
			bCanNormalAttack = false;

			switch (ran)
			{
			case 0:
			case 1:
				AttackNormal1();
				break;

			case 2:
			case 3:
				AttackNormal2();
				break;

			case 4:
			case 5:
				AttackNormal3();
				break;

			case 6:
				Attack(); //특수 일반 공격
				break;

			default:
				break;
			}

			bCanMove = false;
			bCanRot = false;
			bCanAttack = false;
		}

		else
		{
			if ((bCanMove && !bJD) || (Health <= 200 && bCanMove)) //거리 벌리고 이동
			{
				int ran = (rand() % 4);

				if (ran == 0)
				{
					FVector t = hero->GetActorRightVector() * 125 + hero->GetActorLocation();
					AiController->MoveToLocation(t);
				}
				else if (ran == 1)
				{
					FVector t = hero->GetActorRightVector() * (-125) + hero->GetActorLocation();
					AiController->MoveToLocation(t);
				}

				else if (ran == 2)
				{
					FVector t = hero->GetActorForwardVector() * 125 + hero->GetActorLocation();
					AiController->MoveToLocation(t);
				}

				else
				{
					FVector t = hero->GetActorForwardVector() * (-125) + hero->GetActorLocation();
					AiController->MoveToLocation(t);
				}

				GetWorldTimerManager().SetTimer(CanMoveTimer, this, &ABoss1::CanMove, 1.0f, false, 1.0f);

				bCanMove = false;
			}

			else if (bCanMove) //붙는 이동
			{
				FVector ToA = GetActorLocation() - hero->GetActorLocation();
				FRotator RotoA = ToA.Rotation();

				FVector t = hero->GetActorLocation() + RotoA.Vector() * 15;
				AiController->MoveToLocation(t);

				GetWorldTimerManager().SetTimer(CanMoveTimer, this, &ABoss1::CanMove, 0.5f, false, 0.5f);

				bCanMove = false;

				UE_LOG(LogTemp, Warning, TEXT("Move"));
			}

			if (bCanRot) //회전 
			{
				FVector ToT = hero->GetActorLocation() - GetActorLocation();
				FRotator DiRo = ToT.Rotation();
				DiRo.Pitch = 0.0f;
				SetActorRotation(DiRo);

				FRotator BossRot = GetActorRotation();
				FRotator HeroRot = hero->GetActorRotation();

				if (BossRot.Yaw - 45 < HeroRot.Yaw && BossRot.Yaw + 45 > HeroRot.Yaw && bSA  && bCanAttack) //기습 공격
				{
					Surprise_Attack();
					GetWorldTimerManager().SetTimer(SATimer, this, &ABoss1::CanSA, 10.0f, false, 10.0f);
					bSA = false;
					bCanMove = false;
					bCanRot = false;
					bCanAttack = false;
				}
			}
		}
	}
	else
	{
		AiController = Cast<AAIController>(GetController());
	}
}

void ABoss1::PutDamage(int damage, AHero * hero)
{
	Health -= damage;

	if (Health <= 0 && !bDead)
	{
		GetCharacterMovement()->GravityScale = 0.0f;

		UCapsuleComponent * RootCom = Cast<UCapsuleComponent>(RootComponent);

		if (RootCom)
		{
			RootCom->SetCollisionProfileName("NoCollision");
		}
		GetMesh()->SetCollisionProfileName("NoCollision");

		SkeletalMesh->SetCollisionProfileName("NoCollision");

		SkeletalMesh->bGenerateOverlapEvents = false;

		SkeletalMesh->GetAnimInstance()->Montage_Play(DieAnimation);

		GetWorldTimerManager().ClearAllTimersForObject(this);
		
		GetWorldTimerManager().SetTimer(TargetTimer, this, &ABoss1::End, 5.33f, false, 5.33f);

		GetWorldTimerManager().SetTimer(JDTimer, this, &ABoss1::PlusInstance, 0.0533f, true, 0.0f);

		bJD = false;
		bCanMove = false;
		bCanRot = false;
		//bSA = false;
		bCanAttack = false;
		bCanNormalAttack = false;
		bDead = true;
	}

	else if (!bTired && Health <= 200)
	{
		bTired = true;
		SendTired(bTired);
	}

	UE_LOG(LogTemp, Warning, TEXT("%d"), Health);
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Boss Get Damage"));
}

void ABoss1::PauseBoss()
{
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
}

void ABoss1::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AHero * _hero = Cast<AHero>(OtherActor);
	AMonsterMgr * _Monster = Cast<AMonsterMgr>(OtherActor);

	if (_hero && _hero->GetRootComponent() == OtherComp)
	{
		_hero->PutDamage(32);
	}

	else if (_Monster)
	{
		_Monster->PutDamage(999999);
	}
}