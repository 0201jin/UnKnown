// Fill out your copyright notice in the Description page of Project Settings.

#include "GenerateRoom.h"
#include <iostream>
#include <time.h>
#include <thread>
#include "MyLevelScriptActor.h"


bool mm;

GRoomList* room;

GAisleList* AisleList;

GMapList* MakeMapList;

GenerateRoom::GenerateRoom()
{	
}

GenerateRoom::~GenerateRoom()
{
}

void GenerateRoom::BSP_Dungeon_Generate()
{
	int LevelArr[6] = { 3, 4, 5, 6, 7, 8 };
	int RoomSize[6] = { 100, 200, 300, 400, 500, 600};

	srand((unsigned int)time(NULL));

	int count = 0;

	MakeMapList = new GMapList;

	ArrayListInit(MakeMapList);

	for (int a = 1; a <= 10; a++) //10으로 수정하기
	{
		if (a % 2 == 0)
		{
			count++;
		}

		GMap* map = MakeTree();
		map->PSData.sx = RoomSize[count];
		map->PSData.sy = RoomSize[count];
		map->root = map;

		Generate_Map(map, LevelArr[count], 0);

		room = new GRoomList;
		AisleList = new GAisleList;

		ArrayListInit(room);

		ArrayListInit(AisleList);

		DeClareRoom(map, room);

		Generate_Aisle(map, AisleList);

		FirstNode(room);

		GMakeMap * MakeMap = MakeGMakeMap();
		MakeMap->AisleList = AisleList;
		MakeMap->RoomList = room;

		GRoomListAddNode(MakeMapList, MakeMap);
	}

	DataMgr::GetInst()->SetMapList(MakeMapList);

	mm = true;
}

bool GenerateRoom::GetBool()
{
	return mm;
}

GRoomList * GenerateRoom::GetRoom()
{
	return room;
}

GAisleList * GenerateRoom::GetAisle()
{
	return AisleList;
}

GMapList * GenerateRoom::GetMapList()
{
	return MakeMapList;
}

void GenerateRoom::Generate_Map(GMap * map, int level, int count)
{
	if (map->PSData.sx < 10 || map->PSData.sy < 10)
	{
		//비 정상적인 방으로 인한 오류 방지

		if (map->root->left == map)
		{
			map->root->left = nullptr;
		}

		else if(map->root->right == map)
		{
			map->root->right = nullptr;
		}

		delete (map);
		return;
	}

	MakeLeftSubTree(map, MakeTree());
	MakeRightSubTree(map, MakeTree());
	count++;

	//사이즈 연산

	if (count % 2 == 0)
	{
		map->left->PSData.sx = (rand() % (map->PSData.sx / 2)) + (map->PSData.sx / 3);
		map->left->PSData.sy = map->PSData.sy;

		map->right->PSData.sx = map->PSData.sx - map->left->PSData.sx;
		map->right->PSData.sy = map->PSData.sy;

		//위치 연산
		map->left->PSData.x = map->PSData.x;
		map->left->PSData.y = map->PSData.y;

		map->right->PSData.x = map->PSData.x + map->left->PSData.sx;
		map->right->PSData.y = map->PSData.y;

		if (map->left->PSData.sx < 10 || map->left->PSData.sy < 10)
		{
			if (map->root->left == map)
			{
				map->root->left = nullptr;
			}
			else if (map->root->right == map)
			{
				map->root->right = nullptr;
			}

			delete (map);
			return;
		}

		else if (map->right->PSData.sx < 10 || map->right->PSData.sy < 10)
		{
			if (map->root->left == map)
			{
				map->root->left = nullptr;
			}
			else if (map->root->right == map)
			{
				map->root->right = nullptr;
			}

			delete (map);
			return;
		}
	}

	else
	{
		map->left->PSData.sx = map->PSData.sx;
		map->left->PSData.sy = (rand() % (map->PSData.sy / 2)) + (map->PSData.sy / 3);

		map->right->PSData.sx = map->PSData.sx;
		map->right->PSData.sy = map->PSData.sy - map->left->PSData.sy;

		//위치 연산
		map->left->PSData.x = map->PSData.x;
		map->left->PSData.y = map->PSData.y;

		map->right->PSData.x = map->PSData.x;
		map->right->PSData.y = map->PSData.y + map->left->PSData.sy;

		if (map->left->PSData.sx < 10 || map->left->PSData.sy < 10)
		{
			if (map->root->left == map)
			{
				map->root->left = nullptr;
			}
			else if (map->root->right == map)
			{
				map->root->right = nullptr;
			}

			delete (map);
			return;
		}

		else if (map->right->PSData.sx < 10 || map->right->PSData.sy < 10)
		{
			if (map->root->left == map)
			{
				map->root->left = nullptr;
			}
			else if (map->root->right == map)
			{
				map->root->right = nullptr;
			}

			delete (map);
			return;
		}
	}

	if (level == count)
	{
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("%d %d %d %d"), map->left->PSData.sx, map->left->PSData.sy, map->right->PSData.sx, map->right->PSData.sy);

	Generate_Map(map->left, level, count);

	Generate_Map(map->right, level, count);
}

