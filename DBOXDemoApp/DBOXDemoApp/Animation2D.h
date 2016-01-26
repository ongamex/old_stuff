#pragma once 

#include "DBOX2D.h"
#include <vector>

//basic frame
class Frame2D{
public : 
	IDirect3DTexture9* m_pTexture;
	lRect* m_pArea;
	void Destroy(){ SAFE_RELEASE(m_pTexture); SAFE_DELETE(m_pArea);}

	Frame2D() : m_pTexture(0), m_pArea(NULL) {}
	~Frame2D(){ Destroy(); }
};

//basic animation
struct Animation2D{
private : 
	float m_TotalTime;
public :
	std::string m_Name;
	std::vector<float> m_FramesDuration;
	std::vector<Frame2D*> m_Animations;
	float z;
	
	Animation2D() : m_TotalTime(-1) {	
		m_FramesDuration.clear();
		m_Animations.clear();
	}

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

//used by the instance 
struct Animation2DAnimInfo{
	float m_ElapsedTime;
	long  m_CenterType;
	D3DXVECTOR3* m_Pivot;
};

struct Animation2DResult{
	float TimeElapsedInAnimation;// z [0, TotalAnimationTime]
	int TotalFrames;
	int FrameDrawn;
};

Animation2DResult Animation2DAnimate(Animation2D* pAnimation, 
	Animation2DAnimInfo* pInfo, 
	float DeltaTime, 
	LPDBOX2D Engine2D, 
	D3DXMATRIX* pTransf, 
	bool ResetElapsedTime = false);