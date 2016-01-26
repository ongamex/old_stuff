#pragma once 

#include "DBOXTimer.h"
#include "DBOX2D.h"
#include "DBOXMath.h"
#include <vector>

//Animation Frame
struct ASAnimationFrame{ 
	IDirect3DTexture9* mTexture;
	lRect* mpFrame;
	float mTime;

	ASAnimationFrame() : mTexture(NULL), mpFrame(0), mTime(0.f) {}
	ASAnimationFrame(IDirect3DTexture9* pTexture, lRect* pFrame, float Time) : mTexture(pTexture), mpFrame(pFrame), mTime(Time) {}

	void Destroy();

	~ASAnimationFrame() { /*mTexture = NULL; SAFE_DELETE(mpFrame);*/ }
};

//Vector of Frames
struct ASAnimation{ 
protected : 
	float mTotalFramesTime;
public : 
	std::string mName;
	std::vector<ASAnimationFrame> mFrames;
	float mZ;// 0.f --> 1.f 

	D3DXVECTOR3 mOffset;
	DWORD mPivotType;
	D3DXVECTOR3 mPivot;

	ASAnimation() : mFrames(0), mZ(0), mOffset(0,0,0), mName(), mPivotType(DBOX_CENTER_CENTER), mPivot(0,0,0), mTotalFramesTime(-1.f){}//Default invalid argumen 
	~ASAnimation() { mFrames.clear(); }

	float GetTotalTime();
};

//Per instance animating info
struct ASAnimationInfo
{
	private : 
		ASAnimation* mpAnim; 
		float mTotalTime;
		int mTotalFrames;
	public : 
		float mElapsedTime;
		float mTimeRatio;

		ASAnimationInfo() : mElapsedTime(0), mpAnim(NULL), mTimeRatio(1.f),/* mPivotType(DBOX_CENTER_CENTER), mPivot(0,0,0),*/ mTotalFrames(0) {}
		ASAnimationInfo(ASAnimation* pAnim) : mTimeRatio(1.f) //don't call mpCurrAnimation constructor
		{
			ChangeAnimation(pAnim);
		}

		bool ChangeAnimation(ASAnimation* pAnim);

		void Animate(float DeltaTime,const D3DXMATRIX* pTransf);
};