
#pragma once

#ifdef DEVELOPMENT
#define ENCRID_API __declspec(dllexport)
#else
#define ENCRID_API __declspec(dllimport)
#endif

#ifndef _WINDOWS_
#include <Windows.h>
#endif

#include <iostream>
#include <vector>

using namespace std;

class ENCRID_API TDE_KeyboardState
{
public:
	CHAR fDIKeyboardState[256];
	BOOL IsPressed(int k);
};
typedef TDE_KeyboardState* TDEKeyboardState;

class ENCRID_API TDE_JoystickState // BASED IN DIJOYSTATE2
{
public:
    LONG    lX;                     /* x-axis position              */
    LONG    lY;                     /* y-axis position              */
    LONG    lZ;                     /* z-axis position              */
    LONG    lRx;                    /* x-axis rotation              */
    LONG    lRy;                    /* y-axis rotation              */
    LONG    lRz;                    /* z-axis rotation              */
    LONG    rglSlider[2];           /* extra axes positions         */
    DWORD   rgdwPOV[4];             /* POV directions               */
    BYTE    rgbButtons[128];        /* 128 buttons                  */
    LONG    lVX;                    /* x-axis velocity              */
    LONG    lVY;                    /* y-axis velocity              */
    LONG    lVZ;                    /* z-axis velocity              */
    LONG    lVRx;                   /* x-axis angular velocity      */
    LONG    lVRy;                   /* y-axis angular velocity      */
    LONG    lVRz;                   /* z-axis angular velocity      */
    LONG    rglVSlider[2];          /* extra axes velocities        */
    LONG    lAX;                    /* x-axis acceleration          */
    LONG    lAY;                    /* y-axis acceleration          */
    LONG    lAZ;                    /* z-axis acceleration          */
    LONG    lARx;                   /* x-axis angular acceleration  */
    LONG    lARy;                   /* y-axis angular acceleration  */
    LONG    lARz;                   /* z-axis angular acceleration  */
    LONG    rglASlider[2];          /* extra axes accelerations     */
    LONG    lFX;                    /* x-axis force                 */
    LONG    lFY;                    /* y-axis force                 */
    LONG    lFZ;                    /* z-axis force                 */
    LONG    lFRx;                   /* x-axis torque                */
    LONG    lFRy;                   /* y-axis torque                */
    LONG    lFRz;                   /* z-axis torque                */
    LONG    rglFSlider[2];          /* extra axes forces            */
	//
	BOOL IsPressed(int button);
	vector<INT> GetPressedKeys();
};

class ENCRID_API TDE_Joystick
{
public:
	WCHAR ProductName[260];
	WCHAR InstanceName[260];
	GUID guidInstance;
	GUID guidProduct;
	GUID guidFFDriver;
	BOOL IsValid;
};

class ENCRID_API TDE_Input
{
public:
	INT ID;
	INT internalIndex;
	BOOL Created;
	TDE_Input();
	VOID UpdateInternalIndex();
	VOID CreateKeyboardDevice(HWND hwnd);
	VOID CreateJoystickDevice(HWND hwnd, TDE_Joystick* joy);
	vector<TDE_Joystick*> GetJoystickDevices();
	TDE_JoystickState* GetJoystickState();
	TDE_KeyboardState* GetKeyboardState();

};
