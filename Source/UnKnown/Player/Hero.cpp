// Fill out your copyright notice in the Description page of Project Settings.

#include "Hero.h"
#include "Monster/MonsterMgr.h"
#include "Object/SkillCircleCollision.h"
#include "Afterimage.h"

#define AFTERIMAGE_TIMER 0.2f

// Sets default values
AHero::AHero()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1080.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 0.0f;
	GetCharacterMovement()->MaxStepHeight = 0.0f;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	GetCharacterMovement()->AirControl = 0.0f;

	RootComponent->RelativeScale3D	= FVector(0.25f, 0.25f, 0.25f);
	
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletonMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->RelativeLocation	= FVector(0.0f, 0.0f, -90);
	SkeletalMesh->RelativeScale3D	= FVector(1, 1, 1);
	SkeletalMesh->RelativeRotation	= FRotator(0, -90, 0);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> WSkillMo(TEXT("AnimMontage'/Game/Animation/Character/Skill/W_Montage.W_Montage'"));
	W_Skill_Animation = WSkillMo.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ESkillMo(TEXT("AnimMontage'/Game/Animation/Character/Skill/E_Montage.E_Montage'"));
	E_Skill_Animation = ESkillMo.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReinWSkillMo(TEXT("AnimMontage'/Game/Animation/Character/Skill/Rein_W_Montage.Rein_W_Montage'"));
	Rein_W_Skill_Animation = ReinWSkillMo.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReinESkillMo(TEXT("AnimMontage'/Game/Animation/Character/Skill/Rein_E_Montage.Rein_E_Montage'"));
	Rein_E_Skill_Animation = ReinESkillMo.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReinRSkillMo(TEXT("AnimMontage'/Game/Animation/Character/Skill/Rein_R_Montage.Rein_R_Montage'"));
	Rein_R_Skill_Animation = ReinRSkillMo.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> NoAttack1Mo(TEXT("AnimMontage'/Game/Animation/Character/Attack_Montage.Attack_Montage'"));
	NormalAttack1_Animation = NoAttack1Mo.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> NoAttack2Mo(TEXT("AnimMontage'/Game/Animation/Character/Attack1_Montage.Attack1_Montage'"));
	NormalAttack2_Animation = NoAttack2Mo.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> NoAttack3Mo(TEXT("AnimMontage'/Game/Animation/Character/Attack2_Montage.Attack2_Montage'"));
	NormalAttack3_Animation = NoAttack3Mo.Object;

	ConstructorHelpers::FObjectFinder<UParticleSystem> R_Particle(TEXT("ParticleSystem'/Game/Effect/Particle/Skill/RPassive.RPassive'"));
	Rein_R_Particle = R_Particle.Object;
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Rein_R_FX(TEXT("ParticleSystem'/Game/Effect/Particle/Skill/R.R'"));
	Rein_R_Effect = Rein_R_FX.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> W_FX(TEXT("ParticleSystem'/Game/Effect/Particle/Skill/W.W'"));
	W_Effect = W_FX.Object;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skeleton(TEXT("SkeletalMesh'/Game/Animation/Character/default.default'"));
	static ConstructorHelpers::FObjectFinder<UClass> AnimClass(TEXT("/Game/Animation/Character/NewAnimBlueprint.NewAnimBlueprint_C"));
	SkeletalMesh->SetSkeletalMesh(Skeleton.Object);
	SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkeletalMesh->AnimClass = AnimClass.Object;

	ArmourMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmourMesh"));
	ArmourMesh->SetupAttachment(SkeletalMesh);
	ArmourMesh->SetMasterPoseComponent(SkeletalMesh);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Armour(TEXT("SkeletalMesh'/Game/Modeling/Character/Warrior/Armour/Armour.Armour'"));
	ArmourMesh->SetSkeletalMesh(Armour.Object);
	ArmourMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	ArmourMesh->AnimClass = AnimClass.Object;

	Weapon = NewObject<UStaticMeshComponent>(this, TEXT("Weapon"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMesh(TEXT("StaticMesh'/Game/Modeling/Character/Warrior/Sword/Sword.Sword'"));
	Weapon->SetStaticMesh(WeaponMesh.Object);
	Weapon->AttachTo(SkeletalMesh, TEXT("Bone001"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Weapon->RelativeRotation	= FRotator(-0.000052f, 0.000225f, 0.000056f);
	Weapon->RelativeScale3D		= FVector(1.75f, 1.0f, 1.0f);

	WeaponCollision = NewObject<UBoxComponent>(this, TEXT("CollisionBox"));
	WeaponCollision->AttachTo(Weapon);
	WeaponCollision->bGenerateOverlapEvents = true;
	WeaponCollision->RelativeLocation	= FVector(80.0f, 0, 0);
	WeaponCollision->RelativeScale3D	= FVector(1.75, 0.25, 0.125);
	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AHero::OnOverlapBegin);

	RenderCollision = NewObject<UBoxComponent>(this, TEXT("RenderCollisionBox"));
	RenderCollision->AttachTo(RootComponent);
	RenderCollision->bGenerateOverlapEvents = true;
	RenderCollision->RelativeLocation	= FVector(0, 0, -50);
	RenderCollision->RelativeScale3D	= FVector(75, 75, 50);
	RenderCollision->OnComponentBeginOverlap.AddDynamic(this, &AHero::OnRednerCollOverlapBegin);
	RenderCollision->OnComponentEndOverlap.AddDynamic(this, &AHero::OnRednerCollOverlapEnd);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 0.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->RelativeLocation = FVector(-520, 0, 1050);
	FollowCamera->RelativeRotation = FRotator(-60, 0, 0);

	ActiveBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ActiveBox"));
	ActiveBox->AttachTo(RootComponent);
	ActiveBox->RelativeLocation = FVector(55, 0, 0);
	ActiveBox->RelativeScale3D	= FVector(3, 1, 1);

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystem->SetupAttachment(SkeletalMesh);
	ParticleSystem->RelativeLocation = FVector(0, 0, 40);
	//ParticleSystem->SetTemplate(Fire.Object);
}

// Called when the game starts or when spawned
void AHero::BeginPlay()
{
	Super::BeginPlay();

	DashTime	 = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	AttackTime	 = UGameplayStatics::GetRealTimeSeconds(GetWorld());

	CanMove	  = true;
	Alive	  = true;
	CanAttack = true;
	bGodMode  = false;

	//플레이어 정보 설정
	Health		= 100;
	MaxHealth	= 100;
	Mana		= 100;
	MaxMana		= 100;
}

void AHero::InvenUpdate(int _count, UObject * _Image, UObject * _Slot, FString _Name, FString _Explain, bool _Consumable, int _Parts, int _Code)
{
	InvenInfoUpdate.Broadcast(_count, _Image, _Slot, _Name, _Explain, _Consumable, _Parts, _Code);
}

void AHero::ItemUpdate(float _Weight, UObject * _Image, FString _Name, FString _Explain, AItem * _Item, bool _Consumables, int _Parts, int _Code)
{
	ItemInfoUpdate.Broadcast(_Weight, _Image, _Name, _Explain, _Item, _Consumables, _Parts, _Code);
}

void AHero::SendToolTipInfo(FString _Name, FString _Explain, int _Slot)
{
	ToolTipInfoUpdate.Broadcast(_Name, _Explain, _Slot);
}

void AHero::HealHero(int _Heal)
{
	Health += _Heal;
	if (Health > MaxHealth)
	{
		Health = MaxHealth;
	}

	HealthUpdate.Broadcast(Health);
}

// Called every frame
void AHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("FB_Move", this, &AHero::FB_Move);
	PlayerInputComponent->BindAxis("LR_Move", this, &AHero::LR_Move);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AHero::Dash);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AHero::Attack);
	PlayerInputComponent->BindAction("WSkill", IE_Pressed, this, &AHero::WSkill);
	PlayerInputComponent->BindAction("ESkill", IE_Pressed, this, &AHero::ESkill);
	PlayerInputComponent->BindAction("RSkill", IE_Pressed, this, &AHero::RSkill);

	PlayerInputComponent->BindAction("Map", IE_Pressed, this, &AHero::UpdateMap);

	PlayerInputComponent->BindAction("Active1", IE_Pressed, this, &AHero::UseActive1);
	PlayerInputComponent->BindAction("Active2", IE_Pressed, this, &AHero::UseActive2);

	//반드시 나중에 삭제 할 것
	PlayerInputComponent->BindAction("GodMode", IE_Pressed, this, &AHero::GodMode);
	PlayerInputComponent->BindAction("SpeedUp", IE_Pressed, this, &AHero::SpeedUp);
}

