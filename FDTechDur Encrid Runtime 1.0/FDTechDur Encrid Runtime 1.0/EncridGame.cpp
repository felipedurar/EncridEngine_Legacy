
#pragma once

#include <Windows.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define DEVELOPMENT
#include "EncridGame.h"
#include "EncridImage.h"
#include "EncridPosSize.h"
#include "Encrid.h"

using namespace std;

// String Tools
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) 
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
std::vector<std::string> split(const std::string &s, char delim) 
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

wstring ExePath() 
{
	// Get Exe Path
    WCHAR* buffer = new WCHAR[MAX_PATH];
    DWORD len = GetModuleFileName(NULL, buffer, MAX_PATH );
	// Get only the directory
	wstring ws(buffer, len);
	const size_t last_slash_idx = ws.rfind('\\');
	if (std::string::npos != last_slash_idx)
	{
		ws.erase(last_slash_idx);
	    ws = ws.substr(0, last_slash_idx);
	}
	delete[] buffer;
	return ws;
}

//

TDE_GameInfo::TDE_GameInfo()
{
	DirGameObjects = new WCHAR[100];
	DirSprites = new WCHAR[100];
	DirRooms = new WCHAR[100];
	wcsncpy (DirGameObjects, TDE_DEFAULTFOLDERW, 100);
	wcsncpy (DirSprites, TDE_DEFAULTFOLDERW, 100);
	wcsncpy (DirRooms, TDE_DEFAULTFOLDERW, 100);
}

BOOL CompareTF(string nc)
{
	if (nc.compare("1") == 0)
	{
		return true;
	}
	return false;
}

//

#define DEVELOPMENT
#include "EncridPhysics.h"

TDE_GameObject::TDE_GameObject()
{
	gameInfo = new TDE_GameInfo();
	_MInit = true;
	GOPhysicsVals = new TDE_GO_PHYSICS_VALUES();
	GOPhysics = new TDE_GOPhysics(this);
}

VOID TDE_GameObject::Initialize(TDE_GameInfo* gi, TDE_Device* dev)
{
	gameInfo = gi;
	device = dev;
	_MInit = true;
	GOPhysicsVals = new TDE_GO_PHYSICS_VALUES();
	GOPhysics = new TDE_GOPhysics(this);
}

BOOL TDE_GameObject::MInit()
{
	bool bw = _MInit;
	_MInit = false;
	return bw;
}

//

BOOL TDE_GameObject::SpriteExistsByID(INT id)
{
	if (GetSpriteIndexByID(id) != -1) 
	{
		return true;
	}
	return false;
}

INT TDE_GameObject::GetSpriteIndexByID(INT id)
{
	for (int C = 0; C < Sprites.size(); C++)
	{
		if (Sprites[C]->IdL == id)
		{
			return C;
		}
	}
	return -1;
}

BOOL TDE_GameObject::AnimationExistsByID(INT id)
{
	if (GetAnimationIndexByID(id) != -1) 
	{
		return true;
	}
	return false;
}

INT TDE_GameObject::GetAnimationIndexByID(INT id)
{
	for (int C = 0; C < Animations.size(); C++)
	{
		if (Animations[C]->IdL == id)
		{
			return C;
		}
	}
	return -1;
}

//

