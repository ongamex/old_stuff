#pragma once


#include <Windows.h> 
#include <WindowsX.h>
#include <string>
#include "DBOXCore.h"

#define tERROR(_msg,_code) {tstring msg = tstring(__TEXT("Function:\t")) + tstring(__FUNCTION__) + tstring(__TEXT("\n\rFile:\t")) + tstring(__FILE__) + tstring(__TEXT("\n\rError:\t")) + tstring(_msg);this->MessageError(msg.c_str() , _code);}

class DBOXApplication  {
protected :

	LPSTR m_WindowName;
	HWND m_hWnd;
	HMODULE m_hMod;
	long m_Width , m_Height;
	bool m_Windowed ;

public : 

	int MessageError(const TCHAR* ErrorMessage , int ExitCode);
	int MessageInfo(const TCHAR* InfoMessage);
	int MessageYESNO(const TCHAR* Message);

	void Create(const TCHAR* WindowName , bool Windowed , long Width , long Height);
	
	bool Loop();

	virtual void OnCreate() = 0;
	virtual void Run() = 0;
	virtual void OnDestroy() = 0;
	virtual LRESULT MessageHandler(UINT msg , WPARAM wp , LPARAM lp);

	void ResizeClient(long Width , long Height);
};