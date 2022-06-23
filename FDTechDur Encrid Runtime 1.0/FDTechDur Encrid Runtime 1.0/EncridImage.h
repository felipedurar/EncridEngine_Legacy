
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

extern "C" VOID _TDE_GENTEXTURE();

class ENCRID_API TDE_Image
{
public:
	TDE_Image();
	TDE_Image(TDE_Image* imgClone);
	WCHAR* FileName;
	INT W;
	INT H;
	INT Index;
	INT IdL; // Used for Lists
	VOID LoadImageFromFile(TDE_Device* dev, char* fileName);
	DOUBLE ImageCoord[4][2];
	FLOAT SurfaceCoord[4][3];
	VOID CreateSurfaceMatrix(TDE_Device* dev, FLOAT x, FLOAT y);
	VOID Crop(INT x, INT y, INT w, INT h);
	VOID ScaleSize(FLOAT scale);
	VOID RotateMatrixCW();
	VOID ResetImageCoordMatrix();
};
