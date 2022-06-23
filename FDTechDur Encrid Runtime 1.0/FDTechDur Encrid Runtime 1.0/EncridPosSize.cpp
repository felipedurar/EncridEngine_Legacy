
#define DEVELOPMENT
#include "EncridPosSize.h"
#ifdef DEVELOPMENT
#define ENCRID_API __declspec(dllexport)
#else
#define ENCRID_API __declspec(dllimport)
#endif

TDE_Point::TDE_Point() {};
TDE_Point::TDE_Point(FLOAT x, FLOAT y)
{
	X = x; Y = y;
}
TDE_Point::TDE_Point(INT x, INT y)
{
	X = x; Y = y;
}
VOID TDE_Point::SetPoint(FLOAT x, FLOAT y)
{
	X = x; Y = y;
}
VOID TDE_Point::SetPoint(INT x, INT y)
{
	X = x; Y = y;
}

// TDE_Size
TDE_Size::TDE_Size() {};
TDE_Size::TDE_Size(FLOAT w, FLOAT h)
{
	W = w; H = h;
}
TDE_Size::TDE_Size(INT w, INT h)
{
	W = w; H = h;
}
VOID TDE_Size::SetSize(FLOAT w, FLOAT h)
{
	W = w; H = h;
}
VOID TDE_Size::SetSize(INT w, INT h)
{
	W = w; H = h;
}

// TDE_Coordinate
TDE_Coordinate::TDE_Coordinate(float a,float b,float c)
{
    x=a;
    y=b;
    z=c;
}

// TDE_Face
TDE_Face::TDE_Face(int facen,int f1,int f2,int f3,int t1,int t2,int t3,int m)
{
    facenum=facen;
    faces[0]=f1;
    faces[1]=f2;
    faces[2]=f3;
    texcoord[0]=t1;
    texcoord[1]=t2;
    texcoord[2]=t3;
    mat=m;
    four=false;
}
TDE_Face::TDE_Face(int facen,int f1,int f2,int f3,int f4,int t1,int t2,int t3,int t4,int m)
{
    facenum=facen;
    faces[0]=f1;
    faces[1]=f2;
    faces[2]=f3;
    faces[3]=f4;
    texcoord[0]=t1;
    texcoord[1]=t2;
    texcoord[2]=t3;
    texcoord[3]=t4;
    mat=m;
    four=true;
}

// TDE_TexCoordinate
TDE_TexCoordinate::TDE_TexCoordinate(float a,float b)
{
    u=a;
    v=b;
}


extern "C" ENCRID_API TDE_Point TDE_GetPointF(FLOAT x, FLOAT y)
{
	TDE_Point p;
	p.SetPoint(x, y);
	return p;
}

extern "C" ENCRID_API TDE_Point TDE_GetPointI(INT x, INT y)
{
	TDE_Point p;
	p.SetPoint(x, y);
	return p;
}

extern "C" ENCRID_API TDE_Size TDE_GetSizeF(FLOAT w, FLOAT h)
{
	TDE_Size s;
	s.SetSize(w, h);
	return s;
}

extern "C" ENCRID_API TDE_Size TDE_GetSizeI(INT w, INT h)
{
	TDE_Size s;
	s.SetSize(w, h);
	return s;
}
