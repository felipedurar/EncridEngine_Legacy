
#pragma once

#ifdef DEVELOPMENT
#define ENCRID_API __declspec(dllexport)
#else
#define ENCRID_API __declspec(dllimport)
#endif

#ifndef _WINDOWS_
#include <Windows.h>
#endif

#include "EncridPixelFormat.h"
#include "EncridImage.h"
#include "EncridPosSize.h"
#include "EncridCamera.h"
#include "EncridModel.h"

class TDE_Transform;

class ENCRID_API TDE_Device
{
public:
	TDE_Camera* MainCamera;
	INT ID;
	HWND WHandle;
	HDC WHDC;
	HGLRC WHRC;
	//
	TDE_PixelFormat* PixelFormat;
	INT W;
	INT H;
	UINT BBTextureId;  // BackBuffer Texture
	UINT BBFrameBufferId;
	//
	UINT _TexIDCounter;
	//
	TDE_Device();
	TDE_Device(HWND hwnd);
	VOID MakeCurrent();
	VOID Initialize(HWND hwnd);
	VOID Disable();
	VOID UpdatePixelFormat();
	TDE_Transform* GetTransform();
	//
	VOID Begin();
    VOID Flip();
	VOID End();
	//
	VOID Resize(); // Use the current hwnd size
	VOID Resize(INT w, INT h);
	//
	VOID Clear(FLOAT r, FLOAT g, FLOAT b);
	VOID Clear(FLOAT r, FLOAT g, FLOAT b, FLOAT a);
	VOID ClearI(INT r, INT g, INT b);
	VOID ClearI(INT r, INT g, INT b, INT a);
	//
	VOID SetCamera();
	VOID SetCamera(TDE_Camera* cam);
	//
	VOID DrawModel(TDE_Model* model);
	VOID DrawModel(TDE_Model* model, TDE_Coordinate* position);
	VOID DrawImage(TDE_Image* img); // fill the window
	VOID DrawImage(TDE_Image* img, FLOAT x, FLOAT y);

};



