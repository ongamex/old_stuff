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
