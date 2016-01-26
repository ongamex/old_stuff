#ifndef DBOXUI_OLD_H
#define DBOXUI_OLD_H

#include <d3d9.h>
#include <d3dx9.h>
#include "DBOXCore.h"
#include "DBOXApp.h"

#define DBOXUI_OK	DBOX_OK
#define DBOXUIE_	DBOX_ERROR


#define DBOXUI_UP			0x00
#define DBOXUI_CLICKED		0x01
#define DBOXUI_RELEASED		0x10
#define DBOXUI_DOWN			0x11

#define DBOXUI_UNCHECKED	(DWORD)(0)
#define DBOXUI_CHECKED		(DWORD)(1)
#define DBOXUI_LOCKED		(DWORD)(2)

#define DBOXUIUp(keystate)			(!(keystate & 0x01))
#define DBOXUIClicked(keystate)		(keystate == 0x01)
#define DBOXUIReleased(keystate)	(keystate == 0x10)
#define DBOXUIDown(keystate) 		(keystate & 0x01)

struct DBOXUICLIENTAREA{int X,Y;int X1,Y1;};

/* ********************************************************************************************************************************* */

class DBOXUI
{

public  :
	LPD3DXSPRITE Sprite;
	HWND hWnd;
	HWND ActiveWnd;
	DBOXUICLIENTAREA ClientArea;//Screen Client Area

	//Free access
	BOOL EnableMouseClick;
	DWORD PrevLeftMouseButtonState;
	DWORD LeftMouseButtonState;
	BOOL LMBClick;
	POINT CurrPos;

	DBOXUI();
	DBOXUI(LPD3DXSPRITE , HWND);
	~DBOXUI();

	
	HWND			GetTargetWindow();
	void			SetTargetWindow(HWND);
	
	DBOXUICLIENTAREA	GetClientArea();

	void			Update();	
	BOOL			IsApplicationActive();

} ;

typedef DBOXUI* PDBOXUI;
typedef DBOXUI* LPDBOXUI;

LPDBOXUI DBOXCreateUI(LPD3DXSPRITE Sprite , HWND TargetWindow);

/* ********************************************************************************************************************************* */
//Button

class DBOXUIButton
{
protected :
	DBOXUI* pController;
	LPDIRECT3DTEXTURE9 Tex;
	RECT StateRect[3];//0 - button is free ; 1 - mouse is over the button ; 2 - button is clicked
	DWORD	Prev;
	DWORD	Now;
	POINT	Pos;
	BOOL	Hidden;
	BOOL	Locked;
	
	BOOL Activated;

	DWORD State;
	int Frame;

	DWORD HotKey;
public : 
	

	void Create(LPDBOXUI Controller , LPDIRECT3DTEXTURE9 Texture , RECT Areas[3]);
	
	void SetUIController(LPDBOXUI Controller);
	void SetTexture(LPDIRECT3DTEXTURE9 Texture);
	void SetPos(long x,long y);
	void SetPos(POINT Position);
	void SetHotkey(DWORD VK_KeyCode);
	void SetState(DWORD State);

	LPDBOXUI GetController();
	LPDIRECT3DTEXTURE9 GetTexture();
	POINT GetPos();
	void GetRect(void* Data);
	int GetFrame();
	DWORD GetHotkey();
	DWORD GetState();

	BOOL CheckState(DWORD DBOXUIState);

	void Hide(BOOL);
	void Lock(BOOL);

	BOOL IsHidden();
	BOOL IsLocked();

	HRESULT Update();

	void Release();
};

typedef DBOXUIButton* PDBOXUIBUTTON;
typedef DBOXUIButton* LPDBOXUIBUTTON;

/* ********************************************************************************************************************************* */
//ProgressBar

class DBOXUIProgressBar
{
protected :
	DBOXUI* pController;
	LPDIRECT3DTEXTURE9 Tex;
	RECT Area[2];//1 - main frame , 2 - prograss
	float Progress ; //0.0f-1.0f
	POINT Pos;
	long Width;
	BOOL Hidden;
public :
	void Create(LPDBOXUI Controller,LPDIRECT3DTEXTURE9 Texture, RECT Areas[2]);
	void Release();

	void SetUIController(LPDBOXUI Controller);
	void SetTexture(LPDIRECT3DTEXTURE9 Texture);
	LPDBOXUI GetController();
	LPDIRECT3DTEXTURE9 GetTexture();

	void SetPos(long x,long y);
	void SetPos(POINT Position);	

	POINT GetPos();

	float GetProgress();
	void SetProgress(float fProgress);
	void AddProgress(float f);
	
	void Hide(BOOL Val);
	BOOL IsHidden();

	void Update();
};

typedef DBOXUIProgressBar* PDBOXUIPROGRESSBAR;
typedef DBOXUIProgressBar* LPDBOXUIPROGRESSBAR;

//Slider

class DBOXUISlider
{
protected :

	LPDBOXUI pController;
	RECT Parts[2];//Parts : 1:Slider  ,2:Button
	LPDIRECT3DTEXTURE9 Tex;//Texture

	//Private members for calculations
	long MinPos , MaxPos;//in screen space
	long OMinPos , OMaxPos;//in object space
	long SliderWidth;//width of the Slider

	long ButtonWidth;//width of the Button
	long ButtonHeight;

	long OffsetWidth;
	POINT Offset;

	BOOL Activated;
	POINT PrevMousePos;//only in activated mode

	POINT Pos;//position on the screen
	POINT ButtonPos;//

	BOOL Hidden;
	BOOL Locked;

public :

	void Create(LPDBOXUI Controller , LPDIRECT3DTEXTURE9 Texture , RECT Areas[2] , long OffsetX );
	void Release();

	void SetPos(POINT Position);
	void SetPos(long x , long y);

	void Hide(BOOL);
	void Lock(BOOL);

	BOOL IsHidden();
	BOOL IsLocked();

	void Update();
	float GetVal();
	void SetVal(float);
};

typedef DBOXUISlider* PDBOXUISLIDER;
typedef DBOXUISlider* LPDBOXUISLIDER;

/* ********************************************************************************************************************************* */
//Checkbox

class DBOXUICheckbox
{
protected :
	LPDBOXUI pController;
	RECT Frames[3];//FrameDesc : 1 free , 2 checked , 3 locked
	LPDIRECT3DTEXTURE9 Texture;

	POINT Position;//Position 
	
	long State ;   // 0 uncheck , 1 checked  ,2 locked 

public :
	void Create(LPDBOXUI,LPDIRECT3DTEXTURE9 ,RECT*);
	void SetPos(POINT Position);
	void SetPos(long x , long y);
	void Release();

	BOOL IsChecked();
	DWORD GetState();
	void SetState(DWORD);
	BOOL Check(BOOL);

	void Update();

};

#endif
