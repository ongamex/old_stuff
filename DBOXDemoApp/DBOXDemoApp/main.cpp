#include "BackBox.h"
#include "GameEnitity.h"

LPDBOX3D Engine3D = NULL;
LPDBOX2D Engine2D = NULL;
LPDBOXINPUT Input = NULL;
DBOXTimer Timer;

std::vector<Enitity*> ObjBuff;

Animation2D PlayerLegsWalk,
					PlayerTorsoIdle,
					PlayerLegsIdle,
					PlatformIdle,
					BulletIdle;

PlayerObj* pPlayer;

IDirect3DTexture9* pCursor;

void Load2DAnimations()
{
	
	//PLAYER LEGS WALK
	PlayerLegsWalk.m_Name = "walk";
	PlayerLegsWalk.m_Animations.resize(3);
	PlayerLegsWalk.m_FramesDuration.resize(3);
	PlayerLegsWalk.z = 0.4f;

	IDirect3DTexture9* pTex = Engine3D->CreateTextureFromFile("walk1.png", 0xffff00ff);
	PlayerLegsWalk.m_Animations[0] = new Frame2D;
	PlayerLegsWalk.m_Animations[0]->m_pArea = NULL;
	PlayerLegsWalk.m_Animations[0]->m_pTexture = pTex;
	PlayerLegsWalk.m_FramesDuration[0] = .33f;

	pTex = Engine3D->CreateTextureFromFile("walk2.png", 0xffff00ff);
	PlayerLegsWalk.m_Animations[1] = new Frame2D;
	PlayerLegsWalk.m_Animations[1]->m_pArea = NULL;
	PlayerLegsWalk.m_Animations[1]->m_pTexture = pTex;
	PlayerLegsWalk.m_FramesDuration[1] = .33f;

	pTex = Engine3D->CreateTextureFromFile("walk3.png", 0xffff00ff);
	PlayerLegsWalk.m_Animations[2] = new Frame2D;
	PlayerLegsWalk.m_Animations[2]->m_pArea = NULL;
	PlayerLegsWalk.m_Animations[2]->m_pTexture = pTex;
	PlayerLegsWalk.m_FramesDuration[2] = .33f;

	//PLAYER TORSO IDLE
	PlayerTorsoIdle.m_Animations.resize(1);
	PlayerTorsoIdle.m_FramesDuration.resize(1);
	PlayerTorsoIdle.m_FramesDuration[0] = 0.4f;
	
	pTex = Engine3D->CreateTextureFromFile("torso.png", 0xffff00ff);
	PlayerTorsoIdle.m_Animations[0] = new Frame2D;
	PlayerTorsoIdle.m_Animations[0]->m_pTexture = pTex;
	PlayerTorsoIdle.m_Animations[0]->m_pArea = NULL;

	//PLAYER LEGS IDLE

	PlayerLegsIdle.m_Animations.resize(1);
	PlayerLegsIdle.m_FramesDuration.resize(1);

	PlayerLegsIdle.m_Animations[0] = new Frame2D;
	PlayerLegsIdle.m_Animations[0]->m_pTexture = PlayerLegsWalk.m_Animations[0]->m_pTexture;
	PlayerLegsIdle.m_FramesDuration[0] = 0.f;
	PlayerLegsIdle.m_Animations[0]->m_pArea = NULL;
	PlayerLegsIdle.z = 0.4f;

	//Platform

	PlatformIdle.m_Animations.resize(1);
	PlatformIdle.m_FramesDuration.resize(1);
	PlatformIdle.z = 0.2f;
	PlatformIdle.m_Name = "platformIdle";

	pTex = Engine3D->CreateTextureFromFile("platform.png", 0xffff00ff);
	PlatformIdle.m_Animations[0] = new Frame2D;
	PlatformIdle.m_Animations[0]->m_pTexture = pTex;
	PlatformIdle.m_FramesDuration[0] = 0.f;	
	PlatformIdle.m_Animations[0]->m_pArea = NULL;

	//bullet

	BulletIdle.m_Animations.resize(2);
	BulletIdle.m_FramesDuration.resize(2);
	BulletIdle.m_Name = "Idle";
	BulletIdle.z = 0.1f;

	pTex = Engine3D->CreateTextureFromFile("bullet.png", 0xffff00ff);
	BulletIdle.m_Animations[0] = new Frame2D;
	BulletIdle.m_Animations[0]->m_pTexture = pTex;
	BulletIdle.m_Animations[0]->m_pArea = new lRect(0,0,15,15);
	BulletIdle.m_FramesDuration[0] = .1f;
	
	BulletIdle.m_Animations[1] = new Frame2D;
	BulletIdle.m_Animations[1]->m_pTexture = pTex;
	BulletIdle.m_Animations[1]->m_pArea = new lRect(16,0,31,15);
	BulletIdle.m_FramesDuration[1] = 0.8f;

};

