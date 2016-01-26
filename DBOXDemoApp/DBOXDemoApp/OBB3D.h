/**************************************************************
OBB-OBB intersection test in 3 dimensions.
All math functions are provided by d3dx9 math library.
Kostadin Petkov (ongamex@windowslive.com)
***************************************************************/
#pragma once

#include <d3dx9.h>

class OBB3D
{
public : 

	//	0-----------1
	//	|			|
	//	|	 top	|
	//	|			|
	//	2-----------3
	//not used by the user
	D3DXVECTOR3 Edge[8];
	D3DXMATRIX TransformMatrix;
	float sx_ , sy_ , sz_;
	bool Overlap(OBB3D& other);

	//user interface
	OBB3D();
	OBB3D(float sx , float sy , float sz);
	void SetTransform(D3DXMATRIX&);
};

bool OBB3DCollide(OBB3D&,OBB3D&);