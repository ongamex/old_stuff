#pragma once 

#include "DBOXMath.h"
#include <exception>
#include <cmath>


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

//assumes that a<b
float lerp(float t , float a , float b)
{
	return (a + t*(b-a));
}

float Vec3Dist(const D3DXVECTOR3* a ,const D3DXVECTOR3* b)
{
	return sqrtf(sqr(a->x - b->x) + sqr(a->y - b->y)  +sqr(a->z - b->z));
}
