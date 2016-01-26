#ifndef DBOX3D_H
#define DBOX3D_H

#include <d3d9.h>
#include <d3dx9.h>
#include "DBOXCore.h"

#define DBOX3D_OK		DBOX_OK
#define DBOX3DE_		DBOX_ERROR
#define DBOX3DE_DEVICE	(HRESULT)(-2)

#define DBOX3D_DEVSTATE_GOOD	(HRESULT)(5000)
#define DBOX3D_DEVSTATE_LOST	(HRESULT)(5001)
#define DBOX3D_DEVSTATE_RESETED	(HRESULT)(5002)

struct DBOXTextureReference
{
	LPDIRECT3DTEXTURE9 Texture;
	std::string FileName;
};

class DBOX3D
{
private :
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3ddev;
	LPD3DXSPRITE Sprite;
	D3DPRESENT_PARAMETERS d3dpp;
public :
	//cache
	
	HRESULT LoadTextureInCache(std::string FileName , int* CacheId);
	HRESULT CreateFontInCache(std::string FileName , int* CacheId);

	void					CreateInterface();
	HRESULT					CreateDevice(D3DPRESENT_PARAMETERS pp);
	void					Release();
	HRESULT					BeginScene(DWORD Color);
	HRESULT					EndScene();
	HRESULT					HandleLostDevice(DWORD SleepTime);

	LPDIRECT3D9				GetD3D();
	LPDIRECT3DDEVICE9		GetDevice();
	D3DPRESENT_PARAMETERS	GetPresentParameters();

	HRESULT					SetRenderState(D3DRENDERSTATETYPE State,DWORD Value);
	void					SetPresentParameters(D3DPRESENT_PARAMETERS pp);

	LPD3DXFONT				CreateSimpleFont(TCHAR* FontFace , UINT szWidth , UINT szHeight);
	bool					LimitFrames(DWORD Frames);

	//Sprite methods
	LPD3DXSPRITE			GetSprite();
	HRESULT					SpriteBegin(DWORD Flags);
	HRESULT					SpriteEnd();

	//Other methods
	LPDIRECT3DTEXTURE9		CreateTextureFromFile(TCHAR* FileName , LONG Width , LONG Height , DWORD ColorKey);
		
};

typedef DBOX3D* LPDBOX3D;
typedef DBOX3D* PDBOX3D;

LPDBOX3D					DBOXCreate3D();
D3DPRESENT_PARAMETERS		DBOXSetPresentParameters(HWND FocusWnd , UINT BackBufferWidth , UINT BackBufferHeight , D3DFORMAT BackBufferFormat , BOOL Windowed);

RECT	DBOXRect(LONG Left ,LONG Top ,LONG Right ,LONG Bottom);


typedef LPDIRECT3DTEXTURE9 LPTexture;

//2D Render

class DBOXSprite
{
	public : 
		LPDBOX3D p3D;

		LPD3DXSPRITE Sprite;
		void SetZSort();
		HRESULT Begin(DWORD Flags);
		HRESULT End();

};

class XMesh
{
private : 
	LPDIRECT3DDEVICE9 pDevice;
	LPDIRECT3DTEXTURE9* ppTextures;
	D3DMATERIAL9* pMaterials;
	DWORD NumOfMats;
public :
	LPD3DXMESH pMesh;
	bool Load(LPDIRECT3DDEVICE9 Device,char* filename);
	void Render(D3DXMATRIX transf);
	void OnLostDevice();
};

#endif