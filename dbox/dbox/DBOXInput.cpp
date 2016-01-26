
#include "DBOXInput.h"

//Class Creator

LPDBOXINPUT DBOXCreateInput(HWND hWnd)
{
	LPDBOXINPUT retval = new DBOXInput;
	HRESULT hr = retval->Create(hWnd);
	if(hr != DBOXINPUT_OK)return 0;

	return retval;
}

//Class 

DBOXInput::DBOXInput()
{
	dinput = NULL;
	Keyboard = NULL;
	Mouse = NULL;
	ZeroMemory(KeyState , 256);
	ZeroMemory(PrevKeyState , 256);

	ZeroMemory(&MouseState , sizeof(MouseState));
	ZeroMemory(&BufferedMouseState , sizeof(BufferedMouseState));

	DWORD time = timeGetTime();
	for(int i = 0;i < 256; i++)
	{
		KeyTiming[i] = time;
	}
}

DBOXInput::~DBOXInput()
{
	if(dinput){dinput->Release() ; dinput = NULL;}
	if(Keyboard){Keyboard->Release() ; Keyboard = NULL;}
	if(Mouse){Mouse->Release() ; Mouse = NULL;}
}

HRESULT DBOXInput::Create(HWND hWnd)
{
	if(!hWnd) return DBOXINPUTE_;
	this->hWnd = hWnd;

	ZeroMemory(KeyState , 256);
	ZeroMemory(PrevKeyState , 256);

	HRESULT hr = DirectInput8Create(GetModuleHandle(NULL) , DIRECTINPUT_VERSION , IID_IDirectInput8 , (void**)&dinput , NULL);
	if(FAILED(hr)) return DBOXINPUTE_;

	//Create Keyboard
	hr = dinput->CreateDevice(GUID_SysKeyboard , &Keyboard , NULL);if(FAILED(hr)) return DBOXINPUTE_;
	hr = Keyboard->SetDataFormat(&c_dfDIKeyboard);if(FAILED(hr)) return DBOXINPUTE_;
	hr = Keyboard->SetCooperativeLevel(hWnd , DISCL_NONEXCLUSIVE  | DISCL_BACKGROUND);if(FAILED(hr)) return DBOXINPUTE_;

	//Create Mouse
	hr = dinput->CreateDevice(GUID_SysMouse , &Mouse , NULL);if(FAILED(hr)) return DBOXINPUTE_;
	hr = Mouse->SetDataFormat(&c_dfDIMouse2);if(FAILED(hr)) return DBOXINPUTE_;
	hr = Mouse->SetCooperativeLevel(hWnd , DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);if(FAILED(hr)) return DBOXINPUTE_;

	ZeroMemory(&MouseState , sizeof(DIMOUSESTATE2));
	ZeroMemory(&BufferedMouseState , sizeof(DIMOUSESTATE2));

	hr = Mouse->Acquire();if(FAILED(hr)) return DBOXINPUTE_;

	return DBOXINPUT_OK;
}

HRESULT DBOXInput::UpdateDevices()
{
	HRESULT hr;
	memcpy(PrevKeyState , KeyState , 256);
	ZeroMemory(KeyState , 256);

	hr = Keyboard->Acquire();if(FAILED(hr)) return DBOXINPUTE_;
	hr = Keyboard->GetDeviceState(256 , (void*)KeyState);if(FAILED(hr)) return DBOXINPUTE_;

	//set key time
	for(int i = 0; i < 0xED ; i++)
	{
		if(KeyState[i] != PrevKeyState[i])
		{
			KeyTiming[i] = timeGetTime();
		}
	}
	for(int i = 0xF0; i < 0xF8 ; i++)
	{
		DWORD Prev, Now;
		Prev = (BufferedMouseState.rgbButtons[i - 0xF0] & 0x80) ? 1 : 0;
	    Now  = (MouseState.rgbButtons[i - 0xF0] & 0x80) ? 1 : 0;
		if(Prev != Now)
		{
			KeyTiming[i] = timeGetTime();
		}
	}

	BufferedMouseState.lX += MouseState.lX;
	BufferedMouseState.lY += MouseState.lY;
	BufferedMouseState.lZ += MouseState.lZ;
	
	//save old vals
	memcpy(&BufferedMouseState.rgbButtons , &MouseState.rgbButtons , 8);
	
	hr = Mouse->GetDeviceState(sizeof(DIMOUSESTATE2) , (void*)&MouseState);if(FAILED(hr)) return DBOXINPUTE_;

	return DBOXINPUT_OK;
}

