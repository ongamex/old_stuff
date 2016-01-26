#ifndef DBOXCAMERATP_H
#define DBOXCAMERATP_H

#include <d3d9.h>
#include <d3dx9.h>
#include "DBOXCameraBase.h"
#include "DBOXMath.h"

/*****************************************************************************************
Class info :
The constructor of this class will set CameraType to DBOXCAMERA_TP 
******************************************************************************************/

class DBOXCameraTP : public DBOXCameraBase
{
private :

	/********************************
	This is in is in DBOXCamerabase :
	DWORD CameraType;

	float Aspect;
	float Fovy;
	float Near , Far;
	
	D3DXVECTOR3 Eye;
	D3DXVECTOR3 Look , Up ;
	D3DXVECTOR3 Right;

	float Radius;
	*********************************/
	float YawCnt , PitchCnt;
	
public :
	DBOXCameraTP();
	~DBOXCameraTP();

	void NormalizeRotationVectors();

	//set

	void SetEye(D3DXVECTOR3 EyePos);
	//get
	float GetYaw();
	float GetPitch();

	//calc
	void MoveInDirection(D3DXVECTOR3 Direction , float Units);
	
	void Yaw(float Angle);
	void Pitch(float Angle);
	void ImpulseYaw(float Step , bool Impulse);
	void ImpulsePitch(float Step , bool Impulse);
};

typedef DBOXCameraTP* LPDBOXCAMERATP;
typedef DBOXCameraTP* PDBOXCAMERATP; 

//AfterClass Functions

LPDBOXCAMERATP DBOXCreateSimpleCameraTP(LPDIRECT3DDEVICE9 device , D3DXVECTOR3 Target , float Radius);
			   
#endif