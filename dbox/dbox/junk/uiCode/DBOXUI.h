#ifndef DBOXUI_H
#define DBOXUI_H

#include "DBOX2D.h"
#include "DBOXInput.h"
#include "DBOXFont.h"
#include <vector>
#include <string>

#define DBOXUI_LEVEL ((float)(0.01f))
#define DBOXUI_CURSOR_LEVEL ((float)(0.1f))

struct DBOXUIManager
{
	//private : 
	HWND hWnd ;//target window
	LPDBOX2D Engine2d;
	LPDBOXINPUT Input;

	float BaseWidth , BaseHeight ; //of the screen
	float CurrentWidth , CurrentHeight ;
	BOOL Resize;

	D3DXVECTOR3 MouseClientPos;
	DWORD MouseLeftButtonState;
	DWORD MouseRightButtonState;

	D3DXVECTOR3 PrevMouseClientPos;
	DWORD PrevMouseLeftButtonState;
	DWORD PrevMouseRightButtonState;

	void Update(); 
};

typedef DBOXUIManager* PDBOXUIMANAGER;
typedef DBOXUIManager* LPDBOXUIMANAGER;

struct DBOXUIObject //basic UI class
{
	DBOXUIObject();

	DBOXUIManager* mngr;//manager

	bool Activated;
	bool Hidden,Locked;
	virtual void Update();
	virtual void AdaptSizeAndPosition();
};

//Buttons

struct DBOXUIButton : public DBOXUIObject
{
	//state
	DWORD Prev;
	DWORD Now;

	LPDIRECT3DTEXTURE9 Texture;
	LPDBOXFONT Font;
	std::string Text;
	std::string FontFace;
	DWORD FontColor;

	RECT Frames[3];//Original Frames
	D3DXVECTOR3 Position ; //Original Position

	RECT TransformedFrames[3];//Used for bounding box
	D3DXVECTOR3 TransformedPosition;//Used for bounding box
	
	void Create(DBOXUIManager* Manager , RECT aFrames[3] , D3DXVECTOR3* aPosition , LPDIRECT3DTEXTURE9 aTexture);
	void AdaptSizeAndPosition();
	void Update() ; 
	void SetState(DWORD aPrev , DWORD aNow);
	BOOL Is(DWORD State);
};

typedef DBOXUIButton* LPDBOXUIBUTTON ;
typedef DBOXUIButton* PDBOXUIBUTTON ;
//ProgressBar


struct DBOXUIProgressBar : public DBOXUIObject
{
	float Progress  ;
	bool Inverted ; 
	
	LPDIRECT3DTEXTURE9 Texture; 
	
	RECT Frames[2];//1 = 0% , 2 = 100%
	D3DXVECTOR3 Position;

	RECT TransformedFrames[2];
	D3DXVECTOR3 TransformedPosition;

	void Create(DBOXUIManager* Manager , RECT aFrames[2] , D3DXVECTOR3* aPosition , LPDIRECT3DTEXTURE9 aTexture);
	void AdaptSizeAndPosition();
	void Update();
};

typedef DBOXUIProgressBar* LPDBOXUIPROGRESSBAR;
typedef DBOXUIProgressBar* PDBOXUIPROGRESSBAR;
//TextBox

struct DBOXUITextBox : public DBOXUIObject
{
	long MaxStrSize;
	std::string Text;
	std::string FontName;
	LPDBOXFONT Font;
	DWORD Time;
	BOOL PassMode;
	LPDIRECT3DTEXTURE9 Texture;
	RECT TextureArea;

	D3DXVECTOR3 Position;
	int AlphaH , AlphaW ;//Alpha size
	D3DXVECTOR3 TextBoxSize ;
	long xOffset , yOffset;

	D3DXVECTOR3 TransformedPosition ; 
	int TransformedAlphaH , TransformedAlphaW;
	D3DXVECTOR3 TransformedTextBoxSize;
	long TransformedXOffset , TransformedYOffset;

	void Create(DBOXUIManager* Manager , LPDIRECT3DTEXTURE9 Texture , char* aFontName , D3DXVECTOR3* aPosition , RECT* pRect , long NumberOfSymb , long xTextOffset , long yTextOffset);
	void AdaptSizeAndPosition();
	void Update();
};

typedef DBOXUITextBox* LPDBOXUITEXTBOX;

//Afterclass
LPDBOXUIMANAGER DBOXCreateUIManager() ;
#endif