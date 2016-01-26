#include "Animation2D.h"

Animation2DResult Animation2DAnimate(Animation2D* pAnimation, Animation2DAnimInfo* pInfo, float DeltaTime, LPDBOX2D Engine2D, D3DXMATRIX* pTransf, bool ResetElapsedTime)
{
	Animation2DResult retval;

	float PassedTime = pInfo->m_ElapsedTime + DeltaTime;
	float TotalTime = pAnimation->GetTotalTime();
		
	while(PassedTime > TotalTime)PassedTime = 0;
	
	if(ResetElapsedTime)pInfo->m_ElapsedTime = PassedTime;
	else pInfo->m_ElapsedTime += DeltaTime;	

	int FrameToDraw = 0;float Total = 0.f;

	for(int i = 0; i < pAnimation->m_FramesDuration.size(); i++){
		Total += pAnimation->m_FramesDuration[i];
		if(PassedTime <= Total)break;
		FrameToDraw+=1;
	}

	D3DXMATRIX Transform;
	Transform = *pTransf;
	Transform._43 += pAnimation->z;

	RECT Rect;

	if(pAnimation->m_Animations[FrameToDraw]->m_pArea)
	{
		Rect = pAnimation->m_Animations[FrameToDraw]->m_pArea->ToRect();
	}

	Engine2D->DrawToWorld(pAnimation->m_Animations[FrameToDraw]->m_pTexture, 
							&Transform, 
							pAnimation->m_Animations[FrameToDraw]->m_pArea ? &Rect : NULL,
							pInfo->m_CenterType,
							pInfo->m_Pivot,
							0xffffffff);
	
	retval.FrameDrawn = FrameToDraw;
	retval.TotalFrames = pAnimation->m_Animations.size();
	retval.TimeElapsedInAnimation = PassedTime;

	return retval;
}
