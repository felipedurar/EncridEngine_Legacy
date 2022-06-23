
// Windows
#ifndef _WINDOWS_
#include <Windows.h>
#endif

// GL Includes
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
//#include <gl\GL.h>
//#include <gl\GLU.h>
#include <GL\glew.h>

// Proj Includes
#define DEVELOPMENT 1.0
#include "EncridDevice.h"
#include "EncridIDGen.h"
#include "EncridTransform.h"
#include "EncridModel.h"
#include "EncridCamera.h"

VOID _Resize(INT w, INT h, TDE_Device* dev);

VOID _InitializeDevice(TDE_Device* dev)
{
	// Pixel Format and GetDC
	dev->MainCamera = new TDE_Camera();
	dev->PixelFormat = new TDE_PixelFormat();
	if (dev->WHandle != NULL)
	{
		dev->WHDC = GetDC(dev->WHandle);
		dev->PixelFormat->SetHDCPixelFormat(&dev->WHDC);
		//
		RECT rc;
		GetClientRect(dev->WHandle, &rc);
		_Resize(rc.right, rc.bottom, dev);
		dev->W = rc.right;
		dev->H = rc.bottom;
	}
	// Create Context
	dev->WHRC = wglCreateContext(dev->WHDC);
	dev->MakeCurrent();
	//
	glEnable ( GL_TEXTURE_2D );
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Face Culling
	glEnable(GL_CULL_FACE);  
	glCullFace(GL_FRONT);
	// Prepare BackBuffer (Screen Frame Buffer)

}

VOID _FillZero(TDE_Device* dev)
{
	dev->WHandle = NULL;
	dev->WHDC = NULL;
	dev->WHRC = NULL;
	dev->PixelFormat = NULL;
	dev->_TexIDCounter = 0;
}

VOID _Resize(INT w, INT h, TDE_Device* dev)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	GLfloat aspect = (GLfloat)w / (GLfloat)h;
	//gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	if (w >= h) 
	{
	   //gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
	} 
	else 
	{
	   //gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
	}
	//
	dev->MainCamera->AspectRatio = w / h;
	if (dev->MainCamera->CameraMode == TDE_CAMERA_ORTHOGRAPHIC)
	{
	    glOrtho(0, w, h, 0, 0.0, 10.0);
	}
	else if (dev->MainCamera->CameraMode == TDE_CAMERA_PERSPECTIVE)
	{
		gluPerspective(dev->MainCamera->FOV, w / h, dev->MainCamera->Near, dev->MainCamera->Far);
	}
	//
	glMatrixMode(GL_MODELVIEW);
	//
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	//
}

TDE_Device::TDE_Device()
{
	ID = TDE_GetID();
	_FillZero(this);
	glewInit();
}

TDE_Device::TDE_Device(HWND hwnd)
{
	ID = TDE_GetID();
	_FillZero(this);
	glewInit();
	//
	WHandle = hwnd;
	//
	_InitializeDevice(this);
}

VOID TDE_Device::MakeCurrent()
{
	wglMakeCurrent(WHDC, WHRC);
}

VOID TDE_Device::Initialize(HWND hwnd)
{
	WHandle = hwnd;
	_InitializeDevice(this);
}

VOID TDE_Device::Disable()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(WHRC);
	ReleaseDC(WHandle, WHDC);
}

VOID TDE_Device::UpdatePixelFormat()
{
	PixelFormat->SetHDCPixelFormat(&WHDC);
}

TDE_Transform* TDE_Device::GetTransform()
{
	return new TDE_Transform(this);
}

