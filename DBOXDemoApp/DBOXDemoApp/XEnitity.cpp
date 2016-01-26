#include "XEnitity.h"


void XEnitity::Create(IDirect3DDevice9* dev)
{
	m_d3ddev = dev;

	m_MaxBones = 0;
	m_CurrentAnimationSet = 0;
	m_NumAnimationSets = 0;
	m_CurrentTrack = 0;

	m_pBoneMatrices = NULL;
	m_pTopFrame = NULL;

	m_CurrentTime = 0.f;
	m_SpeedAdjust = 1.f;
}

bool XEnitity::Load(const std::string FileName)
{
	m_FileName = FileName;

	MeshAllocation* pMeshAlloc = new MeshAllocation;

	HRESULT hr = D3DXLoadMeshHierarchyFromX(m_FileName.c_str(),
		D3DXMESH_MANAGED,
		m_d3ddev, 
		pMeshAlloc,
		NULL,
		&m_pTopFrame,
		&m_pAnimCtrl);

	SAFE_DELETE(pMeshAlloc);

	if(FAILED(hr))	return false;

	m_NumAnimationSets = m_pAnimCtrl->GetNumAnimationSets();

	if(m_pTopFrame)
	{
		SetupBoneMatrices((CUSTOM_FRAME*) m_pTopFrame);

		m_pBoneMatrices = new D3DXMATRIX[m_MaxBones];
		memset(m_pBoneMatrices, 0, sizeof(D3DXMATRIX)*m_MaxBones);
	}

	return true;
}

void XEnitity::SetupBoneMatrices(CUSTOM_FRAME *pFrame)
{
	CUSTOM_MESHCONTAINER* pMeshContainer = (CUSTOM_MESHCONTAINER*) pFrame->pMeshContainer; 

	while(pMeshContainer && pMeshContainer->pSkinInfo){

		D3DVERTEXELEMENT9 Declaration[MAX_FVF_DECL_SIZE];
		if( FAILED( pMeshContainer->MeshData.pMesh->GetDeclaration(Declaration) ) )
			return;
		//clone the mesh
		pMeshContainer->MeshData.pMesh->CloneMesh(D3DXMESH_MANAGED, Declaration, m_d3ddev, &pMeshContainer->pExSkinMesh);

		//calc the max bones
		DWORD NumBones = pMeshContainer->pSkinInfo->GetNumBones();
		m_MaxBones = max(m_MaxBones, (unsigned int)NumBones);

		for(unsigned int i = 0; i < NumBones; ++i){
			CUSTOM_FRAME* pTmpFrame = (CUSTOM_FRAME*) D3DXFrameFind(m_pTopFrame, pMeshContainer->pSkinInfo->GetBoneName(i));

			pMeshContainer->pExFrameCombinedMatrixPointer[i] = &pTmpFrame->exCombTransformationMatrix;
		}
		
		//loop for all meshes
		pMeshContainer = (CUSTOM_MESHCONTAINER*)pMeshContainer->pNextMeshContainer;
	}//end while

	//pass to siblings
	if(pFrame->pFrameSibling)
		SetupBoneMatrices((CUSTOM_FRAME*)pFrame->pFrameSibling);

	//pass to children
	if(pFrame->pFrameFirstChild)
		SetupBoneMatrices((CUSTOM_FRAME*)pFrame->pFrameFirstChild);

	return;
}

void XEnitity::Render() const
{
	if(m_pTopFrame)
		DrawFrame(m_pTopFrame);
	return;
}

