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

	void							CreateInterface();
	HRESULT					CreateDevice(D3DPRESENT_PARAMETERS pp);
	void							Release();
	HRESULT					BeginScene(DWORD Color);
	HRESULT					EndScene();
	HRESULT					HandleLostDevice(DWORD SleepTime, D3DPRESENT_PARAMETERS NewPP);

	LPDIRECT3D9							GetD3D();
	LPDIRECT3DDEVICE9				GetDevice();
	LPD3DXSPRITE						GetSprite();

	D3DPRESENT_PARAMETERS GetPresentParameters();
	

	//Other methods
	LPDIRECT3DTEXTURE9		CreateTextureFromFile(tstring FileName, DWORD ColorKey, LONG Width = D3DX_DEFAULT, LONG Height = D3DX_DEFAULT);
};

typedef DBOX3D* LPDBOX3D;
typedef DBOX3D* PDBOX3D;

LPDBOX3D					DBOXCreate3D();

class DBOXXMesh
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