VOID TDE_Device::Clear(FLOAT r, FLOAT g, FLOAT b)
{
	glClearColor(r, g, b, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

VOID TDE_Device::Clear(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
	glClearColor(r, g, b, a);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

VOID TDE_Device::ClearI(INT r, INT g, INT b)
{
	glClearColor(((FLOAT)r + 0.00000) / 255, ((FLOAT)g + 0.00000) / 255, ((FLOAT)b + 0.00000) / 255, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

VOID TDE_Device::ClearI(INT r, INT g, INT b, INT a)
{
	glClearColor(((FLOAT)r + 0.00000) / 255, ((FLOAT)g + 0.00000) / 255, ((FLOAT)b + 0.00000) / 255, ((FLOAT)a + 0.00000) / 255);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

VOID TDE_Device::SetCamera()
{
	RECT rc;
	GetClientRect(WHandle, &rc);
	_Resize(rc.right, rc.bottom, this);
}

VOID TDE_Device::SetCamera(TDE_Camera* cam)
{
	RECT rc;
	GetClientRect(WHandle, &rc);
	//
	MainCamera = cam;
	//
	_Resize(rc.right, rc.bottom, this);
}

VOID TDE_Device::Begin()
{
	MakeCurrent();
	/////////
	// Prepare BackBuffer
	
	/////////
	//glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (MainCamera->CameraMode == TDE_CAMERA_ORTHOGRAPHIC)
	{
	    glTranslatef(-1.0, 1.0, 0.0);
	}
	else if (MainCamera->CameraMode == TDE_CAMERA_PERSPECTIVE)
	{
		gluLookAt(MainCamera->CameraPosition->x, MainCamera->CameraPosition->y, MainCamera->CameraPosition->z,
			MainCamera->LookAt->x, MainCamera->LookAt->y, MainCamera->LookAt->z,
			MainCamera->UpVector->x, MainCamera->UpVector->y, MainCamera->UpVector->z);
	}
	//glDisable(GL_LIGHTING);
	//glDisable(GL_LIGHT0);
}

VOID TDE_Device::End()
{
	//glPopMatrix();
}

VOID TDE_Device::Resize()
{
	RECT rc;
	GetClientRect(WHandle, &rc);
	_Resize(rc.right, rc.bottom, this);
}

VOID TDE_Device::Resize(INT w, INT h)
{
	_Resize(w, h, this);
	W = w;
	H = h;
}

VOID TDE_Device::Flip()
{
    SwapBuffers(WHDC);
}

VOID TDE_Device::DrawModel(TDE_Model* model)
{
	if (model->UseGLListIs == true)
	{
		glCallList(model->GLListId);
	}
	else
	{
		MessageBox(NULL, L"Error: Currently is only possible draw using ID", L"Error", MB_OK);
	}
}

VOID TDE_Device::DrawModel(TDE_Model* model, TDE_Coordinate* position)
{
	if (model->UseGLListIs == true)
	{
		//glPushMatrix();
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		glTranslatef(position->x, position->y, position->z);
		glCallList(model->GLListId);
		//glPopMatrix();
	}
	else
	{
		MessageBox(NULL, L"Error: Currently is only possible draw using ID", L"Error", MB_OK);
	}
}

VOID TDE_Device::DrawImage(TDE_Image* img)
{
	// Old Code
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, img->Index);
	//glBegin(GL_QUADS);
	//  glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, 0.0);
	//  glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
	//  glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
	//  glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, 0.0);
	//glEnd();
	//glDisable(GL_TEXTURE_2D);
	///////////////////////////////////////////
	// New Code
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, img->Index);
	glBegin(GL_QUADS);
	  //glTexCoord2f(img->ImageCoord[0][0], img->ImageCoord[0][1]); glVertex3f(nx, ny, 0.0);
	  //glTexCoord2f(img->ImageCoord[1][0], img->ImageCoord[1][1]); glVertex3f(nx + nw, ny, 0.0);
	  //glTexCoord2f(img->ImageCoord[2][0], img->ImageCoord[2][1]); glVertex3f(nx + nw, ny - nh, 0.0);
	  //glTexCoord2f(img->ImageCoord[3][0], img->ImageCoord[3][1]); glVertex3f(nx, ny - nh, 0.0);
	  //
	  //glTexCoord2d(img->ImageCoord[3][0], img->ImageCoord[3][1]); glVertex3f(nx, ny, 0.0);
	  //glTexCoord2d(img->ImageCoord[2][0], img->ImageCoord[2][1]); glVertex3f(nx + nw, ny, 0.0);
	  //glTexCoord2d(img->ImageCoord[1][0], img->ImageCoord[1][1]); glVertex3f(nx + nw, ny - nh, 0.0);
	  //glTexCoord2d(img->ImageCoord[0][0], img->ImageCoord[0][1]); glVertex3f(nx, ny - nh, 0.0);
	  //
	  glTexCoord2d(img->ImageCoord[3][0], img->ImageCoord[3][1]); glVertex3f(img->SurfaceCoord[0][0], img->SurfaceCoord[0][1], img->SurfaceCoord[0][2]);
	  glTexCoord2d(img->ImageCoord[2][0], img->ImageCoord[2][1]); glVertex3f(img->SurfaceCoord[1][0], img->SurfaceCoord[1][1], img->SurfaceCoord[1][2]);
	  glTexCoord2d(img->ImageCoord[1][0], img->ImageCoord[1][1]); glVertex3f(img->SurfaceCoord[2][0], img->SurfaceCoord[2][1], img->SurfaceCoord[2][2]);
	  glTexCoord2d(img->ImageCoord[0][0], img->ImageCoord[0][1]); glVertex3f(img->SurfaceCoord[3][0], img->SurfaceCoord[3][1], img->SurfaceCoord[3][2]);
	  //glTexCoord2d(
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

VOID TDE_Device::DrawImage(TDE_Image* img, FLOAT x, FLOAT y)
{
	img->CreateSurfaceMatrix(this, x, y);
	//
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, img->Index);
	glBegin(GL_QUADS);
	  //
	  glTexCoord2d(img->ImageCoord[3][0], img->ImageCoord[3][1]); glVertex3f(img->SurfaceCoord[0][0], img->SurfaceCoord[0][1], img->SurfaceCoord[0][2]);
	  glTexCoord2d(img->ImageCoord[2][0], img->ImageCoord[2][1]); glVertex3f(img->SurfaceCoord[1][0], img->SurfaceCoord[1][1], img->SurfaceCoord[1][2]);
	  glTexCoord2d(img->ImageCoord[1][0], img->ImageCoord[1][1]); glVertex3f(img->SurfaceCoord[2][0], img->SurfaceCoord[2][1], img->SurfaceCoord[2][2]);
	  glTexCoord2d(img->ImageCoord[0][0], img->ImageCoord[0][1]); glVertex3f(img->SurfaceCoord[3][0], img->SurfaceCoord[3][1], img->SurfaceCoord[3][2]);
	  //glTexCoord2d(
	glEnd();
	glDisable(GL_TEXTURE_2D);

}
