// Fill out your copyright notice in the Description page of Project Settings.

#include "MyLevelScriptActor.h"
#include "thread"
#include "DungeonActor/NextGate.h"
#include "Object/TreasureChest.h"
#include "Object/RomaFlag.h"
#include "Item/Item.h"

//#include <Windows.h>

#include "Monster/Monster.h"
#include "Monster/ReconMonster.h"
#include "Monster/Stage1/ShieldMonster.h"
#include "Monster/Stage1/MiraMonster.h"
#include "Monster/Stage1/Tariarri.h"
#include "Monster/Stage1/Ghost.h"
#include "Monster/Stage1/MushRoom.h"

#include "DungeonActor/Boss1Spawner.h"

#include "Object/Moss.h"

#define OBJECT_COUNT 2

using namespace GenerateMap;

bool good = false;

AMonsterMgr * MonsterArray[10280];

UClass * MonsterDB[10];
UClass * ReconMonsterDB[10];
UClass * Spawner[10];
UClass * EpicMonster[10];
UClass * DecorationDB[32];

UStaticMesh * DecoMeshArray[10][10];

ADungeonActor * Floor = nullptr;
ANextGate * Gate = nullptr;

int MonsterNum = 4;
int ReconMonsterNum = 2;

AMyLevelScriptActor::AMyLevelScriptActor()
{
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> Texture(TEXT("TextureRenderTarget2D'/Game/Map/MiniMapRenderTarget.MiniMapRenderTarget'"));
	SceneTexture = Texture.Object;

	//이끼
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Moss1(TEXT("StaticMesh'/Game/Modeling/Moss/Moss_Plane06.Moss_Plane06'"));
	DecoMeshArray[0][0] = Moss1.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Moss2(TEXT("StaticMesh'/Game/Modeling/Moss/Moss_Plane091.Moss_Plane091'"));
	DecoMeshArray[0][1] = Moss2.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Moss3(TEXT("StaticMesh'/Game/Modeling/Moss/Moss_Plane092.Moss_Plane092'"));
	DecoMeshArray[0][2] = Moss3.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Moss4(TEXT("StaticMesh'/Game/Modeling/Moss/Moss_Plane093.Moss_Plane093'"));
	DecoMeshArray[0][3] = Moss4.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Moss5(TEXT("StaticMesh'/Game/Modeling/Moss/Moss_Plane094.Moss_Plane094'"));
	DecoMeshArray[0][4] = Moss5.Object;

	//기둥
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Pillar1(TEXT("StaticMesh'/Game/Modeling/Pillar/Pillar_Cylinder003.Pillar_Cylinder003'"));
	DecoMeshArray[1][0] = Pillar1.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Pillar2(TEXT("StaticMesh'/Game/Modeling/Pillar/Pillar_Cylinder004.Pillar_Cylinder004'"));
	DecoMeshArray[1][1] = Pillar2.Object;

	/*static ConstructorHelpers::FObjectFinder<UStaticMesh> Pillar3(TEXT("StaticMesh'/Game/Modeling/Pillar/Pillar_Cylinder001.Pillar_Cylinder001'"));
	DecoMeshArray[1][2] = Pillar3.Object;*/

	//포션
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Potion1(TEXT("StaticMesh'/Game/Modeling/Potion/Potion_Box001.Potion_Box001'"));
	DecoMeshArray[2][0] = Potion1.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Potion2(TEXT("StaticMesh'/Game/Modeling/Potion/Potion_Box002.Potion_Box002'"));
	DecoMeshArray[2][1] = Potion2.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Potion3(TEXT("StaticMesh'/Game/Modeling/Potion/Potion_Box003.Potion_Box003'"));
	DecoMeshArray[2][2] = Potion3.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Potion4(TEXT("StaticMesh'/Game/Modeling/Potion/Potion_Box006.Potion_Box006'"));
	DecoMeshArray[2][3] = Potion4.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Potion5(TEXT("StaticMesh'/Game/Modeling/Potion/Potion_Box007.Potion_Box007'"));
	DecoMeshArray[2][4] = Potion5.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Potion6(TEXT("StaticMesh'/Game/Modeling/Potion/Potion_Box008.Potion_Box008'"));
	DecoMeshArray[2][5] = Potion6.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Potion7(TEXT("StaticMesh'/Game/Modeling/Potion/Potion_Box011.Potion_Box011'"));
	DecoMeshArray[2][6] = Potion7.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Potion8(TEXT("StaticMesh'/Game/Modeling/Potion/Potion_Box014.Potion_Box014'"));
	DecoMeshArray[2][7] = Potion8.Object;

	//양초
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Candle1(TEXT("StaticMesh'/Game/Modeling/Candle/Candle_Cylinder001.Candle_Cylinder001'"));
	DecoMeshArray[3][0] = Candle1.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Candle2(TEXT("StaticMesh'/Game/Modeling/Candle/Candle_Cylinder002.Candle_Cylinder002'"));
	DecoMeshArray[3][1] = Candle2.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Candle3(TEXT("StaticMesh'/Game/Modeling/Candle/Candle_Cylinder004.Candle_Cylinder004'"));
	DecoMeshArray[3][2] = Candle3.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Candle4(TEXT("StaticMesh'/Game/Modeling/Candle/Candle_Cylinder007.Candle_Cylinder007'"));
	DecoMeshArray[3][3] = Candle4.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Candle5(TEXT("StaticMesh'/Game/Modeling/Candle/Candle_Object001.Candle_Object001'"));
	DecoMeshArray[3][4] = Candle5.Object;

	//아이템
	static ConstructorHelpers::FObjectFinder<UTexture2D> Small_Postion(TEXT("Texture2D'/Game/Item/Test/Small_Potion.Small_Potion'"));
	Image[0] = Small_Postion.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> Large_Postion(TEXT("Texture2D'/Game/Item/Test/Big_Potion.Big_Potion'"));
	Image[1] = Large_Postion.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> Old_Sword(TEXT("Texture2D'/Game/Item/Test/Weapon_Sword.Weapon_Sword'"));
	Image[2] = Old_Sword.Object;

	//바닥 텍스
	static ConstructorHelpers::FObjectFinder<UTexture2D> S1_Tex(TEXT("Texture2D'/Game/Texture/Tile/1S_Tile.1S_Tile'"));
	Tile_Image[0] = S1_Tex.Object;
	Tile_Image[1] = S1_Tex.Object;
	Tile_Image[2] = S1_Tex.Object;

	//바닥 노말
	static ConstructorHelpers::FObjectFinder<UTexture2D> S1_Normal_Tex(TEXT("Texture2D'/Game/Texture/Tile/1S_Normal.1S_Normal'"));
	Tile_Normal_Image[0] = S1_Normal_Tex.Object;
	Tile_Normal_Image[1] = S1_Normal_Tex.Object;
	Tile_Normal_Image[2] = S1_Normal_Tex.Object;

	//벽
	static ConstructorHelpers::FObjectFinder<UTexture2D> S1_WallTex(TEXT("Texture2D'/Game/Texture/tex4.tex4'"));
	Wall_Image[0] = S1_WallTex.Object;
	Wall_Image[1] = S1_WallTex.Object;
	Wall_Image[2] = S1_WallTex.Object;

	MonsterDB[0] = AMonster::StaticClass();
	MonsterDB[1] = AShieldMonster::StaticClass();
	MonsterDB[2] = AMiraMonster::StaticClass();
	MonsterDB[3] = AMushRoom::StaticClass();

	ReconMonsterDB[0] = AReconMonster::StaticClass();
	ReconMonsterDB[1] = AGhost::StaticClass();

	Spawner[0] = nullptr;
	Spawner[1] = ABoss1Spawner::StaticClass();

	EpicMonster[0] = nullptr;
	EpicMonster[1] = ATariarri::StaticClass();

	//데코 레이션
	DecorationDB[0] = AMoss::StaticClass();
	DecorationDB[1] = AMoss::StaticClass();//기둥 클래스 만들기
	DecorationDB[2] = AMoss::StaticClass();
	DecorationDB[4] = AMoss::StaticClass();
}

void AMyLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	std::thread t1(an.BSP_Dungeon_Generate);

	t1.detach();

	count = 0;

	CheckMon = 1;

	ax = 0;
	ay = 0;

	SceneCapture = GetWorld()->SpawnActor<ASceneCapture2D>(ASceneCapture2D::StaticClass(),
		FTransform(FRotator(-90, 180, -180), FVector(0, 0, 0),
			FVector(1, 1, 1)));

	SceneCapture->GetCaptureComponent2D()->TextureTarget = SceneTexture;
	SceneCapture->GetCaptureComponent2D()->ProjectionType = ECameraProjectionMode::Orthographic;
	SceneCapture->GetCaptureComponent2D()->OrthoWidth = 50;
	SceneCapture->GetCaptureComponent2D()->bCaptureEveryFrame = false;
	SceneCapture->GetCaptureComponent2D()->bCaptureOnMovement = false;
	SceneCapture->GetCaptureComponent2D()->CaptureSource = ESceneCaptureSource::SCS_BaseColor;

	Floor = GetWorld()->SpawnActor<ADungeonActor>(ADungeonActor::StaticClass(),
		FTransform(FRotator(0, 0, 0), FVector(0, 0, 0),
			FVector(0.1, 0.1, 0.1)));

	Gate = GetWorld()->SpawnActor<ANextGate>(ANextGate::StaticClass(),
		FTransform(FRotator(0, 0, 0), FVector(0, 0, 10),
			FVector(0.1, 0.1, 0.1)));

	good = false;
	bDon = true;
	WakGood = true;

	/*DataMgr::GetInst()->NextMapList();
	DataMgr::GetInst()->NextMapList();
	DataMgr::GetInst()->NextMapList();
	DataMgr::GetInst()->NextMapList();
	DataMgr::GetInst()->NextMapList();
	DataMgr::GetInst()->NextMapList();
	DataMgr::GetInst()->NextMapList();
	DataMgr::GetInst()->NextMapList();
	DataMgr::GetInst()->NextMapList();*/
}

void AMyLevelScriptActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

void AMyLevelScriptActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!doo)
	{
		ActorSpawner();
		cc++;
	}

	if (!wak)
	{
		wak = an.GetBool();
	}

	if (WakGood) //데코레이션 스폰
	{
		SpawnDecoration(DecorationCount);
		DecorationCount++;
	}

	if (wak && !boo && doo)
	{
		int size = DataMgr::GetInst()->GetStageSize();
		int Stage = DataMgr::GetInst()->GetStage() - 1;

		Floor->SetActorRelativeScale3D(FVector(
			size,
			size,
			0.001));

		Floor->SetActorRelativeLocation(FVector(
			size * 25.0f,
			size * 25.0f,
			-0.5));

		Floor->SetMatInst(DataMgr::GetInst()->GetStageSize() * 2, Tile_Image[Stage], Tile_Normal_Image[Stage]);

		DataMgr::GetInst()->MapListFirstNode();

		RoomSpawn(DataMgr::GetInst()->GetMapList()->RoomList);

		AisleSpawn(DataMgr::GetInst()->GetMapList()->AisleList);

		DataMgr::GetInst()->MapListFirstNode();

		RoomWallSpawn(DataMgr::GetInst()->GetMapList()->RoomList);

		AisleWallSpawn(DataMgr::GetInst()->GetMapList()->AisleList);

		////디버그용 반드시 지울 것//
		//AllocConsole();

		//freopen("CONOUT$", "wt", stdout);

		//for (int o = 0; o < 200; o++)
		//{
		//	for (int q = 0; q < 200; q++)
		//	{
		//		printf("%d", a[o][q]);
		//	}
		//	printf("\n");
		//}
		////꼭 지울 것//

		std::thread t1(RoomSizeArray, this);

		t1.detach();

		boo = true;
	}

	else if (!good && boo && jjang && !WakGood)
	{
		//던전 벽 배치 및 오브젝트 배치
		ArrayDungeonActor();
	}

	if (!bDon) //정리
	{
		if (CountingMonster < monstercount)
		{
			for (int i = CountingMonster; i < (CountingMonster + 30); i++)
			{
				if (MonsterArray[i] != nullptr)
				{
					MonsterArray[i]->Destroy();
				}
			}
		}

		if (CountingObject < abcount)
		{
			for (int i = CountingObject; i < (CountingObject + 30); i++)
			{
				for (int b = 0; b < OBJECT_COUNT; b++)
				{
					if (ObjectArray[b][i] != nullptr)
					{
						ObjectArray[b][i]->SetActorHiddenInGame(true);
						ObjectArray[b][i]->SetActorEnableCollision(false);
						ObjectArray[b][i]->SetActorRelativeRotation(FRotator(0, 0, 0));
					}
				}
			}
		}

		if (CountingDungeonActor < adcount)
		{
			for (int i = CountingDungeonActor; i < (CountingDungeonActor + 30); i++)
			{
				if (DungeonActorArray[i] != nullptr)
				{
					DungeonActorArray[i]->SetActorHiddenInGame(true);
					DungeonActorArray[i]->SetActorEnableCollision(false);
				}
			}
		}

		if (CountingTrea < treasurechestcount)
		{
			for (int i = CountingTrea; i < (CountingTrea + 30); i++)
			{
				if (TreasureChestArray[i] != nullptr)
				{
					TreasureChestArray[i]->SetActorHiddenInGame(true);
					TreasureChestArray[i]->SetActorEnableCollision(false);
				}
			}
		}

		if (CountingDecoration < 32)
		{
			for (int i = 0; i < 256; i++)
			{
				if (DecorationArray[CountingDecoration][i] != nullptr)
				{
					DecorationArray[CountingDecoration][i]->GoHome();
				}
			}
		}

		CountingDecoration++;
		CountingMonster += 30;
		CountingObject += 30;
		CountingDungeonActor += 30;
		CountingTrea += 30;

		if (CountingObject >= abcount && CountingMonster >= monstercount && CountingDungeonActor >= adcount)
		{
			//몬스터와 오브젝트 정리가 끝나면 실행
			for (int i = 0; i < 150; i++)
			{
				TreasureChestArray[i]->SetActorHiddenInGame(true);
			}

			int iter = 0;
			for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
			{
				UGameplayStatics::GetPlayerCharacter(GetWorld(), iter)->SetActorLocation(FVector(-100, -100, -100));

				iter++;
			}

			DataMgr::GetInst()->NextMapList();
			boo = false;
			good = false;
			jjang = false;
			bDon = true;
			adcount = 0;
			ax = 0;
			ay = 0;

			roomcount = 0;
			monstercount = 0;
			objectcount = 0;
			CheckMon = 1;
			CheckObj = 0;
			abcount = 0;
			treasurechestcount = 0;

			CountingDecoration = 0;
			CountingMonster = 0;
			CountingObject = 0;
			CountingDungeonActor = 0;

			memset(a, 0, sizeof(a));
			memset(Objects, 0, sizeof(Objects));
			memset(MonsterArray, 0, sizeof(MonsterArray));

			UE_LOG(LogTemp, Warning, TEXT("NextFloor"));
		}
	}
}

void AMyLevelScriptActor::ActorSpawner() //오브젝트 생성
{
	for (int x = 0; x < 30; x++)
	{
		for (int y = 0; y < 30; y++)
		{
			if (count < 40000)
			{
				DungeonActorArray[count] = GetWorld()->SpawnActor<ADungeonActor>(ADungeonActor::StaticClass(),
					FTransform(FRotator(0, 0, 0),
						FVector(50 * x, 50 * y, -50 * cc - 50),
						FVector(1, 1, 1)));

				DungeonActorArray[count]->SetActorHiddenInGame(true);
				DungeonActorArray[count]->SetActorEnableCollision(false);
				count++;
			}
		}
	}
	for (int x = 0; x < 12; x++)
	{
		for (int y = 0; y < 6; y++)
		{
			for (int b = 0; b < OBJECT_COUNT; b++)
			{
				switch (b)
				{
				case 0:
					ObjectArray[b][objectcount] = GetWorld()->SpawnActor<ATorch>(ATorch::StaticClass(),
						FTransform(FRotator(0, 0, 0),
							FVector(
								x * 50 + 1500,
								y * 50 + 1500,
								cc * -50 - 50),
							FVector(1, 1, 1)));

					ObjectArray[b][objectcount]->SetActorHiddenInGame(true);
					ObjectArray[b][objectcount]->SetActorEnableCollision(false);
					break;

				case 1:
					ObjectArray[b][objectcount] = GetWorld()->SpawnActor<ARomaFlag>(ARomaFlag::StaticClass(),
						FTransform(FRotator(0, 0, 0),
							FVector(
								x * 50 + 1500,
								y * 50 + 1500,
								cc * -50 - 50),
							FVector(1, 1, 1)));

					ObjectArray[b][objectcount]->SetActorHiddenInGame(true);
					ObjectArray[b][objectcount]->SetActorEnableCollision(false);
					break;

				default:
					break;
				}		
			}
			objectcount++;
		}
	}

	for (int x = 0; x < 5; x++)
	{
		TreasureChestArray[treasurechestcount] = GetWorld()->SpawnActor<ATreasureChest>(ATreasureChest::StaticClass(),
			FTransform(FRotator(0, 0, 0),
				FVector(
					x * 50,
					treasurechestcount * 50,
					-150),
				FVector(1, 1, 1)));

		TreasureChestArray[treasurechestcount]->SetActorHiddenInGame(true);
		TreasureChestArray[treasurechestcount]->SetActorEnableCollision(false);
		treasurechestcount++;
	}

	if (count >= 40000)
	{
		doo = true;
		treasurechestcount = 0;
	}
}

