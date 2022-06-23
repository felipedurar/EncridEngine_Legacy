
#define DEVELOPMENT
#include "Encrid2DAnimation.h"

TDE_2DAnimation::TDE_2DAnimation()
{
	_Counter = 0;
	MaxCounter = 0;
	CFrame = 0;
	FrameIncrease = 1;
	Repeat = TRUE;
	Enabled = TRUE;
}

VOID TDE_2DAnimation::PushFrame(TDE_Image* frame)
{
	Frames.push_back(frame);
}

VOID TDE_2DAnimation::DoStep()
{
	if (Enabled == TRUE)
	{
    	if (_Counter >= MaxCounter)
    	{
    		_Counter = 0;
    		CFrame += FrameIncrease;
			if (Repeat == FALSE)
			{
				Enabled = FALSE;
			}
    	}
	    else
	    {
		    _Counter += 1;
	    }
	}
	//
	if (CFrame >= Frames.size())
	{
		CFrame = 0;
	}
	//
	Index = Frames[CFrame]->Index;
	W = Frames[CFrame]->W;
	H = Frames[CFrame]->H;
	FileName = Frames[CFrame]->FileName;
	//
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			ImageCoord[x][y] = Frames[CFrame]->ImageCoord[x][y];
		}
	}
}

VOID TDE_2DAnimation::RotateMatrixCW_AllFrames()
{
	for (int C = 0; C < Frames.size(); C++)
	{
		if (Frames[C] != NULL)
		{
			Frames[C]->RotateMatrixCW();
		}
	}
}