void XEnitity::DrawFrame(LPD3DXFRAME pFrame) const
{
	LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;

	while(pMeshContainer != NULL){
			DrawMeshContainer(pMeshContainer, pFrame);
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	//pass to sibling
	if(pFrame->pFrameSibling){
		DrawFrame(pFrame->pFrameSibling);
	}

	//pass to child

	if(pFrame->pFrameFirstChild){
		DrawFrame(pFrame->pFrameFirstChild);
	}

	return;
}

void XEnitity::DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase) const
{
	
	CUSTOM_FRAME* pFrame = (CUSTOM_FRAME*)pFrameBase;
	CUSTOM_MESHCONTAINER* pMeshContainer = (CUSTOM_MESHCONTAINER*)pMeshContainerBase;

	if(pMeshContainer->pSkinInfo){
		D3DXMATRIX Ident;
		D3DXMatrixIdentity(&Ident);
		m_d3ddev->SetTransform(D3DTS_WORLD, &Ident);
	}
	else{
		m_d3ddev->SetTransform(D3DTS_WORLD, &pFrame->exCombTransformationMatrix);
	}

	unsigned int NumMaterials = pMeshContainer->NumMaterials;
	for(unsigned int i = 0; i < NumMaterials; ++i){
		
		m_d3ddev->SetMaterial(&pMeshContainer->pExMaterials[i]);
		m_d3ddev->SetTexture(0, pMeshContainer->pExTextures[i]);
	
		LPD3DXMESH pMeshToDraw = (pMeshContainer->pSkinInfo) ? pMeshContainer->pExSkinMesh : pMeshContainer->MeshData.pMesh;

		pMeshToDraw->DrawSubset(i);
	}

	return;
}

void XEnitity::UpdateFrameMatrices(const D3DXFRAME *pFrameBase, const D3DXMATRIX *pParentMatrix)
{
	CUSTOM_FRAME* pFrame = (CUSTOM_FRAME*) pFrameBase;

	if(pParentMatrix != NULL)
		D3DXMatrixMultiply(&pFrame->exCombTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	else
		pFrame->exCombTransformationMatrix = pFrame->TransformationMatrix;

	//pass to siblings
	if(pFrame->pFrameSibling){
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}

	//pass to childern
	if(pFrame->pFrameFirstChild){
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->exCombTransformationMatrix);
	}
	return;
}

void XEnitity::UpdateSkinnedMesh(const D3DXFRAME *pFrameBase)
{
	CUSTOM_FRAME* pFrame = (CUSTOM_FRAME*) pFrameBase;
	CUSTOM_MESHCONTAINER* pMeshContainer = (CUSTOM_MESHCONTAINER*) pFrame->pMeshContainer;

	while(pMeshContainer && pMeshContainer->pSkinInfo){
		
		unsigned int NumBones = pMeshContainer->pSkinInfo->GetNumBones();

		for(unsigned int i = 0; i < NumBones; ++i){
			D3DXMatrixMultiply(&m_pBoneMatrices[i],&pMeshContainer->pExBoneOffsets[i], pMeshContainer->pExFrameCombinedMatrixPointer[i]);
		}

		void* pSrc = 0;
		HRESULT hr = pMeshContainer->MeshData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pSrc);
		if(FAILED(hr))return;

		void* pDest = 0;
		hr = pMeshContainer->pExSkinMesh->LockVertexBuffer(0, (void**)&pDest);

		hr = pMeshContainer->pSkinInfo->UpdateSkinnedMesh(m_pBoneMatrices, NULL, pSrc, pDest);

		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
		pMeshContainer->pExSkinMesh->UnlockVertexBuffer();

		pMeshContainer = (CUSTOM_MESHCONTAINER*) pMeshContainer->pNextMeshContainer;
	}//end while

	if (pFrame->pFrameSibling)
		UpdateSkinnedMesh(pFrame->pFrameSibling);

	if(pFrame->pFrameFirstChild)
		UpdateSkinnedMesh(pFrame->pFrameFirstChild);

	return;
}