void AHero::PutDamage(int damage) //데미지를 받음
{
	if (!bGodMode)
	{
		Health -= damage;

		HealthUpdate.Broadcast(Health);

		UE_LOG(LogTemp, Warning, TEXT("%d"), Health);

		OpenInven.Broadcast();

		if (Health <= 0)
		{
			DieUpdate.Broadcast();
			Alive = false;

			GetCharacterMovement()->GravityScale = 0.0f;

			APlayerController * PlayerController = Cast<APlayerController>(Controller);

			if (PlayerController)
				DisableInput(PlayerController);
		}
	}
}

void AHero::FB_Move(float value)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	FB_Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FB_Value = value;

	if (value && CanMove)
	{
		AddMovementInput(FB_Direction, FB_Value);
	}
}

void AHero::LR_Move(float value)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	LR_Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	LR_Value = value;

	if (value && CanMove)
	{
		AddMovementInput(LR_Direction, LR_Value);
	}
}

void AHero::Dash() //대쉬
{
	if ((DashTime + 5) < UGameplayStatics::GetRealTimeSeconds(GetWorld()))
	{
		DashUpdate.Broadcast();

		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, TEXT("DASH!!"));

		//AddActorWorldOffset(GetActorForwardVector() * 200, true);
		GetWorldTimerManager().SetTimer(ReinESkill_Dash_Timer, this, &AHero::ReinESkill_Dash, 0.01f, true, 0.0f);
		GetWorldTimerManager().SetTimer(ReinESkill_End_Dash_Timer, this, &AHero::ReinESkill_End_Dash, 0.3f, false, 0.3f);

		DashTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	}
}