void AMyLevelScriptActor::ArrayDungeonMonster() //몬스터 배열 | 스폰
{
	int stage = 0;

	int Rstage = DataMgr::GetInst()->GetStage() - 1;

	int MonsterNumber = MonsterNum;

	if (CheckMon <= (roomcount - 1))
	{
		stage = (rand() % (/*DataMgr::GetInst()->GetStage() + 4*/15)); //몬스터 수

		for (int a = 0; a < stage; a++)
		{
			int x = (rand() % (RoomArray[CheckMon]->PSData.sx - 1) * 50);
			int y = (rand() % (RoomArray[CheckMon]->PSData.sy - 1) * 50);

			int r = (rand() % MonsterNumber);

			AMonsterMgr * monster = GetWorld()->SpawnActor<AMonsterMgr>(MonsterDB[r],
				FTransform(FRotator(0, 0, 0),
					FVector(
						x + ((RoomArray[CheckMon]->PSData.x + 1) * 50),
						y + ((RoomArray[CheckMon]->PSData.y + 1) * 50),
						50),
					FVector(1, 1, 1)));

			monster->SpawnDefaultController();

			monster->SetMonNum(monstercount, this, Rstage);
			monster->SetActorHiddenInGame(true);

			monster->SetRoomMaxMin(
				FVector(
				(RoomArray[CheckMon]->PSData.x + 1) * 50,
					(RoomArray[CheckMon]->PSData.y + 1) * 50,
					0),
				FVector(
				(RoomArray[CheckMon]->PSData.sx - 2) * 50,
					(RoomArray[CheckMon]->PSData.sy - 2) * 50,
					0));

			MonsterArray[monstercount] = monster;
			monstercount++;
		}
	}

	stage = (rand() % (DataMgr::GetInst()->GetStage() * 4)) + 1;

	if (CheckMon == 1)
	{
		//ReconMonster
		for (int a = 0; a < stage; a++)
		{
			int i = (rand() % (roomcount - 1)) + 1;
			int x = (rand() % (RoomArray[i]->PSData.sx - 1) * 50);
			int y = (rand() % (RoomArray[i]->PSData.sy - 1) * 50);

			int r = (rand() % ReconMonsterNum);
			if (ReconMonsterDB[r] != nullptr)
			{
				AMonsterMgr * ReconMonster = GetWorld()->SpawnActor<AMonsterMgr>(ReconMonsterDB[r],
					FTransform(FRotator(0, 0, 0),
						FVector(
							x + ((RoomArray[i]->PSData.x + 1) * 50),
							y + ((RoomArray[i]->PSData.y + 1) * 50),
							50),
						FVector(1, 1, 1)));

				ReconMonster->SetRoomInfo(RoomArray, roomcount);

				ReconMonster->SpawnDefaultController();

				//ReconMonster->SetActorHiddenInGame(true);

				ReconMonster->SetMonNum(monstercount, this, Rstage);

				MonsterArray[monstercount] = ReconMonster;
				monstercount++;
			}
		}
	}

	CheckMon++;
}

