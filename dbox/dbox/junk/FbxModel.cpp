#include "FBXModel.h"

namespace  FbxSys
{
	//Direct3D9 render
	IDirect3D9* pD3D;
	IDirect3DDevice9* pDevice;
	//file parser
	KFbxSdkManager* pSdkManager;
	KFbxIOSettings* pIOSettings;
}

bool FbxSetup(IDirect3D9* d3d,IDirect3DDevice9* device)
{
	FbxSys::pD3D = d3d;
	FbxSys::pDevice = device;
	
	FbxSys::pSdkManager = KFbxSdkManager::Create();
	FbxSys::pIOSettings = KFbxIOSettings::Create(FbxSys::pSdkManager , IOSROOT);

	FbxSys::pSdkManager->SetIOSettings(FbxSys::pIOSettings);

	return true ;//success
}


MeshNode* LoadNode(KFbxScene* pScene , KFbxNode* pNode , MeshNode* pParent)
{
	MeshNode* res = new MeshNode;
	D3DXMatrixIdentity(&res->transf);
	D3DXMatrixIdentity(&res->transfComb);

	res->childCnt = pNode->GetChildCount();
	res->parent = pParent;
	
	//Get Transform
	KFbxAnimEvaluator* mySceneEvaluator = pScene->GetEvaluator();	
	KFbxXMatrix& testTrafo = mySceneEvaluator->GetNodeGlobalTransform(pNode);

	KFbxXMatrix::kDouble44& fbxM = testTrafo.Double44();
	res->transf._11 = (float)fbxM[0][0];
	res->transf._12 = (float)fbxM[0][1];
	res->transf._13 = (float)fbxM[0][2];
	res->transf._14 = (float)fbxM[0][3];

	res->transf._21 = (float)fbxM[1][0];
	res->transf._22 = (float)fbxM[1][1];
	res->transf._23 = (float)fbxM[1][2];
	res->transf._24 = (float)fbxM[1][3];

	res->transf._31 = (float)fbxM[2][0];
	res->transf._32 = (float)fbxM[2][1];
	res->transf._33 = (float)fbxM[2][2];
	res->transf._34 = (float)fbxM[2][3];

	res->transf._41 = (float)fbxM[3][0];
	res->transf._42 = (float)fbxM[3][1];
	res->transf._43 = (float)fbxM[3][2];
	res->transf._44 = (float)fbxM[3][3];

	if(res->parent)
	{
		res->transfComb =  res->transf * res->parent->transfComb;
	}
	else
	{
		D3DXMatrixIdentity(&res->transfComb);
	}

	if(pParent)
	{
		int iMaterialCnt = pNode->GetMaterialCount();
		
		int attribCnt = pNode->GetNodeAttributeCount();
		int iMeshCnt = 0 , iCurrMesh = 0;
		for ( int i = 0; i < attribCnt ; ++i )
		{
			KFbxNodeAttribute* attrib = pNode->GetNodeAttributeByIndex(i);
			if(attrib->GetAttributeType() == KFbxNodeAttribute::EAttributeType::eMESH)
				iMeshCnt ++;
		}

		for ( int i = 0; i < attribCnt ; ++i )
		{
			MeshAttribute tmpAttrib;

			KFbxNodeAttribute* attrib = pNode->GetNodeAttributeByIndex(i);
			if(attrib->GetAttributeType() == KFbxNodeAttribute::EAttributeType::eMESH)
			{
				KFbxGeometryConverter geometryConverter(FbxSys::pSdkManager);
				KFbxMesh* fbxMesh = (KFbxMesh *)attrib;
				geometryConverter.TriangulateMesh(fbxMesh);
			
				iCurrMesh ++;

				KFbxLayerElementNormal* elemNorm = fbxMesh->GetLayer(0)->GetNormals();
				if(elemNorm)
				elemNorm->SetMappingMode(KFbxLayerElement::EMappingMode::eBY_CONTROL_POINT);
				KFbxLayerElementUV* elemUV = fbxMesh->GetLayer(0)->GetUVs();
				if(elemUV)
				elemUV->SetMappingMode(KFbxLayerElement::EMappingMode::eBY_CONTROL_POINT);
				if(iMeshCnt > 1 && iCurrMesh != 1)
				{
					KFbxSurfaceMaterial* mat = pNode->GetMaterial(iCurrMesh - 1);
					const void * lMaterialCache = (mat->GetUserDataPtr());
				}

				KFbxVector4* controlPoints = fbxMesh->GetControlPoints();
				
				tmpAttrib.vertexCnt = fbxMesh->GetControlPointsCount();
				tmpAttrib.triCnt = fbxMesh->GetPolygonCount();

				tmpAttrib.VertexData = new FbxVertexAll[tmpAttrib.vertexCnt];

				tmpAttrib.IndexData = fbxMesh->GetPolygonVertices();//index data 

				//load vertex data
				for(int i = 0; i < tmpAttrib.vertexCnt ; ++i)
				{
					tmpAttrib.VertexData[i].p.x = (float)controlPoints[i].GetAt(0);
					tmpAttrib.VertexData[i].p.y = (float)controlPoints[i].GetAt(1);
					tmpAttrib.VertexData[i].p.z = (float)controlPoints[i].GetAt(2);
					tmpAttrib.VertexData[i].p.w = (float)controlPoints[i].GetAt(3);

					if(elemNorm)
					{
						tmpAttrib.VertexData[i].n.x  = elemNorm->GetDirectArray().GetAt(i).GetAt(0);
						tmpAttrib.VertexData[i].n.y  = elemNorm->GetDirectArray().GetAt(i).GetAt(1);
						tmpAttrib.VertexData[i].n.z  = elemNorm->GetDirectArray().GetAt(i).GetAt(2);
					}
					if(elemUV)
					{
						tmpAttrib.VertexData[i].t.x = elemUV->GetDirectArray().GetAt(i).GetAt(0);
						tmpAttrib.VertexData[i].t.x = elemUV->GetDirectArray().GetAt(i).GetAt(1);
					}
				}

				KFbxSurfaceMaterial *lMaterial = pNode->GetMaterial(i);

				/*
				if(lMaterial->GetClassId().Is(KFbxSurfaceLambert::ClassId) )
				{
					((KFbxSurfaceLambert *)lMaterial)->GetAmbientColor();
					((KFbxSurfaceLambert *)lMaterial)->GetDiffuseColor();
				}
				else if(lMaterial->GetClassId().Is(KFbxSurfacePhong::ClassId))
				{
					((KFbxSurfacePhong *) lMaterial)->GetAmbientColor();
					((KFbxSurfacePhong *) lMaterial)->GetDiffuseColor();
				}*/

				//load index Data
				

				//for Direct3DRendering
				using FbxSys::pDevice;

				pDevice->CreateVertexBuffer(sizeof(VertexXYZNormal)*tmpAttrib.vertexCnt , 0 , VertexXYZDiffuseFVF , D3DPOOL_MANAGED , &tmpAttrib.vb , 0);
				VertexXYZNormal* pVertices;
				tmpAttrib.vb->Lock(0 , 0 , (void**)&pVertices , 0);
				for(int i = 0; i < tmpAttrib.vertexCnt ; ++i)
				{
					pVertices[i].p.x = tmpAttrib.VertexData[i].p.x;
					pVertices[i].p.y = tmpAttrib.VertexData[i].p.y;
					pVertices[i].p.z = tmpAttrib.VertexData[i].p.z;

					pVertices[i].n.x = tmpAttrib.VertexData[i].n.x;
					pVertices[i].n.y = tmpAttrib.VertexData[i].n.y;
					pVertices[i].n.z = tmpAttrib.VertexData[i].n.z;
				}
				tmpAttrib.vb->Unlock();

				pDevice->CreateIndexBuffer(sizeof(WORD)*tmpAttrib.triCnt*3 , 0 , D3DFMT_INDEX16 , D3DPOOL_MANAGED , &tmpAttrib.ib , 0);
				WORD* pIndices;
				tmpAttrib.ib->Lock(0 , 0 , (void**)&pIndices , 0);
				for(int i = 0; i < tmpAttrib.triCnt * 3 ; ++i)
				{
					pIndices[i] = (WORD)tmpAttrib.IndexData[i];
				}
				tmpAttrib.ib->Unlock();

				res->mesh.attribList.push_back(tmpAttrib);
			}//if attribute is mesh - closing
		}//attrib counter - closing
	}//if parent - closing
	res->childCnt = pNode->GetChildCount();
	res->child = new MeshNode*[res->childCnt];
	for(int i = 0 ; i < res->childCnt ; ++i)
	{
		res->child[i] = LoadNode(pScene , pNode->GetChild(i) , res);
	}

	return res;
}