void DBOXInput::Release()
{
	if(Keyboard){Keyboard->Unacquire();Keyboard->Release();Keyboard = NULL;}
	if(Mouse){Mouse->Unacquire();Mouse->Release();Mouse = NULL;}
	if(dinput){dinput->Release();dinput = NULL;}

	return ;
}

DWORD DBOXInput::GetKeyTime(UINT DIK_KEYCODE)
{
	return KeyTiming[DIK_KEYCODE];
}

DWORD DBOXInput::GetKeyState(UINT DIK_KEYCODE)
{
 DWORD Prev , Now;//state of this key

 if(DIK_KEYCODE > 0xED && DIK_KEYCODE < 0xF8)
 {
	 Prev = (BufferedMouseState.rgbButtons[DIK_KEYCODE - 0xF0] & 0x80) ? 1 : 0;
	 Now  = (MouseState.rgbButtons[DIK_KEYCODE - 0xF0] & 0x80) ? 1 : 0;
 }
 else
 {
	Prev = (PrevKeyState[DIK_KEYCODE] & 0x80) ? 1 : 0;
	Now = (KeyState[DIK_KEYCODE] & 0x80) ? 1 : 0;
 }

 DWORD KeyState = 0x00;
 KeyState += 0x10*Prev;
 KeyState += 0x01*Now;


 return KeyState;
}

UINT DBOXInput::GetKeyByState(UINT KeyState)
{
	//F7 is last key
	for(UINT i = 0 ; i < 255 ; i++)
	{
		if(GetKeyState(i) == KeyState)
		{
			return i;
		}
	}

	return 0;
}

void DBOXInput::GetKeyByStateEx(UINT KeyState , UINT* Result)
{

	UINT KeyCnt = 0;
	for(UINT i = 0 ; i < 255 ; i++)//F7 is last key
	{if(GetKeyState(i) == KeyState)KeyCnt++;}
}

void DBOXInput::GetKeyboardState(char State[256])
{
	if(State == 0)return ;
	memcpy(State , KeyState , sizeof(char)*256);

	return ;
}

char DBOXInput::GetChar(DWORD dwKey)
{
	char retval = '\0';

	BOOL Shift = this->IsKeyDown(DIK_LSHIFT)||this->IsKeyDown(DIK_RSHIFT);
	
	switch(dwKey)
	{
		case DIK_1 : 
		{
			if(Shift)retval = '!'; else retval = '1';
		}break;

		case DIK_2 : 
		{
			if(Shift)retval = '@'; else retval = '2';
		}break;
		case DIK_3 : 
		{
			if(Shift)retval = '#'; else retval = '3';
		}break;
		case DIK_4 : 
		{
			if(Shift)retval = '$'; else retval = '4';
		}break;
		case DIK_5 : 
		{
			if(Shift)retval = '%'; else retval = '5';
		}break;
		case DIK_6 : 
		{
			if(Shift)retval = '^'; else retval = '6';
		}break;
		case DIK_7 : 
		{
			if(Shift)retval = '&'; else retval = '7';
		}break;
		case DIK_8 : 
		{
			if(Shift)retval = '*'; else retval = '8';
		}break;
		case DIK_9 : 
		{
			if(Shift)retval = '('; else retval = '9';
		}break;
		case DIK_0 : 
		{
			if(Shift)retval = ')'; else retval = '0';
		}break;
		case DIK_MINUS :
		{
			if(Shift)retval = '_'; else retval = '-';
		}break;
		case DIK_EQUALS :
		{
			if(Shift)retval = '+'; else retval = '=';
		}break;
		case DIK_BACKSLASH :
		{
			if(Shift)retval = '|'; else retval = '\\';
		}break;
		case DIK_GRAVE :
		{
			if(Shift)retval = '~'; else retval = '`';
		}break;
		case DIK_LBRACKET :
		{
			if(Shift)retval = '{'; else retval = '[';
		}break;
		case DIK_RBRACKET :
		{
			if(Shift)retval = '}'; else retval = ']';
		}break;
		case DIK_SEMICOLON : //;
		{
			if(Shift)retval = ':'; else retval = ';';
		}break;
		case DIK_APOSTROPHE : //'
		{
			if(Shift)retval = '"\"'; else retval = '\'';
		}break;
		case DIK_COMMA : //,
		{
			if(Shift)retval = '<'; else retval = ',';
		}break;
		case DIK_PERIOD : // .
		{
			if(Shift)retval = '>'; else retval = '.';
		}break;
		case DIK_SLASH : // /
		{
			if(Shift)retval = '?'; else retval = '/';
		}break;
		case DIK_BACK :
		{
			retval = '\b';
		}break;
		case DIK_SPACE :
		{
			retval = ' ';
		}break;
	default : ;
	}
	if(retval != '\0'){ return retval;}
	switch(dwKey)
	{
		case DIK_A : retval = 'a' ; break ;
		case DIK_B : retval = 'b' ; break ;
		case DIK_C : retval = 'c' ; break ;
		case DIK_D : retval = 'd' ; break ;
		case DIK_E : retval = 'e' ; break ;
		case DIK_F : retval = 'f' ; break ;
		case DIK_G : retval = 'g' ; break ;
		case DIK_H : retval = 'h' ; break ;
		case DIK_I : retval = 'i' ; break ;
		case DIK_J : retval = 'j' ; break ;
		case DIK_K : retval = 'k' ; break ;
		case DIK_L : retval = 'l' ; break ;
		case DIK_M : retval = 'm' ; break ;
		case DIK_N : retval = 'n' ; break ;
		case DIK_O : retval = 'o' ; break ;
		case DIK_P : retval = 'p' ; break ;
		case DIK_Q : retval = 'q' ; break ;
		case DIK_R : retval = 'r' ; break ;
		case DIK_S : retval = 's' ; break ;
		case DIK_T : retval = 't' ; break ;
		case DIK_U : retval = 'u' ; break ;
		case DIK_V : retval = 'v' ; break ;
		case DIK_W : retval = 'w' ; break ;
		case DIK_X : retval = 'x' ; break ;
		case DIK_Y : retval = 'y' ; break ;
		case DIK_Z : retval = 'z' ; break ;
		default : ;
	}

	//if(retval[0] != '\0')
	if(Shift)retval = toupper(retval);
	return retval;
}