void AMyLevelScriptActor::ArrayDungeonObject() //오브젝트 배열
{
	if (CheckObj < roomcount)
	{
		int countx = RoomArray[CheckObj]->PSData.sx / 6;
		int county = RoomArray[CheckObj]->PSData.sy / 6;

		for (int c = 0; c < county; c++)
		{
			int RandomB = rand() % 5;

			if (RandomB < 3)
			{
				RandomB = 0;
			}
			else
			{
				RandomB = 1;
			}

			int y = (rand() % (RoomArray[CheckObj]->PSData.sy - 2)) + (RoomArray[CheckObj]->PSData.y + 1);

			int x1 = RoomArray[CheckObj]->PSData.x + 1; //기준
			int x2 = RoomArray[CheckObj]->PSData.x;

			int rx1 = RoomArray[CheckObj]->PSData.x + RoomArray[CheckObj]->PSData.sx - 1; //기준
			int rx2 = RoomArray[CheckObj]->PSData.x + RoomArray[CheckObj]->PSData.sx;

			if (a[x2][y] > 0 && Objects[x1][y] == 0)
			{
				switch (RandomB)
				{
				case 0:
					ObjectArray[RandomB][abcount]->SetActorRelativeLocation(
						FVector(
							x1 * 50 - 20,
							y * 50,
							20));
					break;

				case 1:
					ObjectArray[RandomB][abcount]->SetActorRelativeLocation(
						FVector(
							x1 * 50 - 20,
							y * 50,
							9));
					break;
				}

				//ObjectArray[abcount]->SetActorHiddenInGame(false);
				ObjectArray[RandomB][abcount]->SetActorEnableCollision(true);
				ObjectArray[RandomB][abcount]->ResetActor();

				Objects[x1][y] = 1;

				abcount++;
			}

			y = (rand() % (RoomArray[CheckObj]->PSData.sy - 2)) + (RoomArray[CheckObj]->PSData.y + 1);

			if (a[rx2][y] > 0 && Objects[rx1][y] == 0)
			{
				ObjectArray[RandomB][abcount]->SetActorRelativeRotation(FRotator(0, 180, 0));

				switch (RandomB)
				{
				case 0:
					ObjectArray[RandomB][abcount]->SetActorRelativeLocation(
						FVector(
							x1 * 50 - 20,
							y * 50,
							20));
					break;

				case 1:
					ObjectArray[RandomB][abcount]->SetActorRelativeLocation(
						FVector(
							x1 * 50 - 20,
							y * 50,
							9));
					break;
				}

				//ObjectArray[abcount]->SetActorHiddenInGame(false);
				ObjectArray[RandomB][abcount]->SetActorEnableCollision(true);
				ObjectArray[RandomB][abcount]->ResetActor();

				Objects[rx1][y] = 1;

				abcount++;
			}
		}

		for (int c = 0; c < countx; c++)
		{
			int RandomB = rand() % 5;

			if (RandomB < 3)
			{
				RandomB = 0;
			}
			else
			{
				RandomB = 1;
			}

			int x = (rand() % (RoomArray[CheckObj]->PSData.sx - 2)) + (RoomArray[CheckObj]->PSData.x + 1);

			int y1 = RoomArray[CheckObj]->PSData.y + 1; //기준
			int y2 = RoomArray[CheckObj]->PSData.y;

			int uy1 = RoomArray[CheckObj]->PSData.y + RoomArray[CheckObj]->PSData.sy - 1; //기준
			int uy2 = RoomArray[CheckObj]->PSData.y + RoomArray[CheckObj]->PSData.sy;

			//UE_LOG(LogTemp, Warning, TEXT("%d %d"), x * 50, y2 * 50);

			if (a[x][y2] > 0 && Objects[x][y1] == 0) //주변에 문이 통로가 있는지 확인을 하고 설치
			{
				ObjectArray[RandomB][abcount]->SetActorRelativeRotation(FRotator(0, 90, 0));				

				switch (RandomB)
				{
				case 0:
					ObjectArray[RandomB][abcount]->SetActorRelativeLocation(
						FVector(
							x * 50,
							y1 * 50 - 20,
							20));
					break;

				case 1:
					ObjectArray[RandomB][abcount]->SetActorRelativeLocation(
						FVector(
							x * 50,
							y1 * 50 - 20,
							9));
					break;
				}

				//ObjectArray[abcount]->SetActorHiddenInGame(false);
				ObjectArray[RandomB][abcount]->SetActorEnableCollision(true);
				ObjectArray[RandomB][abcount]->ResetActor();

				Objects[x][y1] = 1;

				abcount++;
			}

			x = (rand() % (RoomArray[CheckObj]->PSData.sx - 2)) + (RoomArray[CheckObj]->PSData.x + 1);

			if (a[x][uy2] > 0 && Objects[x][uy1] == 0)
			{
				ObjectArray[RandomB][abcount]->SetActorRelativeRotation(FRotator(0, -90, 0));
				
				switch (RandomB)
				{
				case 0:
					ObjectArray[RandomB][abcount]->SetActorRelativeLocation(
						FVector(
							x * 50,
							uy1 * 50 + 20,
							20));
					break;

				case 1:
					ObjectArray[RandomB][abcount]->SetActorRelativeLocation(
						FVector(
							x * 50,
							uy1 * 50 + 20,
							9));
					break;
				}

				//ObjectArray[abcount]->SetActorHiddenInGame(false);
				ObjectArray[RandomB][abcount]->SetActorEnableCollision(true);
				ObjectArray[RandomB][abcount]->ResetActor();

				Objects[x][uy1] = 1;

				abcount++;
			}
		}
	}

	if (CheckObj == 1) //처음 반복에만 배치를 한다.
	{
		for (int i = 0; i < DataMgr::GetInst()->GetStage(); i++)
		{
			int room = (rand() % (roomcount - 2)) + 1; //룸을 랜덤으로 정해서 스테이지 만큼 배정
			int x = (rand() % (RoomArray[room]->PSData.sx - 4)) + (RoomArray[room]->PSData.x + 2);
			int y = (rand() % (RoomArray[room]->PSData.sy - 4)) + (RoomArray[room]->PSData.y + 2);

			if (Objects[x][y] == 0)
			{
				TreasureChestArray[treasurechestcount]->SetActorRelativeLocation(FVector(
					x * 50,
					y * 50,
					0));

				TreasureChestArray[treasurechestcount]->SetActorEnableCollision(true);
				//TreasureChestArray[treasurechestcount]->SetActorHiddenInGame(false);

				Objects[x][y] = 2;
				treasurechestcount++;
			}
			else
			{
				i--;
			}
		}
	}
}

void AMyLevelScriptActor::RoomSizeArray(AMyLevelScriptActor * level) //방 크기 정리
{
	for (int i = 0; i < level->roomcount; i++)
	{
		for (int k = i; k < level->roomcount; k++)
		{
			if ((level->RoomArray[i]->PSData.sx * level->RoomArray[i]->PSData.sy) >
				(level->RoomArray[k]->PSData.sx * level->RoomArray[k]->PSData.sy))
			{
				level->Swap(level->RoomArray[i], level->RoomArray[k]);
			}
		}
	}

	level->jjang = true;
	level->WWG = true;
}

void AMyLevelScriptActor::Swap(GRoom * room1, GRoom * room2)
{
	GRoom temp = *room2;
	*room2 = *room1;
	*room1 = temp;
}

void AMyLevelScriptActor::SpawnDecoration(int _num) //데코레이션 스폰
{
	if (_num == 32 || DecorationDB[_num] == nullptr)
	{
		WakGood = false;
		return;
	}

	for (int i = 0; i < 256; i++)
	{
		int random = rand() % 8;

		switch (_num)
		{
		case 0:
			random = rand() % 5;
			break;

		case 1:
			random = rand() % 2;
			break;

		case 2:
			random = rand() % 8;
			break;

		case 3:
			random = rand() % 5;
			break;

		default:
			break;
		}

		if (DecoMeshArray[_num][random] != nullptr)
		{
			DecorationArray[_num][i] = GetWorld()->SpawnActor<AObjectMgr>(DecorationDB[_num],
				FTransform(FRotator(0, 0, 0), FVector((i * 50) + 200, (_num * 50) + 200, -200),
					FVector(1, 1, 1)));

			DecorationArray[_num][i]->SetNum(DecoMeshArray[_num][random]);
			DecorationArray[_num][i]->SetHomeVector(FVector((i * 50) + 200, (_num * 50) + 200, -200));

			switch (_num)
			{
			case 1:
				DecorationArray[_num][i]->SetActorScale3D(FVector(2.5, 2.5, 2.5));
				break;

			default:
				break;
			}
		}

		else
		{
			DecorationArray[_num][i] = GetWorld()->SpawnActor<AObjectMgr>(DecorationDB[_num],
				FTransform(FRotator(0, 0, 0), FVector((i * 50) + 200, (_num * 50) + 200, -200),
					FVector(1, 1, 1)));

			DecorationArray[_num][i]->SetNum(DecoMeshArray[_num][0]);
			DecorationArray[_num][i]->SetHomeVector(FVector((i * 50) + 200, (_num * 50) + 200, -200));

			switch (_num)
			{
			case 1:
				DecorationArray[_num][i]->SetActorScale3D(FVector(2.5, 2.5, 2.5));
				break;

			default:
				break;
			}
		}
	}
}

