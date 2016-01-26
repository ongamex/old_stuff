
#include "DBOXCameraFP.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//AfterClass :

LPDBOXCAMERAFP DBOXCreateSimpleCameraFP(LPDIRECT3DDEVICE9 device , bool RollEnable)
{
	LPDBOXCAMERAFP Camera = new DBOXCameraFP;

	D3DVIEWPORT9 vp;
	device->GetViewport(&vp);

	Camera->SetAspect((float)(vp.Width)/(float)(vp.Height));
	Camera->SetFovy(D3DX_PI/4.0f);
	Camera->SetNear(1.0f);
	Camera->SetFar(100000.0f);
	Camera->SetRollEnable(RollEnable);

	return Camera;
}
///////////////////////////////////////////////////////////////////////////////////////////////////

/*********************************************************************************************
Class definition
**********************************************************************************************/
DBOXCameraFP::DBOXCameraFP() : DBOXCameraBase(DBOXCAMERA_FP)
{
	YawCnt = PitchCnt = RollCnt  = 0.0f;
}

DBOXCameraFP::~DBOXCameraFP()
{}

void DBOXCameraFP::NormalizeRotationVectors()
{
	Right	=	D3DXVECTOR3(1.0f , 0.0f , 0.0f);
	Up		=	D3DXVECTOR3(0.0f , 1.0f , 0.0f);
	Look	=	D3DXVECTOR3(0.0f , 0.0f , 1.0f);

	YawCnt = PitchCnt = RollCnt  = 0.0f;
	return;
}

void DBOXCameraFP::ZeroRoll()
{
	float	YawBuf = YawCnt;
	float	PitchBuf = PitchCnt;

	NormalizeRotationVectors();
	Yaw(YawBuf);
	Pitch(PitchBuf);
	return ;
}

/**********************************************************************************************
Set methods
***********************************************************************************************/
void DBOXCameraFP::SetRollEnable(bool State)
{RollEnable = State; ZeroRoll();return;}

void DBOXCameraFP::SetEye(D3DXVECTOR3 EyePos)
{Eye = EyePos;return;}


/***********************************************************************************************
Get methods
************************************************************************************************/
bool DBOXCameraFP::GetRollEnable()
{return RollEnable;}

float DBOXCameraFP::GetYaw()
{return YawCnt;}

float DBOXCameraFP::GetPitch()
{return PitchCnt;}

float DBOXCameraFP::GetRoll()
{return RollCnt;}

/***********************************************************************************************
Camera control methods
************************************************************************************************/

void DBOXCameraFP::Walk(float Units)
{Eye += Look*Units; return;}

void DBOXCameraFP::Strafe(float Units)
{Eye += Right*Units; return;}

void DBOXCameraFP::MoveInDirection(D3DXVECTOR3 Direction , float Units)
{
	D3DXVec3Normalize(&Direction , &Direction);
	Eye += Direction*Units;
	return;
}

/***********************************************************************************************
Camera vector control methods
************************************************************************************************/
void DBOXCameraFP::Yaw(float Angle)
{
	D3DXMATRIX Transform;
	if(RollEnable == 1) 
		D3DXMatrixRotationAxis(&Transform  , &Up ,  Angle);
	else
	{	
		
		D3DXMatrixRotationY(&Transform  ,Angle);	
		D3DXVec3TransformCoord(&Up , &Up , &Transform);	
	}
	
	D3DXVec3TransformCoord(&Look , &Look , &Transform);
	D3DXVec3TransformCoord(&Right , &Right , &Transform);

	YawCnt += Angle;	
	return;
}

void DBOXCameraFP::Pitch(float Angle)
{
	D3DXMATRIX Transform;
	D3DXMatrixRotationAxis(&Transform  , &Right , Angle);

	D3DXVec3TransformCoord(&Look , &Look , &Transform);
	D3DXVec3TransformCoord(&Up , &Up , &Transform);

	PitchCnt += Angle;
	return;
}

void DBOXCameraFP::Roll(float Angle)
{
	if(RollEnable == 0)return;

	D3DXMATRIX Transform;
	D3DXMatrixRotationAxis(&Transform , &Look , Angle);

	D3DXVec3TransformCoord(&Up , &Up , &Transform);
	D3DXVec3TransformCoord(&Right , &Right , &Transform);

	RollCnt += Angle;
	return;
}

void DBOXCameraFP::ImpulseYaw(float Step , bool Impulse){if(Impulse)Yaw(Step);return;}
void DBOXCameraFP::ImpulsePitch(float Step , bool Impulse){if(Impulse)Pitch(Step);return;}
void DBOXCameraFP::ImpulseRoll(float Step , bool Impulse){if(Impulse)Roll(Step);return;}
