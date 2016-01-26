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
		return sqrtf( SQR(o1->x - o2->x) + SQR(o1->y - o2->y) + SQR(o1->z - o2->z));
	}
}

/***** Bonus Aid *****/

GObjectBonusAid::GObjectBonusAid()
{
	ObjectType = OBJECT_MISC_AID;
	State = PrevState = 0;//const
	Animations = Resources::Animations::Data[5];
	Facing = 1.0f;
	Radius = 21.0f;
	Center = Animations[State].Center;
}

DWORD GObjectBonusAid::Update(long idx)
{
	Animations[State].ProcAnimation(Game::AnimationTicker::Tick);

	float Distance = Vec3Distance(&this->Position , &Game::Player->Position);//distance between aid and player
	if(Distance < Radius)
	{
		//Add health ,adjust health , destroy object
		Game::Player->Health += 45.5f;
		if(Game::Player->Health > 255.0f) Game::Player->Health = 255.0f ; 
		return OBJECT_DESTROY;
	}

	return OBJECT_IDLE;
}

/***** Bonus Coin *****/

GObjectBonusCoin::GObjectBonusCoin()
{
	ObjectType = OBJECT_MISC_COIN ;
	State = PrevState = 1 ; //const for that obj
	Animations = Resources::Animations::Data[5];
	Facing = 1.0f;
	Radius = 20.0f;
	Center = Animations[State].Center;
}

DWORD GObjectBonusCoin::Update(long idx)
{
	Animations[State].ProcAnimation(Game::AnimationTicker::Tick);

	float Distance = Vec3Distance(&this->Position , &Game::Player->Position);//distance between coin and player
	if(Distance < Radius)
	{
		//Add points and destroy object
		Game::Points += 1.0f;
		Game::Player->Health += 1.23f;
		if(Game::Player->Health > 255.f)Game::Player->Health = 255.f;
		if(Game::Points > 99)
		{
			Game::Player->Health = 255.f;
			Game::Grenades += 3;
			Game::Points = 0;
		}
		return OBJECT_DESTROY;
	}
	return OBJECT_IDLE;
}

/********* Bonus Pmp **********/

GObjectBonusPmp::GObjectBonusPmp()
{
	ObjectType = OBJECT_MISC_PMPICO;
	State = PrevState = 2;//const for that obj
	Animations = Resources::Animations::Data[5];
	Facing = 1.0f;
	Radius = 16.0f;
	Center = Animations[State].Center;
}

DWORD GObjectBonusPmp::Update(long idx)
{
	this->Animations[State].ProcAnimation(Game::AnimationTicker::Tick);
	float Distance = Vec3Distance(&this->Position , &Game::Player->Position);

	if(Distance < this->Radius + Game::Player->Radius)
	{
		//give bullets
		Game::PmpAmmo += 5;
		((GObjectPlayer*)Game::Player)->FireFunc = Gun::Pmp::Shoot;

		return OBJECT_DESTROY;
	}

	return OBJECT_IDLE;
}

/******* GObjectBonusCrate ******/
GObjectBonusCrate::GObjectBonusCrate()
{
	ObjectType = OBJECT_MISC_CRATE;
	State = PrevState = 4;//cosnt 
	Animations = Resources::Animations::Data[5];
	Facing = 1.f;
	Radius = 16.f;
	Center = Animations[State].Center;
}

DWORD GObjectBonusCrate::Update(long idx)
{
	this->Animations[State].ProcAnimation(Game::AnimationTicker::Tick);
	float Distance = Vec3Distance(&this->Position , &Game::Player->Position);

	if(Distance < this->Radius + Game::Player->Radius)
	{
		Game::PmpAmmo += 2;
		Game::Grenades += 2;
		Game::Player->Health += 16.f;
		if(Game::Player->Health > 255.f)Game::Player->Health = 255.f;
		return OBJECT_DESTROY;
	}

	return OBJECT_IDLE;
}
/**** Rock ****/
GObjectDecoreRock::GObjectDecoreRock()
{
	ObjectType = OBJECT_STRUCT_ROCK;
	State = PrevState = 5;
	Animations = Resources::Animations::Data[5];
	Facing = 1.f;
	Radius = 15.f;
	Center = Animations[State].Center;

}

DWORD GObjectDecoreRock::Update(long idx)
{
	for(int i = 0 ; i < Game::ObjectBuffer::Buffer.size() ; i++)
	{
		if(IsBullet(Game::ObjectBuffer::Buffer[i]->ObjectType))
		{
			float Delta = Vec3Distance(&Position , &Game::ObjectBuffer::Buffer[i]->Position);
			if(Delta < Radius + Game::ObjectBuffer::Buffer[i]->Radius)
			{
				Game::ObjectBuffer::DestroyObject(i);
				i--;
			}
		}
	}
			return OBJECT_IDLE;
}