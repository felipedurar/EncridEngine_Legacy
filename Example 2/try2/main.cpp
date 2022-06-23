
#include <string>

#define TDE_WINDOW
#include <Encrid.h>

#pragma comment(lib, "FDTechDur Encrid Runtime 1.0.lib")

// Prototypes
TDELoop GameWindowLoop(TDELoopArgs);
VOID Initialize(HWND hwnd);
VOID Update();

// Variables
BOOL Initialized = FALSE;
TDE_Image* sky = new TDE_Image();
TDE_Image* sonicSpriteSheet = new TDE_Image();
TDE_2DAnimation* sonicWalk = new TDE_2DAnimation();
TDE_Room* Room1;

// Input
TDE_Input* Keyboard;

// Objects
TDE_Device* EncridDevice;

INT TDMain()
{
	// Create the Window
	TDE_Window* GameWindow = new TDE_Window();
	GameWindow->CreateWindowClass(L"WINDOW0", GameWindowLoop);
	GameWindow->CreateWindowHandle(L"Game", 10, 10, 800, 600);
	// Initialize Game
	Initialize(GameWindow->HandleWindow);


	return 0;
}

TDELoop GameWindowLoop(TDELoopArgs)
{
	if (_tdew_proc_msg == WM_CREATE)
	{
		SetTimer(_tdew_proc_hwnd, 1, 10, NULL);
	}
	if (_tdew_proc_msg == WM_TIMER && Initialized)
	{
		Update();
	}
	return TDEW_LoopResult(TDELoopData);
}

VOID Initialize(HWND hwnd)
{
	// Initialize Device
	EncridDevice = new TDE_Device(hwnd);

	sky = new TDE_Image();
	//sky->LoadImageFromFile(EncridDevice, "C:\\Projetos\\Encrid\\try2\\Debug\\Sprites\\sky.bmp");
	sky->LoadImageFromFile(EncridDevice, "C:\\Projetos\\Encrid\\try2\\Debug\\Sprites\\Custom Edited - Sonic Series - Azure Lake.jpg");
	sky->Crop(424, 6, 239, 159);
	sky->RotateMatrixCW();
	sky->RotateMatrixCW();

	sonicSpriteSheet = new TDE_Image();
	sonicSpriteSheet->LoadImageFromFile(EncridDevice, "C:\\Projetos\\Encrid\\try2\\Debug\\Sprites\\spriteSheetSonic.jpg");



	TDE_GameInfo* gi = new TDE_GameInfo();
	gi->DirGameObjects = L"GameObjects\\";
	gi->DirSprites = L"Sprites\\";

	Room1 = new TDE_Room(gi, EncridDevice);
	Room1->LoadFromFile("C:\\Projetos\\Encrid\\try2\\Debug\\Rooms\\Room1.room");
	
	TDE_GameObject* sonic = Room1->GetGameObjectByID(1);
	INT animInd = sonic->GetAnimationIndexByID(0);
	sonic->Animations[animInd]->RotateMatrixCW_AllFrames();
	sonic->Animations[animInd]->RotateMatrixCW_AllFrames();

	for (int C = 0; C < sonic->Animations[animInd]->Frames.size(); C++)
	{
		sonic->Animations[animInd]->Frames[C]->CreateSurfaceMatrix(EncridDevice, 10, 10);
		sonic->Animations[animInd]->Frames[C]->ScaleSize(2.0f);
	}

	TDE_GameObject* block = Room1->GetGameObjectByID(2);
	block->Sprite->RotateMatrixCW();
	block->Sprite->RotateMatrixCW();
	block = Room1->GetGameObjectByID(3);
	block->Sprite->RotateMatrixCW();
	block->Sprite->RotateMatrixCW();
    block = Room1->GetGameObjectByID(4);
	block->Sprite->RotateMatrixCW();
	block->Sprite->RotateMatrixCW();
    block = Room1->GetGameObjectByID(5);
	block->Sprite->RotateMatrixCW();
	block->Sprite->RotateMatrixCW();
    block = Room1->GetGameObjectByID(6);
	block->Sprite->RotateMatrixCW();
	block->Sprite->RotateMatrixCW();
    block = Room1->GetGameObjectByID(7);
	block->Sprite->RotateMatrixCW();
	block->Sprite->RotateMatrixCW();
    block = Room1->GetGameObjectByID(8);
	block->Sprite->RotateMatrixCW();
	block->Sprite->RotateMatrixCW();

	//wstring strTex = L"W = " + std::to_wstring(sky->W) + L" | H = " + std::to_wstring(sky->H);
	//MessageBox(NULL, strTex.c_str(), L"INFO", NULL);

	//

	Keyboard = new TDE_Input();
	Keyboard->CreateKeyboardDevice(hwnd);
	
	// Set Initialized as True
	Initialized = TRUE;
}

VOID Update()
{
	// Clear the screen
	EncridDevice->ClearI(102, 174, 255, 255);

	EncridDevice->Begin();

	// Draw the sky
	for (int c = 0; c < 800; c += sky->W)
	{
		EncridDevice->DrawImage(sky, c, 0);
	}

	TDE_KeyboardState* KeyboardState = Keyboard->GetKeyboardState();

	//sonicWalk->DoStep();
	//EncridDevice->DrawImage(sonicWalk, 10, 100);

	if (Room1 != NULL)
	{
		TDE_GameObject* sonic = Room1->GetGameObjectByID(1);
		INT animInd = sonic->GetAnimationIndexByID(0);
		sonic->Animations[animInd]->DoStep();
		sonic->Animations[animInd]->MaxCounter = 2;
		sonic->Sprite = sonic->Animations[animInd];

		sonic->GOPhysics->EnablePhysics();
    	sonic->GOPhysics->SetYGravity(30, 30);
	    sonic->GOPhysics->SetMass(30);
	    sonic->GOPhysics->XForceReductionRatio(80);

		if (KeyboardState != NULL)
		{
			if (KeyboardState->IsPressed(DIK_SPACE))
			{
				sonic->GOPhysics->ApplyYForce(-3);
			}
		}

		Room1->Draw();

	    TDE_GlobalPhysics* gp = new TDE_GlobalPhysics();
	    gp->CollisionPrecision = 50;
	    gp->DoPhysics(Room1->AllGOs);

	}

	EncridDevice->End();

	// Present in the screen
	EncridDevice->Flip();
}
