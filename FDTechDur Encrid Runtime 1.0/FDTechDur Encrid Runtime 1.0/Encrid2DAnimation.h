
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

#include <vector>

using namespace std;

class ENCRID_API TDE_2DAnimation : public TDE_Image
{
public:
	BOOL Enabled;
	INT _Counter; // Counter (for internal use)
	INT MaxCounter; // Counter Tick (Time)
	INT CFrame; // Current Frame
	INT FrameIncrease; // How many will sum the frame
	INT idL; // Only for lists
	BOOL Repeat;
	vector<TDE_Image*> Frames;
	//
	TDE_2DAnimation();
	VOID PushFrame(TDE_Image* frame);
	VOID DoStep();
	//
	VOID RotateMatrixCW_AllFrames();

};