void AHero::Attack() //일반 공격
{
	if (!bUseActiveObject)
	{
		if ((AttackTime + 1) < UGameplayStatics::GetRealTimeSeconds(GetWorld()) && CanAttack)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, TEXT("Attack"));

			UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
			NavSys->SimpleMoveToLocation(GetController(), GetActorLocation());

			if (FB_Value != 0 || LR_Value != 0)
			{
				SetActorRotation(FVector(FB_Value, LR_Value, 0).Rotation());
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, TEXT("Rot"));
			}
			CanAttack = false;

			CanMove = false;

			SwordCollision = true; //웨폰 콜리전 켬

			//AttackUpdate.Broadcast(AttackCount); //블루프린트에게 통신

			switch (AttackCount)
			{
			case 0:
				SkeletalMesh->GetAnimInstance()->Montage_Play(NormalAttack1_Animation);

				GetWorldTimerManager().SetTimer(AttackTimerSetTime, this, &AHero::AttackTimerSet, 1.0f, false, 1.0f);
				GetWorldTimerManager().SetTimer(MoveTimer, this, &AHero::SetCanMove, 0.433f, false, 0.433f);
				GetWorldTimerManager().SetTimer(CollisionTime, this, &AHero::SetSwordCollision, 0.29f, false, 0.29f);//웨폰 콜리전 끔

				if (bReinMode)
				{
					GetWorldTimerManager().SetTimer(ReinMode_Timer, this, &AHero::ReinMode_Attack1, AFTERIMAGE_TIMER, false, AFTERIMAGE_TIMER);
				}
				break;

			case 1:
				SkeletalMesh->GetAnimInstance()->Montage_Play(NormalAttack2_Animation);

				CanMove = true;
				GetWorldTimerManager().SetTimer(AttackTimerSetTime, this, &AHero::AttackTimerSet, 1.0f, false, 1.0f);
				GetWorldTimerManager().SetTimer(CollisionTime, this, &AHero::SetSwordCollision, 0.246f, false, 0.246f);//웨폰 콜리전 끔

				if (bReinMode)
				{
					GetWorldTimerManager().SetTimer(ReinMode_Timer, this, &AHero::ReinMode_Attack2, AFTERIMAGE_TIMER, false, AFTERIMAGE_TIMER);
				}
				break;

			case 2:
				SkeletalMesh->GetAnimInstance()->Montage_Play(NormalAttack3_Animation);

				GetWorldTimerManager().SetTimer(AttackTimerSetTime, this, &AHero::AttackTimerSet, 1.0f, false, 1.0f);
				GetWorldTimerManager().SetTimer(MoveTimer, this, &AHero::SetCanMove, 0.4f, false, 0.4f);
				GetWorldTimerManager().SetTimer(CollisionTime, this, &AHero::SetSwordCollision, 0.12f, false, 0.12f);//웨폰 콜리전 끔

				if (bReinMode)
				{
					GetWorldTimerManager().SetTimer(ReinMode_Timer, this, &AHero::ReinMode_Attack3, AFTERIMAGE_TIMER, false, AFTERIMAGE_TIMER);
				}
				break;

			default:
				break;
			}

			AttackCount++;
			UE_LOG(LogTemp, Warning, TEXT("Attack %d"), AttackCount);

			if (AttackCount > 2) 
			{
				GetWorldTimerManager().ClearTimer(AttackTimerSetTime);
				AttackTimerSet();
				UE_LOG(LogTemp, Warning, TEXT("3 Attack"));
			}
		}
	}
	else
	{
		ActiveObject->SetHero(this);
	}
}
/* 스킬 코드 */
void AHero::WSkill() //검을 치켜드면서 기 방출
{
	if (bReinSkill)
	{
		ReinWSkill();
		return;
	}
	//밑에 기본 스킬 구성 코드

	if ((WSkillCoolTime + 5) < UGameplayStatics::GetRealTimeSeconds(GetWorld()))
	{
		CanMove = false;

		SkeletalMesh->GetAnimInstance()->Montage_Play(W_Skill_Animation);

		GetWorldTimerManager().SetTimer(WSkill_Damage_Timer, this, &AHero::WSkill_Damage, 0.2f, false, 0.2f);

		GetWorldTimerManager().SetTimer(MoveTimer, this, &AHero::SetCanMove, 0.5f, false, 0.5f);

		WSkillCoolTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());

		if (bReinMode)
		{
			GetWorldTimerManager().SetTimer(ReinMode_Timer, this, &AHero::ReinMode_WSkill, AFTERIMAGE_TIMER, false, AFTERIMAGE_TIMER);
		}
	}
}

