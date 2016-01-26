#include "Blackbox.h" 
//Create the GameSate Global;

//GameState

bool ChangeState(CGameState* pState)
{
	GameState->OnLeave();
	if(pState)GameState = pState;else return 0;
	GameState->OnEnter();

	return 1;
}

//load GameSate 
void CGameSateLoad::OnEnter()
{
	
}

void CGameSateLoad::Update()
{
	ChangeState(&gsProfile);
}

void CGameSateLoad::OnLeave()
{

}

//profiles GameState

void CGameSateProfileSelect::OnEnter()
{
	Level::ClearColor = 0xff000000;
	i = -1;
	Profiles::ActiveProfile = 0;
	UIm.ActivePage = "prof";
	UIm.Pages["prof"]["tb"]->InfoState.Hidden = true;
	UIm.Pages["prof"]["ok"]->InfoState.Hidden = true;
	UIm.Pages["prof"]["Name Label"]->InfoState.Hidden = true;

	UIm.Pages["prof"]["prof1"]->InfoState.strValue =  Profiles::profiles[0].Name ;
	UIm.Pages["prof"]["prof2"]->InfoState.strValue =  Profiles::profiles[1].Name ;
	UIm.Pages["prof"]["prof3"]->InfoState.strValue =  Profiles::profiles[2].Name ;
	Profiles::Load();
}

void CGameSateProfileSelect::Update()
{
	UIm.Update();

	if(UIm.Pages["prof"]["prof1"]->InfoState.KeyState == DBOXKEY_RELEASED)i = 0;
	if(UIm.Pages["prof"]["prof2"]->InfoState.KeyState == DBOXKEY_RELEASED)i = 1;
	if(UIm.Pages["prof"]["prof3"]->InfoState.KeyState == DBOXKEY_RELEASED)i = 2;
	
	if(i != -1)
	{
		std::string item = "prof"+ToString(i+1);
		if(UIm.Pages["prof"][item]->InfoState.KeyState == DBOXKEY_RELEASED)
		{
			if(Profiles::profiles[i].SettedUp){
				//Set GameState to Game main
				Profiles::ActiveProfile = &Profiles::profiles[i];
				ChangeState(&gsMainMenu);
				return;
			}
			else{
				UIm.Pages["prof"]["tb"]->InfoState.Hidden = false;	
			}

		}

		UIm.Pages["prof"]["ok"]->InfoState.Hidden = false;
		UIm.Pages["prof"]["Name Label"]->InfoState.Hidden = false;
		if(UIm.Pages["prof"]["ok"]->InfoState.KeyState == DBOXKEY_RELEASED)
		{
			if(UIm.Pages["prof"]["tb"]->InfoState.strValue != std::string("") && i != -1)
			{
				Profiles::profiles[i].Name = UIm.Pages["prof"]["tb"]->InfoState.strValue;
				Profiles::profiles[i].SettedUp = 1;
				Profiles::profiles[i].UnlockedLevel = 1;
				Profiles::ActiveProfile = &Profiles::profiles[i];
				ChangeState(&gsMainMenu);
				//Set GameSate to Game main
				return;
			}
		}
	}
	return;
}

void CGameSateProfileSelect::OnLeave()
{
	i = -1;
	UIm.ChangeActivePage( "prof" );
	UIm.Pages["prof"]["tb"]->InfoState.Hidden = true;
	UIm.Pages["prof"]["ok"]->InfoState.Hidden = true;
	UIm.Pages["prof"]["Name Label"]->InfoState.Hidden = true;

	UIm.Pages["main"]["Hello Label"]->InfoState.strValue = "Hello "+ToString(Profiles::ActiveProfile->Name) + "!";
	Profiles::Save();

}

/////////////////////////////////////////////////////////////
// Main Menu 
/////////////////////////////////////////////////////////////

void CGameSateMainMenu::OnEnter()
{
	Level::ClearColor = 0xff000000;
	UIm.ChangeActivePage( "main" );
}

