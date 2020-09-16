// Fill out your copyright notice in the Description page of Project Settings.

#include "Afterimage.h"
#include "Monster/MonsterMgr.h"
#include "Object/SkillCircleCollision.h"

// Sets default values
AAfterimage::AAfterimage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = Capsule;
	Capsule->RelativeScale3D = FVector(0.25f, 0.25f, 0.25f);
	Capsule->SetCollisionProfileName("NoCollision");

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletonMesh"));
	SkeletalMesh->SetupAttachment(Capsule);
	SkeletalMesh->RelativeLocation = FVector(0.0f, 0.0f, -90);
	SkeletalMesh->RelativeScale3D = FVector(1, 1, 1);
	SkeletalMesh->RelativeRotation = FRotator(0, -90, 0);

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

	static ConstructorHelpers::FObjectFinder<UParticleSystem> W_FX(TEXT("ParticleSystem'/Game/Effect/Particle/Skill/W.W'"));
	W_Effect = W_FX.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial> Mat(TEXT("Material'/Game/Mat/M_Red_Character.M_Red_Character'"));
	Mater = Mat.Object;
	//UMaterialInstanceDynamic * Instance = UMaterialInstanceDynamic::Create(Mater, this);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skeleton(TEXT("SkeletalMesh'/Game/Animation/Character/default.default'"));
	static ConstructorHelpers::FObjectFinder<UClass> AnimClass(TEXT("/Game/Animation/Character/NewAnimBlueprint.NewAnimBlueprint_C"));
	SkeletalMesh->SetSkeletalMesh(Skeleton.Object);
	SkeletalMesh->SetMaterial(0, Mater);
	SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkeletalMesh->AnimClass = AnimClass.Object;
	SkeletalMesh->SetCollisionProfileName("NoCollision");

	Weapon = NewObject<UStaticMeshComponent>(this, TEXT("Weapon"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMesh(TEXT("StaticMesh'/Game/Modeling/Character/Warrior/Sword/Sword.Sword'"));
	Weapon->SetStaticMesh(WeaponMesh.Object);
	Weapon->AttachTo(SkeletalMesh, TEXT("Bone001"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Weapon->RelativeRotation = FRotator(-0.000052f, 0.000225f, 0.000056f);
	Weapon->RelativeScale3D = FVector(1.75f, 1.0f, 1.0f);
	Weapon->SetMaterial(0, Mater);
	Weapon->SetCollisionProfileName("NoCollision");

	WeaponCollision = NewObject<UBoxComponent>(this, TEXT("CollisionBox"));
	WeaponCollision->AttachTo(Weapon);
	WeaponCollision->bGenerateOverlapEvents = true;
	WeaponCollision->RelativeLocation = FVector(80.0f, 0, 0);
	WeaponCollision->RelativeScale3D = FVector(1.75, 0.25, 0.125);
	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AAfterimage::OnOverlapBegin);
}

void AAfterimage::Attack(AHero * _hero, int _AttackDamage, int _AttackCount)
{
	hero = _hero;
	AttackDamage = _AttackDamage;

	switch (_AttackCount)
	{
	case 0:
		SkeletalMesh->GetAnimInstance()->Montage_Play(NormalAttack1_Animation);
		GetWorldTimerManager().SetTimer(Destroy_Timer, this, &AAfterimage::TDestroy, 0.433f, false, 0.433f);
		break;

	case 1:
		SkeletalMesh->GetAnimInstance()->Montage_Play(NormalAttack2_Animation);
		GetWorldTimerManager().SetTimer(Destroy_Timer, this, &AAfterimage::TDestroy, 0.367f, false, 0.367f);
		break;

	case 2:
		SkeletalMesh->GetAnimInstance()->Montage_Play(NormalAttack3_Animation);
		GetWorldTimerManager().SetTimer(Destroy_Timer, this, &AAfterimage::TDestroy, 0.4f, false, 0.4f);
		break;

	default:
		break;
	}
}

void AAfterimage::WSkill(AHero * _hero, int _AttackDamage)
{
	hero = _hero;
	AttackDamage = _AttackDamage;

	SkeletalMesh->GetAnimInstance()->Montage_Play(W_Skill_Animation);

	GetWorldTimerManager().SetTimer(WSkill_Damage_Timer, this, &AAfterimage::WSkill_Damage, 0.2f, false, 0.2f);

	GetWorldTimerManager().SetTimer(Destroy_Timer, this, &AAfterimage::TDestroy, 0.5f, false, 0.5f);
}

void AAfterimage::ESkill(AHero * _hero, int _AttackDamage)
{
	hero = _hero;
	AttackDamage = _AttackDamage;

	SkeletalMesh->GetAnimInstance()->Montage_Play(E_Skill_Animation);

	WeaponCollision->SetRelativeScale3D(FVector(2.5, 2.25, 2.125));

	GetWorldTimerManager().SetTimer(Destroy_Timer, this, &AAfterimage::TDestroy, 0.667f, false, 0.667f);
}

void AAfterimage::ReinWSkill(AHero * _hero, int _AttackDamage)
{
	hero = _hero;
	AttackDamage = _AttackDamage;

	SkeletalMesh->GetAnimInstance()->Montage_Play(Rein_W_Skill_Animation);

	GetWorldTimerManager().SetTimer(ReinWSkill_Jump_Timer, this, &AAfterimage::ReinWSkill_Jump, 0.1f, false, 0.1f);
	GetWorldTimerManager().SetTimer(ReinWSkill_Land_Timer, this, &AAfterimage::ReinWSkill_Land, 0.4f, false, 0.4f);
	GetWorldTimerManager().SetTimer(Destroy_Timer, this, &AAfterimage::TDestroy, 0.833f, false, 0.833f);
}

void AAfterimage::ReinESkill(AHero * _hero, int _AttackDamage)
{
	hero = _hero;
	AttackDamage = _AttackDamage;

	SkeletalMesh->GetAnimInstance()->Montage_Play(Rein_E_Skill_Animation);

	GetWorldTimerManager().SetTimer(ReinESkill_Dash_Timer, this, &AAfterimage::ReinESkill_Dash, 0.01f, true, 0.3f);
	GetWorldTimerManager().SetTimer(ReinESkill_End_Dash_Timer, this, &AAfterimage::ReinESkill_End_Dash, 0.6f, false, 0.6f);
	GetWorldTimerManager().SetTimer(Destroy_Timer, this, &AAfterimage::TDestroy, 0.833f, false, 0.833f);
}

// Called when the game starts or when spawned
void AAfterimage::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAfterimage::WSkill_Damage()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), W_Effect, FVector(GetActorLocation().X, GetActorLocation().Y, 0.051f), GetActorRotation(), FVector(2,2,2));

	GetWorld()->SpawnActor<ASkillCircleCollision>(ASkillCircleCollision::StaticClass(),
		FTransform(FRotator(0, 0, 0),
			FVector(GetActorLocation().X, GetActorLocation().Y, 20),
			FVector(1, 1, 1)))->SetSetting(AttackDamage, hero, FVector(3, 3, 3), 0.075);
}

