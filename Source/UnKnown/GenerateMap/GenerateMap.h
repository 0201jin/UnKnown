// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
namespace GenerateMap
{
	typedef struct _Data
	{
		int x, y, sx, sy;
	}Data;

	typedef struct _AisleData
	{
		int rx, ry, lx, ly;
	}AisleData;

	typedef struct _Map
	{
		Data PSData;
		int rsx, rsy, rx, ry;

		struct _Map * left;
		struct _Map * right;
		struct _Map * root;

	}GMap;

	typedef struct _Room
	{
		Data PSData;

		struct _Room * Back;
	}GRoom;

	typedef struct _Aisle
	{
		AisleData PSData;

		struct _Aisle * Back;
	}GAisle;

	typedef struct _AisleList
	{
		GAisle * Cur;

		GAisle * Header;

		GAisle * Tail;
	}GAisleList;

	typedef struct _RoomList
	{
		GRoom * Cur;

		GRoom * Header;

		GRoom * Tail;
	}GRoomList;

	typedef struct _MakeMap
	{
		GRoomList * RoomList;

		GAisleList * AisleList;

		struct _MakeMap * Back;
	}GMakeMap;

	typedef struct _MapList
	{
		GMakeMap * Cur;

		GMakeMap * Header;

		GMakeMap * Tail;
	}GMapList;

	typedef void FuncBTree(GMap* btn);
	typedef void FuncNode(GRoom* btn);



	GMap * MakeTree();
	Data GetData(GMap* map);
	void SetData(GMap* map, Data data);

	void MakeLeftSubTree(GMap* root, GMap* sub);
	GMap* GetLeftSubTree(GMap* root);

	void MakeRightSubTree(GMap* root, GMap* sub);
	GMap* GetRightSubTree(GMap* root, GMap* sub);

	void DeleteTreeNode(GMap* btn);

	void PostorderTraverse(GMap* btn, FuncBTree func);



	GRoom * MakeGRoom();

	void MakeBackNode(GRoom* room, GRoom* broom);

	void DeleteNodeList(GRoom* room);

	void ArrayListInit(GRoomList * list);

	void GRoomListAddNode(GRoomList * list, GRoom * room);

	void FirstNode(GRoomList * list);

	bool NextNode(GRoomList * list);



	GAisle * MakeGAisle();

	void MakeBackNode(GAisle* room, GAisle* broom);

	void DeleteNodeList(GAisle* room);

	void ArrayListInit(GAisleList * list);

	void GRoomListAddNode(GAisleList * list, GAisle * room);

	void FirstNode(GAisleList * list);

	bool NextNode(GAisleList * list);


	GMakeMap * MakeGMakeMap();

	void MakeBackNode(GMakeMap* room, GMakeMap* broom);

	void DeleteNodeList(GMakeMap* room);

	void ArrayListInit(GMapList * list);

	void GRoomListAddNode(GMapList * list, GMakeMap * room);

	void FirstNode(GMapList * list);

	bool NextNode(GMapList * list);
}