void AHero::ESkill() //검을 2번 벰
{
	if (bReinSkill)
	{
		ReinESkill();
		return;
	}
	//밑에 기본 스킬 구성 코드
	if ((ESkillCoolTime + 3) < UGameplayStatics::GetRealTimeSeconds(GetWorld()))
	{
		SkeletalMesh->GetAnimInstance()->Montage_Play(E_Skill_Animation);

		WeaponCollision->SetRelativeScale3D(FVector(2.5, 2.25, 2.125));

		SwordCollision = true; //웨폰 콜리전 켬
		CanMove = false;

		GetWorldTimerManager().SetTimer(CollisionTime, this, &AHero::SetSwordCollision, 0.667f, false, 0.667f);
		GetWorldTimerManager().SetTimer(MoveTimer, this, &AHero::SetCanMove, 0.667f, false, 0.667f);

		ESkillCoolTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());

		if (bReinMode)
		{
			GetWorldTimerManager().SetTimer(ReinMode_Timer, this, &AHero::ReinMode_ESkill, AFTERIMAGE_TIMER, false, AFTERIMAGE_TIMER);
		}
	}
}

void AHero::RSkill() //스킬 강화
{
	if (bReinSkill)
	{
		ReinRSkill();
		return;
	}

	bReinSkill = true;
}

