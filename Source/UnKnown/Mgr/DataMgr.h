// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenerateMap/GenerateMap.h"

using namespace GenerateMap;

/**
 * 
 */
class UNKNOWN_API DataMgr
{
private:
	static DataMgr* m_pInst;

public:
	DataMgr();
	~DataMgr();

	static DataMgr* GetInst()
	{
		if (m_pInst == nullptr)
		{
			m_pInst = new DataMgr;
		}

		return m_pInst;
	}

	void SetMapList(GMapList* map) { MapList = map; FirstNode(MapList); Stage = 0;}
	GMakeMap * GetMapList();

	void NextMapList();
	void MapListFirstNode();

	int GetStageSize();
	int GetStage() { return Stage + 1; }

protected:
	GMapList * MapList;
	int StageSize[10] = { 100, 200, 200, 300, 300, 400, 400, 500, 500, 600 };
	int Stage;
};
