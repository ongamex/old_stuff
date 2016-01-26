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

RECT CreateRECT(long left , long top , long right , long bottom)
{
	RECT retval  = {left , top , right , bottom};
	return retval ; 
}

//assumes that a<b
float lerp(float t , float a , float b){return (a + t*(b-a));}

float Vec3Dist(const D3DXVECTOR3* a ,const D3DXVECTOR3* b){return sqrtf(sqr(a->x - b->x) + sqr(a->y - b->y)  +sqr(a->z - b->z));}

//fRect functions

fRect MovefRect(const fRect* pRect, const D3DXVECTOR3* pV)
{
	fRect retval;

	retval.left = pRect->left + pV->x;
	retval.right = pRect->right + pV->x;

	retval.top = pRect->top + pV->y;
	retval.bottom = pRect->bottom + pV->y;

	return retval;
}

bool OverlapfRect(fRect* r1, fRect* r2, D3DXVECTOR3* o)
{
	
	float xC1 = (r1->right + r1->left)/2.f;
	float yC1 = (r1->bottom + r1->top)/2.f;

	float xR1 = (r1->right - r1->left)/2.f;
	float yR1 = (r1->bottom - r1->top)/2.f;

	float xC2 = (r2->right + r2->left)/2.f;
	float yC2 = (r2->bottom + r2->top)/2.f;

	float xR2 = (r2->right - r2->left)/2.f;
	float yR2 = (r2->bottom - r2->top)/2.f;

	float px = xR1+xR2 - fabsf(xC1 - xC2);
	float py = yR1+yR2 - fabsf(yC1 - yC2);

	bool Intersect = (px >= 0 && py >= 0);

	if(Intersect){
		D3DXVECTOR3 vc1(xC1, yC1, 0);
		D3DXVECTOR3 vc2(xC2, yC2, 0);
		D3DXVECTOR3 Dir = vc1 - vc2;
		D3DXVec3Normalize(&Dir, &Dir);
		
		if(py < px)
		{
			Dir = D3DXVECTOR3(0, sign(Dir.y)*py, 0);
		}
		else
		{
			Dir = D3DXVECTOR3(sign(Dir.x)*px , 0 , 0);
		}

		*o = Dir;

		return true;
	}
	
	*o = D3DXVECTOR3(0,0,0);
	return false;
}