bool DBOXInput::IsKeyUp(UINT DIK_KEYCODE)
{
 DWORD Prev , Now;//state of this key

 if(DIK_KEYCODE > 0xEF)
 {
	 Prev = (BufferedMouseState.rgbButtons[DIK_KEYCODE - 0xF0] & 0x80) ? 1 : 0;
	 Now  = (MouseState.rgbButtons[DIK_KEYCODE - 0xF0] & 0x80) ? 1 : 0;
 }
 else
 {
	Prev = (PrevKeyState[DIK_KEYCODE] & 0x80) ? 1 : 0;
	Now = (KeyState[DIK_KEYCODE] & 0x80) ? 1 : 0;
 }

 DWORD KeyState = 0x00;
 KeyState += 0x10*Prev;
 KeyState += 0x01*Now;

 return DBOXIsKeyUp(KeyState);
}

bool DBOXInput::IsKeyClicked(UINT DIK_KEYCODE)
{
 DWORD Prev , Now;//state of this key

 if(DIK_KEYCODE > 0xEF)
 {
	 Prev = (BufferedMouseState.rgbButtons[DIK_KEYCODE - 0xF0] & 0x80) ? 1 : 0;
	 Now  = (MouseState.rgbButtons[DIK_KEYCODE - 0xF0] & 0x80) ? 1 : 0;
 }
 else
 {
	Prev = (PrevKeyState[DIK_KEYCODE] & 0x80) ? 1 : 0;
	Now = (KeyState[DIK_KEYCODE] & 0x80) ? 1 : 0;
 }

 DWORD KeyState = 0x00;
 KeyState += 0x10*Prev;
 KeyState += 0x01*Now;

 return DBOXIsKeyClicked(KeyState);
}

bool DBOXInput::IsKeyReleased(UINT DIK_KEYCODE)
{
 DWORD Prev , Now;//state of this key

 if(DIK_KEYCODE > 0xEF)
 {
	 Prev = (BufferedMouseState.rgbButtons[DIK_KEYCODE - 0xF0] & 0x80) ? 1 : 0;
	 Now  = (MouseState.rgbButtons[DIK_KEYCODE - 0xF0] & 0x80) ? 1 : 0;
 }
 else
 {
	Prev = (PrevKeyState[DIK_KEYCODE] & 0x80) ? 1 : 0;
	Now = (KeyState[DIK_KEYCODE] & 0x80) ? 1 : 0;
 }

 DWORD KeyState = 0x00;
 KeyState += 0x10*Prev;
 KeyState += 0x01*Now;

 return DBOXIsKeyReleased(KeyState);
}

