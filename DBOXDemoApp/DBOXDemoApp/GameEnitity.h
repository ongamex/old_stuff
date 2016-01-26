#pragma once

#include "BackBox.h"
#include "DBOXMath.h"

class BulletObj;

enum Type{
tp_player,
tp_platform,
tp_bullet,
};

class Enitity{
public : 

	Type type;
	D3DXVECTOR3 Position;
	fRect BoundingBox;
	bool AllowJump ;

	virtual void Initialize() {};
	virtual void Update(int idx) {};

};

class BulletObj : public Enitity{
public : 
	D3DXVECTOR3 Dir;
	float Speed;
	Animation2DAnimInfo IdleInfo;
	void Initialize()
	{
		type = tp_bullet;
		IdleInfo.m_CenterType = DBOX_CENTER_CENTER;
		IdleInfo.m_ElapsedTime = 0;
		Speed = 300.f;
		BoundingBox = fRect(-8,-8,8,8);
	}

	void Update(int idx)
	{
		Position += Dir*Speed*Timer.fDelta;
		///Position.y += 140.0f*Timer.fDelta;
		Speed -= 8.f*Timer.fDelta;
		if(Speed < 0)Speed = 0;
		Dir.y += 1.f*Timer.fDelta;

		D3DXMATRIX Transform;
		D3DXMatrixTranslation(&Transform, Position.x, Position.y , 0);

		Animation2DAnimate(&BulletIdle, &IdleInfo, Timer.fDelta, Engine2D,& Transform, true);
	}
	
};

class PlayerObj : public Enitity{

public  :

	Animation2DAnimInfo TorsoInfo;
	Animation2DAnimInfo LegsInfo;
	float Facing;
	float JumpTime;
	
	void Initialize()
	{
		type = tp_player;
		Position = D3DXVECTOR3(0,0,0);

		TorsoInfo.m_CenterType = DBOX_CENTER_CENTER;
		TorsoInfo.m_ElapsedTime = 0.f;

		LegsInfo.m_CenterType = DBOX_CENTER_CENTER;
		LegsInfo.m_ElapsedTime = 0.f;

		BoundingBox = fRect(-64,-64 , 64,64);
		Facing = 1.f;
		JumpTime = 0;
		AllowJump = false;
	}

	void Update(int idx)
	{
		D3DXVECTOR3 OldPos = Position;
		bool Moving = false;
		if(Input->IsKeyDown(DIK_A)){ Position.x -= 70.f*Timer.fDelta;Facing = -1.f;Moving = true;}
		if(Input->IsKeyDown(DIK_D)){ Position.x += 70.f*Timer.fDelta;Facing = 1.f;Moving = true;}
		//if(Input->IsKeyDown(DIK_W)){ Position.y -= 50.f*Timer.fDelta;Moving = true;}
		//if(Input->IsKeyDown(DIK_S)){ Position.y += 50.f*Timer.fDelta;Moving = true;}
		if(Input->IsKeyClicked(DIK_SPACE)){

			if(JumpTime <= 0.f && AllowJump)
				JumpTime = 0.61f;
		}
		
		if(JumpTime > 0.f)
		{
			Position.y -= 90.f*Timer.fDelta;
			JumpTime -= Timer.fDelta;
		}
		else{
			AllowJump = false;
			Position.y += 110.0f*Timer.fDelta;
		}

		Engine2D->MoveCameraTo(&Position);

		
		D3DXMATRIX Transform;
		D3DXMatrixTranslation(&Transform, Position.x, Position.y , 0);
		Transform._11 = Facing;
		if(Moving)
			Animation2DAnimate(&PlayerLegsWalk, &LegsInfo, Timer.fDelta, Engine2D,& Transform, true);
		else 
			Animation2DAnimate(&PlayerLegsIdle, &LegsInfo, Timer.fDelta, Engine2D,& Transform, true);

		D3DXMATRIX TT,TS;
			
		D3DXVECTOR3 MousePos = Input->GetMousePosClient();
		MousePos.x -= Engine2D->ScreenWidth/2;
		MousePos.y -= Engine2D->ScreenHeight/2;
		MousePos.z = 0;
		D3DXVec3Normalize(&MousePos, &MousePos);
		float Angle = 0;
			
		float Dot = D3DXVec3Dot(&D3DXVECTOR3(0,1,0), &MousePos);
		Angle = acosf(Dot);
		if(Angle < D3DX_PI/6.f) Angle = D3DX_PI/6;
		if(Angle > D3DX_PI - D3DX_PI/6) Angle = D3DX_PI - D3DX_PI/6;
		Angle -= D3DX_PI/2.f;
		if(MousePos.x > 0) Angle = -Angle;

		D3DXMatrixTranslation(&Transform, Position.x, Position.y , 0);
		D3DXMatrixRotationZ(&TT, Angle);
		D3DXMatrixScaling(&TS, MousePos.x/fabsf(MousePos.x), 1, 1);

		TT = TS*TT ;

		Animation2DAnimate(&PlayerTorsoIdle, &TorsoInfo, Timer.fDelta, Engine2D, &(TT*Transform), true);

		if(Input->IsKeyDown(DIK_LMB))
		{
			BulletObj* obj = new BulletObj;

			obj->Initialize();
			obj->Position = Position;
			Angle -= D3DX_PI/2.f;
			D3DXVECTOR3 MousePos = Input->GetMousePosClient();
			MousePos.x -= Engine2D->ScreenWidth/2;
			MousePos.y -= Engine2D->ScreenHeight/2;
			MousePos.z = 0;
			//D3DXVec3Normalize(&MousePos, &MousePos);
			MousePos.x += 1;
			MousePos.y += 1;
			MousePos.x /= 130.f;
			MousePos.y /= 130.f;
			obj->Dir = MousePos;

			ObjBuff.push_back(obj);
		}

	}
};

struct PlatformObj : public Enitity{

	Animation2DAnimInfo IdleInfo;

	void Initialize()
	{
		type = tp_platform;
		Position = D3DXVECTOR3(0,  + 128.f, 0);
		BoundingBox = fRect(-128, -32, 128 , 32);

		IdleInfo.m_CenterType = DBOX_CENTER_CENTER;
		IdleInfo.m_ElapsedTime = 0.f;
	}
	void Update(int idx)
	{
		D3DXMATRIX Transform;
		D3DXMatrixTranslation(&Transform, Position.x, Position.y , 0);
		
		for(int i = 0; i < ObjBuff.size(); i++)
		{
			if (idx == i)continue;

			fRect myTr = MovefRect(&BoundingBox, &Position);
			fRect elTr = MovefRect(&ObjBuff[i]->BoundingBox, &ObjBuff[i]->Position);
			D3DXVECTOR3 Dir;
			if(OverlapfRect(&elTr, &myTr, &Dir))
			{
				ObjBuff[i]->AllowJump = true;

				if(ObjBuff[i]->type == tp_bullet)
				{
					if(Dir.y != 0)
					((BulletObj*)ObjBuff[i])->Dir.y = -((BulletObj*)ObjBuff[i])->Dir.y*0.80f;
					if(Dir.x != 0)
					((BulletObj*)ObjBuff[i])->Dir.x = -((BulletObj*)ObjBuff[i])->Dir.x*0.90;
				}
			}
			ObjBuff[i]->Position += Dir;
		}

		Animation2DAnimate(&PlatformIdle, &IdleInfo, Timer.fDelta, Engine2D,& Transform, true);
	}

};

