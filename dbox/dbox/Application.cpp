#include <iostream>
#include <memory.h>
#include "Application.h"
#include <string>
#include <process.h>
#include "XEnitity.h"

LPDIRECT3DDEVICE9 g_pd3dDevice;

LPDBOX3D Engine3D = NULL;
LPDBOX2D Engine2D = NULL;
LPDBOXINPUT Input = NULL;
DBOXCameraTP Camera;

XEnitity pEnitity,Success;

LPDBOXFONT Font = NULL;
DBOXTimer Timer;

void MoveCamera()
{
	D3DXVECTOR3 mv = Input->GetMouseMoveVector();

	if(Input->IsKeyDown(DIK_RMB))
	{
		Camera.Yaw(mv.x*0.002f);
		Camera.Pitch(mv.y*0.002f);
	}

	static float radius = 650;
	radius -= mv.z*0.6f;
	if(radius < 5.f)radius = 3.f;

	Camera.Radius = radius;
	Camera.SetTransforms();

	return;
}

void RenderXYZ()
{
	struct DirV{
		float x,y,z;DWORD col;DirV(){}
		DirV(float _x,float _y,float _z , DWORD _col) : x(_x), y(_y) , z(_z), col(_col){}
	};

	DirV a[] = {	
		DirV(0,0,0, D3DCOLOR_XRGB(255,0,0)),//x
		DirV(1000,0,0, D3DCOLOR_XRGB(255,0,0)),
		DirV(0,0,0,	D3DCOLOR_XRGB(0,255,0)),//y
		DirV(0,1000,0,	D3DCOLOR_XRGB(0,255,0)),
		DirV(0,0,0,	D3DCOLOR_XRGB(0,0,255)),//z
		DirV(0,0,1000,	D3DCOLOR_XRGB(0,0,255))};
		
	Engine3D->GetDevice()->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
	Engine3D->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	Engine3D->GetDevice()->SetTransform(D3DTS_WORLD, &DBOXMatIdent);
	Engine3D->GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST, 3, a, sizeof(DirV));
	Engine3D->GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);

	g_pd3dDevice = Engine3D->GetDevice();

	return;
}

class MyApplication : public DBOXApplication
{
	LRESULT MessageHandler(UINT msg , WPARAM wp , LPARAM lp)
	{
		return DefWindowProc(m_hWnd, msg, wp, lp);
	}

	void OnCreate()
	{
		D3DPRESENT_PARAMETERS d3dpp = {0};
		d3dpp.BackBufferCount = 1;
		d3dpp.BackBufferWidth = m_Width;
		d3dpp.BackBufferHeight = m_Height;
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		d3dpp.Windowed = TRUE;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.hDeviceWindow = m_hWnd;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		Engine3D = DBOXCreate3D();
		Engine3D->CreateDevice(d3dpp);
		Engine2D = DBOXCreate2D(Engine3D);
		Engine2D->SetUp(this->m_Width, this->m_Height, 1.f , 1.f);

		Engine3D->GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);
		Engine3D->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		D3DLIGHT9 Light;
		D3DMATERIAL9 Mat = {0};

		memset(&Light, 0, sizeof(Light));

		Light.Type = D3DLIGHT_DIRECTIONAL;
		Light.Direction = D3DXVECTOR3(0, -0.707f , +0.707f);
		Light.Diffuse.r = Light.Diffuse.g = Light.Diffuse.b = Light.Diffuse.a = 1.f;
		Light.Ambient = Light.Diffuse;
		Light.Range = 10000.f;

		Mat.Diffuse = Light.Diffuse;
		Mat.Ambient.r = Mat.Ambient.g = Mat.Ambient.b = 0.3f;
		Mat.Ambient.a = 1.f;

		Engine3D->GetDevice()->SetLight(0, &Light);
		Engine3D->GetDevice()->SetMaterial(&Mat);

		Engine3D->GetDevice()->LightEnable(0, TRUE);

		Camera.Create(Engine3D->GetDevice(), D3DXVECTOR3(0,0,0), 1400.f);
		
		Font = DBOXCreateFont(__TEXT("Arial") , Engine3D->GetDevice() , 12 , 19 , 700 ,0xffffffff);
		
		Input = DBOXCreateInput(m_hWnd);

		Timer.Initialize();

		pEnitity.Create( Engine3D->GetDevice() );
		
		pEnitity.Load("ArmyMen.X");
		pEnitity.m_SpeedAdjust = 1.f;
		pEnitity.Clone(&Success);
		pEnitity.SetAnimationSet(0);
		Success.SetAnimationSet(0);
		return;
	}

	void Run()
	{
		Timer.Update();
		Input->UpdateDevices();

		Engine3D->BeginScene(0xff909090);
		if(DBOX3D_DEVSTATE_GOOD != Engine3D->HandleLostDevice(200, Engine3D->GetPresentParameters()))return;
		//Font->DrawText(val2tstr(floorf(Timer.fps)), 0, 0);
		
		Engine2D->MoveCameraTo(&D3DXVECTOR3(0,0,0));
	
		MoveCamera();
		RenderXYZ();

		static int i = 0, j = 0;

		static D3DXMATRIX t;
		D3DXMatrixTranslation(&t, 13, 0,0);

		if(Input->IsKeyClicked(DIK_SPACE)){i++;pEnitity.SetAnimationSet(i);}
		if(Input->IsKeyClicked(DIK_A)){j++;Success.SetAnimationSet(j);}	
	
		pEnitity.FrameMove(Timer.fDelta, &DBOXMatIdent);
		pEnitity.Render();

		Success.FrameMove(Timer.fDelta, &t);
		Success.Render();
		
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
	App->Create(__TEXT("Application"), true, 1024, 768);
	
	while(App->Loop());

	return 0;
}