void AHero::ReinWSkill() //제자리에서 점프하면서 검을 역수로 잡고 바닥에 내려찍음
{
	bReinSkill = false;
	CanMove = false;

	SkeletalMesh->GetAnimInstance()->Montage_Play(Rein_W_Skill_Animation);

	GetWorldTimerManager().SetTimer(ReinWSkill_Jump_Timer, this, &AHero::ReinWSkill_Jump, 0.1f, false, 0.1f);
	GetWorldTimerManager().SetTimer(ReinWSkill_Land_Timer, this, &AHero::ReinWSkill_Land, 0.4f, false, 0.4f);
	GetWorldTimerManager().SetTimer(MoveTimer, this, &AHero::SetCanMove, 0.833f, false, 0.833f);

	if (bReinMode)
	{
		GetWorldTimerManager().SetTimer(ReinMode_Timer, this, &AHero::ReinMode_ReinWSkill, AFTERIMAGE_TIMER, false, AFTERIMAGE_TIMER);
	}
}

void AHero::ReinESkill() //검을 잡고 빠르게 날아가면서 벰
{
	bReinSkill = false;
	CanMove = false;

	SwordCollision = true;

	int ad = (Stat_Weapon.Attack + AttackDamage) * 1.5;
	AttackDamage = ad;

	SkeletalMesh->GetAnimInstance()->Montage_Play(Rein_E_Skill_Animation);

	GetWorldTimerManager().SetTimer(ReinESkill_Dash_Timer, this, &AHero::ReinESkill_Dash, 0.01f, true, 0.3f);
	GetWorldTimerManager().SetTimer(ReinESkill_End_Dash_Timer, this, &AHero::ReinESkill_End_Dash, 0.6f, false, 0.6f);
	GetWorldTimerManager().SetTimer(MoveTimer, this, &AHero::SetCanMove, 0.833f, false, 0.833f);
	GetWorldTimerManager().SetTimer(CollisionTime, this, &AHero::SetSwordCollision, 0.833f, false, 0.833f);

	if (bReinMode)
	{
		GetWorldTimerManager().SetTimer(ReinMode_Timer, this, &AHero::ReinMode_ReinESkill, AFTERIMAGE_TIMER, false, AFTERIMAGE_TIMER);
	}
}

void AHero::ReinRSkill() //기방출 후 공격 강화
{
	bReinSkill = false;
	CanMove = false;
	bGodMode = true;
	bReinMode = true;

	SkeletalMesh->GetAnimInstance()->Montage_Play(Rein_R_Skill_Animation);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Rein_R_Effect, FVector(GetActorLocation().X, GetActorLocation().Y, 0.051f), GetActorRotation());
	ParticleSystem->SetTemplate(Rein_R_Particle);

	GetWorldTimerManager().SetTimer(ReinRSkill_Circle_Timer, this, &AHero::ReinRSkill_Circle, 0.74f, false, 0.74f);
	GetWorldTimerManager().SetTimer(MoveTimer, this, &AHero::SetCanMove, 1.33f, false, 1.33f);
	GetWorldTimerManager().SetTimer(EndReinMode_Timer, this, &AHero::EndReinMode, 60.0f, false, 60.0f);
}

/* 스킬 코드 */
void AHero::AttackTimerSet()
{
	AttackTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	AttackCount = 0;
	CanAttack = true;

	UE_LOG(LogTemp, Warning, TEXT("Clear Attack Timer"));
}

void AHero::SetCanMove() //움직임 가능
{
	GetWorldTimerManager().ClearTimer(MoveTimer);
	CanMove = true;
	bGodMode = false;
}

void AHero::SetSwordCollision() //무기 콜리전 오프
{
	WeaponCollision->SetRelativeScale3D(FVector(1.75, 0.25, 0.125));

	SwordCollision = false;
	CanAttack = true;
}

void AHero::UpdateMap() //지도 열기
{
	MapUpdate.Broadcast();
}

void AHero::UseActive1() //액티브 아이템 사용 
{
	UseActiveItem(Active1);
}

void AHero::UseActive2() //액티브 아이템 사용 
{
	UseActiveItem(Active2);
}

void AHero::UseActiveItem(FString _Name) //액티브 아이템
{
	if (_Name == TEXT("작은 포션"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, TEXT("회복"));
		Health += 20; //체력이 차는 정도 
		if (Health > MaxHealth)
		{
			Health = MaxHealth;
		}
		HealthUpdate.Broadcast(Health);
		ActiveItemInfoUpdate.Broadcast(_Name, 5);
	}

	else if (_Name == TEXT("큰 포션"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, TEXT("회복"));
		Health += 50; //체력이 차는 정도
		if (Health > MaxHealth)
		{
			Health = MaxHealth;
		}
		HealthUpdate.Broadcast(Health);
		ActiveItemInfoUpdate.Broadcast(_Name, 10);
	}
}