void XEnitity::FrameMove(float ElapsedTime,const D3DXMATRIX *pMatWorld)
{
	//m_pAnimCtrl->ResetTime();

	ElapsedTime/=m_SpeedAdjust;
	m_CurrentTime += ElapsedTime;
	if(m_pAnimCtrl)
		m_pAnimCtrl->AdvanceTime(ElapsedTime, NULL);
	
	UpdateFrameMatrices(m_pTopFrame, pMatWorld);
	
	UpdateSkinnedMesh(m_pTopFrame);
	
	return;
}

void XEnitity::SetAnimationSet(unsigned int index)
{
	const float MoveTransitionTime=0.25f;

	while (index>=m_NumAnimationSets)
		index-=m_NumAnimationSets;

	if (index==m_CurrentAnimationSet)
		return;

	m_CurrentAnimationSet=index;
	
	DWORD NewTrack = (m_CurrentTrack == 0 ? 1 : 0);

	LPD3DXANIMATIONSET set;
	m_pAnimCtrl->GetAnimationSet(m_CurrentAnimationSet, &set );	
	m_pAnimCtrl->SetTrackAnimationSet( NewTrack, set );
	set->Release();	

	m_pAnimCtrl->UnkeyAllTrackEvents(m_CurrentTrack);
	m_pAnimCtrl->UnkeyAllTrackEvents(NewTrack);

	m_pAnimCtrl->KeyTrackEnable(m_CurrentTrack, FALSE, m_CurrentTime + MoveTransitionTime);
	m_pAnimCtrl->KeyTrackSpeed(m_CurrentTrack, 0.0f, m_CurrentTime, MoveTransitionTime, D3DXTRANSITION_LINEAR);
	m_pAnimCtrl->KeyTrackWeight(m_CurrentTrack, 0.0f, m_CurrentTime, MoveTransitionTime, D3DXTRANSITION_LINEAR);

	m_pAnimCtrl->SetTrackEnable(NewTrack, TRUE);
	m_pAnimCtrl->KeyTrackSpeed(NewTrack, 1.f, m_CurrentTime, MoveTransitionTime, D3DXTRANSITION_LINEAR);
	m_pAnimCtrl->KeyTrackWeight(NewTrack, 1.f, m_CurrentTime, MoveTransitionTime, D3DXTRANSITION_LINEAR);

	m_CurrentTrack = NewTrack;

	return;
}

bool XEnitity::Clone(XEnitity* pEnitity)
{
	HRESULT hr = m_pAnimCtrl->CloneAnimationController(	m_pAnimCtrl->GetMaxNumAnimationOutputs(), 
														m_pAnimCtrl->GetMaxNumAnimationSets(),
														m_pAnimCtrl->GetMaxNumTracks(),
														m_pAnimCtrl->GetMaxNumEvents(),
														&pEnitity->m_pAnimCtrl);
	if(FAILED(hr)){
		pEnitity->m_pAnimCtrl = NULL;
		return false;
	}
	
	pEnitity->m_d3ddev = m_d3ddev;
	pEnitity->m_FileName = m_FileName;
	pEnitity->m_MaxBones = m_MaxBones;
	
	pEnitity->m_CurrentTime = 0;
	pEnitity->m_CurrentAnimationSet = m_CurrentAnimationSet;
	pEnitity->m_SpeedAdjust = m_SpeedAdjust;
	pEnitity->m_CurrentTrack = 0;
	pEnitity->m_pTopFrame = m_pTopFrame;
	
	pEnitity->m_pBoneMatrices = new D3DXMATRIX[m_MaxBones];
	memset(pEnitity->m_pBoneMatrices, 0, sizeof(D3DXMATRIX)*m_MaxBones);
	
	pEnitity->m_pFirstMesh = m_pFirstMesh;
	pEnitity->m_NumAnimationSets = m_NumAnimationSets;
	
	pEnitity->m_pBoneMatrices = new D3DXMATRIX[pEnitity->m_MaxBones];
	memcpy(pEnitity->m_pBoneMatrices, m_pBoneMatrices, sizeof(D3DXMATRIX)*m_MaxBones);

	return true;
}

