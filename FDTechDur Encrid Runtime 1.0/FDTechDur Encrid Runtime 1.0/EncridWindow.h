
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

using namespace std;

class ENCRID_API TDE_Window
{
public:
	HWND HandleWindow;
	WNDCLASSW WindowClass;

	VOID CreateWindowClass(LPCWSTR className, WNDPROC wndProc); // Defined Class
	VOID CreateWindowClass(WNDPROC wndProc);
	//
	VOID CreateWindowHandle(LPCWSTR className, LPCWSTR wTitle, INT x, INT y, INT w, INT h); // Defined Class and Position
	VOID CreateWindowHandle(LPCWSTR wTitle, INT x, INT y, INT w, INT h); // Defined Position
	VOID CreateWindowHandle(LPCWSTR wTitle, INT w, INT h);

};

#define TDELoop LRESULT CALLBACK
#define TDELoopArgs HWND _tdew_proc_hwnd, UINT _tdew_proc_msg, WPARAM _tdew_proc_wParam, LPARAM _tdew_proc_lParam
#define TDELoopData _tdew_proc_hwnd, _tdew_proc_msg, _tdew_proc_wParam, _tdew_proc_lParam

extern "C" ENCRID_API VOID TDEW_StartWindowLoop();
extern "C" ENCRID_API LRESULT TDEW_LoopResult(HWND _tdew_proc_hwnd, UINT _tdew_proc_msg, WPARAM _tdew_proc_wParam, LPARAM _tdew_proc_lParam);
ENCRID_API extern  BOOL TDEW_Running;
ENCRID_API extern  MSG TDEW_MainMessage;
ENCRID_API extern  INT TDEW_LoopDelay;
ENCRID_API extern  UINT TDEW_LoopIgnCycle;
ENCRID_API extern VOID (*TDEW_LoopFunction)();

#ifdef TDE_WINDOW
INT TDMain();
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	TDEW_LoopFunction = NULL;
	//
	INT retVal = TDMain();
	TDEW_StartWindowLoop();
	return retVal;
}
#endif