bool DBOXInput::IsKeyDown(UINT DIK_KEYCODE)
{
 DWORD Prev , Now;//state of this key

 if(DIK_KEYCODE > 0xEF)
 {
	 Prev = (BufferedMouseState.rgbButtons[DIK_KEYCODE - 0xF0] & 0x80) ? 1 : 0;
	 Now  = (MouseState.rgbButtons[DIK_KEYCODE - 0xF0] & 0x80) ? 1 : 0;
 }
 else
 {
	Prev = (PrevKeyState[DIK_KEYCODE] & 0x80) ? 1 : 0;
	Now = (KeyState[DIK_KEYCODE] & 0x80) ? 1 : 0;
 }

 DWORD KeyState = 0x00;
 KeyState += 0x10*Prev;
 KeyState += 0x01*Now;

 return DBOXIsKeyDown(KeyState);
}

D3DXVECTOR3 DBOXInput::GetMouseMoveVector()
{
	D3DXVECTOR3 retval;

	retval.x = (float)(MouseState.lX);
	retval.y = (float)(MouseState.lY);
	retval.z = (float)(MouseState.lZ);

	return retval;
}

D3DXVECTOR3 DBOXInput::GetMousePos()
{
	D3DXVECTOR3 retval ;
	POINT Pos;
	GetCursorPos(&Pos);
	retval.x = (float)Pos.x;
	retval.y = (float)Pos.y;
	retval.z = 0.0f;

	return retval;
}

D3DXVECTOR3 DBOXInput::GetMousePosClient()
{
	D3DXVECTOR3 retval ;

	POINT Pos;
	GetCursorPos(&Pos);
	::ScreenToClient(this->hWnd , &Pos);
	retval.x = (float)Pos.x;
	retval.y = (float)Pos.y;
	retval.z = 0.0f;

	return retval;
}


DIMOUSESTATE2 DBOXInput::GetMouseState(){return MouseState;}
DIMOUSESTATE2 DBOXInput::GetBufferedMouseState(){return BufferedMouseState;}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DBOX keycode to string converter 

