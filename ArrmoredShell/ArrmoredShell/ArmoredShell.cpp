#include "BlackBox.h"

IDirect3DTexture9* pPointTex;
std::vector<D3DXVECTOR3> PointArr;

float IntNoise(int x)			 
{
    x = (x<<13) ^ x;
    return ( 1.f - ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.f);    
}

void Generate()
{
	for(int i = 0; i < 6000; i+= 1){
		float t = IntNoise(i)*200.f;
		PointArr.push_back(D3DXVECTOR3((float)i*20.f, t, 1.f));
	}

}

void Smooth(){
	
	for(int i = 1; i < PointArr.size() - 1; i++){
		
		PointArr[i].y = (PointArr[i-1].y + PointArr[i].y  + PointArr[i+1].y)/3.f;
	}
}


class ArmoredShellApplication : public DBOXApplication {

	//Addition message handler
	LRESULT MessageHandler(UINT msg , WPARAM wp , LPARAM lp)
	{
		return DefWindowProc(m_hWnd, msg, wp, lp);
	}

	//OnCreate
	void OnCreate()
	{
		//Create The data mutex 
		System::DataMutex = CreateMutex(NULL, FALSE, __TEXT("DataMutex"));//Inital owned by main thread

		//prepare object 
		System::GameTimer.Initialize();

		//Create the rendering device
		D3DPRESENT_PARAMETERS d3dpp = {0};

		d3dpp.Windowed = m_Windowed;
		if(!d3dpp.Windowed){
			d3dpp.BackBufferWidth = 1280;
			d3dpp.BackBufferHeight = 720;
			d3dpp.BackBufferCount = 1;
		}

		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

		d3dpp.hDeviceWindow = m_hWnd;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

		System::Ri3D = DBOXCreate3D();
		System::Ri3D->CreateDevice(d3dpp);

		System::Ri = DBOXCreate2D(System::Ri3D);
		System::Ri->SetUp(1280, 720, (float)1.f, (float)1.f);

		System::Input = DBOXCreateInput(m_hWnd);

		pPointTex = System::Ri3D->CreateTextureFromFile("point.png", 0xffff00ff);

		Generate();
		
		return;
	}
	
	//Handles all the logic
	
	void Run()
	{

		if(System::Input->IsKeyDown(DIK_R)) Smooth();

		System::GameTimer.Update();
		System::Input->UpdateDevices();

		System::Ri3D->BeginScene(0xffaaaaaa);

		D3DXMATRIX transf;
		for(int i = 0; i < PointArr.size() ; i++){
			
			D3DXMatrixTranslation(&transf, PointArr[i].x, PointArr[i].y, 0.f);
		
			System::Ri->DrawToWorld(pPointTex, &transf, nullptr, DBOX_CENTER_CENTER, nullptr);
			if(System::Input->IsKeyDown(DIK_LMB))
				System::Ri->MoveCameraBy( &(-System::Input->GetMouseMoveVector()/4420.f) );
		}

		System::Ri3D->EndScene();
	}

	//Exit application
	virtual void OnDestroy()
	{
		Settings::Export();

#ifdef _DEBUG
		printf("\n\tApplcation::Destroy successful"); 
#endif
	}
};

//#define _CONSOLE
#ifndef _DEBUG
	#pragma comment( linker, "/subsystem:windows" ) 
	int WINAPI WinMain(HMODULE, HMODULE, LPSTR , int)
#else
	//if debug show console for debug data : 
	#pragma comment( linker, "/subsystem:console" ) 
	int main(int argc, char* argv[] ) 
#endif
//MAIN function goes here
{
	//Load settings
		Settings::Import();

	System::pApp = new ArmoredShellApplication;
#ifndef _DEBUG
	System::pApp->Create(__TEXT("ArromoredShell"), Settings::Windowed, 1280, 720);
#else
	System::pApp->Create(__TEXT("ArromoredShell-Debug"), Settings::Windowed, Settings::Width, Settings::Height);
#endif

	while(System::pApp->Loop()){//Update game
		//addition Logic
	}

	return 0;
}