#pragma once 


#include "DBOXApp.h"

//Callback function for the program
LRESULT CALLBACK DBOXMessageProc(HWND hWnd,UINT msg ,WPARAM wp,LPARAM lp) 
{
	switch(msg){
				case WM_CREATE : break;
				case WM_DESTROY : PostQuitMessage(0);break;
				default : return DefWindowProc(hWnd , msg , wp , lp);
				}
	return 0;
}

//CreateApplication Function

LPDBOXAPP DBOXCreateApp(HINSTANCE hInst)
{
	LPDBOXAPP retval = new DBOXApp;
	retval->SetInstance(hInst);
	return retval;
}

//DBOXApp class definition

DBOXApp::DBOXApp()
{
	hWnd = NULL;
}

void DBOXApp::SetInstance(HINSTANCE hInst)
{
	hThisInstance = hInst;
	return;
}

HRESULT DBOXApp::CreateWindowApp(char* WindowName , UINT ClientWidth , UINT ClientHeight , BOOL POPUP, WNDPROC lpfn)
{
	ZeroMemory(&WinCl , sizeof(WNDCLASSEX));
	hWnd = NULL;
	TCHAR* ClassName = WindowName;

	WinCl.cbSize = sizeof(WNDCLASSEX);
	WinCl.hInstance = hThisInstance;
	WinCl.lpszClassName = "a nice name";
	WinCl.lpfnWndProc = lpfn;
	WinCl.style = CS_OWNDC;
	WinCl.hbrBackground = (HBRUSH)COLOR_WINDOW;
	WinCl.hIcon = WinCl.hIconSm = LoadIcon(NULL , IDI_APPLICATION);
	WinCl.hCursor = LoadCursor(NULL , IDC_ARROW);

	if(RegisterClassEx(&WinCl) == 0)return DBOXAPPE_CLASS;

	DWORD WindowStyle;
	WindowStyle = (POPUP) ? WS_POPUP | WS_VISIBLE :  WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE ; 

	RECT rcWnd = {0};
	rcWnd.right = ClientWidth;
	rcWnd.bottom = ClientHeight;

	AdjustWindowRect(&rcWnd , WindowStyle , FALSE);

	hWnd = CreateWindowEx(0 , "a nice name"  , WindowName , WindowStyle, CW_USEDEFAULT , CW_USEDEFAULT , rcWnd.right , rcWnd.bottom , 0 , NULL , hThisInstance, NULL);
	if(hWnd == NULL) return DBOXAPPE_WINDOW;

	//if(!POPUP)
	//DBOXSetClientRect(hWnd , ClientWidth , ClientHeight);

	return DBOXAPP_OK;
}
UINT DBOXApp::ProcessMessage()
{
	UINT retval = WM_NULL;
	if(PeekMessage(&Message , 0 , 0 , 0 , PM_REMOVE))
	{	
		TranslateMessage(&Message);
		DispatchMessage(&Message);
		retval = Message.message;
	}
	return (retval != WM_QUIT);
}

HINSTANCE DBOXApp::GetInstance()
{
	return hThisInstance;
}

MSG DBOXApp::GetMessage()
{
	return Message;
}

MSG	DBOXApp::GetPrevMessage()
{
	return PrevMessage;
}

HWND DBOXApp::GetWindowHandle()
{
	return hWnd;
}

void DBOXApp::Release()
{
	if(hWnd)DestroyWindow(hWnd);
	UnregisterClass("a nice name" , this->hThisInstance);
}
//AfterClass Functions
namespace
{
	bool operator==(RECT r1 , RECT r2)
	{
		if(r1.left != r2.left)return false;
		if(r1.top != r2.top)return false;
		//if(r1.right != r2.right)return false;
		//if(r1.bottom != r2.bottom)return false;

		return true;
	}
}


POINT DBOXGetClientPos(HWND hWnd)
{
	POINT retval = {-1,-1};//invalid output - function failure
	if(hWnd == NULL)return retval;

	RECT Wnd , Cln;//window and client area rectangles

	GetWindowRect(hWnd , &Wnd);
	GetClientRect(hWnd , &Cln);

	LONG Style = GetWindowLong(hWnd , GWL_STYLE);

	if(Wnd == Cln) 
	{
		retval.x = 0;
		retval.y = 0;
		return retval;
	}

	/*
	Structure of simple windowed(rofl) window withot popup
	---------------------------
	|						 x|
	---------------------------
	| |						| |
	| |						| |
	| |						| |
	| |						| |
	| |						| |
	| |						| |
	| ----------------------- |
	---------------------------

	Top border(big border) is the main unknown value . 
	Left , right and bottom(small border) borders are with equal values
	*/
	
	long WndWidth = Wnd.right - Wnd.left;
	long WndHeigh = Wnd.bottom - Wnd.top;

	long SmallBorder = (WndWidth - Cln.right) / 2l ; 
	long BigBorder	 = WndHeigh - (SmallBorder + Cln.bottom);
	
	//Client area on the screen
	retval.x = Wnd.left + SmallBorder;//x pos of client area on the screen
	retval.y = Wnd.top + BigBorder;//y pos of client area on the screen

	return retval;

}