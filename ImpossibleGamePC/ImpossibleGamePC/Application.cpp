#include "Blackbox.h"

void Set3DSettings()
{
	Engine3D->GetDevice()->SetRenderState(D3DRS_ZENABLE,TRUE);
	Engine3D->GetDevice()->SetRenderState(D3DRS_LIGHTING,TRUE);

	Engine3D->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	Engine3D->GetDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	Engine3D->GetDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	D3DLIGHT9 light;
	ZeroMemory( &light, sizeof(D3DLIGHT9) );
	light.Type       = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r  = 0.9f;
	light.Diffuse.g  = 0.9f;
	light.Diffuse.b  = 0.9f;
	light.Diffuse.a  = 1.0f;
	light.Ambient.r = light.Diffuse.r*0.3f;
	light.Ambient.g = light.Diffuse.g*0.3f;
	light.Ambient.b = light.Diffuse.b*0.3f;
	light.Ambient.a = light.Diffuse.a;
	light.Range      = 1000.0f;



	D3DXVECTOR3 vecDir;
	vecDir = D3DXVECTOR3(0.0f,-0.3f,0.5);
	D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );

	Engine3D->GetDevice()->SetLight(0 , &light);
	Engine3D->GetDevice()->LightEnable(0 , 1);
}

void OnResize()
{
		RECT crc;
		D3DPRESENT_PARAMETERS pp = Engine3D->GetPresentParameters();
		
		GetClientRect(pp.hDeviceWindow , &crc);

		GameProperties::Width = crc.right;
		GameProperties::Height = crc.bottom;	
		
		Engine3D->GetDevice()->Reset(&DBOXSetPresentParameters(pp.hDeviceWindow , GameProperties::Width ,GameProperties::Height ,pp.BackBufferFormat , GameProperties::Windowed));

		Engine3D->GetSprite()->OnLostDevice();
		Engine3D->GetSprite()->OnResetDevice();
		
		Engine2D->SetUp(GameProperties::Width, GameProperties::Height, Engine2D->ScaleX , Engine2D->ScaleY);
		Set3DSettings();
		UIm.UpdateContent();
}

int HandleOnLoadOnReset()
{
		HRESULT hr = Engine3D->HandleLostDevice(30);
		if(hr == DBOX3D_DEVSTATE_LOST)
		{
			return 1 ;
		}
		else if(hr == DBOX3D_DEVSTATE_RESETED)
		{
			Set3DSettings();
			UIm.UpdateContent();//resize the UI
			OnResize();
			return 1 ;
		}
		return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd , UINT msg , WPARAM wp , LPARAM lp)
{
	switch(msg)
	{
	case WM_CLOSE :
	case WM_QUIT : PostQuitMessage(0);
	break;
	case WM_SIZE :if(Engine3D)OnResize();break;
	default : break;
	}
	return DefWindowProc(hWnd, msg , wp , lp);
}



int WINAPI WinMain(HINSTANCE hInst , HINSTANCE hPrevInst , LPSTR , int) 
{
	GameState = &gsLoad;
	//load the user data
	Profiles::Load();
	GameProperties::Load() ;

	App = DBOXCreateApp(hInst);
	App->CreateWindowApp("ImpossibleGame for PC" , GameProperties::Width , GameProperties::Height ,! GameProperties::Windowed , WndProc);
	Engine3D = DBOXCreate3D();
	Engine3D->CreateDevice(DBOXSetPresentParameters(App->GetWindowHandle() , GameProperties::Width , GameProperties::Height , D3DFMT_A8R8G8B8 , GameProperties::Windowed));
	Engine2D = DBOXCreate2D(Engine3D);
	Input = DBOXCreateInput(App->GetWindowHandle());
	Engine2D->SetUp(GameProperties::Width , GameProperties::Height , 1.f , 1.f);
	Set3DSettings();
	SetupUI();

	//sound engine
	//SndEngine = irrklang::createIrrKlangDevice();
	//SndEngine->play2D("song.mp3" , true);
	//Load Meshes
	Meshes[0].Load(Engine3D->GetDevice() , "models\\cube.X");
	Meshes[1].Load(Engine3D->GetDevice() , "models\\cone.X");
	Meshes[2].Load(Engine3D->GetDevice() , "models\\stair.X");
	Meshes[3].Load(Engine3D->GetDevice() , "models\\creep.X");
	Meshes[4].Load(Engine3D->GetDevice() , "models\\line.X");
	//Meshes[5].Load(Engine3D->GetDevice() , "models\\line.X");
	Meshes[6].Load(Engine3D->GetDevice() , "models\\star.X");
	Meshes[7].Load(Engine3D->GetDevice() , "models\\check.X");
	//Create the camera

	Camera = DBOXCreateSimpleCameraTP(Engine3D->GetDevice() , D3DXVECTOR3(0,0,0) , 102.f);
	Camera->Yaw(0.12f);
	Camera->Pitch(-D3DX_PI/8.1f);

	FilterTexture = Engine3D->CreateTextureFromFile("ScrGradient.png" , 2048 , 1024 , 0xffff00ff);
	Particles::Stars::Star = Engine3D->CreateTextureFromFile("particle\\star_particle.png" , 32 , 32 , 0xffff00ff);

	DWORD LastTime = timeGetTime();
	TimeDelta = timeGetTime();
	Delta = 0.f;
	LARGE_INTEGER f;
	while(App->ProcessMessage())
	{
		QueryPerformanceCounter(&f);
		OutputDebugString(ToString(f.HighPart).c_str());
		OutputDebugString(ToString("\n").c_str());
		TimeDelta = timeGetTime()-LastTime;
		LastTime = timeGetTime();

		if(App->GetWindowHandle()!=GetActiveWindow())continue;
		if(HandleOnLoadOnReset())continue;

		//if(Engine3D->LimitFrames(60)){continue;}

		Engine3D->BeginScene(Level::ClearColor);
		Input->UpdateDevices();
		GameState->Update();
		Engine3D->EndScene();
		
		Delta = (float)(TimeDelta)/1000.f;
	}

	Engine3D->Release();
	App->Release();

	//save user data at exit
	Profiles::Save();
	GameProperties::Save() ;
	return 0;
}