void AMyLevelScriptActor::ArrayDecoration() //데코레이션 배열
{
	if (CheckObj < roomcount)
	{
		int random = (rand() % 32) + 1;
		for (int i = 0; i < random; i++)
		{
			int randsx = rand() % RoomArray[CheckObj]->PSData.sx - 1;
			int randsy = rand() % RoomArray[CheckObj]->PSData.sy - 1;
			int x = RoomArray[CheckObj]->PSData.x + 1;
			int y = RoomArray[CheckObj]->PSData.y + 1;

			int F = (rand() % 10);
			int S = (rand() % 256);

			int x1 = randsx + x;
			int y1 = randsy + y;

			float x2 = x1 * 50;
			float y2 = y1 * 50;

			if (DecorationArray[F][S] != nullptr && Objects[x1][y1] == 0)
			{
				DecorationArray[F][S]->SetActorRelativeLocation(FVector(x2, y2, 0));
				DecorationArray[F][S]->SetActorHiddenInGame(false);
				Objects[x1][y1] = 3;
			}
		}
	}
}

void AMyLevelScriptActor::NextFloor()
{
	bNextFloor = false;
	bDon = false;

	Gate->SetActorLocation(FVector(0, 0, -150));
}

void AMyLevelScriptActor::InitMonsterArray(int num) //몬스터 배열 초기화
{
	MonsterArray[num] = nullptr;
}

void AMyLevelScriptActor::SpawnItem(FVector SpawnLocation, int ItemCode) //아이템 스폰 
{
	if (Image[ItemCode] != nullptr)
	{
		AItem * Item = GetWorld()->SpawnActor<AItem>(AItem::StaticClass(),
			FTransform(FRotator(0, 0, 0),
				SpawnLocation,
				FVector(1, 1, 1)));

		Item->SetItemSetting(Image[ItemCode], ItemData[ItemCode].Weight, this, ItemData[ItemCode].Name,
			ItemData[ItemCode].Explain, ItemData[ItemCode].Consumables, ItemData[ItemCode].Parts,
			ItemData[ItemCode].ItemCode);
	}
}

int AMyLevelScriptActor::RandomItemCode()
{
	int r = (rand() % 5);

	return r;
}

void AMyLevelScriptActor::ShowGate()
{
	//게이트 위치 변경
	int lx = (RoomArray[roomcount - 1]->PSData.sx / 2 + RoomArray[roomcount - 1]->PSData.x) * 50;
	int ly = (RoomArray[roomcount - 1]->PSData.sy / 2 + RoomArray[roomcount - 1]->PSData.y) * 50;

	bNextFloor = true;
	Gate->SetActorLocation(FVector(lx, ly, 0)); //게이트 위치
	Gate->SetActorHiddenInGame(false);
}

void AMyLevelScriptActor::Block_BossRoom()
{
	int Stage = DataMgr::GetInst()->GetStage() - 1;

	int lx = RoomArray[roomcount - 1]->PSData.x;
	int ly = RoomArray[roomcount - 1]->PSData.y;
	int rx = RoomArray[roomcount - 1]->PSData.x + RoomArray[roomcount - 1]->PSData.sx;
	int ry = RoomArray[roomcount - 1]->PSData.y + RoomArray[roomcount - 1]->PSData.sy;

	for (int i = lx; i <= rx; i++)
	{
		for (int j = ly; j <= ry; j++)
		{
			if (i == lx || j == ly ||
				i == rx ||
				j == ry)
			{
				if (a[i][j] == 0)
				{
					DungeonActorArray[adcount]->SetActorRelativeLocation(FVector(50 * i, 50 * j, 25));
					DungeonActorArray[adcount]->SetActorHiddenInGame(false);
					DungeonActorArray[adcount]->SetActorEnableCollision(true);
					DungeonActorArray[adcount]->SetWallMaetInst(Wall_Image[Stage]);
					adcount++;

					UE_LOG(LogTemp, Warning, TEXT("Array Block BossRoom"));
				}
			}
		}
	}
}

