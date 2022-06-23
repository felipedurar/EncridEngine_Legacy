
#pragma once

#ifndef _WINDOWS_
#include <Windows.h>
#endif

#ifdef DEVELOPMENT
#define ENCRID_API __declspec(dllexport)
#else
#define ENCRID_API __declspec(dllimport)
#endif



class ENCRID_API TDE_PixelFormat
{
public:
	INT Format;
	WORD Version;
	DWORD Flags;
	BYTE PixelType;
	BYTE ColorBits;
	BYTE DepthBits;
	BYTE LayerType;
	TDE_PixelFormat();
	VOID SetDefaultValues();
	VOID SetHDCPixelFormat(HDC* hdc);
};