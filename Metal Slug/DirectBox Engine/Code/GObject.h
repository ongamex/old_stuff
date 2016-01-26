#pragma once

#include <Windows.h>
#include <WindowsX.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include "ObjectDefines.h"

struct GAnimation
{
	/*Data*/
	RECT* Data;
	LPDIRECT3DTEXTURE9 Texture;
	long Width;
	long Height;
	long FramesCnt;
	float Right;
	D3DXVECTOR3 Center;

	/*animation controller*/
	long CurrentFrame;
	long Ended;
	

	void SetData(RECT* pFramesData  , long NumOfFrames , float RightScale , LPDIRECT3DTEXTURE9 pTexture);

	void ProcAnimation(BOOL);
	void ResetAnimation();
};

//endclass

/* base class of all objects */

struct GObject
{
	GObject();

	DWORD ObjectType;//ID of the object
	
	std::vector<GAnimation> Animations;
	
	float Health;
	DWORD State ; //what is happenning to the object
	DWORD PrevState;
	D3DXVECTOR3 Position;
	float Facing ;// -1.0f for left , 1.0f for right
	float Radius;
	D3DXVECTOR3 Center;

	DWORD LastFireTime;

	float BaseSpeed ; 
	float CurrentSpeed  ;


	virtual BOOL Collision(D3DXVECTOR3 NewPosition,long ObjectIndex);
	virtual DWORD Update(long idx);
	virtual void AdjustPositionToWorld(D3DXVECTOR3& newPosition);
};

// Background object
//these objects are auto drawnable
struct GObjectBackground : public GObject
{
	GObjectBackground();
	D3DXVECTOR3 TopLeftPosition;
	DWORD Update(long idx);
};

/****************** Bullets **********************************/

struct GObjectBullet : public GObject
{
	BOOL Collision(D3DXVECTOR3 NewPosition,long ObjectIndex);
};

struct GObjectBulletMachinegun: public GObjectBullet
{
	float Power ;
	GObjectBulletMachinegun();
	DWORD Update(long idx);
};

struct GObjectBulletPmp : public GObjectBullet
{
	float Power ;
	GObjectBulletPmp();
	DWORD Update(long idx);
};

struct GObjectBulletGrenade : public GObjectBullet
{
	GObjectBulletGrenade();
	DWORD Update(long idx);
};

struct GObjectBulletSniper : public GObjectBullet
{
	float Power ;
	GObjectBulletSniper();
	DWORD Update(long idx);
};



/****************** Player **********************************/

namespace Gun
{
	namespace Machinegun
	{
		HRESULT Shoot(GObject*);
	}
	namespace Pmp
	{
		HRESULT Shoot(GObject*);
	}
	namespace Grenade
	{
		HRESULT Shoot(GObject*);
	}
}

struct GObjectPlayer : public GObject
{
	HRESULT (*FireFunc)(GObject* Obj);
	GObjectPlayer();
	DWORD Update(long idx);
};

/****************** Enemies **********************************/
struct GObjectEnemy : public GObject
{
};

struct GObjectEnemyKamikaze1 : public GObjectEnemy
{
	GObjectEnemyKamikaze1();
	DWORD Update(long idx);
};

struct GObjectEnemySniper : public GObjectEnemy
{
	GObjectEnemySniper();
	DWORD Update(long idx);
	D3DXVECTOR3 RandDir;
	DWORD Time;
};

struct GObjectEnemyNomad : public GObjectEnemy
{
	GObjectEnemyNomad();
	DWORD Update(long idx);
};

/****************** Explosions **********************************/

struct GObjectExplosion : public GObject
{
};

struct GObjectExplosionSmall1 : public GObjectExplosion
{
	GObjectExplosionSmall1();
	DWORD Update(long idx);
};

struct GObjectExplosionSmall2 : public GObjectExplosion
{
	GObjectExplosionSmall2();
	DWORD Update(long idx);
};

/****************** Misc **********************************/

struct GObjectBonus : public GObject
{
};

struct GObjectBonusAid : public GObjectBonus
{
	GObjectBonusAid();
	DWORD Update(long idx);
};

struct GObjectBonusCoin : public GObjectBonus
{
	GObjectBonusCoin();
	DWORD Update(long idx);
};

struct GObjectBonusPmp : public GObjectBonus
{
	GObjectBonusPmp();
	DWORD Update(long idx);
};

struct GObjectBonusCrate : public GObjectBonus
{
	GObjectBonusCrate();
	DWORD Update(long idx);
};

//decore

struct GObjectDecoreRock : public GObjectBonus
{
	GObjectDecoreRock();
	DWORD Update(long idx);
};
