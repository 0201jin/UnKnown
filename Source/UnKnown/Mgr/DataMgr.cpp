// Fill out your copyright notice in the Description page of Project Settings.

#include "DataMgr.h"

DataMgr* DataMgr::m_pInst = nullptr;

DataMgr::DataMgr()
{
}

DataMgr::~DataMgr()
{
}

GMakeMap * DataMgr::GetMapList()
{
	return MapList->Cur;
}

void DataMgr::NextMapList()
{
	Stage++;
	NextNode(MapList);
}

void DataMgr::MapListFirstNode()
{
	FirstNode(MapList->Cur->AisleList);

	FirstNode(MapList->Cur->RoomList);
}

int DataMgr::GetStageSize()
{
	return StageSize[Stage];
}
