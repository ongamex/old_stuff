#ifndef DBOXINPUT_H
#define DBOXINPUT_H

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>
#include <d3dx9.h>
#include <string>
#include "DBOXCore.h"

#define DBOXINPUT_OK	DBOX_OK
#define DBOXINPUTE_		DBOX_ERROR

#define DBOXKEY_UP			0x00
#define DBOXKEY_CLICKED		0x01
#define DBOXKEY_RELEASED	0x10
#define DBOXKEY_DOWN		0x11

#define DIK_LMB		0xF0
#define DIK_RMB		0xF1
#define DIK_MMB		0xF2
#define DIK_4MB     0xF3
#define DIK_5MB     0xF4
#define DIK_6MB     0xF5
#define DIK_7MB     0xF6
#define DIK_8MB     OxF7


#define DBOXIsKeyUp(keystate)			(!(keystate & 0x01))
#define DBOXIsKeyClicked(keystate)		(keystate == 0x01)
#define DBOXIsKeyReleased(keystate)		(keystate == 0x10)
#define DBOXIsKeyDown(keystate) 		(keystate & 0x01)

class DBOXInput
{
protected :

	HWND hWnd;

	LPDIRECTINPUT8		 dinput;
	LPDIRECTINPUTDEVICE8 Keyboard;
	LPDIRECTINPUTDEVICE8 Mouse;

	char KeyState[256];
	char PrevKeyState[256];

	DIMOUSESTATE2 MouseState;
	DIMOUSESTATE2 BufferedMouseState;

public :
	DBOXInput();
	~DBOXInput();
	HRESULT Create(HWND hWnd);
	HRESULT UpdateDevices();
	void Release();

	//keboard
	DWORD GetKeyState(UINT DIK_KEYCODE);
	UINT GetKeyByState(UINT KeyState);
	void GetKeyByStateEx(UINT KeyState ,UINT* Result);//returns an array with keys in that state last element is 0(no key)
	void GetKeyboardState(char State[256]);
	
	char GetChar(DWORD dwKeyState);

	bool IsKeyUp(UINT DIK_KEYCODE);
	bool IsKeyClicked(UINT DIK_KEYCODE);
	bool IsKeyReleased(UINT DIK_KEYCODE);
	bool IsKeyDown(UINT DIK_KEYCODE);
	
	D3DXVECTOR3 GetMouseMoveVector();
	D3DXVECTOR3 GetMousePos();
	D3DXVECTOR3 GetMousePosClient();

	DIMOUSESTATE2 GetMouseState();
	DIMOUSESTATE2 GetBufferedMouseState();
};

typedef DBOXInput* LPDBOXINPUT;
typedef DBOXInput* PDBOXINPUT;

//Class creator

LPDBOXINPUT DBOXCreateInput(HWND hWnd);

//Afterclass

std::string DBOXGetKeyString(UINT DIKKeyCode);

#endif