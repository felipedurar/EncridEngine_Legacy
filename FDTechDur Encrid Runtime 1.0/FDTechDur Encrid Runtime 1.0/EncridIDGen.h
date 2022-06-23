
#pragma once

#ifdef DEVELOPMENT
#define ENCRID_API __declspec(dllexport)
#else
#define ENCRID_API __declspec(dllimport)
#endif
#ifndef _WINDOWS_
#include <Windows.h>
#endif

#include <iostream>
#include <vector>

using namespace std;

static INT IDCounter = 0;

extern "C"
{
	ENCRID_API INT TDE_GetID();

};

class ENCRID_API TDE_IDGEN
{
public:
	vector<INT> _IDs;
	INT AddID(INT id); // -1 = Error (this ID already exists) | >= 0 = Index
	INT GetID();
	BOOL IDExists(INT id);
	VOID RemoveID(INT id);
};