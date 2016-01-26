
#include "AnimationManager.h"

namespace System {
	extern LPDBOX3D			Ri3D;
	extern LPDBOX2D			Ri;

	extern DBOXTimer		GameTimer;
	extern DBOXTimer		RenderingTimer;//just for fps dump
};


//Frame methods

void ASAnimationFrame::Destroy(){
	//SAFE_RELEASE(mTexture);//this is managed by texture manager
	SAFE_DELETE(mpFrame);
}

//Animation methods

float ASAnimation::GetTotalTime()
{
	if(mTotalFramesTime < 0.f) {
		mTotalFramesTime = 0.f;
		for(auto itr = mFrames.begin(); itr != mFrames.end(); ++itr)
			mTotalFramesTime += itr->mTime;
	}//endif

	return mTotalFramesTime;
}

//AnimationInfo

bool ASAnimationInfo::ChangeAnimation(ASAnimation* pAnim)
{ 
	#ifdef _DEBUG 
		if (!pAnim) return false;
	#endif

	mpAnim = pAnim;
	mElapsedTime = 0.f;
	mTotalTime = mpAnim->GetTotalTime();
	mTotalFrames = mpAnim->mFrames.size(); 
	//save Time ratio // TimeRatio = 1.f;
	return true;
} 

void ASAnimationInfo::Animate(float DeltaTime,const D3DXMATRIX* pTransf){ 
		#ifdef _DEBUG 
			if (mTimeRatio == 0.f) printf("\nError ASAnimationInfo::Animate - TimeRatio == 0;");
		#endif

		DeltaTime /= mTimeRatio;

		mElapsedTime = mElapsedTime + DeltaTime;

		while(mElapsedTime > mTotalTime)mElapsedTime -= mTotalTime;

		int FrameToDraw = 0;
		float TimeSum = 0.f;

		for(auto itr = mpAnim->mFrames.begin(); itr != mpAnim->mFrames.end(); ++itr){
			TimeSum += itr->mTime;
			if(mElapsedTime <= TimeSum)break;
			FrameToDraw+=1;
		}

		#ifdef _DEBUG
			if(FrameToDraw >= mTotalFrames)printf("\nERROR ASAnimationInfo::Animate Invalid frame argument!");
		#endif

		D3DXMATRIX Transform = *pTransf;

		Transform._41 += mpAnim->mOffset.x;
		Transform._42 += mpAnim->mOffset.y;
		//Transform._43 += mpAnim->mOffset.z;//turn off z offset

		RECT* pArg = NULL;
		if(mpAnim->mFrames[FrameToDraw].mpFrame != NULL){
			pArg = new RECT;
			*pArg = mpAnim->mFrames[FrameToDraw].mpFrame->ToRect();
		}

		System::Ri->DrawToWorld(mpAnim->mFrames[FrameToDraw].mTexture, &Transform, pArg, mpAnim->mPivotType, &mpAnim->mPivot);

		SAFE_DELETE(pArg);

	return;
}