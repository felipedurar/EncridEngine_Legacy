
#include <iostream>
#include <vector>

// DX
#include <dinput.h>

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#define DEVELOPMENT
#include "EncridInput.h"
#include "EncridIDGen.h"

#ifndef _WINDOWS_
#include <Windows.h>
#endif

using namespace std;

// DI Core
LPDIRECTINPUT8 fDI = NULL;
class _TD_DI_Core
{
public:
    LPDIRECTINPUTDEVICE8 fDIDev;
	int id;
};

vector<_TD_DI_Core> AllDICores;

INT GetDICoreIndexByID(int id)
{
	for (int C = 0; C < AllDICores.size(); C++)
	{
		if (AllDICores[C].id == id)
		{
			return C;
		}
	}
	return -1;
}

VOID _TD_InitDirectInput()
{
	if (fDI != NULL) return;
	//
	DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&fDI,NULL);
    if (fDI == NULL)
    {
        MessageBox(NULL, L"DirectInput Connection Creation Failed!", L"ERROR", MB_OK);
        return ;
    }
}

// TDE_Input
TDE_Input::TDE_Input()
{
	Created = FALSE;
	_TD_InitDirectInput(); 
}
VOID TDE_Input::UpdateInternalIndex()
{
	internalIndex = GetDICoreIndexByID(ID);
}
VOID TDE_Input::CreateKeyboardDevice(HWND hwnd)
{
	ID = TDE_GetID();
	//
	_TD_DI_Core diC;
	diC.id = ID;
	//
	// Create the connection to the keyboard device
	fDI->CreateDevice(GUID_SysKeyboard, &diC.fDIDev, NULL);
	if (diC.fDIDev)
	{
		diC.fDIDev->SetDataFormat(&c_dfDIKeyboard);
		diC.fDIDev->SetCooperativeLevel(hwnd,DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		diC.fDIDev->Acquire();
	}
	else
	{
		MessageBox(NULL, L"DirectInput Keyboard initialization Failed!", L"Error", MB_OK);
		return ;
	}
	//
	AllDICores.push_back(diC);
	//
	internalIndex = GetDICoreIndexByID(ID);
	Created = true;
}
TDE_KeyboardState* TDE_Input::GetKeyboardState()
{
	TDE_KeyboardState* ks = new TDE_KeyboardState();
	//
	HRESULT hr;
	// get the keyboard state
	hr = AllDICores[internalIndex].fDIDev->GetDeviceState(sizeof(ks->fDIKeyboardState),(LPVOID)&ks->fDIKeyboardState);
	if (FAILED(hr))
	{
		AllDICores[internalIndex].fDIDev->Acquire();
    }
	//
	return ks;
}
//
_TD_DI_Core* currentUsedDICore;
vector<TDE_Joystick*> tmpJoysticks;
BOOL CALLBACK enumCallBack(const DIDEVICEINSTANCE* instance, VOID* conntext)
{
	TDE_Joystick* joystick = new TDE_Joystick();
	wcscpy(joystick->ProductName, instance->tszProductName);
	wcscpy(joystick->InstanceName, instance->tszInstanceName);
	joystick->guidProduct = instance->guidProduct;
	joystick->guidInstance = instance->guidInstance;
	joystick->guidFFDriver = instance->guidFFDriver;
	joystick->IsValid = TRUE;
	tmpJoysticks.push_back(joystick);
	return DIENUM_CONTINUE;
}
BOOL CALLBACK enumAxesCallback(const DIDEVICEOBJECTINSTANCE* instance, VOID* context)
{
	HWND hDlg = (HWND)context;
	//
	DIPROPRANGE propRange;
	propRange.diph.dwSize = sizeof(DIPROPRANGE);
	propRange.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	propRange.diph.dwHow = DIPH_BYID;
	propRange.diph.dwObj = instance->dwType;
	propRange.lMin = -1000;
	propRange.lMax = +1000;
	//
	if (FAILED(currentUsedDICore->fDIDev->SetProperty(DIPROP_RANGE, &propRange.diph)))
	{
		return DIENUM_STOP;
	}
	return DIENUM_CONTINUE;
}
vector<TDE_Joystick*> TDE_Input::GetJoystickDevices()
{
	tmpJoysticks.clear();
	//
	HRESULT hr;
	if (FAILED(hr = fDI->EnumDevices(DI8DEVCLASS_GAMECTRL, enumCallBack, NULL, DIEDFL_ATTACHEDONLY)))
	{
		// ERROR
	}
	//
	return tmpJoysticks;
}
VOID TDE_Input::CreateJoystickDevice(HWND hwnd, TDE_Joystick* joy)
{
	if (joy == NULL) return;
	//
	ID = TDE_GetID();
	//
	_TD_DI_Core diC;
	diC.id = ID;
	//
	HRESULT hr;
	if (FAILED(hr = fDI->CreateDevice(joy->guidInstance, &diC.fDIDev, NULL)))
	{
		// ERROR
	}
	//
	DIDEVCAPS capabilities;
	if (FAILED(hr = diC.fDIDev->SetDataFormat(&c_dfDIJoystick2)))
	{
		// ERROR
	}
	if (FAILED(hr = diC.fDIDev->SetCooperativeLevel(NULL, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
	{
		// ERROR
	}
	capabilities.dwSize = sizeof(DIDEVCAPS);
	if (FAILED(hr = diC.fDIDev->GetCapabilities(&capabilities)))
	{
		// ERROR
	}
	// Enum Axes
	currentUsedDICore = &diC;
	if (FAILED(hr = diC.fDIDev->EnumObjects(enumAxesCallback, NULL, DIDFT_AXIS)))
	{
		// ERROR
	}
	AllDICores.push_back(diC);
	Created = true;
}
TDE_JoystickState* TDE_Input::GetJoystickState()
{
	HRESULT hr;
	if (AllDICores[GetDICoreIndexByID(ID)].fDIDev == NULL)
	{
		return NULL;
	}
	//
	hr = AllDICores[GetDICoreIndexByID(ID)].fDIDev->Poll();
	if (FAILED(hr))
	{
		hr = AllDICores[GetDICoreIndexByID(ID)].fDIDev->Acquire();
		while (hr == DIERR_INPUTLOST)
		{
			hr = AllDICores[GetDICoreIndexByID(ID)].fDIDev->Acquire();
		}
		//
		if ((hr == DIERR_INVALIDPARAM) || (hr == DIERR_NOTINITIALIZED))
		{
			// ERROR
		}
		if (hr == DIERR_OTHERAPPHASPRIO)
		{
			// OK
		}
	}
	//
	DIJOYSTATE2 js;
	TDE_JoystickState* joystate = new TDE_JoystickState();
	if (FAILED(hr = AllDICores[GetDICoreIndexByID(ID)].fDIDev->GetDeviceState(sizeof(DIJOYSTATE2), &js)))
	{
		// ret HR
	}
	//
    joystate->lX = js.lX;
    joystate->lY = js.lY;
    joystate->lZ = js.lZ;
	joystate->lRx = js.lRx;
	joystate->lRy = js.lRy;
	joystate->lRz = js.lRz;
	// Slider [2]
	joystate->rglSlider[0] = js.rglSlider[0];
	joystate->rglSlider[1] = js.rglSlider[1];
	// POV [4]
	joystate->rgdwPOV[0] = js.rgdwPOV[0];
	joystate->rgdwPOV[1] = js.rgdwPOV[1];
	joystate->rgdwPOV[2] = js.rgdwPOV[2];
	joystate->rgdwPOV[3] = js.rgdwPOV[3];
	// Buttons [128]
	for (int C = 0; C < 128; C++)
	{
		joystate->rgbButtons[C] = js.rgbButtons[C];
	}
	//
	joystate->lVX = js.lVX;
    joystate->lVY = js.lVY;
    joystate->lVZ = js.lVZ;
	joystate->lVRx = js.lVRx;
	joystate->lVRy = js.lVRy;
	joystate->lVRz = js.lVRz;
	// VSlider [2]
	joystate->rglVSlider[0] = js.rglVSlider[0];
	joystate->rglVSlider[1] = js.rglVSlider[1];
	//
    joystate->lAX = js.lAX;
    joystate->lAY = js.lAY;
    joystate->lAZ = js.lAZ;
    joystate->lARx = js.lARx;
    joystate->lARy = js.lARy;
    joystate->lARz = js.lARz;
	// ASlider [2]
	joystate->rglASlider[0] = js.rglASlider[0];
	joystate->rglASlider[1] = js.rglASlider[1];
	//
    joystate->lFX = js.lFX;
    joystate->lFY = js.lFY;
    joystate->lFZ = js.lFZ;
    joystate->lFRx = js.lFRx;
    joystate->lFRy = js.lFRy;
    joystate->lFRz = js.lFRz;
	// FSlider [2]
	joystate->rglFSlider[0] = js.rglFSlider[0];
	joystate->rglFSlider[1] = js.rglFSlider[1];
	//
	return joystate;
}

// TDE_JoystickState
BOOL TDE_JoystickState::IsPressed(int button)
{
	if( rgbButtons[button] & 0x80 )
	{
		return true;
	}
	return false;
}
vector<INT> TDE_JoystickState::GetPressedKeys()
{
	vector<INT> keys;
	//
	for (int C = 0; C > 128; C++)
	{
		if( rgbButtons[C] & 0x80 )
		{
			keys.push_back(C);
		}
	}
	//
	return keys;
}

// TDE_KeyboardState
#define KEYDOWN(name, key) (name[key] & 0x80)
BOOL TDE_KeyboardState::IsPressed(int k)
{
	if (KEYDOWN(fDIKeyboardState, k))
	{
		return TRUE;
	}
	return FALSE;
}
