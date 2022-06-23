
#pragma once

#include <vector>

#include "EncridPosSize.h"

#ifdef DEVELOPMENT
#define ENCRID_API __declspec(dllexport)
#else
#define ENCRID_API __declspec(dllimport)
#endif

#ifndef _WINDOWS_
#include <Windows.h>
#endif

class ENCRID_API TDE_Material
{
public:
    std::string name;       //the name of the material
    float alpha,ns,ni;      //some property, alpha, shininess, and some other, which we not used
    float dif[3],amb[3],spec[3];    //the color property (diffuse, ambient, specular)
    int illum;      //illum - we not use it
    int texture;    //the id for the texture, if there is no texture than -1
    TDE_Material(const char* na,float al,float n,float ni2,float* d,float* a,float* s,int i,int t);
};

class ENCRID_API TDE_Model
{
public:
	int GLListId;
	bool UseGLListIs;
	//
    std::vector<TDE_Coordinate*> vertex;        //all vertexes
    std::vector<TDE_Face*> faces;                                       //all faces
    std::vector<TDE_Coordinate*> normals;       //all normal vectors
    std::vector<unsigned int> texture;//the id for all the textures (so we can delete the textures after use it)
    std::vector<unsigned int> lists;        //the id for all lists (so we can delete the lists after use it)
    std::vector<TDE_Material*> materials;       //all materials
    std::vector<TDE_TexCoordinate*> texturecoordinate;       //all texture coorinate (UV coordinate)
	//
	TDE_Model::TDE_Model();

};

