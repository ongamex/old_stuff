
#include "FBXModel.h"

namespace FbxSys
{
	KFbxSdkManager* FbxMngr; 
	KFbxIOSettings* IOSettings;

	IDirect3DDevice9* d3ddev; 
};

void TriangulateRecursive(KFbxNode* Node)
{
	KFbxNodeAttribute* NodeAttrib = Node->GetNodeAttribute();
	
	if(NodeAttrib)
	{
		if (	NodeAttrib->GetAttributeType() == KFbxNodeAttribute::eMESH ||
                NodeAttrib->GetAttributeType() == KFbxNodeAttribute::eNURB ||
                NodeAttrib->GetAttributeType() == KFbxNodeAttribute::eNURBS_SURFACE ||
                NodeAttrib->GetAttributeType() == KFbxNodeAttribute::ePATCH)
		{
			KFbxGeometryConverter GeomConv(Node->GetFbxSdkManager());
			GeomConv.TriangulateInPlace(Node);
		}
	}

	//repeat for child

	int ChildCnt = Node->GetChildCount();
	for(int i = 0; i < ChildCnt; i++)
		TriangulateRecursive(Node->GetChild(i));
	
	return;
}

void FillPoseArray(KFbxScene* pScene, KArrayTemplate<KFbxPose*>& rPoseArray)
{
	const int PoseCnt = pScene->GetPoseCount();
	for(int i = 0; i < PoseCnt ; i++)
		rPoseArray.Add(pScene->GetPose(i));
	return;
}

//class

HRESULT IntializeFbx(IDirect3DDevice9* d3ddev)
{
	FbxSys::d3ddev = d3ddev;
	
	using namespace FbxSys;
	
	FbxMngr = KFbxSdkManager::Create();
	if(!FbxMngr)return -1;

	IOSettings = KFbxIOSettings::Create(FbxMngr, IOSROOT);
	if(!IOSettings)return -1;

	return S_OK;
}

bool FbxSceneManager::Load(std::string FileName)
{
	mFileName = FileName;
	KFbxImporter* FbxImpt = KFbxImporter::Create(FbxSys::FbxMngr, "");
	
	FbxImpt->Initialize(mFileName.c_str(), -1, FbxSys::IOSettings);//set up the importer
	
	mFbxScene = KFbxScene::Create(FbxSys::FbxMngr, "");
	FbxImpt->Import(mFbxScene);//import scene

	KFbxAxisSystem OpenGLAxisSystem(KFbxAxisSystem::eOpenGL);
	OpenGLAxisSystem.ConvertScene(mFbxScene);//convert to right-handed coord

	{
		KFbxNode* pRoot = mFbxScene->GetRootNode();	
		const float framerate = static_cast<float>(KTime::GetFrameRate(mFbxScene->GetGlobalSettings().GetTimeMode()));
		pRoot->ResetPivotSetAndConvertAnimation( framerate, false, false );
	}

	mFbxScene->FillAnimStackNameArray(mAnimStackNameArray);
	
	if(mAnimStackNameArray.GetCount() == 0) 
		mHasAnimation = false; 
	else true;

	TriangulateRecursive(mFbxScene->GetRootNode());

	FillPoseArray(mFbxScene, mPoseArray);

	return true;
}