std::string DBOXGetKeyString(UINT DIKKeyCode)
{

	std::string KeyCodes[255];

	for(int i = 0 ; i < 255 ; i++)
		KeyCodes[i] = "UNK Key";

	KeyCodes[0] = "Undefined key"; 
	KeyCodes[1] = "Escape";
	KeyCodes[2] = "1";
	KeyCodes[3] = "2";
	KeyCodes[4] = "3";
	KeyCodes[5] = "4";
	KeyCodes[6] = "5";
	KeyCodes[7] = "6";
	KeyCodes[8] = "7";
	KeyCodes[9] = "8";
	KeyCodes[10] = "9";
	KeyCodes[11] = "0";
	KeyCodes[0x0C] = "-";
	KeyCodes[0x0D] = "+";
	KeyCodes[0x0E] = "Backspace";
	KeyCodes[0x0F] = "Tab";
	KeyCodes[0x10] = "Q";
	KeyCodes[0x11] = "W";
	KeyCodes[0x12] = "E";
	KeyCodes[0x13] = "R";
	KeyCodes[0x14] = "T";
	KeyCodes[0x15] = "Y";
	KeyCodes[0x16] = "U";
	KeyCodes[0x17] = "I";
	KeyCodes[0x18] = "O";
	KeyCodes[0x19] = "P";
	KeyCodes[0x1A] = "["; //fix
	KeyCodes[0x1B] = "]";
	KeyCodes[0x1C] = "Enter";
	KeyCodes[0x1D] = "L Ctrl";
	KeyCodes[0x1E] = "A";
	KeyCodes[0x1F] = "S";
	KeyCodes[0x20] = "D";
	KeyCodes[0x21] = "F";
	KeyCodes[0x22] = "G";
	KeyCodes[0x23] = "H";
	KeyCodes[0x24] = "J";
	KeyCodes[0x25] = "K";
	KeyCodes[0x26] = "L";
	KeyCodes[0x27] = ";";
	KeyCodes[0x28] = "'";
	KeyCodes[0x29] = "`"; //fix
	KeyCodes[0x2A] = "L Shift";
	KeyCodes[0x2B] = "\\"; 
	KeyCodes[0x2C] = "Z";
	KeyCodes[0x2D] = "X";
	KeyCodes[0x2E] = "C";
	KeyCodes[0x2F] = "V";
	KeyCodes[0x30] = "B";
	KeyCodes[0x31] = "N";
	KeyCodes[0x32] = "M";
	KeyCodes[0x33] = ",";
	KeyCodes[0x34] = "."; //fix (period)
	KeyCodes[0x35] = "/" ; //note slash
	KeyCodes[0x36] = "R Shift";
	KeyCodes[0x37] = "*" ; //note * on Num
	KeyCodes[0x38] = "L Alt";
	KeyCodes[0x39] = "Space";
	KeyCodes[0x3A] = "CapsLock";
	KeyCodes[0x3B] = "F1";
	KeyCodes[0x3C] = "F2";
	KeyCodes[0x3D] = "F3";
	KeyCodes[0x3E] = "F4";
	KeyCodes[0x3F] = "F5";
	KeyCodes[0x40] = "F6";
	KeyCodes[0x41] = "F7";
	KeyCodes[0x42] = "F8";
	KeyCodes[0x43] = "F9";
	KeyCodes[0x44] = "F10";
	KeyCodes[0x45] = "NumLock";
	KeyCodes[0x46] = "Scrl Lock";
	KeyCodes[0x47] = "Num7";
	KeyCodes[0x48] = "Num8";
	KeyCodes[0x49] = "Num9";
	KeyCodes[0x4A] = "Num -"; //note / substract
	KeyCodes[0x4B] = "Num4";
	KeyCodes[0x4C] = "Num5";
	KeyCodes[0x4D] = "Num6";
	KeyCodes[0x4E] = "Num +";
	KeyCodes[0x4F] = "Num1";
	KeyCodes[0x50] = "Num2";
	KeyCodes[0x51] = "Num3";
	KeyCodes[0x52] = "Num0";
	KeyCodes[0x53] = "Num ."; //note Decimal/Del
	KeyCodes[0x54] = "undefined key";
	KeyCodes[0x55] = "undefined key";
	KeyCodes[0x56] = "OEM_102"; //fix
	KeyCodes[0x57] = "F11";
	KeyCodes[0x58] = "F12";
/*
	KeyCodes[0x59] = "undefined key";
	KeyCodes[0x5A] = "undefined key";
	KeyCodes[0x5B] = "undefined key";
	KeyCodes[0x5C] = "undefined key";
	KeyCodes[0x5D] = "undefined key";
	KeyCodes[0x5E] = "undefined key";
	KeyCodes[0x5F] = "undefined key";
	KeyCodes[0x60] = "undefined key";
	KeyCodes[0x61] = "undefined key";
	KeyCodes[0x62] = "undefined key";
	KeyCodes[0x63] = "undefined key";
*/
	KeyCodes[0x64] = "F13";
	KeyCodes[0x65] = "F14";
	KeyCodes[0x66] = "F15";
	KeyCodes[0x70] = "Japan Kana";
	
	KeyCodes[0x9C] = "NumEnter";
	KeyCodes[0x9D] = "R Ctrl";

	KeyCodes[0xB5] = "/";
	KeyCodes[0xB7] = "PrntScr";

	KeyCodes[0xB8] = "R Alt";
	KeyCodes[0xC5] = "Pause";
	KeyCodes[0xC7] = "Home";
	KeyCodes[0xC8] = "Up";
	KeyCodes[0xC9] = "PageUp";
	KeyCodes[0xCB] = "Left";
	KeyCodes[0xCD] = "Right";
	KeyCodes[0xCF] = "End";
	KeyCodes[0xD0] = "Down";
	KeyCodes[0xD1] = "PageDown";
	KeyCodes[0xD2] = "Insert";
	KeyCodes[0xD3] = "Delete";
	KeyCodes[0xDB] = "L Win";
	KeyCodes[0xDC] = "R Win";
	KeyCodes[0xDD] = "Apps";
	KeyCodes[0xDE] = "Power";
	KeyCodes[0xDF] = "Sleep";

//Special for DirectBOX Input
/*
	#define DIK_LMB		0xF0
	#define DIK_RMB		0xF1
	#define DIK_MMB		0xF2
	#define DIK_4MB     0xF3
	#define DIK_5MB     0xF4
	#define DIK_6MB     0xF5
	#define DIK_7MB     0xF6
	#define DIK_8MB     OxF7
*/
	
	KeyCodes[0xF0] = "Mouse Left";
	KeyCodes[0xF1] = "Mouse Right";
	KeyCodes[0xF2] = "Mouse Middle";	
	KeyCodes[0xF3] = "Mouse 4";
	KeyCodes[0xF4] = "Mouse 5";
	KeyCodes[0xF5] = "Mouse 6";
	KeyCodes[0xF6] = "Mouse 7";
	KeyCodes[0xF7] = "Mouse 8";

	if(DIKKeyCode > 254) return KeyCodes[0];

	return KeyCodes[DIKKeyCode];

}