#define BUFFER_SIZE 300
INT TDE_GameObject::LoadFromFile(char* fileName)
{
	wchar_t* completeDir = new wchar_t[4096];
	wchar_t* fn = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, fileName, -1, fn, 4096);
	swprintf(completeDir, L"%s\\%s%s", ExePath().c_str(), gameInfo->DirGameObjects, fn);
	//
	std::ifstream infile(completeDir);
	//
	if (!infile.good())
	{
		MessageBox(NULL, L"Error, GameObject file doesn't exists", L"Error", MB_OK);
		return 1;
	}
	//
    std::string line;
    while (std::getline(infile, line))
    {
        // test if contains '|'
		if (line.find_first_of("=") != string::npos)
		{
			vector<string> splParts = split(line, '=');
			if (splParts[0].compare("NAME") == 0) // Equal
			{
				// Create a Game Object
				wstring wcsc = wstring(splParts[1].begin(), splParts[1].end());
				wchar_t* stw = new wchar_t[100];
				wcscpy(stw, wcsc.c_str());
				GOName = stw;
			}
			if (splParts[0].compare("VISIBLE") == 0) // Equal
			{
				Visible = CompareTF(splParts[1]);
			}
			if (splParts[0].compare("FIXED") == 0) // Equal
			{
				Fixed = CompareTF(splParts[1]);
			}
			if (splParts[0].compare("SOLID") == 0) // Equal
			{
				Solid = CompareTF(splParts[1]);
			}
			if (splParts[0].compare("ENABLEG") == 0) // Equal
			{
				EnableGravity = CompareTF(splParts[1]);
			}
			if (splParts[0].compare("MASS") == 0) // Equal
			{
				Mass = atoi(splParts[1].c_str());
			}
			if (splParts[0].compare("CROP_SPRITE") == 0) // Equal
			{
				vector<string> fanimArgs = split(splParts[1], ':');
				//
				INT spriteId = atoi(fanimArgs[0].c_str());
				INT xPos = atoi(fanimArgs[1].c_str());
				INT yPos = atoi(fanimArgs[2].c_str());
				INT w = atoi(fanimArgs[3].c_str());
				INT h = atoi(fanimArgs[4].c_str());
				//
				if (SpriteExistsByID(spriteId) == TRUE)
				{
					Sprites[GetSpriteIndexByID(spriteId)]->Crop(xPos, yPos, w, h);
				} 
				else
				{
					MessageBox(NULL, L"This Sprite id doesn't exists!", L"Error", NULL);
				}
			}
			if (splParts[0].compare("CURRENT_SPRITE") == 0) // Equal
			{
				INT spriteId = atoi(splParts[1].c_str());
				if (SpriteExistsByID(spriteId) == true)
				{
					Sprite = Sprites[GetSpriteIndexByID(spriteId)];
				}
			}
			if (splParts[0].compare("PUSH_ANIM_FRAME") == 0) // Equal
			{
				vector<string> fanimArgs = split(splParts[1], ':');
				if (fanimArgs.size() != 7)
				{
					MessageBox(NULL, L"Error, invalid animation data!", L"Error", MB_OK);
					//return 1;
				}
				//
				INT animId = atoi(fanimArgs[0].c_str());
				INT animMode = atoi(fanimArgs[1].c_str());
				INT spriteId = atoi(fanimArgs[2].c_str());
				INT xPos = atoi(fanimArgs[3].c_str());
				INT yPos = atoi(fanimArgs[4].c_str());
				INT w = atoi(fanimArgs[5].c_str());
				INT h = atoi(fanimArgs[6].c_str());
				//
				if (AnimationExistsByID(animId) == FALSE)
				{
					TDE_2DAnimation* animTmp = new TDE_2DAnimation();
					animTmp->IdL = animId;
					Animations.push_back(animTmp);
				}
				//
				if (animMode == 0) // CropImage Mode
				{
					if (SpriteExistsByID(spriteId) == TRUE)
					{
						TDE_Image* frameImg = new TDE_Image(Sprites[GetSpriteIndexByID(spriteId)]);
						frameImg->Crop(xPos, yPos, w, h);
						Animations[GetAnimationIndexByID(animId)]->PushFrame(frameImg);
					}
					else
					{
						MessageBox(NULL, L"Error: Sprite id doesn't exists!", L"Error", MB_OK);
					}
				}
				else if (animMode == 1)
				{
					if (SpriteExistsByID(spriteId) == TRUE)
					{
						TDE_Image* frameImg = new TDE_Image(Sprites[GetSpriteIndexByID(spriteId)]);
						Animations[GetAnimationIndexByID(animId)]->PushFrame(frameImg);
					}
					else
					{
						MessageBox(NULL, L"Error: Sprite id doesn't exists!", L"Error", MB_OK);
					}
				}
				else
				{
					MessageBox(NULL, L"Error: invalid animation mode!", L"Error", MB_OK);
				}

			}
			if (splParts[0].compare("SPRITE") == 0) // Equal
			{
				vector<string> fnid = split(splParts[1], ':');
				wstring sprName(fnid[0].begin(), fnid[0].end());
				//wstring sprName(splParts[1].begin(), splParts[1].end());
				wchar_t* nsprd = new wchar_t[100];
				swprintf(nsprd, L"%s\\%s%s", ExePath().c_str(), gameInfo->DirSprites, sprName.c_str());
				SpriteDirectory = nsprd;
				//=
				std::ifstream testLI(completeDir);
				if (testLI.good())
				{
                    char buffer[150];
                    wcstombs ( buffer, nsprd, sizeof(buffer) );
					//
					//Sprite = new TDE_Image();
					//Sprite->LoadImageFromFile(device, buffer);
					TDE_Image* SpriteLoad = new TDE_Image();
					SpriteLoad->LoadImageFromFile(device, buffer);
					SpriteLoad->IdL = atoi(fnid[1].c_str());
					Sprites.push_back(SpriteLoad);
				}
				else
				{
					MessageBox(NULL, L"Error, sprite file doesn't exists", L"Error", MB_OK);
				}
				delete[] nsprd;
			}
		}

    }
	infile.close();
	//
	delete[] completeDir;
	delete[] fn;
	return 0;
}

