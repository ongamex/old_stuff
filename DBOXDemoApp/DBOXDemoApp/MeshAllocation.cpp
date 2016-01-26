#include "MeshAllocation.h"

HRESULT MeshAllocation::CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame)
{
	CUSTOM_FRAME* pCustFrame = new CUSTOM_FRAME;
	*ppNewFrame = pCustFrame;//set theretval

	memset(pCustFrame, 0, sizeof(CUSTOM_FRAME));
	pCustFrame->pMeshContainer = NULL;

	int NameLenght = 0;
	if(Name)
	NameLenght = strlen(Name);

	if(Name && NameLenght)
	{
		pCustFrame->Name = new char[NameLenght+1];
		memcpy(pCustFrame->Name, Name, NameLenght+1);
	}

	return S_OK;
}

HRESULT MeshAllocation::CreateMeshContainer(LPCSTR Name,
                                   CONST D3DXMESHDATA* pMeshData,
                                   CONST D3DXMATERIAL* pMaterials,
                                   CONST D3DXEFFECTINSTANCE* pEffectInstances,
                                   DWORD NumMaterials,
                                   CONST DWORD* pAdjacency,
                                   LPD3DXSKININFO pSkinInfo,
                                   LPD3DXMESHCONTAINER* ppNewMeshContainer)
{

	CUSTOM_MESHCONTAINER* pCustContainer = new CUSTOM_MESHCONTAINER;
	*ppNewMeshContainer = pCustContainer;//set the retval

	//alloc the name of the mesh container
	int NameLenght = 0;
	if(Name)
	NameLenght = strlen(Name);
	if(Name && NameLenght){
		pCustContainer->Name = new char[NameLenght+1];
		memcpy(pCustContainer->Name, Name, NameLenght+1);
	}

	//only normal mesh
	if(pMeshData->Type != D3DXMESHTYPE_MESH){
		DestroyMeshContainer(pCustContainer);
		//ppNewMeshContainer = NULL;
		return E_FAIL;
	}

	pCustContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	//add adjacency
	DWORD Faces = pMeshData->pMesh->GetNumFaces();
	pCustContainer->pAdjacency = new DWORD[Faces*3];
	if(pAdjacency){
		memcpy(pCustContainer->pAdjacency, pAdjacency, sizeof(DWORD)*Faces*3);
	}
	else {
		if(FAILED(pCustContainer->MeshData.pMesh->GenerateAdjacency(1e-6f, pCustContainer->pAdjacency)))
			FillMemory(pCustContainer->pAdjacency, Faces*3, 255);
	}

	//addref to mesh data
	pCustContainer->MeshData.pMesh = pMeshData->pMesh;
	pCustContainer->MeshData.pMesh->AddRef();

	//get the device
	IDirect3DDevice9* d3ddev = NULL;
	pMeshData->pMesh->GetDevice(&d3ddev);

	//copy and alloc materials
	pCustContainer->NumMaterials = max(NumMaterials, 1);//at least 1(or default material)
	//allocs at least 1
	pCustContainer->pExMaterials = new D3DMATERIAL9[pCustContainer->NumMaterials];
	pCustContainer->pExTextures = new IDirect3DTexture9*[pCustContainer->NumMaterials];
	//pCustContainer->pMaterials = new D3DXMATERIAL[pCustContainer->NumMaterials];
	
	pCustContainer->pMaterials = NULL;//ignore D3DXMATEIRALs
	if(NumMaterials > 0){//if we have more then 1 mat. by default
		for(DWORD i = 0; i < pCustContainer->NumMaterials ; ++i){
			/*ignore D3DXMATEIRALs
			pCustContainer->pMaterials[i] = pMaterials[i];*/

			pCustContainer->pExMaterials[i] = pMaterials[i].MatD3D;
			pCustContainer->pExMaterials[i].Ambient = pCustContainer->pExMaterials[i].Diffuse;
			
			//Alloc the textures
			pCustContainer->pExTextures[i] = NULL;//null the result 
			if( pMaterials[i].pTextureFilename != NULL ){
				if(FAILED(D3DXCreateTextureFromFile(d3ddev, pMaterials[i].pTextureFilename, &pCustContainer->pExTextures[i])))
					pCustContainer->pExTextures[i] = NULL;
			}
		}//end for
	}
	else{
		//default material
		memset(&pCustContainer->pExMaterials[0], 0, sizeof(pCustContainer->pExMaterials[0]));
		
		pCustContainer->pExMaterials[0].Diffuse.r = 0.5f;
		pCustContainer->pExMaterials[0].Diffuse.g = 0.5f;
		pCustContainer->pExMaterials[0].Diffuse.b = 0.5f;
		pCustContainer->pExMaterials[0].Diffuse.a = 1.0f;
		pCustContainer->pExMaterials[0].Specular = pCustContainer->pExMaterials[0].Diffuse;

		pCustContainer->pExTextures[0] = NULL;
	}

	//skin info
	if(pSkinInfo){
		pCustContainer->pSkinInfo = pSkinInfo;
		pCustContainer->pSkinInfo->AddRef();

		unsigned int NumBones = pCustContainer->pSkinInfo->GetNumBones();
		pCustContainer->pExBoneOffsets = new D3DXMATRIX[NumBones];
		pCustContainer->pExFrameCombinedMatrixPointer = new D3DXMATRIX*[NumBones];

		for(unsigned int i = 0; i < NumBones ; ++i){
			pCustContainer->pExBoneOffsets[i] = * (pCustContainer->pSkinInfo->GetBoneOffsetMatrix(i));
		}
	}//end if
	else{//no skin info
		pCustContainer->pSkinInfo = NULL;
		pCustContainer->pExBoneOffsets = NULL;
		pCustContainer->pExFrameCombinedMatrixPointer = NULL;
		pCustContainer->pExSkinMesh = 0;//????
	}

	pCustContainer->pEffects = NULL;

	d3ddev->Release();

	return S_OK;
}

HRESULT MeshAllocation::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	CUSTOM_FRAME* Frame = (CUSTOM_FRAME*) pFrameToFree;
	SAFE_DELETE_ARRAY(Frame->Name);
	delete Frame;

	return S_OK;
}

HRESULT MeshAllocation::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	CUSTOM_MESHCONTAINER* Container = (CUSTOM_MESHCONTAINER*) pMeshContainerToFree;
	if(!Container)return S_OK;

	SAFE_DELETE_ARRAY(Container->Name);
	SAFE_DELETE_ARRAY(Container->pMaterials);
	SAFE_DELETE_ARRAY(Container->pExMaterials);
	if(Container->pExTextures){
		for(unsigned int i = 0; i < Container->NumMaterials ; ++i){
			if(Container->pExTextures[i])SAFE_RELEASE(Container->pExTextures[i]);
		}
	}
	SAFE_DELETE_ARRAY(Container->pExTextures);
	SAFE_DELETE_ARRAY(Container->pAdjacency);
	SAFE_DELETE_ARRAY(Container->pExBoneOffsets);
	SAFE_DELETE_ARRAY(Container->pExFrameCombinedMatrixPointer);

	SAFE_RELEASE(Container->pExSkinMesh);
	SAFE_RELEASE(Container->MeshData.pMesh);
	SAFE_RELEASE(Container->pSkinInfo);

	SAFE_DELETE(Container);

	return S_OK;
}