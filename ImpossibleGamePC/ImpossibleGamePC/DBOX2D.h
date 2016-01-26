#ifndef DBOX2D_H
#define DBOX2D_H

#include "DBOX3D.h"
#include "DBOXMath.h"

#define DBOX_CENTER_LEFTTOP ((long)(0))
#define DBOX_CENTER_CENTER ((long)(1))
#define DBOX_CENTER_CUSTOM ((long)(2))

class DBOX2D
{
public  :

	HWND hWnd;
	LPD3DXSPRITE pSprite;
	long ScreenWidth , ScreenHeight;
	float ScaleX , ScaleY;
	//camera 
	D3DXVECTOR3 CamPos;
	float CamZRotation;
	D3DXMATRIX CamTransl;
	D3DXMATRIX CamTranslInv;//cam translation inverce


	static const DWORD SpriteFlags;
	//set
	void SetUp(long Width , long Height , float ScaleX ,float ScaleY);
	

	void SetSprite(LPD3DXSPRITE pSprite);

	//get
	LPD3DXSPRITE GetSprite();
	DBOX2D();
	~DBOX2D();

	//camera fix
	void AdjustCameraMatrix();
	void MoveCameraTo(D3DXVECTOR3* pVector);
	void MoveCameraBy(D3DXVECTOR3* pVector);
	

	HRESULT DrawToScreen(LPDIRECT3DTEXTURE9 Texture ,D3DXMATRIX *Transform ,RECT* Rect , long CenterType , D3DXVECTOR3* pCenter , DWORD ColorFilter = 0xffffffff );
	HRESULT DrawToWorld(LPDIRECT3DTEXTURE9 Texture ,D3DXMATRIX *Transform ,RECT* Rect , long CenterType , D3DXVECTOR3* pCenter , DWORD ColorFilter = 0xffffffff );
};


typedef DBOX2D* PDBOX2D;
typedef DBOX2D* LPDBOX2D;

//Afterclass

LPDBOX2D DBOXCreate2D(LPDBOX3D p3D);

void ClientResize(HWND hWnd, int nWidth, int nHeight);
#endif