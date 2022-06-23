
#pragma once

#ifdef DEVELOPMENT
#define ENCRID_API __declspec(dllexport)
#else
#define ENCRID_API __declspec(dllimport)
#endif

#ifndef _WINDOWS_
#include <Windows.h>
#endif

class TDE_Device;
class TDE_Point;
class TDE_Size;
class TDE_Coordinate;

class ENCRID_API TDE_Transform
{
public:
	TDE_Device* _dev;
	TDE_Transform();
	TDE_Transform(TDE_Device* dev);
	VOID Reset();
	VOID Rotate2D(FLOAT Angle, TDE_Point Position);
	VOID Scale2D(TDE_Size sz);
	VOID Translate2D(TDE_Size sz);
	//
	VOID Rotate3D(FLOAT Angle, TDE_Coordinate Axis);
};