void AMyLevelScriptActor::ArrayDungeonActor()
{
	int size = DataMgr::GetInst()->GetStageSize() + 8;
	int Stage = DataMgr::GetInst()->GetStage() - 1;

	int c = ax + 30;
	int b = ay + 30;

	//던전 액터 배치
	for (int x = ax; x < c; x++)
	{
		for (int y = ay; y < b; y++)
		{
			if (x < size && y < size)
			{
				if (a[x][y] > 0)
				{
					DungeonActorArray[adcount]->SetActorRelativeLocation(FVector(50 * x, 50 * y, 25));
					DungeonActorArray[adcount]->SetActorHiddenInGame(false);
					DungeonActorArray[adcount]->SetActorEnableCollision(true);
					DungeonActorArray[adcount]->SetWallMaetInst(Wall_Image[Stage]);
					adcount++;
				}
			}
		}
	}

	ay += 30;

	if (ay > size)
	{
		ax += 30;
		ay = 0;
	}

	else if (ax > size)
	{
		if (CheckMon == 1)
		{
			int size = DataMgr::GetInst()->GetStageSize();

			SceneCapture->GetCaptureComponent2D()->OrthoWidth = 50 * size;

			SceneCapture->SetActorLocation(FVector(size * 25, size * 25, 60));

			Floor->SetActorHiddenInGame(false);

			SceneCapture->GetCaptureComponent2D()->CaptureScene();

			for (int a = 0; a < adcount; a++)
			{
				DungeonActorArray[a]->SetActorHiddenInGame(true);
			}

			UE_LOG(LogTemp, Warning, TEXT("Capture"));

			//게이트 위치 변경
			int lx = (RoomArray[roomcount - 1]->PSData.sx / 2 + RoomArray[roomcount - 1]->PSData.x) * 50;
			int ly = (RoomArray[roomcount - 1]->PSData.sy / 2 + RoomArray[roomcount - 1]->PSData.y) * 50;

			//bNextFloor = true;
			//Gate->SetActorLocation(FVector(lx, ly, 10)); //게이트 위치
			//Gate->SetActorHiddenInGame(false);

			//보스 오브젝트 소환
			if (Spawner[DataMgr::GetInst()->GetStage() - 1] != nullptr)
			{
				GetWorld()->SpawnActor<AActor>(Spawner[DataMgr::GetInst()->GetStage() - 1],
					FTransform(FRotator(0, 0, 0),
						FVector(
							lx, ly, 10),
						FVector(1, 1, 1)));
				Objects[lx][ly] = 99;
				Objects[lx + 1][ly] = 99;
				Objects[lx - 1][ly] = 99;
				Objects[lx][ly + 1] = 99;
				Objects[lx][ly - 1] = 99;
				Objects[lx + 1][ly + 1] = 99;
				Objects[lx + 1][ly - 1] = 99;
				Objects[lx - 1][ly + 1] = 99;
				Objects[lx - 1][ly - 1] = 99;
			}

			else
			{
				ShowGate();
			}

			if (EpicMonster[DataMgr::GetInst()->GetStage() - 1] != nullptr)
			{
				int Rstage = DataMgr::GetInst()->GetStage() - 1;
				int RandomR = (rand() % (roomcount - 2)) + 1;

				int llx = (RoomArray[RandomR]->PSData.sx / 2 + RoomArray[RandomR]->PSData.x) * 50;
				int lly = (RoomArray[RandomR]->PSData.sy / 2 + RoomArray[RandomR]->PSData.y) * 50;

				AMonsterMgr * monster = GetWorld()->SpawnActor<AMonsterMgr>(EpicMonster[DataMgr::GetInst()->GetStage() - 1],
					FTransform(FRotator(0, 0, 0),
						FVector(
							llx, lly, 10),
						FVector(1, 1, 1)));

				monster->SpawnDefaultController();

				monster->SetMonNum(monstercount, this, Rstage);
				monster->SetActorHiddenInGame(true);

				monster->SetRoomMaxMin(
					FVector(
					(RoomArray[CheckMon]->PSData.x + 1) * 50,
						(RoomArray[CheckMon]->PSData.y + 1) * 50,
						0),
					FVector(
					(RoomArray[CheckMon]->PSData.sx - 2) * 50,
						(RoomArray[CheckMon]->PSData.sy - 2) * 50,
						0));

				MonsterArray[monstercount] = monster;
				monstercount++;
			}
		}

		if (CheckMon <= (roomcount - 1))
		{
			ArrayDungeonMonster(); //몬스터 스폰
			//UE_LOG(LogTemp, Warning, TEXT("MonSpawn"));
		}

		else if (CheckObj <= roomcount)
		{
			ArrayDungeonObject(); //오브젝트 스폰
			ArrayDecoration(); //데코레이션 배치
			CheckObj++;
			//UE_LOG(LogTemp, Warning, TEXT("ObjSpawn"));
		}

		else
		{
			StageLevelUpdate.Broadcast((size / 100));

			int iter = 0;

			//캐릭터를 자리로 이동
			for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
			{
				float x = (RoomArray[0]->PSData.sx / 2 + RoomArray[0]->PSData.x) * 50;
				float y = (RoomArray[0]->PSData.sy / 2 + RoomArray[0]->PSData.y) * 50;

				UGameplayStatics::GetPlayerCharacter(GetWorld(), iter)->SetActorLocation(FVector(x, y, 10));

				iter++;
			}

			good = true;
			UE_LOG(LogTemp, Warning, TEXT("Done"));
		}
	}
}

/* 여기부터는 방,길 데이터 생성 및 정리 */
void AMyLevelScriptActor::RoomSpawn(GRoomList * room)
{
	if (room->Cur == nullptr)
	{
		return;
	}

	int x = room->Cur->PSData.x;
	int y = room->Cur->PSData.y;
	int sx = room->Cur->PSData.sx;
	int sy = room->Cur->PSData.sy;
	int Stage = DataMgr::GetInst()->GetStage() - 1;

	for (int b = (x - 8); b <= (sx + x + 8); b++)
	{
		for (int c = (y - 8); c <= (sy + y + 8); c++)
		{
			if (b < 0 || c < 0)
			{
				DungeonActorArray[adcount]->SetActorRelativeLocation(FVector(50 * b, 50 * c, 25));
				DungeonActorArray[adcount]->SetActorHiddenInGame(false);
				DungeonActorArray[adcount]->SetActorEnableCollision(true);
				DungeonActorArray[adcount]->SetWallMaetInst(Wall_Image[Stage]);
				adcount++;
			}

			else
			{
				a[b][c] = 1;
			}
		}
	}

	/*for (int b = x; b <= (sx + x); b++)
	{
		for (int c = y; c <= (sy + y); c++)
		{
			a[b][c] = 1;
		}
	}*/

	/*for (int b = x + 1; b <= (sx + x) - 1; b++)
	{
		for (int c = y + 1; c <= (sy + y) - 1; c++)
		{
			a[b][c] = -1;
		}
	}*/

	RoomArray[roomcount] = room->Cur;

	roomcount++;

	NextNode(room);
	RoomSpawn(room);
}

