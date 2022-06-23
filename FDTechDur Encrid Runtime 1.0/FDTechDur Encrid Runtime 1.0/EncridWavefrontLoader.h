
#pragma once

#include <vector>
#include "EncridPosSize.h"
#include "EncridModel.h"
#include "EncridDevice.h"
#include "EncridImage.h"

#ifdef DEVELOPMENT
#define ENCRID_API __declspec(dllexport)
#else
#define ENCRID_API __declspec(dllimport)
#endif

#ifndef _WINDOWS_
#include <Windows.h>
#endif
 
//the main class for the object loader
class ENCRID_API TDE_WavefrontLoader : public TDE_Model
{
public:
    std::vector<std::string*> coord;        //every line of code from the obj file
    bool ismaterial,isnormals,istexture;    //obvious
    unsigned int LoadTexture(TDE_Device* dev, char* filename); //private load texture function
    void Clean();   //free all of the used memory
    //
    TDE_WavefrontLoader();   
    ~TDE_WavefrontLoader();   //free the textures and lists
    int Load(TDE_Device* dev, const char* filename); //the main model load function
};
 