bool FbxLoadMeshData(char* filename , MeshNode** Root)
{
	KFbxImporter* pImporter = KFbxImporter::Create(FbxSys::pSdkManager , "");
	pImporter->Initialize(filename , -1 , FbxSys::pIOSettings);

	KFbxScene* pScene = KFbxScene::Create(FbxSys::pSdkManager , "");

	pImporter->Import(pScene);

	//Convert Geometry
	KFbxGeometryConverter geometryConverter(FbxSys::pSdkManager);
	geometryConverter.SplitMeshesPerMaterial(pScene);

	//Change AxisSystem
	KFbxAxisSystem OpenGLAxisSystem(KFbxAxisSystem::eOpenGL);
	OpenGLAxisSystem.ConvertScene(pScene);

	*Root = LoadNode(pScene , pScene->GetRootNode() , NULL);

	return true ;//success
}


void Render(MeshNode* Root)
{
	using FbxSys::pDevice;

	if(!Root->mesh.attribList.empty())
	{
		for(int i = 0; i < Root->mesh.attribList.size() ; ++i)
		{
			pDevice->SetTransform(D3DTS_WORLD , & ( Root->transfComb) );
			pDevice->SetStreamSource(0 , Root->mesh.attribList[i].vb ,  0 , sizeof(VertexXYZNormal));
			pDevice->SetFVF(VertexXYZDiffuseFVF);
			pDevice->SetIndices(Root->mesh.attribList[i].ib);
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,  0 , 0 , Root->mesh.attribList[i].vertexCnt , 0 , Root->mesh.attribList[i].triCnt);
		}
	}

	for(int i = 0; i < Root->childCnt ; i++)
	{
		Render(Root->child[i]);
	}
}
