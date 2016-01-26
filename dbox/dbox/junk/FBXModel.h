/*********************************
*FbxModel.h
*Kostadin Petkov (ongamex@windowslive.com)
**********************************/

#pragma once

#include <fbxsdk.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

//Vertex types

struct FbxVertexAll {
	D3DXVECTOR4 p;//vertex position and vertex weight
	D3DXVECTOR3 n;//normal
	D3DXVECTOR2 t;//texture coord
	DWORD diffuse;//diffuse color
};

struct FbxVertexXYZNormalTex{
	D3DXVECTOR3 p;//vertex position
	D3DXVECTOR3 n;//normal
	D3DXVECTOR2 t;//texture coord(u,v)
};
#define FbxVertexXYZNormalTexFVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX0)


struct VertexXYZNormal {
D3DXVECTOR3 p,n;

};

#define VertexXYZDiffuseFVF (D3DFVF_XYZ | D3DFVF_NORMAL)

//externals

namespace  FbxSys
{
	//Direct3D9 render
	extern IDirect3D9* pD3D;
	extern IDirect3DDevice9* pDevice;
	//file parser
	extern KFbxSdkManager* pSdkManager;
	extern KFbxIOSettings* pIOSettings;
}

/*********************************************
*
*Mesh data 
*
**********************************************/

struct MeshAttribute {
	//loaded Data
	FbxVertexAll* VertexData;
	int* IndexData;
	int vertexCnt , triCnt;
	//vertex and index buffer for rendering
	IDirect3DVertexBuffer9*	vb;
	IDirect3DIndexBuffer9*	ib;

};

struct FbxMaterial
{
	bool ValidMaterial;//wtf
	D3DMATERIAL9 mat;
	IDirect3DTexture9* tex;
};

struct FbxMesh {	
	std::vector<MeshAttribute> attribList;
	
	//material cnt 
	FbxMaterial* mtrlList;
	int mtrlCnt;
};

struct MeshNode {
	MeshNode* parent;
	MeshNode** child;
	int childCnt;
	
	D3DXMATRIX transf , transfComb;
	FbxMesh mesh;
};

bool FbxSetup(IDirect3D9* ,IDirect3DDevice9*);
bool FbxLoadMeshData(char* filename , MeshNode** Root);
void Render(MeshNode* Root);