void GenerateRoom::DeClareRoom(GMap * map, GRoomList * room)
{
	if (map == nullptr)
	{
		return;
	}

 	else if (map->left == nullptr && map->right == nullptr) //확률적인 버그
	{
		CheckRoomSize(map, room);

		return;
	}

	DeClareRoom(map->left, room);
	DeClareRoom(map->right, room);
}

void GenerateRoom::CheckRoomSize(GMap * map, GRoomList * room)
{
	int sx, sy, x, y;
	bool check = false;

	sx = (rand() % (map->PSData.sx / 3)) + map->PSData.sx / 2;
	sy = (rand() % (map->PSData.sy / 3)) + map->PSData.sy / 2;

	x = (rand() % (map->PSData.sx - (sx))) + map->PSData.x;
	y = (rand() % (map->PSData.sy - (sy))) + map->PSData.y;

	if (sx + x == map->PSData.sx / 2 + map->PSData.x)
	{
		x++;
	}
	else if (x == map->PSData.sx / 2 + map->PSData.x)
	{
		x--;
	}
	else if (sy + y == map->PSData.sy / 2 + map->PSData.y)
	{
		y++;
	}
	else if (y == map->PSData.sy / 2 + map->PSData.y)
	{
		y--;
	}

	if (sx < 4 || sy < 4) //방이 4 * 4보다 작으면 함수 다시 실행
	{
		CheckRoomSize(map, room);
	}

	while (sx > sy * 3)
	{
		sx--;
		if (!(x + sx > map->PSData.sx / 2 + map->PSData.x + 1))
		{
			x++;
		}
	}

	while (sy > sx * 3)
	{
		sy--;
		if (!(y + sy > map->PSData.sy / 2 + map->PSData.y + 1))
		{
			y++;
		}
	}

	map->rsx = sx;
	map->rsy = sy;

	map->rx = x;
	map->ry = y;

	GRoom * roomdata = MakeGRoom();
	roomdata->PSData.sx = sx;
	roomdata->PSData.sy = sy;

	roomdata->PSData.x = x;
	roomdata->PSData.y = y;

	GRoomListAddNode(room, roomdata);

	return;
}

void GenerateRoom::Generate_Aisle(GMap * map, GAisleList * list)
{
	if (map == nullptr)
	{
		return;
	}

	if (map->left != nullptr)
	{
		Generate_Aisle(map->left, list);
	}

	if (map->right != nullptr)
	{
		Generate_Aisle(map->right, list);
	}

	//후위순회

	GAisle * aisledata = MakeGAisle();

	aisledata->PSData.lx = map->PSData.x + map->PSData.sx / 2;
	aisledata->PSData.ly = map->PSData.y + map->PSData.sy / 2;

	aisledata->PSData.rx = map->root->PSData.x + map->root->PSData.sx / 2;
	aisledata->PSData.ry = map->root->PSData.y + map->root->PSData.sy / 2;

	GRoomListAddNode(list, aisledata);
}