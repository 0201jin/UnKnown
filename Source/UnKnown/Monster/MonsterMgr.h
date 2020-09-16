// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Player/Hero.h"
#include "MyLevelScriptActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterMgr.generated.h"

UCLASS()
class UNKNOWN_API AMonsterMgr : public ACharacter
{
	GENERATED_BODY()

public:
	virtual void PutDamage(int damage, AHero * hero);
	virtual void PutDamage(int damage);
	virtual void SetMonNum(int number, AMyLevelScriptActor * cla, int Stage);
	virtual void SetPause();
	virtual void SetUnPause();
	virtual void SetRoomMaxMin(FVector _Location, FVector _Size);
	virtual void SetRoomInfo(GRoom * _RoomArray[256], int _RoomCount);
	virtual void PauseBoss();
};
