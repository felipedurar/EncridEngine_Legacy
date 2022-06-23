
#pragma once

#ifdef DEVELOPMENT
#define ENCRID_API __declspec(dllexport)
#else
#define ENCRID_API __declspec(dllimport)
#endif

#ifndef _WINDOWS_
#include <Windows.h>
#endif

#include "EncridImage.h"

class TDE_Device;

class ENCRID_API TDE_Bitmap : public TDE_Image
{
public:
	VOID LoadBMPFromFile(TDE_Device* dev, CHAR* fileName);

};

