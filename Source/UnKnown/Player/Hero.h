// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/BoxComponent.h"
#include "Engine.h"
#include "Object/ObjectMgr.h"
#include "Item/Item.h"
#include "Object/ActiveObjectMgr.h"

#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Engine/World.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hero.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateAttack, int, Count);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateMap);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateDie);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateDash);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOpenInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateMode, const bool, Mode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateMaxHealth, int, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateHealth, int, Health);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateMaxMana, int, MaxMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateMana, int, Mana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateMaxExp, int, MaxExp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateExp, int, Exp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_EightParams(FUpdateInvenInfo, int, Count, UObject *, Image, UObject *, Slot, FString, Name, FString, Explain, const bool, Consumables, int, Parts, int, ItemCode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_EightParams(FUpdateItemInfo, float, Weight, UObject *, Image, FString, Name, FString, Explain, AItem *, Item, const bool, Consumables, int, Parts, int, ItemCode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FUpdateToolTipInfo, FString, _Name, FString, _Explain, int, _Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateActiveItemInfo, FString, Name, float, Weight);

UCLASS()
class UNKNOWN_API AHero : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent * Weapon;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent * ArmourMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UBoxComponent * ActiveBox;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UBoxComponent * WeaponCollision;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnRednerCollOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnRednerCollOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	typedef struct _Stat
	{
		int Health;			  //체력
		int Vigor;			  //기력
		int Attack;			  //공격력
		int Amor;			  //아머
		int WalkSpeed;		  //이동 속도
		int ArmorPenetration; //방어구 관통력
		int Critical;		  //치명타
		int CoolDown;		  //재사용 대기시간 감소

		void Init()
		{
			Health = 0;
			Vigor = 0;
			Attack = 0;
			Amor = 0;
			WalkSpeed = 0;
			ArmorPenetration = 0;
			Critical = 0;
			CoolDown = 0;
		};
	}Stat;

