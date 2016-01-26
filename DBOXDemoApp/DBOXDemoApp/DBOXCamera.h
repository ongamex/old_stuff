#pragma once

#include <d3dx9.h>

class DBOXCameraTP
{
public :
	IDirect3DDevice9* pDevice;
	float YawCnt,  PitchCnt;
public : 
	D3DXVECTOR3 Origin;
	float Radius;
	
	DBOXCameraTP(){}

	void Create(IDirect3DDevice9* device,const D3DXVECTOR3& origin, float radius);
	void Yaw(float a);
	void Pitch(float a);
	void SetTransforms();
};