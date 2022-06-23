////////////////////////////////////
//  FDTechDur Encrid Runtime 1.0  //
//  Visual Studio Version         //
//  Developed by Felipe Durar     //
//  Date: 3/2015                  //
////////////////////////////////////

// STDs
#include <Windows.h>
#include <stdio.h>
#include <iostream>

// AL Includes
#include <AL/al.h>
#include <AL/alc.h>

// ES Header
#include "EncridSound.h"

using namespace std;

// Encrid Sound uses OpenAL Soft, an Open Source library
#pragma comment (lib, "OpenAL32.lib")

// AL Objects
ALCdevice *device;
ALCcontext *context;

#define ENCRID_API __declspec(dllexport)

extern "C"
{
	ENCRID_API VOID TDE_SoundInit()
	{
        device = alcOpenDevice(NULL);                             
        if(!device) MessageBox(NULL, L"No sound Device", L"Encrid Runtime Error!", MB_OK); return;
        context = alcCreateContext(device, NULL);                
        alcMakeContextCurrent(context);                   
        if(!context) MessageBox(NULL, L"No sound Context", L"Encrid Runtime Error!", MB_OK); return;
	};

	ENCRID_API VOID TDE_SoundClose()
	{
		alcMakeContextCurrent(NULL);                                                //Make no context current
        alcDestroyContext(context);                                                 //Destroy the OpenAL Context
        alcCloseDevice(device);                                                     //Close the OpenAL Device
	};

};

INT TDE_Wave::LoadFile(char* fileName)
{
    FILE *fp = NULL;                                                        
    fp=fopen(fileName,"rb");                                       
	if (!fp) 
	{ 
		MessageBox(NULL, L"Failed to open file", L"Encrid Runtime Error!", MB_OK); 
		return 1; 
	}                 
    
    //Check that the WAVE file is OK
    fread(type,sizeof(char),4,fp);                                             
    if(type[0]!='R' || type[1]!='I' || type[2]!='F' || type[3]!='F')         
	{
		MessageBox(NULL, L"Encrid Sound: No RIFF!", L"Encrid Runtime Error!", MB_OK); return 1;    
	}

    fread(&size, sizeof(DWORD),1,fp);                                        
    fread(type, sizeof(char),4,fp);                                           
    if (type[0]!='W' || type[1]!='A' || type[2]!='V' || type[3]!='E')         
	{
		MessageBox(NULL, L"Encrid Sound: Missing WAVE!", L"Encrid Runtime Error!", MB_OK); return 1;    
	}
    
    fread(type,sizeof(char),4,fp);                                          
    if (type[0]!='f' || type[1]!='m' || type[2]!='t' || type[3]!=' ')        
	{
		MessageBox(NULL, L"Encrid Sound: Missing FMT!", L"Encrid Runtime Error!", MB_OK); return 1;    
	}
    
    //Now we know that the file is a acceptable WAVE file
    //Info about the WAVE data is now read and stored
    fread(&chunkSize,sizeof(DWORD),1,fp);
    fread(&formatType,sizeof(short),1,fp);
    fread(&channels,sizeof(short),1,fp);
    fread(&sampleRate,sizeof(DWORD),1,fp);
    fread(&avgBytesPerSec,sizeof(DWORD),1,fp);
    fread(&bytesPerSample,sizeof(short),1,fp);
    fread(&bitsPerSample,sizeof(short),1,fp);
    
    fread(type,sizeof(char),4,fp);
    if (type[0]!='d' || type[1]!='a' || type[2]!='t' || type[3]!='a')   
	{
        MessageBox(NULL, L"Encrid Sound: Missing Data!", L"Encrid Runtime Error!", MB_OK); return 1;    
	}
    
    fread(&dataSize,sizeof(DWORD),1,fp);                                   
        
    buf= new unsigned char[dataSize];                       
    cout << fread(buf,sizeof(BYTE),dataSize,fp) << " bytes loaded\n";       
        
    fclose(fp);

	// AL
	frequency = sampleRate;
	format = 0;

    alGenBuffers(1, &buffer);                                                    //Generate one OpenAL Buffer and link to "buffer"
    alGenSources(1, &source);                                                   //Generate one OpenAL Source and link to "source"
    if(alGetError() != AL_NO_ERROR) 
	{
		MessageBox(NULL, L"Encrid Sound: GenSource Error!", L"Encrid Runtime Error!", MB_OK); return 1;    
	}
    
    //Figure out the format of the WAVE file
    if(bitsPerSample == 8)
    {
        if(channels == 1)
            format = AL_FORMAT_MONO8;
        else if(channels == 2)
            format = AL_FORMAT_STEREO8;
    }
    else if(bitsPerSample == 16)
    {
        if(channels == 1)
            format = AL_FORMAT_MONO16;
        else if(channels == 2)
            format = AL_FORMAT_STEREO16;
    }
    if(!format)
	{
		wchar_t t[50];
		swprintf(t, L"Encrid Sound: Wrong BitPerSample! %d", bitsPerSample);
		MessageBox(NULL, t, L"Encrid Runtime Error!", MB_OK); return 1;  
	}

    alBufferData(buffer, format, buf, dataSize, frequency);                    //Store the sound data in the OpenAL Buffer
    if(alGetError() != AL_NO_ERROR) 
	{
		MessageBox(NULL, L"Encrid Sound: Error loading AL Buffer", L"Encrid Runtime Error!", MB_OK); return 1;    
	}
}

VOID TDE_Wave::Release()
{
    alDeleteSources(1, &source);                                                //Delete the OpenAL Source
    alDeleteBuffers(1, &buffer);                                                 //Delete the OpenAL Buffer
	delete[] buf;
}

INT TDE_Wave::Play()
{ 
    //Sound setting variables
    ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };                                    //Position of the source sound
    ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };                                    //Velocity of the source sound
    ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };                                  //Position of the listener
    ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };                                  //Velocity of the listener
    ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };                 //Orientation of the listener
                                                                                //First direction vector, then vector pointing up) 
    //Listener                                                                               
    alListenerfv(AL_POSITION,    ListenerPos);                                  //Set position of the listener
    alListenerfv(AL_VELOCITY,    ListenerVel);                                  //Set velocity of the listener
    alListenerfv(AL_ORIENTATION, ListenerOri);                                  //Set orientation of the listener
    
    //Source
    alSourcei (source, AL_BUFFER,   buffer);                                 //Link the buffer to the source
    alSourcef (source, AL_PITCH,    1.0f     );                                 //Set the pitch of the source
    alSourcef (source, AL_GAIN,     1.0f     );                                 //Set the gain of the source
    alSourcefv(source, AL_POSITION, SourcePos);                                 //Set the position of the source
    alSourcefv(source, AL_VELOCITY, SourceVel);                                 //Set the velocity of the source
    alSourcei (source, AL_LOOPING,  AL_FALSE );                                 //Set if source is looping sound
    
    //PLAY 
    alSourcePlay(source);                                                       //Play the sound buffer linked to the source
    if(alGetError() != AL_NO_ERROR) 
	{
		MessageBox(NULL, L"Encrid Sound: Error playing sound!", L"Encrid Runtime Error!", MB_OK); 
		return 1;    
	}                                                   //Pause to let the sound play
    
    //Clean-up
	//alSourceQueueBuffers(source, 1, &buffer);
}

