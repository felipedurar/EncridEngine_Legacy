////////////////////////////////////
//  FDTechDur Encrid Runtime 1.0  //
//  Visual Studio Version         //
//  Developed by Felipe Durar     //
//  Date: 3/2015                  //
////////////////////////////////////

#ifndef _WINDOWS_
#include <Windows.h>
#endif

//#define DEVELOPMENT
#ifdef DEVELOPMENT
#define ENCRID_API __declspec(dllexport)
#else
#define ENCRID_API __declspec(dllimport)
#endif

typedef unsigned int uint;

extern "C"
{
	ENCRID_API VOID TDE_SoundInit();
	ENCRID_API VOID TDE_SoundClose();
};

class ENCRID_API TDE_Wave
{
public:
	// Wave Header
	char type[4];
    DWORD size,chunkSize;
    short formatType,channels;
    DWORD sampleRate,avgBytesPerSec;
    short bytesPerSample,bitsPerSample;
    DWORD dataSize;
	// AL Vars
    uint source;                                                              //Is the name of source (where the sound come from)
    uint buffer;                                                           //Stores the sound data
    uint frequency;                                               //The Sample Rate of the WAVE file
    uint format;                                                            //The audio format (bits per sample, number of channels)
	//
	BYTE* buf;
	//
	INT LoadFile(char* fileName);
	VOID Release();
	INT Play();

};



