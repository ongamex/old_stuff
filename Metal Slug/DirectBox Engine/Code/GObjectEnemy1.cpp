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

GObjectEnemyKamikaze1::GObjectEnemyKamikaze1()
{
	ObjectType = OBJECT_ENEMY_KAMIKAZE_1;
	Animations = Resources::Animations::Data[9];
	Center = Animations[0].Center;
	Radius = 20;
	State = PrevState = 0;
	BaseSpeed = CurrentSpeed = 6.00f;
	Health = 43.5f;
}

////////////////////////////////////////////////////
//Update
DWORD GObjectEnemyKamikaze1::Update(long idx)
{
	BOOL IsAttacking = State == 1;
	BOOL IsDieing = State == 2;
	PrevState = State;

	if(IsDieing)
	{
		Animations[State].ProcAnimation(Game::AnimationTicker::Tick);
		if(Animations[State].Ended)
		{
			//spawn little bonus
			GObject* pCoin = Game::ObjectBuffer::SpawnObject(OBJECT_MISC_COIN , this->Position , 0);

			int i = rand()% 100 + 1 ;
			if(i > 89)
			{
					Game::ObjectBuffer::SpawnObject(OBJECT_MISC_AID , this->Position  , 0);
			}
			return OBJECT_DESTROY;
		}
		return OBJECT_IDLE;
	}

	if(IsAttacking)
	{
		if(Animations[State].Ended)
		{
			//spawn explosion & DestroyThatObject
			Game::ObjectBuffer::SpawnObject(OBJECT_EXPLOSION_SMALL2 , this->Position  , 0);

			int i = rand()% 100 + 1 ;
			if(i > 97) Game::ObjectBuffer::SpawnObject(OBJECT_MISC_AID , this->Position  , 0);
			
			return OBJECT_DESTROY;
		}
		
		Animations[State].ProcAnimation(Game::AnimationTicker::Tick);
		
		return OBJECT_IDLE;
	}

	State = 0;
	//is player around
	float Dist = Vec3Distance(&this->Position , &Game::Player->Position);
	if(Dist > 650)return OBJECT_IDLE;
	if(Dist < 42)
	{
		State = 1;//attack player state
	}

	if(fabsf(Position.x - Game::Player->Position.x)  < 340 + 60)
	{
			//go to player and attack
			Animations[State].ProcAnimation(Game::AnimationTicker::Tick);
			D3DXVECTOR3 NewPos;
			D3DXVECTOR3 MoveVector(0,0,0);		
	
			if(rand()%100 > 40)
			{
				if(Game::Player->Position.x > this->Position.x)
					MoveVector.x += 1;
			
				if(Game::Player->Position.x < this->Position.x)
					MoveVector.x -= 1;
			}	
						
			if(rand()%100 > 40)
			{
				if(Game::Player->Position.y > this->Position.y)
					MoveVector.y += 1;
			
				if(Game::Player->Position.y < this->Position.y)
					MoveVector.y -= 1;
			}
			
			//adjust new position
			D3DXVec3Normalize(&MoveVector , &MoveVector);
			NewPos = Position + MoveVector*CurrentSpeed;

			
			if(fabsf(Game::Player->Position.x - NewPos.x) < CurrentSpeed)NewPos.x = Game::Player->Position.x - Game::Player->Facing*0.1f;
			if(fabsf(Game::Player->Position.y - NewPos.y) < CurrentSpeed)NewPos.y = Game::Player->Position.y - 0.1f;
			
			this->AdjustPositionToWorld(NewPos);
			bool IsPositionAcceptable = 1;
			for(size_t i = 0 ; i < Game::ObjectBuffer::Buffer.size() ; i++)
			{
				if(i == idx)continue;
				if(IsMisc(Game::ObjectBuffer::Buffer[i]->ObjectType))continue;
				IsPositionAcceptable = IsPositionAcceptable && !this->Collision(NewPos , i) ;
			}

			if(IsPositionAcceptable){Position = NewPos;}	
	}
	else
	{
		return OBJECT_IDLE;
	}

	if(Health < 0.0f)
	{
		State = 2;
		return OBJECT_IDLE;
	}

	if(Game::Player->Position.x > this->Position.x)
	{
		this->Facing = 1.0f;
	}
	else
	{
		this->Facing = -1.0f;
	}
	
	return OBJECT_IDLE;
}

