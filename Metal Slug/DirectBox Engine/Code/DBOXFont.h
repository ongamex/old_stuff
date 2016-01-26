#ifndef DBOXFONT_H
#define DBOXFONT_H

#include <d3d9.h>
#include <d3dx9.h>
#include "DBOXCore.h"

#define DBOXFONT_OK		DBOX_OK
#define DBOXFONTE_		DBOX_ERROR

class DBOXFont
{
protected :
	LPD3DXFONT Font;
	DWORD Color ; 
public : 
	DBOXFont();
	~DBOXFont();
	
	LPD3DXFONT	GetFont();
	DWORD		GetColor();

	void		SetFont(LPD3DXFONT pFont);
	void		SetColor(DWORD TextColor);

	HRESULT Create(STRTYPE FontFace ,LPDIRECT3DDEVICE9 d3ddev, int Width , int Height , int Weight , DWORD TextColor);
	void	Release();

	HRESULT DrawText(const STRTYPE Text ,int x ,int y);
	HRESULT DrawText(const STRTYPE Text , long TextLenght , int x , int y);
	HRESULT DrawTextEx(const STRTYPE Text , DWORD Format ,const LPRECT Rect);

};

typedef DBOXFont* PDBOXFONT;
typedef DBOXFont* LPDBOXFONT;

//Afterclass

LPDBOXFONT DBOXCreateFont(STRTYPE FontFace ,LPDIRECT3DDEVICE9 d3ddev, int Width , int Height , int Weight , DWORD TextColor);

#endif