#pragma once

#include "DBOXCameraBase.h"

/******************************************************************************************
Class constructor and destructior ;
Note : Any Class constructor will normalize directions of vectors;
*******************************************************************************************/

//constructors
DBOXCameraBase::DBOXCameraBase()
{
	CameraType = DBOXCAMERA_NULL;
	Eye		=	D3DXVECTOR3(0.0f , 0.0f , -10.0f);

	Right	=	D3DXVECTOR3(1.0f , 0.0f , 0.0f);
	Up		=	D3DXVECTOR3(0.0f , 1.0f , 0.0f);
	Look	=	D3DXVECTOR3(0.0f , 0.0f , 1.0f);

	dwLighting = FALSE;

	Radius = 1.0f;
	AutoTarget = NULL;
	
}

DBOXCameraBase::DBOXCameraBase(DWORD dwCameraType)
{
	CameraType = dwCameraType;
	Eye		=	D3DXVECTOR3(0.0f , 0.0f , -10.0f);

	Right	=	D3DXVECTOR3(1.0f , 0.0f , 0.0f);
	Up		=	D3DXVECTOR3(0.0f , 1.0f , 0.0f);
	Look	=	D3DXVECTOR3(0.0f , 0.0f , 1.0f);
	
	dwLighting = FALSE;

	Radius = 1.0f;
	AutoTarget = NULL;
}

//destructor
DBOXCameraBase::~DBOXCameraBase()
{
	//empty
}
/******************************************************************************************
Set methods;
*******************************************************************************************/

void DBOXCameraBase::SetAspect(float aspect)
{Aspect = aspect;return;}

void DBOXCameraBase::SetFovy(float fovy)
{Fovy = fovy;return;}

void DBOXCameraBase::SetNear(float zNear)
{Near = zNear;return;}

void DBOXCameraBase::SetFar(float zFar)
{Far = zFar;return;}

//set all settings at once

 void DBOXCameraBase::SetProjectionSettings(float fAspect , float fFovy , float fNear , float fFar)
{Aspect = fAspect;Fovy = fFovy;Near = fNear;Far = fFar;return;}

 void DBOXCameraBase::SetRadius(float Units)
 {Radius = Units;}

 void DBOXCameraBase::SetAutoTarget(D3DXVECTOR3 *p)
 {AutoTarget = p;return;}
/******************************************************************************************
Get methods
*******************************************************************************************/
//projection settings
float DBOXCameraBase::GetAspect()
{return Aspect;}

float DBOXCameraBase::GetFovy()
{return Fovy;}

float DBOXCameraBase::GetNear()
{return Near;}

float DBOXCameraBase::GetFar()
{return Far;}

float DBOXCameraBase::GetRadius()
{return Radius;}

//type
DWORD DBOXCameraBase::GetType()
{return CameraType;}

///Vector
D3DXVECTOR3 DBOXCameraBase::GetEye()
{return Eye;}

D3DXVECTOR3 DBOXCameraBase::GetLook()
{return Look;}

D3DXVECTOR3 DBOXCameraBase::GetUp()
{return Up;}

D3DXVECTOR3 DBOXCameraBase::GetRight()
{return Right;}
/******************************************************************************************
Capture method !
*******************************************************************************************/

 HRESULT DBOXCameraBase::Capture(LPDIRECT3DDEVICE9 d3ddev)
{
	D3DXMATRIX View , Projection;
	
	if(dwLighting)
		if(FAILED(d3ddev->SetRenderState(D3DRS_LIGHTING , TRUE))) return -1;
	
	if(AutoTarget)
	{
		if(CameraType == DBOXCAMERA_TP)Eye = *AutoTarget;
	}

	if((CameraType == DBOXCAMERA_NULL) || (CameraType == DBOXCAMERA_FP))
	{
			D3DXMatrixLookAtLH(&View , &Eye , &(Eye+Look) , &Up);  
			D3DXMatrixPerspectiveFovLH(&Projection , Fovy , Aspect , Near , Far);
	}
	else if (CameraType == DBOXCAMERA_TP)
	{
			D3DXMatrixLookAtLH(&View , &(Look*Radius+Eye) , &Eye , &Up);  
			D3DXMatrixPerspectiveFovLH(&Projection , Fovy , Aspect , Near , Far);
	}

	if(FAILED(d3ddev->SetTransform(D3DTS_PROJECTION , &Projection)))	return -1;
	if(FAILED(d3ddev->SetTransform(D3DTS_VIEW , &View)))				return -1;

	return 1;
}

 HRESULT DBOXCameraBase::SkyRenderMode(LPDIRECT3DDEVICE9 d3ddev)
 {
	D3DXMATRIX View , Projection;

	if((CameraType == DBOXCAMERA_NULL) || (CameraType == DBOXCAMERA_FP))
	{
			D3DXMatrixLookAtLH(&View , &Eye , &(Eye+Look) , &Up);  
			D3DXMatrixPerspectiveFovLH(&Projection , Fovy , Aspect , Near , Far);
	}
	else if (CameraType == DBOXCAMERA_TP)
	{
			D3DXMatrixLookAtLH(&View , &(Look*Radius+Eye) , &Eye , &Up);  
			D3DXMatrixPerspectiveFovLH(&Projection , Fovy , Aspect , Near , Far);
	}


	if(FAILED(d3ddev->GetRenderState(D3DRS_LIGHTING , &dwLighting)))	return -1;
	if(FAILED(d3ddev->SetRenderState(D3DRS_LIGHTING , FALSE)))			return -1;

	View._41 = 0.0f;View._42 = 0.0f;View._43 = 0.0f;

	if(FAILED(d3ddev->SetTransform(D3DTS_WORLD , &DBOXMatIdent)))		return -1;
	if(FAILED(d3ddev->SetTransform(D3DTS_PROJECTION , &Projection)))	return -1;
	if(FAILED(d3ddev->SetTransform(D3DTS_VIEW , &View)))				return -1;

	return 1;
 }