/**************** Nomad *************/

GObjectEnemyNomad::GObjectEnemyNomad()
{
	ObjectType = OBJECT_ENEMY_NOMAD;
	Animations = Resources::Animations::Data[8];
	Center = Animations[0].Center;
	Radius = 27;
	State = PrevState = 0;
	BaseSpeed = CurrentSpeed = 6.70f;
	Health = 20.0f;
	Facing = 1.f;
}

DWORD GObjectEnemyNomad::Update(long idx)
{
	if(Health <= 0)State = 2;


	bool IsAttacking = State == 1;
	bool IsDieing = State == 2;

	float Distance = Vec3Distance(&Position , &Game::Player->Position);
	float Delta = fabsf(Position.x - Game::Player->Position.x);

	if(Position.x > Game::Player->Position.x)
	{
		Facing = -1.f;
	}
	else 
	{
		Facing = 1.f;
	}
	if(IsDieing)
	{
		Animations[State].ProcAnimation(Game::AnimationTicker::Tick);
		if(Animations[State].Ended)
		{
			Game::ObjectBuffer::SpawnObject(OBJECT_MISC_COIN , Position , 0);
			int i = rand()% 100 + 1 ;
			if(i > 93)
			{
					Game::ObjectBuffer::SpawnObject(OBJECT_MISC_AID , this->Position  , 0);
			}
			return OBJECT_DESTROY;
		}
		return OBJECT_IDLE;
	}

	if(IsAttacking)
	{
		Animations[State].ProcAnimation(Game::AnimationTicker::Tick);
		if(Animations[State].Ended)
		{
			Animations[State].ResetAnimation();
			if(Distance < Radius + Game::Player->Radius + 5)
			{
				Game::Player->Health -= 38.f;
				return OBJECT_IDLE;
			}
			State = 0;
		}
		return OBJECT_IDLE;
	}

	

	if(Delta > 410)return OBJECT_IDLE;

	if(Distance < Radius + Game::Player->Radius + 5)
	{
		State = 1;
		return OBJECT_IDLE;
	}

	//go to player and attack
	Animations[0].ProcAnimation(Game::AnimationTicker::Tick);
	D3DXVECTOR3 NewPos;
	D3DXVECTOR3 MoveVector(0,0,0);		
	
	if(rand()%100 > 40)
	{
		if(Game::Player->Position.x > this->Position.x)
			MoveVector.x += 1;
			
		if(Game::Player->Position.x < this->Position.x)
			MoveVector.x -= 1;
	}	
						
	if(rand()%100 > 40)
	{
		if(Game::Player->Position.y > this->Position.y)
			MoveVector.y += 1;
			
		if(Game::Player->Position.y < this->Position.y)
		MoveVector.y -= 1;
	}
			
	//adjust new position
	D3DXVec3Normalize(&MoveVector , &MoveVector);
	NewPos = Position + MoveVector*CurrentSpeed;

			
	if(fabsf(Game::Player->Position.x - NewPos.x) < CurrentSpeed)NewPos.x = Game::Player->Position.x - Game::Player->Facing*0.1f;
	if(fabsf(Game::Player->Position.y - NewPos.y) < CurrentSpeed)NewPos.y = Game::Player->Position.y - 0.1f;
			
	this->AdjustPositionToWorld(NewPos);
	bool IsPositionAcceptable = 1;
	for(size_t i = 0 ; i < Game::ObjectBuffer::Buffer.size() ; i++)
	{
		if(i == idx)continue;
		if(IsMisc(Game::ObjectBuffer::Buffer[i]->ObjectType))continue;
		IsPositionAcceptable = IsPositionAcceptable && !this->Collision(NewPos , i) ;
	}

	if(IsPositionAcceptable){Position = NewPos;}

	return OBJECT_IDLE;
}
