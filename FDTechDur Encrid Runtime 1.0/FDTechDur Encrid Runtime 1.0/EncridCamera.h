
#pragma once

#ifdef DEVELOPMENT
#define ENCRID_API __declspec(dllexport)
#else
#define ENCRID_API __declspec(dllimport)
#endif

#ifndef _WINDOWS_
#include <Windows.h>
#endif

#include "EncridPosSize.h"

#define TDE_CAMERA_ORTHOGRAPHIC 0
#define TDE_CAMERA_PERSPECTIVE 1

class ENCRID_API TDE_Camera
{
public:
	INT CameraMode;
	FLOAT FOV;
	FLOAT Near;
	FLOAT Far;
	FLOAT AspectRatio;
	TDE_Coordinate* CameraPosition;
	TDE_Coordinate* LookAt;
	TDE_Coordinate* UpVector;
	TDE_Camera();
};
