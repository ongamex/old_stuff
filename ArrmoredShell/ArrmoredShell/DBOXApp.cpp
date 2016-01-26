#include "DBOXApp.h"

LRESULT CALLBACK WndProc(HWND hWnd , UINT Msg , WPARAM wp , LPARAM lp)
{
	DBOXApplication* App = (DBOXApplication*)GetWindowLong(hWnd , GWL_USERDATA);

	switch(Msg)
	{
	//case WM_CREATE :  App->OnCreate();return 0;
	case WM_DESTROY : 
	case WM_CLOSE : PostQuitMessage(0);
	return 0;
	default : break;
	}
	
	if(App != nullptr)
	{
		return (App->MessageHandler(Msg , wp , lp));
	}
	else
		return DefWindowProc(hWnd , Msg , wp , lp);

	//never 
	return 0;
}

void DBOXApplication::Create(const TCHAR* WindowName , bool Windowed , long Width , long Height)
{
	m_Width = Width;
	m_Height = Height;
	m_Windowed = Windowed;
	m_hMod = GetModuleHandle(0);
	m_WindowName = (LPSTR)WindowName;
	WNDCLASSEX WinCl = {0};
	WinCl.cbSize = sizeof(WinCl);
	WinCl.hIcon = LoadIcon(0 , IDI_APPLICATION);
	WinCl.hIconSm  = LoadIcon(0 , IDI_APPLICATION);
	WinCl.hCursor = LoadCursor(nullptr , IDC_ARROW);
	//WinCl.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	WinCl.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	WinCl.hInstance = m_hMod;
	WinCl.lpszClassName = WindowName;
	WinCl.lpfnWndProc = WndProc;
	WinCl.cbWndExtra = sizeof(DBOXApplication*);

	RegisterClassEx(&WinCl);

	Style = (Windowed) ? WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE /*| WS_CLIPCHILDREN */: WS_POPUP | WS_SYSMENU | WS_VISIBLE /*| WS_CLIPCHILDREN*/;

	RECT ClientRect = { 0 , 0 , Width , Height};
	AdjustWindowRect(&ClientRect , Style , false);

	m_hWnd = CreateWindowEx(WS_EX_ACCEPTFILES , WinCl.lpszClassName , WindowName , Style , CW_USEDEFAULT , CW_USEDEFAULT , ClientRect.right , ClientRect.bottom , 
		nullptr , nullptr , m_hMod , nullptr);

	//push this in user data
	SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);

	OnCreate();
}

bool DBOXApplication::Loop()
{
	MSG Message;
	if(PeekMessage(&Message , nullptr , 0 , 0 , PM_REMOVE))
	{
		if(Message.message == WM_QUIT){
			OnDestroy();//destroy
			return false;
		}
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	this->Run();
	return true;
}

int DBOXApplication::MessageError(const TCHAR* ErrorMessage , int ExitCode)
{
	MessageBox(m_hWnd , ErrorMessage , __TEXT("Error") , MB_OK | MB_ICONERROR);
	exit(ExitCode);
	return 0;
}

int DBOXApplication::MessageInfo(const TCHAR* InfoMessage)
{
	return MessageBox(m_hWnd , InfoMessage , __TEXT("Information") , MB_OK | MB_ICONINFORMATION);
}

int DBOXApplication::MessageYESNO(const TCHAR* Message)
{
	return MessageBox(m_hWnd , Message , __TEXT("Question") , MB_YESNO | MB_ICONQUESTION);
}

LRESULT DBOXApplication::MessageHandler(UINT msg , WPARAM wp , LPARAM lp)
{
	return DefWindowProc(m_hWnd , msg , wp , lp);
}


void DBOXApplication::ResizeClient(long Width , long Height)
{
	/*RECT rcClient, rcWindow;
	POINT ptDiff;
    GetClientRect(m_hWnd, &rcClient);
    GetWindowRect(m_hWnd, &rcWindow);
	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
	SetWindowPos(m_hWnd, NULL,rcWindow.left, rcWindow.top, Width + ptDiff.x, Height + ptDiff.y, 0);*/

	tagRECT New = {0,0,Width,Height};
	AdjustWindowRect(&New, Style , false);

	MoveWindow(m_hWnd, 20, 20, New.right, New.bottom, true);

	m_Width = Width;
	m_Height = Height;
	return;
}
