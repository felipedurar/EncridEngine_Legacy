
#pragma once

#ifdef DEVELOPMENT
#define ENCRID_API __declspec(dllexport)
#else
#define ENCRID_API __declspec(dllimport)
#endif

#ifndef _WINDOWS_
#include <Windows.h>
#endif

#include "EncridModel.h"

class ENCRID_API TDE_Shapes
{
public:
	static TDE_Model* CreatePlane(FLOAT w, FLOAT h);



};


