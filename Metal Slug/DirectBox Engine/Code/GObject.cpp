#include "GObject.h"
#include "BlackBox.h"

#include <XInput.h>

#pragma comment(lib, "xinput.lib")

namespace
{
	//private 
#ifndef SQR
#define SQR(x) ((x)*(x))
#endif
	float Vec3Distance(D3DXVECTOR3* o1 , D3DXVECTOR3* o2)
	{
		return sqrtf( SQR(o1->x - o2->x) + SQR(o1->y - o2->y) + SQR(o1->z - o2->z));
	}

}

/* ************************************************************************************************************************************ */
/* GAnimation Definition */
void GAnimation::SetData(RECT* pFramesData  , long NumOfFrames , float RightScale , LPDIRECT3DTEXTURE9 pTexture)
{
	Data = pFramesData; 
	FramesCnt = NumOfFrames ; 
	Width = Data[0].right - Data[0].left;
	Height = Data[0].bottom - Data[0].top;
	Texture = pTexture;
	Center.x = float(Width/2);
	Center.y = float(Height/2);
	Center.z = 0;
	Right = RightScale;
	//NULL some info 
	CurrentFrame = 0;
	Ended = 0;

	return ;
}

void GAnimation::ProcAnimation(BOOL MoveFrame)
{
	if(MoveFrame) CurrentFrame += 1;

	if(CurrentFrame == FramesCnt)
	{
			Ended += 1 ;
			CurrentFrame = 0;
	}

	return ;
}

void GAnimation::ResetAnimation()
{
	CurrentFrame = 0 ;
	Ended = 0;
	return ;
}

/* ************************************************************************************************************************************ */
/* GObject Definition */

GObject::GObject()
{
	ObjectType = OBJECT_NULL;
	LastFireTime = timeGetTime();
}

/* 0 no collision ; 1 collision */

BOOL GObject::Collision(D3DXVECTOR3 NewPosition ,long ObjectIndex )
{
	GObject* Obj2 = Game::ObjectBuffer::Buffer[ObjectIndex];
	
	if(IsMisc(Obj2->ObjectType)) return 0 ;
	int distance = (int)Vec3Distance(&NewPosition , &Obj2->Position);
	return ( distance + EPSYLON < (int)(Radius + Obj2->Radius) ); 
	
}

void  GObject::AdjustPositionToWorld(D3DXVECTOR3& newPosition)
{
	if(newPosition.x - Radius < Game::InGameArea.x){newPosition.x = Game::InGameArea.x + Radius ;}
	if(newPosition.x + Radius > Game::InGameArea.z){newPosition.x = Game::InGameArea.z - Radius ;}

	if(newPosition.y - Radius < Game::InGameArea.y){newPosition.y = Game::InGameArea.y + Radius ;}
	if(newPosition.y + Radius > Game::InGameArea.w){newPosition.y = Game::InGameArea.w - Radius ;}

	return ;
}

DWORD GObject::Update(long id) { return OBJECT_NULL; };


/* ************************************************************************************************************************************ */
//GObjectPlayer


namespace Gun
{
	namespace Machinegun
	{
		HRESULT Shoot(GObject* Obj)
		{
			if(Obj == NULL) return -1 ;
			float BonusDmg = 0;
			DWORD Delta = timeGetTime() - Obj->LastFireTime ;

			if(Delta < 100) return -1;

			if(Delta > 5200) Delta = 5200;

			BonusDmg = (float)(Delta) / 180.0f;

			D3DXVECTOR3 Position = Obj->Position ; 
			Position.x += (Obj->Radius + 1.0f)*Obj->Facing;
			Position.y += sinf( float(rand()%6280)/1000.f  - 6.28f)*5.f - 2.5f;

			GObjectBulletMachinegun* BulletObj = (GObjectBulletMachinegun*)Game::ObjectBuffer::SpawnObject(OBJECT_BULLET_MACHINEGUN, Position , &Obj->Facing);
			BulletObj->Power += BonusDmg;
			return 0;
		}
	}//end MachineGun

	namespace Pmp
	{		
		HRESULT Shoot(GObject* Obj)
		{
			if(Obj == NULL) return -1;
			if(Game::PmpAmmo < 1) //do we have ammo
			{
				//set Gun to default(Gun::Machinegun)
				((GObjectPlayer*)Game::Player)->FireFunc = Gun::Machinegun::Shoot;
				return -1;
			}
			DWORD Delta = timeGetTime() - Obj->LastFireTime ;//time Delta
				
			if(Delta < 700) return -1;
				
			//reduce ammo
			Game::PmpAmmo -= 1;

			D3DXVECTOR3 Position = Obj->Position ; 
			float Facing = Obj->Facing;

			Position.x += Facing*(Obj->Radius + 25.0f);
			Position.y -= 3.0f;
				
			Game::ObjectBuffer::SpawnObject(OBJECT_BULLET_PMP , Position , &Facing);
				
			return 0;
		}
	}