public:
	// Sets default values for this character's properties
	AHero();

	void InvenUpdate(int _count, UObject * _Image, UObject * _Slot, FString _Name, FString _Explain, bool _Consumable, int _Parts, int _Code); //InventoryWidget과 통신
	void ItemUpdate(float Weight, UObject * _Image, FString _Name, FString _Explain, AItem * _Item, bool _Consumables, int _Parts, int _Code); //Item과 통신
	void SendToolTipInfo(FString _Name, FString _Explain, int _Slot);
	void HealHero(int _Heal);
	void SetActive1(FString _Name);
	void SetActive2(FString _Name);
	void InitActive(FString _Name);
	void ChangeItem(int _NewItemCode);
	void SetActiveObject(AActiveObjectMgr * _Object);
	void GetPlayerInfo();
	USceneComponent * GetActiveBox() const { return ActiveBox; }
	USceneComponent * GetWeaponBox() const { return WeaponCollision; }

	bool bUseActiveObject;
	bool GetAlive() { return Alive; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PutDamage(int damage);

	bool GetSwordColli() { return SwordCollision; }

protected:
	void FB_Move(float value);
	void LR_Move(float value);
	void Dash();

	void Attack();
	void WSkill();
	void ESkill();
	void RSkill();

	void ReinWSkill();
	void ReinESkill();
	void ReinRSkill();

	void SetCanMove();
	void SetSwordCollision();
	void UpdateMap();
	void UseActive1();
	void UseActive2();
	void UseActiveItem(FString _Name);
	void AttackTimerSet();

	//삭제 해야 할 녀석들 
	void GodMode();
	void SpeedUp();

protected:
	uint32 bMoveToMouseCursor : 1;

	USkeletalMeshComponent * SkeletalMesh;

	UCameraComponent * FollowCamera;
	USpringArmComponent * CameraBoom;

	UBoxComponent * RenderCollision;

	UPhysicsHandleComponent* PhysicsHandle;

	class UAnimMontage * NormalAttack1_Animation;
	class UAnimMontage * NormalAttack2_Animation;
	class UAnimMontage * NormalAttack3_Animation;
	class UAnimMontage * W_Skill_Animation;
	class UAnimMontage * E_Skill_Animation;
	class UAnimMontage * Rein_W_Skill_Animation;
	class UAnimMontage * Rein_E_Skill_Animation;
	class UAnimMontage * Rein_R_Skill_Animation;

	FTimerHandle MoveTimer;
	FTimerHandle CollisionTime;
	FTimerHandle AttackTimerSetTime;

	float FB_Value;
	float LR_Value;

	FVector FB_Direction;
	FVector LR_Direction;

	bool CanMove;
	bool SwordCollision;
	bool AttackRot;
	bool AttackMode;
	bool KeyBoard;

	bool Alive;

	AActiveObjectMgr * ActiveObject;

	UPROPERTY(BluePrintAssignable, Category = "MyCharacter")
		FUpdateAttack AttackUpdate;

	UPROPERTY(BluePrintAssignable, Category = "MyCharacter")
		FUpdateMap MapUpdate;

	UPROPERTY(BluePrintAssignable, Category = "MyCharacter")
		FUpdateMode ModeUpdate;

	UPROPERTY(BluePrintAssignable, Category = "MyCharacter")
		FUpdateMaxHealth MaxHealthUpdate;

	UPROPERTY(BluePrintAssignable, Category = "MyCharacter")
		FUpdateHealth HealthUpdate;

	UPROPERTY(BluePrintAssignable, Category = "MyCharacter")
		FUpdateMaxMana MaxManaUpdate;

	UPROPERTY(BluePrintAssignable, Category = "MyCharacter")
		FUpdateMana ManaUpdate;

	UPROPERTY(BluePrintAssignable, Category = "MyCharacter")
		FUpdateMaxExp MaxExpUpdate;

	UPROPERTY(BluePrintAssignable, Category = "MyCharacter")
		FUpdateExp ExpUpdate;

	UPROPERTY(BluePrintAssignable, Category = "MyCharacter")
		FOpenInventory OpenInven;

	UPROPERTY(BluePrintAssignable, Category = "UMG Function")
		FUpdateInvenInfo InvenInfoUpdate;

	UPROPERTY(BluePrintAssignable, Category = "UMG Function")
		FUpdateItemInfo ItemInfoUpdate;

	UPROPERTY(BluePrintAssignable, Category = "UMG Function")
		FUpdateToolTipInfo ToolTipInfoUpdate;

	UPROPERTY(BluePrintAssignable, Category = "UMG Function")
		FUpdateActiveItemInfo ActiveItemInfoUpdate;

	UPROPERTY(BluePrintAssignable, Category = "MayCharacter")
		FUpdateDie DieUpdate;

	UPROPERTY(BluePrintAssignable, Category = "MayCharacter")
		FUpdateDash DashUpdate;

protected: //플레이어 정보
	float DashTime;
	float AttackTime;
	float WSkillCoolTime;
	float ESkillCoolTime;

	int AttackCount;

	bool CanAttack;
	bool bReinSkill;
	bool bGodMode;
	bool bReinMode;

	FString Active1;
	FString Active2;

protected: //스킬 관련
	FTimerHandle WSkill_Damage_Timer;

	FTimerHandle ReinWSkill_Jump_Timer;
	FTimerHandle ReinWSkill_Land_Timer;

	FTimerHandle ReinESkill_Dash_Timer;
	FTimerHandle ReinESkill_End_Dash_Timer;

	FTimerHandle EndReinMode_Timer;

	FTimerHandle ReinRSkill_Circle_Timer;

	FTimerHandle ReinMode_Timer;

	UParticleSystem * Rein_R_Effect;
	UParticleSystem * Rein_R_Particle;
	UParticleSystem * W_Effect;
	UParticleSystemComponent * ParticleSystem;

	void WSkill_Damage();

	void ReinMode_Attack1();
	void ReinMode_Attack2();
	void ReinMode_Attack3();

	void ReinMode_WSkill();
	void ReinMode_ESkill();

	void ReinMode_ReinWSkill();
	void ReinMode_ReinESkill();
	
	void ReinWSkill_Jump();
	void ReinWSkill_Land();

	void ReinESkill_Dash();
	void ReinESkill_End_Dash();

	void EndReinMode();

	void ReinRSkill_Circle();

protected: //직업별 변경을 위한 정보
	float MoveSpeed = 250.0f;
	float AttackModeMoveSpeed = 100.0f;
	int AttackDamage = 25;

	int Health = 100;
	int MaxHealth = 100;
	int Mana = 100;
	int MaxMana = 100;

	Stat Stat_Head;
	Stat Stat_Accessory;
	Stat Stat_Body;
	Stat Stat_Shoose;
	Stat Stat_Weapon;
};