class MyApplication : public DBOXApplication
{
	LRESULT MessageHandler(UINT msg , WPARAM wp , LPARAM lp)
	{
		return DefWindowProc(m_hWnd, msg, wp, lp);
	}

	void OnCreate()
	{
		ShowCursor(false);
		D3DPRESENT_PARAMETERS d3dpp = {0};
		d3dpp.Windowed = TRUE;
		if(!d3dpp.Windowed)
		{
			d3dpp.BackBufferCount = 1;
			d3dpp.BackBufferWidth = m_Width;
			d3dpp.BackBufferHeight = m_Height;
		}

		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		d3dpp.EnableAutoDepthStencil = TRUE;
		
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.hDeviceWindow = m_hWnd;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
	//	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		
		Engine3D = DBOXCreate3D();
		Engine3D->CreateDevice(d3dpp);
		Engine3D->GetDevice()->SetRenderState(D3DRS_ZENABLE, FALSE);
		Engine3D->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
		Engine3D->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		Engine2D = DBOXCreate2D(Engine3D);
		Engine2D->SetUp(this->m_Width, this->m_Height, 1.f , 1.f);
	
		Input = DBOXCreateInput(m_hWnd);
		Timer.Initialize();

		Load2DAnimations();

		pCursor = Engine3D->CreateTextureFromFile("cursor.png", 0xffff00ff);

		pPlayer = new PlayerObj;
	
		pPlayer->Initialize();

		PlatformObj* pPlatform = new PlatformObj;
		pPlatform->Initialize();

		ObjBuff.clear();
		ObjBuff.push_back(pPlayer);
		ObjBuff.push_back(pPlatform);

		pPlatform = new PlatformObj;
		pPlatform->Initialize();
		pPlatform->Position = D3DXVECTOR3(-256, 128,0);
		ObjBuff.push_back(pPlatform);

		pPlatform = new PlatformObj;
		pPlatform->Initialize();
		pPlatform->Position = D3DXVECTOR3(512, 128,0);
		ObjBuff.push_back(pPlatform);

		pPlatform = new PlatformObj;
		pPlatform->Initialize();
		pPlatform->Position = D3DXVECTOR3(256, 128,0);
		ObjBuff.push_back(pPlatform);
		return;
	}

	void Run()
	{

		SetWindowText(m_hWnd, ("DBOXApplication fps:"+val2tstr(floorf(Timer.fps))).c_str());

		Timer.Update();
		Input->UpdateDevices();
		if(Timer.fDelta > 10.f)return;
		Engine3D->BeginScene(0xff909090);
		{
			for(int i = 0; i < ObjBuff.size(); i++)
			{
				ObjBuff[i]->Update(i);
			}
		}

		D3DXMATRIX t;
		D3DXMatrixTranslation(&t, Input->GetMousePosClient().x, Input->GetMousePosClient().y , 0.f);

		Engine2D->DrawToScreen(pCursor, &t,0,DBOX_CENTER_CENTER, 0);

		Engine3D->EndScene();
	}

	void OnDestroy()
	{
		Engine3D->Release();
	}
};

//#define _CONSOLE
#ifndef _CONSOLE
#pragma comment( linker, "/subsystem:windows" ) 
int WINAPI WinMain(HMODULE, HMODULE, LPSTR , int)
#else
#pragma comment( linker, "/subsystem:console" ) 
int main(int argc, char* argv[] ) 
#endif
{
	MyApplication* App = new MyApplication;
	App->Create(__TEXT("Application"), true, 1280, 720);
	
	while(App->Loop());

	return 0;
}