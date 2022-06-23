// try.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "try.h"

#pragma comment(lib, "FDTechDur Encrid Runtime 1.0.lib")

#define TDE_WINDOW
#include <Encrid.h>

TDE_Device* dev;

TDELoop loopA(TDELoopArgs);
VOID RenderLoop();

void Init(HWND hwnd);
void Update();

BOOL initialized = FALSE;

INT TDMain()
{
	TDE_Window* form1 = new TDE_Window();
	form1->CreateWindowClass(L"WINDOW0", loopA);
	form1->CreateWindowHandle(L"Game", 10, 10, 800, 600);

	

	Init(form1->HandleWindow);


	return 0;
}

TDELoop loopA(TDELoopArgs)
{

	if (msg == WM_CREATE)
	{
		SetTimer(hwnd, 1, 10, NULL);
	}

	if (msg == WM_TIMER && initialized )
	{
		Update();
	}

	if (msg == WM_CLOSE)
	{
		PostQuitMessage(0);
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

TDE_Bitmap* bmp = NULL;
TDE_JPEG* jp = NULL;

TDE_Room* r = NULL;
TDE_Input *keyb = NULL;
TDE_Input *joy = NULL;

TDE_WavefrontLoader* monkey;
TDE_WavefrontLoader* terrain;

TDE_GLSL *light;
TDE_GLSLManager SM;

TDE_Model* plane;

void Init(HWND hwnd)
{
	dev = new TDE_Device(hwnd);
	bmp = new TDE_Bitmap();
	//bmp->LoadFromFile(dev, "C:\\A\\A.bmp");
	//
	jp = new TDE_JPEG();
	//jp->LoadFromFile(dev, "C:\\A\\A.jpg");

	keyb = new TDE_Input();
	keyb->CreateKeyboardDevice(hwnd);

	joy = new TDE_Input();
	vector<TDE_Joystick*> joys = joy->GetJoystickDevices();
	//vector<TDE_Joystick*> joys;
	if (joys.size() == 0)
	{
		MessageBox(NULL, L"No Joysticks Found!", L"", MB_OK);
	}
	else
	{
		MessageBox(NULL, joys[0]->ProductName, L"", MB_OK);
		joy->CreateJoystickDevice(hwnd, joys[0]);
	}

	WCHAR sz[20];
	//swprintf_s(sz, L"%d", TextureCounter);
	//MessageBox(NULL, sz, L"E", MB_OK);
	for (int C = 0; C < 3; C++)
	{
		WCHAR txt[80];
		//swprintf_s(txt, L"%d - %d", C, TexturesIDs[C]);
		//MessageBox(NULL, txt, L"Index - Val", MB_OK);
	}

	monkey = new TDE_WavefrontLoader();
	monkey->Load(dev, "C:\\Projetos\\Encrid\\try\\Debug\\cube.obj");

	terrain = new TDE_WavefrontLoader();
	terrain->Load(dev, "C:\\Projetos\\Encrid\\try\\Debug\\shadTest.obj");

	TDE_GameInfo* gi = new TDE_GameInfo();
	gi->DirGameObjects = L"GameObjects\\";
	gi->DirSprites = L"Sprites\\";
	//
	r = new TDE_Room(gi, dev);
    r->LoadFromFile("C:\\Projetos\\Encrid\\try\\Debug\\Rooms\\principal.room");

	//light = SM.loadfromFile("C:\\A\\fp0.frag.vert", "C:\\A\\fp0.frag.frag");
	light = SM.loadfromFile("C:\\A\\EncridShader.vert", "C:\\A\\EncridShader.frag");

	plane = TDE_Shapes::CreatePlane(10.0, 10.0);
	initialized = true;
}

void Update()
{
	TDE_KeyboardState* state = keyb->GetKeyboardState();
	TDE_JoystickState* joyState = NULL;
	if (joy->Created)
	{
		joyState = joy->GetJoystickState();
	}

	dev->Clear(0.0f, 0.0f, 1.0f);

	static float angle = 0.000f;
	static float x = 5.000f;
	static float y = 1.500f;
	static float z = -5.000f;

	if (state->IsPressed(DIK_A))
	{
		angle += 0.01f;
	}
	if (state->IsPressed(DIK_D))
	{
		angle -= 0.01f;
	}

	if (state->IsPressed(DIK_UP))
	{
		y += 0.01f;
	}
	if (state->IsPressed(DIK_DOWN))
	{
		y -= 0.01f;
	}

    float lx = x + (sin(angle) * 150);
	float ly = 0;
	float lz = z + (cos(angle) * 150);

	if (state->IsPressed(DIK_W))
	{
		x += sin(angle) / 50.0f;
		z += cos(angle) / 50.0f;
	}
	if (state->IsPressed(DIK_S))
	{
		x -= sin(angle) / 50.0f;
		z -= cos(angle) / 50.0f;
	}

    //dev->MainCamera->CameraMode = TDE_CAMERA_PERSPECTIVE;
	//dev->MainCamera->CameraPosition = new TDE_Coordinate(x, y, z);
	//dev->MainCamera->LookAt = new TDE_Coordinate(lx, ly, lz);
	//dev->SetCamera();
	dev->Begin();

	TDE_GameObject* go = r->GetGameObjectByID(1);
	TDE_GameObject* plat = r->GetGameObjectByID(2);

	if (go != NULL && plat != NULL)
	{

	     TDE_Size sz;
	     sz.SetSize(-(go->GetX() - 300), 0.0f);
    	TDE_Transform* dt = dev->GetTransform();
    	dt->Translate2D(sz);

    	if (state->IsPressed(DIK_R))
		{
			go->SetPosition(10, 10);
			go->GOPhysics->SetYForce(0);
		}

		if (state->IsPressed(DIK_F))
		{
			WCHAR txt[80];
			swprintf_s(txt, L"%d", (int)go->GetX());
			MessageBox(NULL, txt, L"", MB_OK);
		}

	    go->GOPhysics->EnablePhysics();
    	go->GOPhysics->SetYGravity(30, 30);
	    go->GOPhysics->SetMass(30);
	    go->GOPhysics->XForceReductionRatio(80);

		if (state->IsPressed(DIK_LEFT))
		{
  	      //go->X-=5;
			go->GOPhysics->ApplyXForce(-2);
		}
		if (joy->Created)
		{
			if (joyState->lX == -1000)
			{
				go->GOPhysics->ApplyXForce(-2);
			}
			if (joyState->lX == 1000)
			{
				go->GOPhysics->ApplyXForce(2);   
			}
		}
		if (state->IsPressed(DIK_RIGHT))
		{
			//go->X += 5;
			go->GOPhysics->ApplyXForce(2);
		}

	}

	r->Draw();

	TDE_GlobalPhysics* gp = new TDE_GlobalPhysics();
	gp->CollisionPrecision = 50;
	gp->DoPhysics(r->AllGOs);

	if (state->IsPressed(DIK_SPACE))
	{ 
		go->GOPhysics->SetYForce(-30);
		go->GOPhysics->DoPhysics();
		//go->GOPhysics->SetXForce(10);
	}
	if (joy->Created)
	{
	    if (joyState->IsPressed(0))
	    {
			if (go->GOPhysics->ColisionVari(plat->GOPhysics, 1.0f))
			{
	    	    go->GOPhysics->SetYForce(-30);
	    	    go->GOPhysics->DoPhysics();
			}
	    }
	}

	//dev->DrawImage(bmp, 10.0, 10.0);
	//dev->DrawImage(jp, bmp->W + 20.0, 10.0);

	if (false) // (monkey != NULL && light != NULL)
	{

		light->enable();
		light->begin();

		if (light->setUniform3f("mambient", 0.2, 0.2, 0.2, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
		if (light->setUniform3f("mdiffuse", 0.8, 0.8, 0.8, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
		if (light->setUniform3f("mspecular", 1.0, 1.0, 1.0, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
		if (light->setUniform1f("UseTexture", 0, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
	
		if (light->setUniform3f("Lights[0].lambient", 0.2, 0.2, 0.2, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
		if (light->setUniform3f("Lights[0].ldiffuse", 0.8, 0.8, 0.8, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
		if (light->setUniform3f("Lights[0].lspecular", 1.0, 1.0, 1.0, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
		if (light->setUniform3f("Lights[0].lightPos", 0.0, 2.0, 0.0, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
		if (light->setUniform3f("Lights[0].lightDir", 0.0, 1.0, 1.0, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
		
		if (light->setUniform1f("Lights[0].shininess", 2, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
		if (light->setUniform1f("Lights[0].Enable", 1, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
		if (light->setUniform1f("Lights[0].type", 0, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
		//light->setUniform1i("NumLight", 50, 0);

		TDE_Transform* transf = dev->GetTransform();
		transf->Rotate3D(90, TDE_Coordinate(1, 0, 0));

		TDE_Coordinate* coord = new TDE_Coordinate(0.0000f, 0.0000f, 0.0000f);

		dev->DrawModel(plane, coord);
		//dev->DrawModel(terrain, coord);


		light->end();

		light->enable();
		light->begin();

		if (light->setUniform3f("mambient", 0.5, 0.5, 0.5, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
		if (light->setUniform3f("mdiffuse", 1.0, 1.0, 1.0, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
		if (light->setUniform3f("mspecular", 1.0, 1.0, 1.0, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
		if (light->setUniform1f("UseTexture", 0, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
	
		if (light->setUniform3f("Lights[0].lambient", 0.5, 0.5, 0.5, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
		if (light->setUniform3f("Lights[0].ldiffuse", 1.0, 1.0, 1.0, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
		if (light->setUniform3f("Lights[0].lspecular", 1.0, 1.0, 1.0, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
		if (light->setUniform3f("Lights[0].lightPos", 4.0, 0.5, -4.0, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
		
		if (light->setUniform1f("Lights[0].shininess", 25, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
		if (light->setUniform1f("Lights[0].Enable", 1, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
		if (light->setUniform1f("Lights[0].type", 0, 0) == false) MessageBox(0, L"ERR", L"ERR", 0);
		//light->setUniform1i("NumLight", 50, 0);

		//dev->DrawModel(terrain, coord);

		light->end();

	}
	
	
	dev->End();
	dev->Flip();

}
