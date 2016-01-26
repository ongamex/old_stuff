#pragma once
 
#include <d3dx9.h>
#include "DBOXCore.h"

////////////////////////////////////////////////////////////////////////////

struct CUSTOM_FRAME : public D3DXFRAME
{
    D3DXMATRIX exCombTransformationMatrix; 
};

struct CUSTOM_MESHCONTAINER : public D3DXMESHCONTAINER
{
    D3DXMATRIX** pExFrameCombinedMatrixPointer;
    D3DXMATRIX*  pExBoneOffsets;
	LPD3DXMESH pExSkinMesh;
    
	LPDIRECT3DTEXTURE9* pExTextures;
	D3DMATERIAL9* pExMaterials;
};

////////////////////////////////////////////////////////////////////////////

class MeshAllocation : public ID3DXAllocateHierarchy
{
    STDMETHOD(CreateFrame)(LPCSTR Name, LPD3DXFRAME* ppNewFrame);
	STDMETHOD(CreateMeshContainer)(LPCSTR Name,
                                   CONST D3DXMESHDATA* pMeshData,
                                   CONST D3DXMATERIAL* pMaterials,
                                   CONST D3DXEFFECTINSTANCE* pEffectInstances,
                                   DWORD NumMaterials,
                                   CONST DWORD* pAdjacency,
                                   LPD3DXSKININFO pSkinInfo,
                                   LPD3DXMESHCONTAINER* ppNewMeshContainer);
    STDMETHOD(DestroyFrame)(LPD3DXFRAME pFrameToFree);
    STDMETHOD(DestroyMeshContainer)(LPD3DXMESHCONTAINER pMeshContainerToFree);
};