void CGameSateMainMenu::Update()
{
	UIm.Update();

	//Main menu functions
	if(UIm.ActivePage == "main")
	{
		//back to profiles
		if(UIm.Pages["main"]["Profiles"]->InfoState.KeyState == DBOXKEY_RELEASED)ChangeState(&gsProfile);
		
		//go to settings
		if(UIm.Pages["main"]["Settings"]->InfoState.KeyState == DBOXKEY_RELEASED)
			UIm.ChangeActivePage( "settings" );
		if(UIm.Pages["main"]["Exit"]->InfoState.KeyState == DBOXKEY_RELEASED)
		{
			PostQuitMessage(0);
		}
		//go to level select
		if(UIm.Pages["main"]["Start"]->InfoState.KeyState == DBOXKEY_RELEASED)
		{
			UIm.Pages["level"]["l1"]->InfoState.Locked = (Profiles::ActiveProfile->UnlockedLevel > 0) ? 0 : 1;
			UIm.Pages["level"]["l2"]->InfoState.Locked = (Profiles::ActiveProfile->UnlockedLevel > 1) ? 0 : 1;
			UIm.Pages["level"]["l3"]->InfoState.Locked = (Profiles::ActiveProfile->UnlockedLevel > 2) ? 0 : 1;
			UIm.ChangeActivePage( "level" );
		}
	}
	//settings functions
	if(UIm.ActivePage == "settings")
	{
		if(UIm.Pages["settings"]["640x480"]->InfoState.KeyState == DBOXKEY_RELEASED)
		{
			ClientResize(App->GetWindowHandle() , 640 , 480);
		
		}
		if(UIm.Pages["settings"]["800x600"]->InfoState.KeyState == DBOXKEY_RELEASED)
		{
			ClientResize(App->GetWindowHandle() , 800 , 600);	
		}
		if(UIm.Pages["settings"]["1024x768"]->InfoState.KeyState == DBOXKEY_RELEASED)
		{
			ClientResize(App->GetWindowHandle() , 1024 , 768);
		}
		if(UIm.Pages["settings"]["1280x1024"]->InfoState.KeyState == DBOXKEY_RELEASED)
		{
			ClientResize(App->GetWindowHandle() , 1280 , 1024);
		}
		if(UIm.Pages["settings"]["Toggle"]->InfoState.KeyState == DBOXKEY_RELEASED)
		{
			GameProperties::Windowed = !GameProperties::Windowed;

			int cx = GetSystemMetrics(SM_CXSCREEN);	
			int cy = GetSystemMetrics(SM_CYSCREEN);
			DWORD WindowStyle = (GameProperties::Windowed ) ? WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_CLIPCHILDREN : WS_POPUP | WS_CLIPCHILDREN | WS_VISIBLE ;
			if(GameProperties::Windowed)
			{
				SetWindowLong(App->GetWindowHandle() , GWL_STYLE , WindowStyle);
			}
			else
			{
				SetWindowLong(App->GetWindowHandle() , GWL_STYLE , WindowStyle);
			//	ClientResize(App->GetWindowHandle() , cx , cy);
			}


		}
		if(UIm.Pages["settings"]["Back"]->InfoState.KeyState == DBOXKEY_RELEASED)
		{UIm.ChangeActivePage( "main" );}
	}
	if(UIm.ActivePage == "level")
	{
		if(UIm.Pages["level"]["l1"]->InfoState.KeyState == DBOXKEY_RELEASED)
		{
			Level::LevelNumber = 1;
			Level::LevelToLoad = "levels\\level-0.lev";
			ChangeState(&gsLevel);
		}

		if(UIm.Pages["level"]["l2"]->InfoState.KeyState == DBOXKEY_RELEASED)
		{
			Level::LevelNumber = 2;
			Level::LevelToLoad = "levels\\level-1.lev";
			ChangeState(&gsLevel);
		}

		if(UIm.Pages["level"]["l3"]->InfoState.KeyState == DBOXKEY_RELEASED)
		{
			Level::LevelNumber = 3;
			Level::LevelToLoad = "levels\\level-2.lev";
			ChangeState(&gsLevel);
		}

		if(UIm.Pages["level"]["Back"]->InfoState.KeyState == DBOXKEY_RELEASED)UIm.ChangeActivePage( "main" );
	}
	return;
}

void CGameSateMainMenu::OnLeave() 
{
}
//////////////////////////////////////
//Level
//////////////////////////////////////////

void ClearColorEffect()
{
	static DWORD r,g,b;
	static DWORD tr = rand()%255;
	static DWORD tg = rand()%255;
	static DWORD tb = rand()%255;
	static DWORD timer = 0;
	if(timer + 5000 < timeGetTime())
	{
		timer = timeGetTime();
		r = rand()%128;g = rand()%128;b = rand()%128;
		r += 64;g += 64;b += 64;
	}

	Goto<DWORD>(tr,r,2);
	Goto<DWORD>(tg,g,2);
	Goto<DWORD>(tb,b,2);

	Level::ClearColor = D3DCOLOR_XRGB(tr,tg,tb);
}

void CGameStateLevel::OnEnter()
{
	Level::ClearColor = D3DCOLOR_XRGB(63,115,205);
	ClearEnitityBuffer();
	UIm.ChangeActivePage( "ingame" );
	LoadLevel(Level::LevelToLoad);
	Particles::Stars::RemoveAll();
	Particles::Stars::Timer = timeGetTime();
}

void CGameStateLevel::Update()
{
	//ClearColorEffect();
	if(Particles::Stars::Timer + 15 < timeGetTime())
	{
		Particles::Stars::AddNew();
		Particles::Stars::Timer = timeGetTime();
	}
		Particles::Stars::Process();
	//game running
	if(UIm.ActivePage == "ingame")
	{
		//update objects only when playing
		for(int i = 0; i < EnitityBuffer.size() ;++i)
		{	
				EnitityBuffer[i]->Update();
		}
		
		if(UIm.Pages["ingame"]["Menu"]->InfoState.KeyState == DBOXKEY_RELEASED || Input->IsKeyClicked(DIK_ESCAPE))
		{
			UIm.ChangeActivePage( "paused" );
		}
	}
	//paused
	if(UIm.ActivePage == "paused")
	{
		if(UIm.Pages["paused"]["Resume"]->InfoState.KeyState == DBOXKEY_RELEASED)
		{
			UIm.ChangeActivePage( "ingame" );
		}
		if(UIm.Pages["paused"]["Leave"]->InfoState.KeyState == DBOXKEY_RELEASED)
		{
			ChangeState(&gsMainMenu);
		}
	}
	
	if(UIm.ActivePage == "lcompleate")
	{
		if(UIm.Pages["lcompleate"]["Menu"]->InfoState.KeyState == DBOXKEY_RELEASED)
		{
			UIm.ChangeActivePage("level");
			ChangeState(&gsMainMenu);
		}
	}
	//render the scene
	Camera->Capture(Engine3D->GetDevice());
	RenderEnitities();
	DrawFilterTexture();
	UIm.Update();
}

void CGameStateLevel::OnLeave()
{
	Profiles::Save();
	ClearEnitityBuffer();
}
