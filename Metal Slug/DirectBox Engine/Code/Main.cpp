#include "blackbox.h"

using namespace std;

int WINAPI WinMain(HINSTANCE hInst ,HINSTANCE hPrevInst ,
					LPSTR cmd,int ShowFlag)
{ 	
	
		srand(timeGetTime());
		//Read data 
		GlobalData::LoadDataFromIni();

		// Create the window
		GlobalData::Application = DBOXCreateApp(hInst);
		GlobalData::Application->CreateWindowApp("Metal Slug : United Force (admirer eddition)" , GlobalData::WindowWidth , GlobalData::WindowHeight , false);
		GlobalData::hWnd = GlobalData::Application->GetWindowHandle();

		//Intialize GF controller
		GlobalData::GFCtrl = DBOXCreate3D();
		GlobalData::GFCtrl->CreateDevice(DBOXSetPresentParameters(GlobalData::hWnd , GlobalData::WindowWidth , GlobalData::WindowHeight , D3DFMT_A8R8G8B8 , GlobalData::Windowed));
		GlobalData::d3ddev = GlobalData::GFCtrl->GetDevice();
		GlobalData::Sprite = GlobalData::GFCtrl->GetSprite();

		//CreateInputController
		GlobalData::Input = DBOXCreateInput(GlobalData::hWnd);
		GlobalData::UICtrl = DBOXCreateUI(GlobalData::Sprite , GlobalData::hWnd);
	
		Resources::Textures::Load(0);
		Resources::Animations::Load(0);
	
		Game::ObjectBuffer::ClearAll();
		SetActiveWindow(GlobalData::hWnd);

		//MainLoop
		while(GlobalData::Application->ProcessMessage() != WM_QUIT)
		{
			Game::AnimationTicker::Update();

			if(!Game::AnimationTicker::Tick)
			{
				Sleep(10);
				continue ;			
			}

			if(CheckDevice(0))continue;
			if(GlobalData::hWnd != GetActiveWindow())continue;
			HANDLE hInputThread = CreateThread(0 , 0 , (LPTHREAD_START_ROUTINE)InputThread  , 0 , 0 , 0);
		
			GlobalData::GFCtrl->BeginScene(0xFF000000);
			GameProc(0);
			GlobalData::GFCtrl->EndScene();

			WaitForSingleObject(hInputThread , 0);
			CloseHandle(hInputThread);
		}

		//save data
		GlobalData::SaveDataToIni();
		GlobalData::GFCtrl->Release();

	return 0;
}