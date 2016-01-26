#ifndef DBOXCAMERAFP_H
#define DBOXCAMERAFP_H

#include <d3d9.h>
#include <d3dx9.h>
#include "DBOXCameraBase.h"
#include "DBOXMath.h"

/*****************************************************************************************
Class info :
The constructor of this class will set CameraType to DBOXCAMERA_FP 
******************************************************************************************/

class DBOXCameraFP : public DBOXCameraBase
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
	*********************************/

	
	bool RollEnable;
	float YawCnt , PitchCnt , RollCnt;
		
public :
	DBOXCameraFP();
	~DBOXCameraFP();

	void NormalizeRotationVectors();
	void ZeroRoll();
	//set
	void SetRollEnable(bool State);

	void SetEye(D3DXVECTOR3 EyePos);
	//get
	bool GetRollEnable();
	float GetYaw();
	float GetPitch();
	float GetRoll();

	//calc
	void Walk(float Uints);
	void Strafe(float Units);
	void MoveInDirection(D3DXVECTOR3 Direction , float Unist);
	
	void Yaw(float Angle);
	void Pitch(float Angle);
	void Roll(float Angle);
	void ImpulseYaw(float Step , bool Impulse);
	void ImpulsePitch(float Step , bool Impulse);
	void ImpulseRoll(float Step , bool Impulse);
};

typedef DBOXCameraFP* LPDBOXCAMERAFP;
typedef DBOXCameraFP* PDBOXCAMERAFP; 

//AfterClass Functions

LPDBOXCAMERAFP DBOXCreateSimpleCameraFP(LPDIRECT3DDEVICE9 device , bool RollEnable);				   

#endif