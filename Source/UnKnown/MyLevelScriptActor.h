// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GenerateMap/GenerateRoom/GenerateRoom.h"
#include "GenerateMap/GenerateMap.h"
#include "Mgr/DataMgr.h"
#include "DungeonActor/DungeonActor.h"

#include "Object/Torch.h"
#include "Object/ObjectMgr.h"
#include "Engine.h"

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MyLevelScriptActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateStageLevel, const int, Level);

UCLASS()
class UNKNOWN_API AMyLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

	typedef struct _Data
	{
		FString Name;
		FString Explain;
		bool Consumables;
		int Parts;
		float Weight;
		int ItemCode;
	}Data;

	typedef struct _GSData
	{
		bool bData;
		FVector StartLocation;
		FVector EndLocation;

		void Init()
		{
			bData = false;
			StartLocation = FVector(0, 0, 0);
			EndLocation = FVector(0, 0, 0);
		};

	}GSData;

protected:

	virtual void BeginPlay();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	virtual void Tick(float DeltaSeconds) override;

	void RoomSpawn(GRoomList * room);
	void AisleSpawn(GAisleList * aisle);
	void RoomWallSpawn(GRoomList * room);
	void AisleWallSpawn(GAisleList * aisle);

	void ArrayDungeonActor();
	void ActorSpawner();

	void ArrayDungeonMonster();
	void ArrayDungeonObject();

	static void RoomSizeArray(AMyLevelScriptActor * level);
	void Swap(GRoom * room1, GRoom * room2);

	void SpawnDecoration(int _num);
	void ArrayDecoration();

public:
	AMyLevelScriptActor();

	void NextFloor();

	void InitMonsterArray(int num);

	bool GetNextFloor() { return bNextFloor; }

	void SpawnItem(FVector SpawnLocation, int ItemCode);

	int RandomItemCode();

	void ShowGate();

	void Block_BossRoom();

protected:
	bool bNextFloor, bDon;
	bool boo, doo, wak, jjang, WWG, WakGood;
	int count, cc, adcount, ax, ay, abcount, DecorationCount;
	int roomcount;
	int CheckMon, CheckObj;
	int CountingMonster, CountingObject, CountingDungeonActor, CountingTrea, CountingDecoration;
	int monstercount, objectcount, treasurechestcount;
	int iCountGSD;
	int a[808][808] = { 0 }; //최대 단계가 늘어나면 이것도 늘린다.
	int Objects[808][808] = { 0 };
	GenerateRoom an;
	ADungeonActor * DungeonActorArray[65000]; //벽 배열
	AObjectMgr * ObjectArray[2][30000]; //횃불 배열
	AObjectMgr * TreasureChestArray[150]; //보물 상자 배열 
	AObjectMgr * DecorationArray[32][256]; //데코 배열
	AActor * BossSpawner;
	AActor * RoomGate[256][8];
	GSData * GateSpawnDataArray[256][8];
	GRoom * RoomArray[256];

	ASceneCapture2D * SceneCapture;
	UTextureRenderTarget2D * SceneTexture;

	UPROPERTY(BluePrintAssignable, Category = "MyCharacter")
		FUpdateStageLevel StageLevelUpdate;

	UObject * Image[256];
	UTexture * Tile_Image[10];
	UTexture * Tile_Normal_Image[10];
	UTexture * Wall_Image[10];

	Data ItemData[256] = //아이템 등록
	{
		//이름, 툴팁, 소모성, 파츠, 무게, 아이템 코드
		/*  
		파츠
		0 소모품
		1 무기
		2 머리
		3 갑옷
		4 악세사리
		5 신발
		*/
		{TEXT("작은 포션"), TEXT("마셔도 배가 부르지 않을 만큼의 포션이다"), true, 0, 5, 0},
		{TEXT("큰 포션"), TEXT("마시면 배가 터질거 만큼의 포션이다."), true, 0, 10, 1},
		{TEXT("낡은 검"), TEXT("낡은 검으로 고기도 못 썰 거 같다."), false, 1, 15, 2}
	};
};