void AHero::GodMode()//삭제 할 것
{
	Health = 99999999;
}

void AHero::SpeedUp()//삭제 할 것
{
	GetCharacterMovement()->MaxWalkSpeed = 600;
}

void AHero::ReinWSkill_Jump()
{
	SkeletalMesh->SetRelativeLocation(FVector(0, 0, 300));
}

void AHero::ReinWSkill_Land()
{
	SkeletalMesh->SetRelativeLocation(FVector(0, 0, -90));

	int ad = (Stat_Weapon.Attack + AttackDamage) * 2;

	GetWorld()->SpawnActor<ASkillCircleCollision>(ASkillCircleCollision::StaticClass(),
		FTransform(FRotator(0, 0, 0),
			FVector(GetActorLocation().X, GetActorLocation().Y, 20),
			FVector(1, 1, 1)))->SetSetting(ad, this, FVector(2, 2, 2), 0.075);
}

void AHero::ReinESkill_Dash()
{
	UCapsuleComponent * RootCom = Cast<UCapsuleComponent>(RootComponent);
	if (RootCom)
	{
		RootCom->SetCollisionProfileName("IgnoreOnlyPawn");
	}

	WeaponCollision->SetRelativeScale3D(FVector(2,2,2));

	FHitResult * HitRe = new FHitResult();

	AddActorWorldOffset(GetActorForwardVector()*10, true, HitRe);
	
	if (Cast<AMonsterMgr>(HitRe->Actor) || Cast<AItem>(HitRe->Actor))
	{
		HitRe->GetActor()->GetName();

		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, HitRe->GetActor()->GetName());

		AddActorWorldOffset(GetActorForwardVector() * 10);
	}
}

void AHero::ReinESkill_End_Dash()
{
	UCapsuleComponent * RootCom = Cast<UCapsuleComponent>(RootComponent);
	if (RootCom)
	{
		RootCom->SetCollisionProfileName("Pawn");
	}

	WeaponCollision->SetRelativeScale3D(FVector(1.75, 0.25, 0.125));

	AttackDamage = 25;

	GetWorldTimerManager().ClearTimer(ReinESkill_Dash_Timer);
}

void AHero::EndReinMode()
{
	bReinMode = false;
	ParticleSystem->SetTemplate(nullptr);
}

void AHero::ReinRSkill_Circle()
{
	int ad = (Stat_Weapon.Attack + AttackDamage) * 2.5;

	GetWorld()->SpawnActor<ASkillCircleCollision>(ASkillCircleCollision::StaticClass(),
		FTransform(FRotator(0, 0, 0),
			FVector(GetActorLocation().X, GetActorLocation().Y, 20),
			FVector(1, 1, 1)))->SetSetting(ad, this, FVector(2, 2, 2), 0.075);
}

void AHero::SetActive1(FString _Name) //액티브 아이템 세팅
{
	Active1 = _Name;
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, _Name);
}

void AHero::SetActive2(FString _Name) //액티브 아이템 세팅
{
	Active2 = _Name;
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, _Name);
}

void AHero::InitActive(FString _Name) //액티브 아이템 초기화
{
	if (Active1 == _Name)
	{
		Active1 = "";
	} 

	if (Active2 == _Name)
	{
		Active2 = "";
	}
}

void AHero::ChangeItem(int _NewItemCode) //아이템 변경
{
	switch (_NewItemCode)
	{
	case 2:
		Stat_Weapon.Init();
		Stat_Weapon.Attack = 50;
		break;
	}
}

void AHero::SetActiveObject(AActiveObjectMgr * _Object)
{
	ActiveObject = _Object;
}

void AHero::GetPlayerInfo()
{
	MaxHealthUpdate.Broadcast(MaxHealth);
	HealthUpdate.Broadcast(Health);
	MaxManaUpdate.Broadcast(MaxMana);
	ManaUpdate.Broadcast(Mana);
}