void AAfterimage::ReinWSkill_Jump()
{
	SkeletalMesh->SetRelativeLocation(FVector(0, 0, 300));
}

void AAfterimage::ReinWSkill_Land()
{
	SkeletalMesh->SetRelativeLocation(FVector(0, 0, -90));

	GetWorld()->SpawnActor<ASkillCircleCollision>(ASkillCircleCollision::StaticClass(),
		FTransform(FRotator(0, 0, 0),
			FVector(GetActorLocation().X, GetActorLocation().Y, 20),
			FVector(1, 1, 1)))->SetSetting(AttackDamage, hero, FVector(2, 2, 2), 0.075);
}

void AAfterimage::ReinESkill_Dash()
{
	WeaponCollision->SetRelativeScale3D(FVector(2, 2, 2));

	FHitResult * HitRe = new FHitResult();

	AddActorWorldOffset(GetActorForwardVector() * 10, true, HitRe);

	if (Cast<AMonsterMgr>(HitRe->Actor) || Cast<AItem>(HitRe->Actor))
	{
		HitRe->GetActor()->GetName();

		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, HitRe->GetActor()->GetName());

		AddActorWorldOffset(GetActorForwardVector() * 10);
	}
}

void AAfterimage::ReinESkill_End_Dash()
{
	WeaponCollision->SetRelativeScale3D(FVector(1.75, 0.25, 0.125));

	AttackDamage = 25;

	GetWorldTimerManager().ClearTimer(ReinESkill_Dash_Timer);
}

void AAfterimage::ReinRSkill_Circle()
{
	GetWorld()->SpawnActor<ASkillCircleCollision>(ASkillCircleCollision::StaticClass(),
		FTransform(FRotator(0, 0, 0),
			FVector(GetActorLocation().X, GetActorLocation().Y, 20),
			FVector(1, 1, 1)))->SetSetting(AttackDamage, hero, FVector(2, 2, 2), 0.075);
}

void AAfterimage::TDestroy()
{
	Destroy();
}

// Called every frame
void AAfterimage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAfterimage::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AMonsterMgr * mon = Cast<AMonsterMgr>(OtherActor);
	if (mon && hero != nullptr)
	{
		int ad = AttackDamage;
		mon->PutDamage(ad, hero);
	}
}