	namespace Grenade
	{
		HRESULT Shoot(GObject* Obj)
		{
			if(Obj == NULL)return 0;
			if(Game::Grenades < 1)
			{
				//no grenades
				return -1;
			}

			D3DXVECTOR3 Position ; 
			Position = Obj->Position;
			Position.x += Obj->Facing*80.f;
			Position.y -= 22.0f;
			Game::ObjectBuffer::SpawnObject(OBJECT_BULLET_GRENADE , Position , &(Obj->Facing) );
			Game::Grenades -= 1;
			return 0;
		}
	}//end Grenades
}//end Gun


GObjectPlayer::GObjectPlayer()
{
	ObjectType = OBJECT_PLAYER1;

	Animations = Resources::Animations::Data[0];
	Center = Animations[0].Center;
	Radius = 21;
	State = PrevState = 0;
	BaseSpeed = 5.49f;
	CurrentSpeed  = BaseSpeed;

	FireFunc = Gun::Machinegun::Shoot;
}
DWORD GObjectPlayer::Update(long idx)
{
	XINPUT_STATE joypad;
	DWORD Result = XInputGetState(0, &joypad);

	PrevState = State;
	State = 0;
	BOOL IsWalking = 0;
	BOOL IsShooting = 0;
	
	if(Health < 0.0f)
	{
		State = 4;
		
		if(this->Animations[State].Ended == 1 )
		{
			this->Animations[State].CurrentFrame = this->Animations[State].FramesCnt - 2;
			//player is dead
			this->Animations[State].ResetAnimation();
			Game::GameState = GAME_END;
			State = PrevState = 0;
			return OBJECT_DESTROY;
		}
		
		this->Animations[State].ProcAnimation(Game::AnimationTicker::Tick);
		return OBJECT_IDLE;
	}

	//Handle the input
	D3DXVECTOR3 MoveVector(0 , 0 , 0);
	
	//&& (joypad.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
	if(IS_KEY_DOWN(mPlayer1KeyDown) || (joypad.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN))
	{
		MoveVector.y = 1.0f;
		IsWalking = 1;
	}
	if(IS_KEY_DOWN(mPlayer1KeyUp) || (joypad.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP))
	{
		MoveVector.y = -1.0f;
		IsWalking = 1;
	}
	if(IS_KEY_DOWN(mPlayer1KeyLeft) || (joypad.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT))
	{
		MoveVector.x = -1.0f;
		Facing = -1.0f;
		IsWalking = 1;
	}
	if(IS_KEY_DOWN(mPlayer1KeyRight) || (joypad.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT))
	{
		MoveVector.x = 1.0f;
		Facing = 1.0f;
		IsWalking = 1;
	}

	//Adjust MoveVector
	D3DXVec3Normalize(&MoveVector , &MoveVector);
	D3DXVECTOR3 NewPosition = Position + MoveVector*CurrentSpeed;

	if(IS_KEY_DOWN(mPlayer1KeyFire) || (joypad.Gamepad.wButtons & XINPUT_GAMEPAD_X))
	{
		IsShooting = 1; 
		if(SUCCEEDED(FireFunc(this)))
		{
			this->LastFireTime = timeGetTime();
		}
	}

	if(IS_KEY_CLICKED(mPlayer1KeyGrenade) || (joypad.Gamepad.wButtons & XINPUT_GAMEPAD_A))
	{
		Gun::Grenade::Shoot(this);
	}

	this->AdjustPositionToWorld(NewPosition);
	bool IsPositionAcceptable = 1;
	for(size_t i = 0 ; i < Game::ObjectBuffer::Buffer.size() ; i++)
	{
		if(i == idx)continue;
		IsPositionAcceptable = IsPositionAcceptable && !this->Collision(NewPosition , i) ;
	}

	if(Position == NewPosition){IsWalking = 0;}
	if(IsPositionAcceptable){Position = NewPosition;}
	
	//setup current state
	if(IsWalking){State = 1;}
	if(IsShooting){State |= 2;}
	
	Animations[State].ProcAnimation(Game::AnimationTicker::Tick);
	return OBJECT_IDLE;
}