VOID TDE_GameObject::_SetPosNC(FLOAT xP, FLOAT yP)
{
	X = xP;
	Y = yP;
}

VOID TDE_GameObject::SetPosition(FLOAT xP, FLOAT yP)
{
	// Last Pos
	GOPhysicsVals->_XLP = X;
	GOPhysicsVals->_YLP = Y;
	// Reset
	GOPhysicsVals->_XLD = 0;
	GOPhysicsVals->_YLD = 0;
	// X
	if (xP > X)
	{
		GOPhysicsVals->_XLD = 1;
	}
	else if (xP < X)
	{
		GOPhysicsVals->_XLD = -1;
	}
	else if (xP == X)
	{
		GOPhysicsVals->_XLD = 0;
	}
	// Y
	if (yP > Y)
	{
		GOPhysicsVals->_YLD = 1;
	}
	else if (yP < Y)
	{
		GOPhysicsVals->_YLD = -1;
	}
	else if (yP == Y)
	{
		GOPhysicsVals->_YLD = 0;
	}
	// Set Vals
	X = xP;
	Y = yP;
}

TDE_Point TDE_GameObject::GetPosition()
{
	TDE_Point p;// = TDE_GetPointF(X, Y);
	p.SetPoint(X, Y);
	return p;
}

FLOAT TDE_GameObject::GetX()
{
	return X;
}

FLOAT TDE_GameObject::GetY()
{
	return Y;
}

//

TDE_Room::TDE_Room()
{
	gameInfo = new TDE_GameInfo();
}

TDE_Room::TDE_Room(TDE_GameInfo* gi, TDE_Device* dev)
{
	gameInfo = gi;
	device = dev;
}

VOID TDE_Room::Initialize(TDE_GameInfo* gi, TDE_Device* dev)
{
	gameInfo = gi;
	device = dev;
}

INT TDE_Room::LoadFromFile(char* fileName)
{
	//
	std::ifstream infile(fileName);
	//
	int idgen = 0;
	//
    std::string line;
    while (std::getline(infile, line))
    {
        // test if contains '|'
		if (line.find_first_of("|") != string::npos)
		{
			vector<string> splParts = split(line, '|');
			if (splParts[0].compare("ADDGO") == 0) // Equal
			{
				// Create a Game Object
				TDE_GameObject* go = new TDE_GameObject();
				go->device = device;
				// Second part; Splits ':'
				if (line.find_first_of(":") != string::npos)
				{
					vector<string> fnp = split(splParts[1], ':');
					// 0 = fileName; 1 = posX; 2 = posY
					go->gameInfo = gameInfo;
					go->LoadFromFile(const_cast<char*>(fnp[0].c_str()));
					go->_SetPosNC(atoi(fnp[1].c_str()), atoi(fnp[2].c_str()));
					if (fnp.size() > 2)
					{
					    go->GOID = atoi(fnp[3].c_str());
					}
					else
					{
						go->GOID = idgen;
						idgen++;
					}
					//go->GOPosition.SetPoint(atoi(fnp[1].c_str()), atoi(fnp[2].c_str()));
				}
				AllGOs.push_back(go);
			}

		}

    }

	infile.close();
	return 0;
}

VOID TDE_Room::Draw()
{
	if (this == 0x0) return;
	for (int C = 0; C < AllGOs.size(); C++)
	{
		if (AllGOs[C] != NULL)
		{
			if (AllGOs[C]->Visible == true)
			{
			    device->DrawImage(AllGOs[C]->Sprite, AllGOs[C]->GetX(), AllGOs[C]->GetY());
			}
		}
	}
}

TDE_GameObject* TDE_Room::GetGameObjectByID(INT id)
{
	TDE_GameObject* obj = NULL;
	if (this == 0x0) return obj;
	for (int C = 0; C < AllGOs.size(); C++)
	{
		if (AllGOs[C] != NULL)
		{
			if (AllGOs[C]->GOID == id)
			{
				obj = AllGOs[C];
				break;
			}
		}
	}
	return obj;
}

INT TDE_Room::GetGameObjectIndexByID(INT id)
{
	if (this == 0x0) return -1;
	for (int C = 0; C < AllGOs.size(); C++)
	{
		if (AllGOs[C] != NULL)
		{
			if (AllGOs[C]->GOID == id)
			{
				return C;
			}
		}
	}
	return -1;
}
