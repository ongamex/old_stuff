#pragma once

#include "DBOXCameraTP.h"
//AfterClass

LPDBOXCAMERATP DBOXCreateSimpleCameraTP(LPDIRECT3DDEVICE9 device , D3DXVECTOR3 Target , float Radius)
{
	LPDBOXCAMERATP Camera = new DBOXCameraTP;

	D3DVIEWPORT9 vp;
	device->GetViewport(&vp);

	Camera->SetAspect((float)(vp.Width)/(float)(vp.Height));
	Camera->SetFovy(D3DX_PI/4.0f);
	Camera->SetNear(2.0f);
	Camera->SetFar(100000.0f);
	Camera->SetEye(Target);
	Camera->SetRadius(Radius);
	Camera->Yaw(D3DX_PI);

	return Camera;
}

/*********************************************************************************************
Class definition
**********************************************************************************************/
DBOXCameraTP::DBOXCameraTP() : DBOXCameraBase(DBOXCAMERA_TP)
{
	Radius = 10.0f;
	YawCnt = PitchCnt  = 0.0f;
}

DBOXCameraTP::~DBOXCameraTP()
{}

void DBOXCameraTP::NormalizeRotationVectors()
{
	Right	=	D3DXVECTOR3(1.0f , 0.0f , 0.0f);
	Up		=	D3DXVECTOR3(0.0f , 1.0f , 0.0f);
	Look	=	D3DXVECTOR3(0.0f , 0.0f , 1.0f);

	YawCnt = PitchCnt = 0.0f;
}


/**********************************************************************************************
Set methods
***********************************************************************************************/

void DBOXCameraTP::SetEye(D3DXVECTOR3 EyePos)
{Eye = EyePos;return;}


/***********************************************************************************************
Get methods
************************************************************************************************/

float DBOXCameraTP::GetYaw()
{return YawCnt;}

float DBOXCameraTP::GetPitch()
{return PitchCnt;}

/***********************************************************************************************
Camera control methods
************************************************************************************************/

void DBOXCameraTP::MoveInDirection(D3DXVECTOR3 Direction , float Units)
{
	D3DXVec3Normalize(&Direction , &Direction);
	Eye += Direction*Units;
	return;
}

/***********************************************************************************************
Camera vector control methods
************************************************************************************************/
void DBOXCameraTP::Yaw(float Angle)
{
	D3DXMATRIX Transform;
		
	D3DXMatrixRotationY(&Transform  ,Angle);	
	D3DXVec3TransformCoord(&Up , &Up , &Transform);	
	
	D3DXVec3TransformCoord(&Look , &Look , &Transform);
	D3DXVec3TransformCoord(&Right , &Right , &Transform);

	YawCnt += Angle;	
	return;
}

void DBOXCameraTP::Pitch(float Angle)
{
	D3DXMATRIX Transform;
	D3DXMatrixRotationAxis(&Transform  , &Right , Angle);

	D3DXVec3TransformCoord(&Look , &Look , &Transform);
	D3DXVec3TransformCoord(&Up , &Up , &Transform);

	PitchCnt += Angle;
	return;
}

void DBOXCameraTP::ImpulseYaw(float Step , bool Impulse)
{
	if(Impulse)Yaw(Step);
	return;
}
void DBOXCameraTP::ImpulsePitch(float Step , bool Impulse)
{
	if(Impulse)Pitch(Step);
	return;
}

