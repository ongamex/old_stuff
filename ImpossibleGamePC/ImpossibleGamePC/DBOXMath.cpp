#pragma once 

#include "DBOXMath.h"

D3DXMATRIX DBOXMatIdent(	1.0f , 0.0f , 0.0f , 0.0f ,
							0.0f , 1.0f , 0.0f , 0.0f ,
							0.0f , 0.0f , 1.0f , 0.0f ,
							0.0f , 0.0f , 0.0f , 1.0f	);

float DBOXNormalizeAngle(float Angle)
{
	int factor = (int)((Angle)/(D3DX_PI * 2.0f));
	(Angle) -=(float)(factor)*(D3DX_PI * 2.0f);
	if((Angle) < 0.0f ) (Angle) += (D3DX_PI * 2.0f);
	return (Angle);
}

RECT Rect(long left , long top , long right , long bottom)
{
	RECT retval  = {left , top , right , bottom};
	return retval ; 
}

float lerp(float t , float a , float b)
{
	return (a + t*(b-a));
}

float Vec3Dist(D3DXVECTOR3* a , D3DXVECTOR3* b)
{
	return sqrtf(DBOXSQR(a->x - b->x) + DBOXSQR(a->y - b->y)  +DBOXSQR(a->z - b->z));
}

//TripleAxis


void TripleAxis::Yaw(float a)
{
	D3DXMATRIX Transform;
	D3DXMatrixRotationAxis(&Transform , &Up , a);
	D3DXVec3TransformCoord(&Look , &Look , &Transform);
	D3DXVec3TransformCoord(&Right , &Right , &Transform);
}

void TripleAxis::Pitch(float a)
{
	D3DXMATRIX Transform;
	D3DXMatrixRotationAxis(&Transform , &Right , a);
	D3DXVec3TransformCoord(&Look , &Look , &Transform);
	D3DXVec3TransformCoord(&Up , &Up , &Transform);
}

void TripleAxis::Roll(float a)
{
	D3DXMATRIX Transform;
	D3DXMatrixRotationAxis(&Transform , &Look , a);
	D3DXVec3TransformCoord(&Right , &Right , &Transform);
	D3DXVec3TransformCoord(&Up , &Up , &Transform);
}
