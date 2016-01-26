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
		return sqrtf( SQR(o1->x - o2->x) + SQR(o1->y - o2->y));
	}

}

/* ************************************************************************************************************************************ */
//GObjectBackground

GObjectBackground::GObjectBackground()
{
	Position = D3DXVECTOR3(9999 , 9999 , 0);
	Radius = 0;
	ObjectType = OBJECT_BACKGROUND_TEST;
	Animations = Resources::Animations::Data[1];
	State = PrevState = 0;
	Facing = 1.0f;
}

DWORD GObjectBackground::Update(long idx)
{
	//is in the scene 
	BOOL IsInTheScene = 1;

	D3DXMATRIX CameraTransformInverse;
	D3DXMATRIX ObjTransl , ObjScale;

	D3DXMatrixTranslation(&CameraTransformInverse , -(Game::ObjectBuffer::Render::CameraPosition.x - (float)GlobalData::WindowWidth/2.0f), -(Game::ObjectBuffer::Render::CameraPosition.y - (float)GlobalData::WindowHeight/2.0f) , 0.0f);

	D3DXMatrixTranslation(&ObjTransl , TopLeftPosition.x , TopLeftPosition.y , 0.0f);
	D3DXMatrixScaling(&ObjScale , Animations[0].Right*Facing , 1.0f , 1.0f);

	GlobalData::Sprite->Begin(D3DXSPRITE_ALPHABLEND);
	GlobalData::Sprite->SetTransform(&(ObjScale*ObjTransl*CameraTransformInverse));
	GlobalData::Sprite->Draw(Animations[0].Texture , &Animations[0].Data[0] , &D3DXVECTOR3(0,0,0) , &D3DXVECTOR3(0,0,0) , 0xffffffff);
	GlobalData::Sprite->SetTransform(&DBOXMatIdent);
	GlobalData::Sprite->End();
	return OBJECT_IDLE;
}

/* ************************************************************************************************************************************ */
//GObjectBullet

BOOL GObjectBullet::Collision(D3DXVECTOR3 NewPosition,long ObjectIndex)
{
	BOOL Result = 0;
	GObject* Obj2 = Game::ObjectBuffer::Buffer[ObjectIndex];
	
	if(IsMisc(Obj2->ObjectType)) return 0 ;

	int distance = (int)sqrt((float)((NewPosition.x - Obj2->Position.x)*(NewPosition.x - Obj2->Position.x) + (NewPosition.y - Obj2->Position.y)*(NewPosition.y - Obj2->Position.y)));
	Result = distance < (int)(Radius + Obj2->Radius) ; 

	return Result ;
}

//GObjectMachinegunBullet

GObjectBulletMachinegun::GObjectBulletMachinegun()
{
	ObjectType = OBJECT_BULLET_MACHINEGUN;
	Radius = 5;
	Power = 9.33f;//normal power
	BaseSpeed = 17.9f;
	CurrentSpeed = BaseSpeed;
	Animations = Resources::Animations::Data[2];
	State = PrevState = 0;//const for that object
}


DWORD GObjectBulletMachinegun::Update(long idx)
{
	float Delta = fabs(Game::Player->Position.x - this->Position.x);
	if(Delta > 365.0f)
	{
		return OBJECT_DESTROY;
	}

	Position.x += Facing*CurrentSpeed;

	for(size_t i = 0 ; i < Game::ObjectBuffer::Buffer.size() ; i++)
	{
		if(i == idx)continue;
		if(Collision(Position , i))
		{
			Game::ObjectBuffer::Buffer[i]->Health -= Power;
			Game::LastDamage = Power;
			return OBJECT_DESTROY;
		}
	}

	return OBJECT_IDLE;
}

//GObjectBulletPmp

GObjectBulletPmp::GObjectBulletPmp()
{
	ObjectType = OBJECT_BULLET_PMP;
	Radius = 51.1f;
	Power = 66.0f;//normal power
	BaseSpeed = 0.0f;
	CurrentSpeed = BaseSpeed;
	Animations = Resources::Animations::Data[2];
	State = PrevState = 1;//const for that object
}

DWORD GObjectBulletPmp::Update(long idx)
{
	this->Animations[State].ProcAnimation(Game::AnimationTicker::Tick);

	if(this->Animations[State].CurrentFrame == 1 )
	{
		//do the dmg and destroy obj

		for(size_t i = 0; i < Game::ObjectBuffer::Buffer.size() ; i++)
		{
			GObject* Obj = Game::ObjectBuffer::Buffer[i];
			if(IsPlayer(Obj->ObjectType) || IsBullet(Obj->ObjectType)) continue;

			float Distance = Vec3Distance(&this->Position , &Obj->Position);
			if(Distance < this->Radius + Obj->Radius )
			{
				Obj->Health -= Power;
				Game::LastDamage = Power;
			}
		}

	}

	if(this->Animations[State].Ended)
	{
		this->Animations[State].ResetAnimation();
		return OBJECT_DESTROY;
	}

	return OBJECT_IDLE;
}

//Grenade

GObjectBulletGrenade::GObjectBulletGrenade()
{
	ObjectType = OBJECT_BULLET_GRENADE ; 
	Radius = 0;
	Animations = Resources::Animations::Data[2];
	State = PrevState =  2;
	Center = Animations[State].Center;

}

DWORD GObjectBulletGrenade::Update(long idx)
{
	this->Animations[State].ProcAnimation(Game::AnimationTicker::Tick);

	if(this->Animations[State].Ended)
	{
		this->Animations[State].ResetAnimation();
		D3DXVECTOR3 ExplosionPos;
		ExplosionPos.x = Position.x + 38.0f*Facing;
		ExplosionPos.y = Position.y - 21.f;
		Game::ObjectBuffer::SpawnObject(OBJECT_EXPLOSION_SMALL1 , ExplosionPos , 0);
		return OBJECT_DESTROY;
	}

	return OBJECT_IDLE;
}

//GObjectMachinegunBullet

GObjectBulletSniper::GObjectBulletSniper()
{
	ObjectType = OBJECT_BULLET_SNIPER;
	Radius = 3;
	Power = 70.33f;//normal power
	BaseSpeed = 5.59f;
	CurrentSpeed = BaseSpeed;
	Animations = Resources::Animations::Data[7];
	State = PrevState = 0;//const for that object
}


DWORD GObjectBulletSniper::Update(long idx)
{
	float Distance = Vec3Distance(&Game::Player->Position , &this->Position);
	Position.x += Facing*BaseSpeed;
	if(fabsf(Position.x - Game::Player->Position.x) > 480.0f)
	{
		return OBJECT_DESTROY;
	}
	else if(Game::Player->Radius + this->Radius  >= Distance)
	{
		Game::Player->Health -= this->Power;
		return OBJECT_DESTROY;
	}

	return OBJECT_IDLE;
}