void AMyLevelScriptActor::AisleSpawn(GAisleList * aisle)
{
	if (aisle->Cur == nullptr)
	{
		return;
	}

	int lx = aisle->Cur->PSData.lx;
	int rx = aisle->Cur->PSData.rx;
	int ly = aisle->Cur->PSData.ly;
	int ry = aisle->Cur->PSData.ry;
	int Stage = DataMgr::GetInst()->GetStage() - 1;

	/*for (int b = (lx - 8); b <= (rx + 8); b++)
	{
		for (int c = (ly - 8); c <= (ry + 8); c++)
		{
			if (b < 0 || c < 0)
			{
				DungeonActorArray[adcount]->SetActorRelativeLocation(FVector(50 * b, 50 * c, 20));
				DungeonActorArray[adcount]->SetActorHiddenInGame(false);
				DungeonActorArray[adcount]->SetActorEnableCollision(true);
				adcount++;
			}

			else
			{
				a[b][c] = 1;
			}
		}
	}*/

	if (lx == rx)
	{
		if (ly < ry)
		{
			for (int b = (lx - 8); b <= (rx + 8); b++)
			{
				for (int c = (ly - 8); c <= (ry + 8); c++)
				{
					if (b < 0 || c < 0)
					{
						DungeonActorArray[adcount]->SetActorRelativeLocation(FVector(50 * b, 50 * c, 25));
						DungeonActorArray[adcount]->SetActorHiddenInGame(false);
						DungeonActorArray[adcount]->SetActorEnableCollision(true);
						DungeonActorArray[adcount]->SetWallMaetInst(Wall_Image[Stage]);
						adcount++;
					}

					else
					{
						a[b][c] = 1;
					}
				}
			}
		}

		else
		{
			for (int b = (lx - 8); b <= (rx + 8); b++)
			{
				for (int c = (ry - 8); c <= (ly + 8); c++)
				{
					if (b < 0 || c < 0)
					{
						DungeonActorArray[adcount]->SetActorRelativeLocation(FVector(50 * b, 50 * c, 25));
						DungeonActorArray[adcount]->SetActorHiddenInGame(false);
						DungeonActorArray[adcount]->SetActorEnableCollision(true);
						DungeonActorArray[adcount]->SetWallMaetInst(Wall_Image[Stage]);
						adcount++;
					}

					else
					{
						a[b][c] = 1;
					}
				}
			}
		}
	}
	else
	{
		if (lx < rx)
		{
			for (int b = (lx - 8); b <= (rx + 8); b++)
			{
				for (int c = (ly - 8); c <= (ry + 8); c++)
				{
					if (b < 0 || c < 0)
					{
						DungeonActorArray[adcount]->SetActorRelativeLocation(FVector(50 * b, 50 * c, 25));
						DungeonActorArray[adcount]->SetActorHiddenInGame(false);
						DungeonActorArray[adcount]->SetActorEnableCollision(true);
						DungeonActorArray[adcount]->SetWallMaetInst(Wall_Image[Stage]);
						adcount++;
					}

					else
					{
						a[b][c] = 1;
					}
				}
			}
		}

		else
		{
			for (int b = (rx - 8); b <= (lx + 8); b++)
			{
				for (int c = (ly - 8); c <= (ry + 8); c++)
				{
					if (b < 0 || c < 0)
					{
						DungeonActorArray[adcount]->SetActorRelativeLocation(FVector(50 * b, 50 * c, 25));
						DungeonActorArray[adcount]->SetActorHiddenInGame(false);
						DungeonActorArray[adcount]->SetActorEnableCollision(true);
						DungeonActorArray[adcount]->SetWallMaetInst(Wall_Image[Stage]);
						adcount++;
					}

					else
					{
						a[b][c] = 1;
					}
				}
			}
		}
	}

	NextNode(aisle);
	AisleSpawn(aisle);
}

void AMyLevelScriptActor::RoomWallSpawn(GRoomList * room)
{
	if (room->Cur == nullptr)
	{
		return;
	}

	int x = room->Cur->PSData.x;
	int y = room->Cur->PSData.y;
	int sx = room->Cur->PSData.sx;
	int sy = room->Cur->PSData.sy;

	for (int b = x + 1; b <= (sx + x) - 1; b++)
	{
		for (int c = y + 1; c <= (sy + y) - 1; c++)
		{
			a[b][c] = 0;
		}
	}

	NextNode(room);
	RoomWallSpawn(room);
}

void AMyLevelScriptActor::AisleWallSpawn(GAisleList * aisle)
{
	if (aisle->Cur == nullptr)
	{
		return;
	}

	int Area = (rand() % 3) + 1;

	int lx = aisle->Cur->PSData.lx;
	int rx = aisle->Cur->PSData.rx;
	int ly = aisle->Cur->PSData.ly;
	int ry = aisle->Cur->PSData.ry;

	if (lx == rx)
	{
		if (ly < ry)
		{
			for (int b = ly; b <= ry; b++)
			{
				for (int c = 0; c < Area; c++)
				{
					a[lx + c][b] = 0;
				}
			}
		}
		else
		{
			for (int b = ry; b <= ly; b++)
			{
				for (int c = 0; c < Area; c++)
				{
					a[lx + c][b] = 0;
				}
			}
		}
	}
	else
	{
		if (lx < rx)
		{
			for (int b = lx; b <= rx; b++)
			{
				for (int c = 0; c < Area; c++)
				{
					a[b][ly + c] = 0;
				}
			}
		}

		else
		{
			for (int b = rx; b <= lx; b++)
			{
				for (int c = 0; c < Area; c++)
				{
					a[b][ly + c] = 0;
				}
			}
		}
	}

	NextNode(aisle);
	AisleWallSpawn(aisle);
}