#include "GObject.h"
#include "Blackbox.h"

namespace
{
	//private 
#ifndef SQR
#define SQR(x) ((x)*(x))
#endif
	float Vec3Distance(D3DXVECTOR3* o1 , D3DXVECTOR3* o2)
	{
		return sqrtf( SQR(o1->x - o2->x) + SQR(o1->y - o2->y));
	}
}

/******************* Small Explosions ****************************/

//ExplosionSmall 1
GObjectExplosionSmall1::GObjectExplosionSmall1()
{
	ObjectType = OBJECT_EXPLOSION_SMALL1;
	Facing = 1.0f;
	Animations = Resources::Animations::Data[4];
	State = PrevState = 2;//const for that see Animations
	Radius = 40.F;
	
	Center = Animations[0].Center;
}

inline DWORD GObjectExplosionSmall1::Update(long idx)
{
	Animations[State].ProcAnimation(Game::AnimationTicker::Tick);
	float Distance = 1000.f;
	if(Animations[State].CurrentFrame == 2)
	{
		for(size_t i = 0 ; i < Game::ObjectBuffer::Buffer.size() ; i++)
		{
			GObject* CurrObj = Game::ObjectBuffer::Buffer[i];

		//	if(IsMisc(CurrObj->ObjectType)) continue ; 
			 Distance = Vec3Distance(&this->Position , &CurrObj->Position);
		
			 if(Distance <= ( this->Radius + CurrObj->Radius )) CurrObj->Health -= 150.0f;
			
		}
	}

	if(Animations[State].Ended)
	{
		return OBJECT_DESTROY;
	}

	return OBJECT_IDLE;
}

//ExplosionSmall 2
GObjectExplosionSmall2::GObjectExplosionSmall2()
{
	ObjectType = OBJECT_EXPLOSION_SMALL2;
	Facing = 1.0f;
	State = PrevState = 1;//const for that see Animations
	Radius = 50;
	Animations = Resources::Animations::Data[4];
	Center = Animations[State].Center;
}

DWORD GObjectExplosionSmall2::Update(long idx)
{
	Animations[State].ProcAnimation(Game::AnimationTicker::Tick);

	if(Animations[State].CurrentFrame == 1)
	{
		float Distance = Vec3Distance(&this->Position , &Game::Player->Position);
		if(Distance <= Radius + 22)
		{
			Game::Player->Health -= 55.0f;
		}
			if(Distance <= Radius + 35)
		{
			Game::Player->Health -= 25.0f;
		}
	}

	if(Animations[State].Ended)
	{
		return OBJECT_DESTROY;
	}

	return OBJECT_IDLE;
}