
#pragma once

#include <iostream>
#include <vector>

using namespace std;

#include "EncridGame.h"

#ifndef _WINDOWS_
#include <Windows.h>
#endif

#ifdef DEVELOPMENT
#define ENCRID_API __declspec(dllexport)
#else
#define ENCRID_API __declspec(dllimport)
#endif

class ENCRID_API TDE_GO_PHYSICS_VALUES
{
public:
	BOOL _PENABLE;
	BOOL _GRAVITY;
	FLOAT _YForce;
	FLOAT _XForce;
	FLOAT _YMAXF;
	FLOAT _YMINF;
	FLOAT _XMAXF;
	FLOAT _XMINF;
	FLOAT _PMASS;
	FLOAT _XFS;
	FLOAT _YFS;
	FLOAT _XFSR;
	FLOAT _YFSR;
	// Last Pos
	FLOAT _XLP;
	FLOAT _YLP;
	// LAST DIR
	INT _XLD;
	INT _YLD;
	// BackUp X and Y
	FLOAT _BX;
	FLOAT _BY;
	// Move Ratio
	FLOAT _RX;
	FLOAT _RY;
	TDE_GO_PHYSICS_VALUES();
};

class ENCRID_API TDE_CircleCollider
{
public:
	FLOAT X;
	FLOAT Y;
	FLOAT Radius;
};

class ENCRID_API TDE_BoxCollider
{
public:
	FLOAT X;
	FLOAT Y; 
	FLOAT W;
	FLOAT H;
};

class ENCRID_API TDE_Colliders
{
	vector<TDE_CircleCollider*> Circles; // From object position
	vector<TDE_BoxCollider*> Boxes;
};

class ENCRID_API TDE_GOPhysics
{
public:
	TDE_GameObject* go;
	TDE_GOPhysics();
	TDE_GOPhysics(TDE_GameObject* gop);
	BOOL Colision(TDE_GOPhysics* goPhy);
	BOOL ColisionVari(TDE_GOPhysics* goPhy, FLOAT vari);
	VOID EnablePhysics();
	VOID SetYGravity(FLOAT force, FLOAT maxForce);
	VOID SetMass(INT m);
	VOID DoPhysics();
	VOID ApplyXForce(FLOAT xf);
	VOID ApplyYForce(FLOAT yf);
	VOID SetXForce(FLOAT xf);
	VOID SetYForce(FLOAT yf);
	VOID XForceReductionRatio(FLOAT xs);
	VOID YForceReductionRatio(FLOAT ys);

};

class ENCRID_API TDE_Side
{
public:
	INT xs;
	INT ys;
	TDE_Side();
	TDE_Side GetSideColision(TDE_GameObject* go1, TDE_GameObject* go2);
};

class ENCRID_API TDE_GlobalPhysics
{
public:
	vector<TDE_GameObject*> AllGOs;
	INT CollisionPrecision;
	TDE_GlobalPhysics();
	VOID DoPhysics(const vector<TDE_GameObject*>& GOs);
	VOID SolveCollision(const vector<TDE_GameObject*>& GOs);

};