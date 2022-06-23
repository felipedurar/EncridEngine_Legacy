
#define DEVELOPMENT 1.0
#include "EncridPixelFormat.h"

TDE_PixelFormat::TDE_PixelFormat()
{
	SetDefaultValues();
}

VOID TDE_PixelFormat::SetDefaultValues()
{
	Version = 1;
	Flags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	PixelType = PFD_TYPE_RGBA;
	ColorBits = 24;
	DepthBits = 16;
	LayerType = PFD_MAIN_PLANE;
}

VOID TDE_PixelFormat::SetHDCPixelFormat(HDC* hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = Version;
	pfd.dwFlags = Flags;
	pfd.iPixelType = PixelType;
	pfd.cColorBits = ColorBits;
	pfd.cDepthBits = DepthBits;
	pfd.iLayerType = LayerType;
	//
	Format = ChoosePixelFormat(*hdc, &pfd);
	SetPixelFormat(*hdc, Format, &pfd);
}

