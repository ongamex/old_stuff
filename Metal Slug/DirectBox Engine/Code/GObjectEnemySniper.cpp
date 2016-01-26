#include "GObject.h"
#include "BlackBox.h"

namespace
{
	//private 
#ifndef SQR
#define SQR(x) ((x)*(x))
#endif
	float Vec3Distance(D3DXVECTOR3* o1 , D3DXVECTOR3* o2)
	{
		return sqrtf( SQR(o1->x - o2->x) + SQR(o1->y - o2->y) );
	}

}
/****************Small Enemies *********************/

/**********Kamikaze 1*********************/

GObjectEnemySniper::GObjectEnemySniper()
{
	ObjectType = OBJECT_ENEMY_SNIPER;
	Animations = Resources::Animations::Data[6];
	Center = Animations[0].Center;
	Radius = 21;
	State = PrevState = 0;
	BaseSpeed = CurrentSpeed = 3.70f;
	Health = 15.5f;
	Time = timeGetTime();
	RandDir = D3DXVECTOR3(0,0,0);
	Facing = 1.f;
}

////////////////////////////////////////////////////
//Update
DWORD GObjectEnemySniper::Update(long idx)
{
	if(Health <= 0)
	{
		State = 2;
	}

	BOOL IsAttacking = State == 1;
	BOOL IsDieing = State == 2;

	if(IsDieing)
	{
		Animations[2].ProcAnimation(Game::AnimationTicker::Tick);
		if(Animations[2].Ended)
		{
			Game::ObjectBuffer::SpawnObject(OBJECT_MISC_COIN , this->Position , 0);
			int i = rand()% 100 + 1 ;
			if(i > 95)
			{
					Game::ObjectBuffer::SpawnObject(OBJECT_MISC_AID , this->Position  , 0);
			}
			return OBJECT_DESTROY;
		}
		return OBJECT_IDLE;
	}

	if(IsAttacking)
	{
		Animations[1].ProcAnimation(Game::AnimationTicker::Tick);
		if(Animations[1].CurrentFrame == 2)
		{
			D3DXVECTOR3 BulletPosition = Position;
			BulletPosition.x += Facing*10.f;
			BulletPosition.y += sinf( float(rand()%6280)/1000.f  - 6.28f)*7.f - 3.5f;
			Game::ObjectBuffer::SpawnObject(OBJECT_BULLET_SNIPER , BulletPosition , &Facing);
		}
		if(Animations[1].Ended)
		{
			Animations[1].ResetAnimation();
			D3DXVECTOR3 BulletPosition = Position;
			BulletPosition.x += Facing*10.f;
			Game::ObjectBuffer::SpawnObject(OBJECT_BULLET_SNIPER , BulletPosition , &Facing);
			RandDir = D3DXVECTOR3(0,0,0);
			State = 0;
		}
		return OBJECT_IDLE;
	}

	if(State == 0)
	{
		if(RandDir == D3DXVECTOR3(0,0,0))
		{
			this->RandDir.x = (float) (rand()% 100);
			this->RandDir.y = (float) (rand()% 100);
			float xdir = (float) (rand()% 100) - 50.f; xdir /= fabsf(xdir);
			float ydir = (float) (rand()% 100) - 50.f; ydir /= fabsf(ydir);

			this->RandDir.x *= xdir;
			this->RandDir.y *= ydir;

			this->RandDir.z = 0;
			D3DXVec3Normalize(&this->RandDir , &this->RandDir);
		}
		
		if(timeGetTime() - Time > 3000)
		{
			State = 1;
			Time = timeGetTime();
			return OBJECT_IDLE;		
		}

		D3DXVECTOR3 NewPos = Position + RandDir*CurrentSpeed;
		this->AdjustPositionToWorld(NewPos);

		bool IsPositionAcceptable = 1;
		for(size_t i = 0 ; i < Game::ObjectBuffer::Buffer.size() ; i++)
		{
			if(i == idx)continue;
			if(IsMisc(Game::ObjectBuffer::Buffer[i]->ObjectType))continue;
			IsPositionAcceptable = IsPositionAcceptable && !this->Collision(NewPos , i) ;
		}
		if(IsPositionAcceptable)
		{
			if(Position.x < NewPos.x)Facing = 1.f;
			else Facing = -1.f;
			float PosDelta = Vec3Distance(&Position , &NewPos);
			Position = NewPos;
			Animations[0].ProcAnimation(Game::AnimationTicker::Tick);
			if(PosDelta < BaseSpeed*0.8f)
			{
				 RandDir.x *= -1.f;
				 RandDir.y *= -1.f;
			}
		}
		else RandDir = D3DXVECTOR3(0,0,0);
	}//end if(State == 0)

	return OBJECT_IDLE;
}
