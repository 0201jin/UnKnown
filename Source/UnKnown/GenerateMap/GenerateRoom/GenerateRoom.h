// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "DungeonActor/DungeonActor.h"

#include "CoreMinimal.h"
#include "GenerateMap/GenerateMap.h"
#include "Mgr/DataMgr.h"

using namespace GenerateMap;

/**
 * 
 */
class UNKNOWN_API GenerateRoom
{
public:
	GenerateRoom();
	~GenerateRoom();

	static void BSP_Dungeon_Generate();

	bool GetBool();

	GRoomList* GetRoom();
	GAisleList* GetAisle();
	GMapList* GetMapList();

protected:
	static void Generate_Map(GMap * map, int level, int count);
	static void DeClareRoom(GMap * map, GRoomList * room);
	static void CheckRoomSize(GMap * Map, GRoomList * room);
	static void Generate_Aisle(GMap * map, GAisleList * list);
};