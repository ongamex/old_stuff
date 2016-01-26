#include "MeshAllocation.h" 
#include <string>
#include "DBOXCore.h"
class XEnitity{
public  :
//private :
	IDirect3DDevice9* m_d3ddev;

	LPD3DXFRAME m_pTopFrame;
	LPD3DXANIMATIONCONTROLLER m_pAnimCtrl;
	CUSTOM_MESHCONTAINER* m_pFirstMesh;

	unsigned int m_MaxBones;
	D3DXMATRIX* m_pBoneMatrices;
	

	std::string m_FileName;

	unsigned int m_CurrentAnimationSet;	
	unsigned int m_NumAnimationSets;
	unsigned int m_CurrentTrack;

	float m_CurrentTime;
	float m_SpeedAdjust;

	void SetupBoneMatrices(CUSTOM_FRAME *pFrame);
	void DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase) const;
	void DrawFrame(LPD3DXFRAME pFrame) const;

	void UpdateFrameMatrices(const D3DXFRAME *pFrameBase, const D3DXMATRIX *pParentMatrix);
	void UpdateSkinnedMesh(const D3DXFRAME *pFrameBase);
//public : 
	XEnitity(){};
	void Create(IDirect3DDevice9* dev);
	~XEnitity(){};

	bool Load(const std::string FileName);
	bool Clone(XEnitity* pEnitity);

	void FrameMove(float ElapsedTime,const D3DXMATRIX *pMatWorld);
	void Render() const;
	void SetAnimationSet(unsigned int index = 0);
};