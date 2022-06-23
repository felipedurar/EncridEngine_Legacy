
#define DEVELOPMENT 1.0
#include "EncridImage.h"

#include "EncridBitmap.h"
#include "EncridJPEG.h"
#include "EncridPNG.h"
#include "EncridDevice.h"

#include <gl\GL.h>

#include <iostream>
#include <string>

using namespace std;

class TDE_Device;

TDE_Image::TDE_Image()
{
	// Image Coord Matrix
	ImageCoord[0][0] = 0.0f;
	ImageCoord[0][1] = 0.0f;
	ImageCoord[1][0] = 1.0f;
	ImageCoord[1][1] = 0.0f;
	ImageCoord[2][0] = 1.0f;
	ImageCoord[2][1] = 1.0f;
	ImageCoord[3][0] = 0.0f;
	ImageCoord[3][1] = 1.0f;
	// Surface Matrix
	for (INT X = 0; X < 4; X++)
	{
		for (INT Y = 0; Y < 3; Y++)
		{
			SurfaceCoord[X][Y] = 0.0000f;
		}
	}
}

TDE_Image::TDE_Image(TDE_Image* imgClone)
{
	// Copy Mat
	ImageCoord[0][0] = imgClone->ImageCoord[0][0];
	ImageCoord[0][1] = imgClone->ImageCoord[0][1];
	ImageCoord[1][0] = imgClone->ImageCoord[1][0];
	ImageCoord[1][1] = imgClone->ImageCoord[1][1];
	ImageCoord[2][0] = imgClone->ImageCoord[2][0];
	ImageCoord[2][1] = imgClone->ImageCoord[2][1];
	ImageCoord[3][0] = imgClone->ImageCoord[3][0];
	ImageCoord[3][1] = imgClone->ImageCoord[3][1];
	//
	Index = imgClone->Index;
	W = imgClone->W;
	H = imgClone->H;
	FileName = imgClone->FileName;
}

VOID TDE_Image::LoadImageFromFile(TDE_Device* dev, char* fileName)
{
    std::string fn = fileName;
    if(fn.substr(fn.find_last_of(".") + 1) == "bmp" || fn.substr(fn.find_last_of(".") + 1) == "dib")
	{
        // Load BMP
		TDE_Bitmap* bmp = new TDE_Bitmap();
		bmp->LoadBMPFromFile(dev, fileName);
		W = bmp->W;
		H = bmp->H;
		Index = bmp->Index;
		delete bmp;
    } 
	else if(fn.substr(fn.find_last_of(".") + 1) == "jpg" || fn.substr(fn.find_last_of(".") + 1) == "jpeg")
	{
        // Load JPG
		TDE_JPEG* jpg = new TDE_JPEG();
		jpg->LoadJPGFromFile(dev, fileName);
		W = jpg->W;
		H = jpg->H;
		Index = jpg->Index;
		delete jpg;
    }
	else if (fn.substr(fn.find_last_of(".") + 1) == "png")
	{
		TDE_PNG* png = new TDE_PNG();
		png->LoadPNGFromFile(dev, fileName);
		W = png->W;
		H = png->H;
		Index = png->Index;
		delete png;
	}
}

VOID TDE_Image::CreateSurfaceMatrix(TDE_Device* dev, FLOAT x, FLOAT y)
{
	FLOAT nx = (2.0 * x) / dev->W;
	FLOAT ny = (2.0 * y) / dev->H;
	ny *= -1;
	//
	FLOAT nw = (2.0 * W) / dev->W;
	FLOAT nh = (2.0 * H) / dev->H;
	//
	SurfaceCoord[0][0] = nx;      SurfaceCoord[0][1] = ny;      SurfaceCoord[0][2] = 0.0f;
	SurfaceCoord[1][0] = nx + nw; SurfaceCoord[1][1] = ny;      SurfaceCoord[1][2] = 0.0f;
	SurfaceCoord[2][0] = nx + nw; SurfaceCoord[2][1] = ny - nh; SurfaceCoord[2][2] = 0.0f;
	SurfaceCoord[3][0] = nx;      SurfaceCoord[3][1] = ny - nh; SurfaceCoord[3][2] = 0.0f;
}

VOID TDE_Image::Crop(INT x, INT y, INT w, INT h)
{
	DOUBLE nx = (DOUBLE)x / (DOUBLE)W;
	DOUBLE ny = (DOUBLE)y / (DOUBLE)H;
	DOUBLE nw = (DOUBLE)w / (DOUBLE)W;
	DOUBLE nh = (DOUBLE)h / (DOUBLE)H;
	//
	ImageCoord[0][0] = nx;
	ImageCoord[0][1] = ny;
	ImageCoord[1][0] = nx + nw;
	ImageCoord[1][1] = ny;
	ImageCoord[2][0] = nx + nw;
	ImageCoord[2][1] = ny + nh;
	ImageCoord[3][0] = nx;
	ImageCoord[3][1] = ny + nh;
	//
	W = w;
	H = h;
}

VOID TDE_Image::ScaleSize(FLOAT scale)
{
	FLOAT wS = SurfaceCoord[1][0] - SurfaceCoord[0][0];
	FLOAT hS = SurfaceCoord[3][1] - SurfaceCoord[0][1];
	//
	SurfaceCoord[1][0] = SurfaceCoord[0][0] + (wS * scale);
	SurfaceCoord[2][0] = SurfaceCoord[0][0] + (wS * scale);
	SurfaceCoord[2][1] = SurfaceCoord[0][1] + (hS * scale);
	SurfaceCoord[3][1] = SurfaceCoord[0][1] + (hS * scale);
}

VOID TDE_Image::RotateMatrixCW()
{
	DOUBLE BackUpMat[4][2];
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			BackUpMat[y][x] = ImageCoord[y][x];
		}
	}
	//
	ImageCoord[0][0] = BackUpMat[1][0];
	ImageCoord[0][1] = BackUpMat[1][1];
	ImageCoord[1][0] = BackUpMat[2][0];
	ImageCoord[1][1] = BackUpMat[2][1];
	ImageCoord[2][0] = BackUpMat[3][0];
	ImageCoord[2][1] = BackUpMat[3][1];
	ImageCoord[3][0] = BackUpMat[0][0];
	ImageCoord[3][1] = BackUpMat[0][1];
    //
	//W = ImageCoord[1][0] - ImageCoord[0][0];
	//H = ImageCoord[3][1] - ImageCoord[0][1];
}

VOID TDE_Image::ResetImageCoordMatrix()
{
	// Image Coord Matrix
	ImageCoord[0][0] = 0.0f;
	ImageCoord[0][1] = 0.0f;
	ImageCoord[1][0] = 1.0f;
	ImageCoord[1][1] = 0.0f;
	ImageCoord[2][0] = 1.0f;
	ImageCoord[2][1] = 1.0f;
	ImageCoord[3][0] = 0.0f;
	ImageCoord[3][1] = 1.0f;
}