// Fill out your copyright notice in the Description page of Project Settings.

#include "GenerateMap.h"

using namespace GenerateMap;

GMap * GenerateMap::MakeTree()
{
	GMap* btn = new GMap;

	btn->PSData.sx = 0;
	btn->PSData.sy = 0;
	btn->PSData.x = 0;
	btn->PSData.y = 0;

	btn->rsx = 0;
	btn->rsy = 0;

	btn->left = nullptr;
	btn->right = nullptr;

	return btn;
}

Data GenerateMap::GetData(GMap * map)
{
	return map->PSData;
}

void GenerateMap::SetData(GMap * map, Data data)
{
	map->PSData = data;
}

void GenerateMap::MakeLeftSubTree(GMap * root, GMap * sub)
{
	if (root->left != nullptr)
	{
		PostorderTraverse(root, DeleteTreeNode);
	}
	root->left = sub;
	sub->root = root;
}

GMap * GenerateMap::GetLeftSubTree(GMap * root)
{
	return root->left;
}

void GenerateMap::MakeRightSubTree(GMap * root, GMap * sub)
{
	if (root->right != nullptr)
	{
		PostorderTraverse(root, DeleteTreeNode);
	}
	root->right = sub;
	sub->root = root;
}

GMap * GenerateMap::GetRightSubTree(GMap * root, GMap * sub)
{
	return root->right;
}

void GenerateMap::DeleteTreeNode(GMap* btn)
{
	delete(btn);
}

void GenerateMap::PostorderTraverse(GMap * btn, FuncBTree func)
{
	if (btn == nullptr)
		return;

	PostorderTraverse(btn->left, func);
	PostorderTraverse(btn->right, func);
	func(btn);
}

GRoom * GenerateMap::MakeGRoom()
{
	GRoom* btn = new GRoom;

	btn->PSData.sx = 0;
	btn->PSData.sy = 0;
	btn->PSData.x = 0;
	btn->PSData.y = 0;

	btn->Back = nullptr;

	return btn;
}

void GenerateMap::MakeBackNode(GRoom * room, GRoom * broom)
{
	room->Back = broom;
}

void GenerateMap::DeleteNodeList(GRoom * room)
{
	delete(room);
}

void GenerateMap::ArrayListInit(GRoomList * list)
{
	list->Cur = nullptr;
	list->Header = nullptr;
	list->Tail = nullptr;
}

void GenerateMap::GRoomListAddNode(GRoomList * list, GRoom * room)
{
	if (list->Header == nullptr)
	{
		room->Back = nullptr;
		list->Header = room;
		list->Tail = room;

		return;
	}

	list->Tail->Back = room;
	list->Tail = room;
}

void GenerateMap::FirstNode(GRoomList * list)
{
	if (list->Header == nullptr)
		return;

	list->Cur = list->Header;
	return;
}

bool GenerateMap::NextNode(GRoomList * list)
{
	if (list->Cur == nullptr)
	{
		list->Cur = list->Header;
		return false;
	}


	list->Cur = list->Cur->Back;
	return true;
}

GAisle * GenerateMap::MakeGAisle()
{
	GAisle * room = new GAisle();

	room->PSData.rx = 0;
	room->PSData.ry = 0;
	room->PSData.lx = 0;
	room->PSData.ly = 0;

	return room;
}

void GenerateMap::MakeBackNode(GAisle * room, GAisle * broom)
{
	room->Back = broom;
}

void GenerateMap::DeleteNodeList(GAisle * room)
{
	delete(room);
}

void GenerateMap::ArrayListInit(GAisleList * list)
{
	list->Cur = nullptr;
	list->Header = nullptr;
	list->Tail = nullptr;
}

void GenerateMap::GRoomListAddNode(GAisleList * list, GAisle * room)
{
	if (list->Header == nullptr)
	{
		room->Back = nullptr;
		list->Header = room;
		list->Tail = room;

		return;
	}

	list->Tail->Back = room;
	list->Tail = room;
}

void GenerateMap::FirstNode(GAisleList * list)
{
	if (list->Header == nullptr)
		return;

	list->Cur = list->Header;
	return;
}

bool GenerateMap::NextNode(GAisleList * list)
{
	if (list->Cur == nullptr)
	{
		list->Cur = list->Header;
		return false;
	}

	list->Cur = list->Cur->Back;
	return true;
}

GMakeMap * GenerateMap::MakeGMakeMap()
{
	GMakeMap * map = new GMakeMap;

	map->AisleList = nullptr;
	map->RoomList = nullptr;
	map->Back = nullptr;

	return map;
}

void GenerateMap::MakeBackNode(GMakeMap * map, GMakeMap * bmap)
{
	map->Back = bmap;
}

void GenerateMap::DeleteNodeList(GMakeMap * map)
{
	delete (map);
}

void GenerateMap::ArrayListInit(GMapList * list)
{
	list->Cur = nullptr;
	list->Header = nullptr;
	list->Tail = nullptr;
}

void GenerateMap::GRoomListAddNode(GMapList * list, GMakeMap * map)
{
	if (list->Header == nullptr)
	{
		map->Back = nullptr;
		list->Header = map;
		list->Tail = map;

		return;
	}

	list->Tail->Back = map;
	list->Tail = map;
}

void GenerateMap::FirstNode(GMapList * list)
{
	if (list->Header == nullptr)
		return;

	list->Cur = list->Header;
	return;
}

bool GenerateMap::NextNode(GMapList * list)
{
	if (list->Cur == nullptr)
	{
		list->Cur = list->Header;
		return false;
	}

	list->Cur = list->Cur->Back;
	return true;
}