void AHero::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (SwordCollision)
	{
		AMonsterMgr * mon = Cast<AMonsterMgr>(OtherActor);
		if (mon)
		{
			int ad = Stat_Weapon.Attack + AttackDamage;
			mon->PutDamage(ad, this);
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, OtherComp->GetName());
		}
	}
}

void AHero::OnRednerCollOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	OtherActor->SetActorHiddenInGame(false);

	AMonsterMgr * mon = Cast<AMonsterMgr>(OtherActor);
	AObjectMgr * obj = Cast<AObjectMgr>(OtherActor);

	if (mon)
	{
		mon->SetUnPause();
	}

	else if (obj)
	{
		obj->ResetActor();
	}
}

void AHero::OnRednerCollOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMonsterMgr * mon = Cast<AMonsterMgr>(OtherActor);
	if (mon)
	{
		mon->SetPause();
		mon->PauseBoss();
		return;
	}

	OtherActor->SetActorHiddenInGame(true);
}

void AHero::WSkill_Damage()
{
	int ad = (Stat_Weapon.Attack + AttackDamage) * 1.5;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), W_Effect, FVector(GetActorLocation().X, GetActorLocation().Y, 0.051f), GetActorRotation());

	GetWorld()->SpawnActor<ASkillCircleCollision>(ASkillCircleCollision::StaticClass(),
		FTransform(FRotator(0, 0, 0),
			FVector(GetActorLocation().X, GetActorLocation().Y, 20),
			FVector(1, 1, 1)))->SetSetting(ad, this, FVector(1.5, 1.5, 1.5), 0.075);
}

void AHero::ReinMode_Attack1()
{
	int ad = Stat_Weapon.Attack + AttackDamage;

	GetWorld()->SpawnActor<AAfterimage>(AAfterimage::StaticClass(),
		FTransform(
			FRotator(GetActorRotation()),
			FVector(GetActorLocation()),
			FVector(1, 1, 1)))->Attack(this, ad, 0);
}

void AHero::ReinMode_Attack2()
{
	int ad = Stat_Weapon.Attack + AttackDamage;

	GetWorld()->SpawnActor<AAfterimage>(AAfterimage::StaticClass(),
		FTransform(
			FRotator(GetActorRotation()),
			FVector(GetActorLocation()),
			FVector(1, 1, 1)))->Attack(this, ad, 1);
}

void AHero::ReinMode_Attack3()
{
	int ad = Stat_Weapon.Attack + AttackDamage;

	GetWorld()->SpawnActor<AAfterimage>(AAfterimage::StaticClass(),
		FTransform(
			FRotator(GetActorRotation()),
			FVector(GetActorLocation()),
			FVector(1, 1, 1)))->Attack(this, ad, 2);
}

void AHero::ReinMode_WSkill()
{
	int ad = (Stat_Weapon.Attack + AttackDamage) * 1.5;

	GetWorld()->SpawnActor<AAfterimage>(AAfterimage::StaticClass(),
		FTransform(
			FRotator(GetActorRotation()),
			FVector(GetActorLocation()),
			FVector(1, 1, 1)))->WSkill(this, ad);
}

void AHero::ReinMode_ESkill()
{
	int ad = (Stat_Weapon.Attack + AttackDamage);

	GetWorld()->SpawnActor<AAfterimage>(AAfterimage::StaticClass(),
		FTransform(
			FRotator(GetActorRotation()),
			FVector(GetActorLocation()),
			FVector(1, 1, 1)))->ESkill(this, ad);
}

void AHero::ReinMode_ReinWSkill()
{
	int ad = (Stat_Weapon.Attack + AttackDamage) * 2;

	GetWorld()->SpawnActor<AAfterimage>(AAfterimage::StaticClass(),
		FTransform(
			FRotator(GetActorRotation()),
			FVector(GetActorLocation()),
			FVector(1, 1, 1)))->ReinWSkill(this, ad);
}

void AHero::ReinMode_ReinESkill()
{
	int ad = (Stat_Weapon.Attack + AttackDamage) * 1.5;

	GetWorld()->SpawnActor<AAfterimage>(AAfterimage::StaticClass(),
		FTransform(
			FRotator(GetActorRotation()),
			FVector(GetActorLocation()),
			FVector(1, 1, 1)))->ReinESkill(this, ad);
}
