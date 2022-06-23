////////////////////////////////////
//  FDTechDur Encrid Runtime 1.0  //
//  Visual Studio Version         //
//  Developed by Felipe Durar     //
//  Date: 3/2015                  //
//////////////////////////////////////////////////////////////
// This file contains game management functions and classes //
//////////////////////////////////////////////////////////////

#pragma once

#ifndef _WINDOWS_
#include <Windows.h>
#endif

#include <vector>

using namespace std;

//#define DEVELOPMENT
#ifdef DEVELOPMENT
#define ENCRID_API __declspec(dllexport)
#else
#define ENCRID_API __declspec(dllimport)
#endif

#ifndef _TDE_ENCRID

#endif

#define TDE_DEFAULTFOLDER "Data\\"
#define TDE_DEFAULTFOLDERW L"Data\\"

class ENCRID_API TDE_GameInfo
{
public:
	WCHAR* DirGameObjects;
	WCHAR* DirSprites;
	WCHAR* DirRooms;
	TDE_GameInfo();
};

class TDE_Image;
class TDE_Device;
class TDE_GO_PHYSICS_VALUES;
class TDE_GOPhysics;
class TDE_Point;
class TDE_Colliders;
class TDE_2DAnimation;

class ENCRID_API TDE_GameObject
{
private:
    FLOAT X, Y;
public:
	WCHAR* GOName;
	WCHAR* SpriteDirectory;
	BOOL _MInit;
	TDE_Image* Sprite;
	TDE_GameInfo* gameInfo;
	TDE_Device* device;
	INT GOID;
	BOOL Visible;
	BOOL Fixed;
	BOOL Solid;
	BOOL EnableGravity;
	INT Mass;
	INT LoadFromFile(char* fileName);
	TDE_GameObject();
	VOID Initialize(TDE_GameInfo* gi, TDE_Device* dev);
	BOOL MInit();
	//
	vector<TDE_Image*> Sprites;
	vector<TDE_2DAnimation*> Animations;
	BOOL SpriteExistsByID(INT id);
	INT GetSpriteIndexByID(INT id);
	BOOL AnimationExistsByID(INT id);
	INT GetAnimationIndexByID(INT id);
	// Py
	TDE_Colliders* GOColliders;
	TDE_GO_PHYSICS_VALUES* GOPhysicsVals;
	TDE_GOPhysics* GOPhysics;
	//
	VOID _SetPosNC(FLOAT xP, FLOAT yP);
	VOID SetPosition(FLOAT xP, FLOAT yP);
	TDE_Point GetPosition();
	FLOAT GetX();
	FLOAT GetY();
};

class ENCRID_API TDE_Room
{
public:
	WCHAR* RName;
	INT RID;
	vector<TDE_GameObject*> AllGOs;
	TDE_GameInfo* gameInfo;
	TDE_Device* device;
	INT LoadFromFile(char* fileName);
	TDE_Room();
	TDE_Room(TDE_GameInfo* gi, TDE_Device* dev);
	VOID Initialize(TDE_GameInfo* gi, TDE_Device* dev);
	VOID Draw();
	TDE_GameObject* GetGameObjectByID(INT id);
	INT GetGameObjectIndexByID(INT id);


};