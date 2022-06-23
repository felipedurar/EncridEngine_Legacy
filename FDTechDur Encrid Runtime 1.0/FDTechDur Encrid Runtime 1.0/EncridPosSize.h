
#pragma once

#ifdef DEVELOPMENT
#define ENCRID_API __declspec(dllexport)
#else
#define ENCRID_API __declspec(dllimport)
#endif

#ifndef _WINDOWS_
#include <Windows.h>
#endif

class ENCRID_API TDE_Point
{
public:
	FLOAT X, Y;
	TDE_Point();
	TDE_Point(FLOAT x, FLOAT y);
	TDE_Point(INT x, INT y);
	VOID SetPoint(FLOAT x, FLOAT y);
	VOID SetPoint(INT x, INT y);
};

class ENCRID_API TDE_Size
{
public:
	FLOAT W, H;
	TDE_Size();
	TDE_Size(FLOAT w, FLOAT h);
	TDE_Size(INT w, INT h);
	VOID SetSize(FLOAT w, FLOAT h);
	VOID SetSize(INT w, INT h);
};


class ENCRID_API TDE_Coordinate
{
public:
	float x,y,z;
	TDE_Coordinate(float a,float b,float c);
};
 

class ENCRID_API TDE_Face
{
public:
	int facenum;    //the number of the face (it's start from 1 not 0, so if you use it as an index, subtract 1 from it), it's used for the normal vectors
	bool four;              //if true, than it's a quad else it's a triangle
	int faces[4];   //indexes for every vertex, which makes the face (it's start from 1 not 0, so if you use it as an index, subtract 1 from it)
	int texcoord[4];        //indexes for every texture coorinate that is in the face (it's start from 1 not 0, so if you use it as an index, subtract 1 from it)
	int mat;                                        //the index for the material, which is used by the face
	TDE_Face(int facen,int f1,int f2,int f3,int t1,int t2,int t3,int m);        //constuctor for triangle
	TDE_Face(int facen,int f1,int f2,int f3,int f4,int t1,int t2,int t3,int t4,int m);  //-"- for quad
};

class ENCRID_API TDE_TexCoordinate
{
public:
        float u,v;
        TDE_TexCoordinate(float a,float b);
};

extern "C" ENCRID_API TDE_Point TDE_GetPointF(FLOAT x, FLOAT y);

extern "C" ENCRID_API TDE_Point TDE_GetPointI(INT x, INT y);

extern "C" ENCRID_API TDE_Size TDE_GetSizeF(FLOAT w, FLOAT h);

extern "C" ENCRID_API TDE_Size TDE_GetSizeI(INT w, INT h);