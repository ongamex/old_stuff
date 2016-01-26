/*#pragma once

#include "DBOXCore.h"

#include <d3d9.h>
#include <d3dx9.h>
#include <fbxsdk.h>

namespace FbxSys
{
	extern KFbxSdkManager* FbxMngr; 
	extern KFbxIOSettings* IOSettings;

	extern IDirect3DDevice9* d3ddev; 
};

HRESULT IntializeFbx(IDirect3DDevice9* d3ddev);



class FbxSceneManager
{ 

private : 
	std::string		mFileName;

	bool			mHasAnimation;
	KFbxScene*		mFbxScene;

	KArrayTemplate<KString*> mAnimStackNameArray;
	KArrayTemplate<KFbxPose*> mPoseArray;
public : 
	bool		Load(std::string FileName);
};*/