#ifndef DBOX2D_H
#define DBOX2D_H

#include "DBOX3D.h"
#include "DBOXMath.h"
#include <vector>

#define DBOX_CENTER_LEFTTOP ((long)(0))
#define DBOX_CENTER_CENTER ((long)(1))
#define DBOX_CENTER_CUSTOM ((long)(2))
#define DBOX_CENTER_CENTERBOTTOM ((long)(3))

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
	D3DXMATRIX CamTranslInv;//cam translation inverse


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

/*2D Animation */


class Frame2D{

public : 

	IDirect3DTexture9* m_pTexture;
	lRect* m_pArea;

	void Destroy(){ SAFE_RELEASE(m_pTexture); SAFE_DELETE(m_pArea);}

	Frame2D() : m_pTexture(0), m_pArea(NULL) {}
	~Frame2D(){ Destroy(); }

};

struct Animation2D{
private : 
	float m_TotalTime;
public :

	std::vector<float> m_FramesDuration;
	std::vector<Frame2D*> m_Animations;

	Animation2D() : m_TotalTime(-1) {}

	float GetTotalTime()
	{
		if(m_TotalTime == -1)
		{
			m_TotalTime = 0.f;
			for(int i = 0; i < m_FramesDuration.size(); i++)
			m_TotalTime += m_FramesDuration[i];
		}
		return m_TotalTime;
	}


	
};

struct Animation2DAnimInfo{
	float m_ElapsedTime;
	long  m_CenterType;
	D3DXVECTOR3* m_Pivot;
};

void Animation2DAnimate(Animation2D* pAnimation, 
	Animation2DAnimInfo* pInfo, 
	float DeltaTime, 
	LPDBOX2D Engine2D, 
	D3DXMATRIX* pTransf, 
	bool ResetElapsedTime = false);


#endif