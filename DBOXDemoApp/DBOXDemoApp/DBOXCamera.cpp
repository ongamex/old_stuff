#include "DBOXCamera.h"
#include "DBOXMath.h"
#include "DBOXInput.h"

extern LPDBOXINPUT Input;

void DBOXCameraTP::Create(IDirect3DDevice9* device,const D3DXVECTOR3& origin, float radius)
{
	this->pDevice = device;

	Radius = radius;
	Origin = Origin;
	YawCnt = PitchCnt = 0;
}

void DBOXCameraTP::Yaw(float a){YawCnt += a;}
void DBOXCameraTP::Pitch(float a){PitchCnt += a;}

void DBOXCameraTP::SetTransforms()
{
	D3DVIEWPORT9 vp;
	pDevice->GetViewport(&vp);
	float asp = (float)vp.Width/(float)vp.Height;
	
	D3DXVECTOR3 Eye;
	Eye.x = sinf(YawCnt)*cosf(PitchCnt) + Origin.x;
	Eye.z = cosf(YawCnt)*cosf(PitchCnt) + Origin.z;
	Eye.y = sinf(PitchCnt) + Origin.y;

	Eye = Eye * Radius;

	D3DXVECTOR3 Up;

	Up.x = sinf(YawCnt)*cosf(PitchCnt + D3DX_PI/2.f);
	Up.z = cosf(YawCnt)*cosf(PitchCnt + D3DX_PI/2.f);
	Up.y = sinf(PitchCnt + D3DX_PI/2.f);
	
	D3DXMATRIX Proj, View;
	D3DXMatrixPerspectiveFovLH(&Proj, D3DX_PI/4.f, 4.f/3.f, 1.f , 7000.f);
	D3DXMatrixLookAtLH(&View, &Eye, &Origin , &Up);
	
	pDevice->SetTransform(D3DTS_PROJECTION, &Proj);
	pDevice->SetTransform(D3DTS_VIEW, &View);
	return;
}