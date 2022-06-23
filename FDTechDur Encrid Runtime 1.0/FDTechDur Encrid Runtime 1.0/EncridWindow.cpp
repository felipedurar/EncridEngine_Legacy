
#define DEVELOPMENT
#include "EncridWindow.h"
#include "EncridIDGen.h"

#include <string>

//static MSG MainMessage;
//extern "C" VOID (*TDEW_LoopFunction)();
//ENCRID_API VOID TDEW_StartWindowLoop();
ENCRID_API BOOL TDEW_Running = TRUE;
ENCRID_API MSG TDEW_MainMessage;
ENCRID_API INT TDEW_LoopDelay = 10;
ENCRID_API UINT TDEW_LoopIgnCycle = 10;
ENCRID_API VOID (*TDEW_LoopFunction)();

UINT CycleAccumulator = 0;

VOID TDE_Window::CreateWindowClass(LPCWSTR className, WNDPROC wndProc )
{
	//WindowClass = { 0 };
	WindowClass.lpszClassName = className;
	WindowClass.hInstance = GetModuleHandle(NULL);
	WindowClass.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	WindowClass.lpfnWndProc = wndProc;
	WindowClass.hCursor = LoadCursor(0, IDC_ARROW);
	RegisterClassW(&WindowClass);
}

VOID TDE_Window::CreateWindowClass(WNDPROC wndProc)
{
	wstring className = L"WINDOW" + std::to_wstring(TDE_GetID());
	//
	WindowClass.lpszClassName = className.c_str();
	WindowClass.hInstance = GetModuleHandle(NULL);
	WindowClass.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	WindowClass.lpfnWndProc = wndProc;
	WindowClass.hCursor = LoadCursor(0, IDC_ARROW);
	RegisterClassW(&WindowClass);
}

VOID TDE_Window::CreateWindowHandle(LPCWSTR className, LPCWSTR wTitle, INT x, INT y, INT w, INT h)
{
	HandleWindow = CreateWindow(className, wTitle, WS_OVERLAPPEDWINDOW | WS_VISIBLE, x, y, w, h, 0, 0, GetModuleHandle(NULL), 0);
}

VOID TDE_Window::CreateWindowHandle(LPCWSTR wTitle, INT x, INT y, INT w, INT h)
{
	HandleWindow = CreateWindow(WindowClass.lpszClassName, wTitle, WS_OVERLAPPEDWINDOW | WS_VISIBLE, x, y, w, h, 0, 0, GetModuleHandle(NULL), 0);
}

VOID TDE_Window::CreateWindowHandle(LPCWSTR wTitle, INT w, INT h)
{
	HandleWindow = CreateWindow(WindowClass.lpszClassName, wTitle, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, 0, w, h, 0, 0, GetModuleHandle(NULL), 0);
}

extern "C" VOID TDEW_StartWindowLoop()
{
	while (TDEW_Running)
	{
		if (PeekMessage(&TDEW_MainMessage, NULL, 0, 0, PM_REMOVE))
		{
			if (TDEW_MainMessage.message == WM_QUIT)
			{
				TDEW_Running = FALSE;
			}
			else
			{
	    	    TranslateMessage(&TDEW_MainMessage);
	    	    DispatchMessage(&TDEW_MainMessage);
			}
		}
		else
		{
			if (TDEW_LoopFunction != NULL)
			{
				if (CycleAccumulator >= TDEW_LoopIgnCycle)
				{
				    TDEW_LoopFunction();
					CycleAccumulator = 0;
				}
				CycleAccumulator++;
				Sleep(TDEW_LoopDelay);
			}
		}
	}
}

extern "C" LRESULT TDEW_LoopResult(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CLOSE)
	{
		PostQuitMessage(0);
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


