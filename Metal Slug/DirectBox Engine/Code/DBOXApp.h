#ifndef DBOXAPP_H
#define DBOXAPP_H

#include <windows.h>
#include <windowsx.h>
#include "DBOXCore.h"

#define WIN_KEY_DOWN(vk_code) ((GetAsyncKeyState((int)(vk_code)) & 0x8000) ? 1 : 0)
#define WIN_KEY_UP(vk_code) ((GetAsyncKeyState((int)(vk_code)) & 0x8000) ? 0 : 1)

#define DBOXAPP_OK			DBOX_OK
#define	DBOXAPPE_			DBOX_ERROR		
#define DBOXAPPE_UNDEFINED	(HRESULT)(-2)
#define DBOXAPPE_CLASS      (HRESULT)(-4)
#define DBOXAPPE_WINDOW		(HRESULT)(-6)

LRESULT CALLBACK DBOXMessageProc(HWND hWnd , UINT Msg , WPARAM wp , LPARAM lp);

class DBOXApp
{
private:
	
	HWND hWnd;
	WNDCLASSEX WinCl;
	DWORD WindowStyle;
	MSG Message;
	MSG PrevMessage;
	HINSTANCE hThisInstance;
	
public :
	DBOXApp();//default Constructior

	 HRESULT			CreateWindowApp(STRTYPE WindowName , UINT ClientWidth , UINT ClinetHeight , BOOL POPUP);
	 UINT				ProcessMessage();
	 
	 void				SetInstance(HINSTANCE hinst);
	 HINSTANCE			GetInstance();

	 MSG				GetMessage();
	 MSG				GetPrevMessage();
		 
	 HWND				GetWindowHandle();
};

typedef DBOXApp* LPDBOXAPP	;
typedef DBOXApp* PDBOXAPP	; 

LPDBOXAPP	DBOXCreateApp(HINSTANCE hInst);
BOOL		DBOXSetClientRect(HWND hwnd ,long width,long height);
POINT		DBOXGetClientPos(const HWND hWnd);

#endif