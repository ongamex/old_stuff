#include "GameMainMenu.h"
#include "GameLogic.h"

DWORD GameMainMenu(void*)
{
	OPEN_RESOURCES;
	Menu::Logo.Update();
	
	size_t BufferSize = Game::ObjectBuffer::Buffer.size() ;
	
	//update buttond : newgame/continue , Settings , Quit
	if(BufferSize == 0)Menu::NewGame.Update();
	else Menu::Continue.Update();
	Menu::Settings.Update();
	Menu::Quit.Update();

	if(BufferSize == 0)
	{
		if(Menu::NewGame.CheckState(DBOXUI_RELEASED)) 
		{	
			Game::GameState = GAME_LOAD_NEXT_LEVEL;
		}
	}else
	{
		if(Menu::Continue.CheckState(DBOXUI_RELEASED))
		{
			Sleep(200);
			Game::GameState = GAME_PLAYING;
		}
	}

	if(Menu::Settings.CheckState(DBOXUI_RELEASED))Game::GameState = GAME_MENU_SETTINGS;
	if(Menu::Quit.CheckState(DBOXUI_RELEASED))
		{
			SendMessage(GlobalData::hWnd , WM_CLOSE , 0 , 0);
		}
	
	return 0 ;
}

DWORD GameSettingsMenu(void*)
{
	OPEN_RESOURCES ; 
	
	Menu::FontSettings->DrawTextEx("Settings" , DT_CENTER | DT_TOP , &DBOXRect(0 , 0 , 640 , 33));
	
	static int Activated = 0;

	Menu::Left.Update();
	Menu::Right.Update();
	Menu::Up.Update();
	Menu::Down.Update();
	Menu::Fire.Update();
	Menu::Grenade.Update();
	
	Menu::Back.Update();

	{
		using Menu::FontSettings;
		FontSettings->DrawText(DBOXGetKeyString(GlobalData::Player1KeyLeft).c_str() , 297 ,  Menu::Left.GetPos().y + 11);
		FontSettings->DrawText(DBOXGetKeyString(GlobalData::Player1KeyRight).c_str() , 297 ,  Menu::Right.GetPos().y + 11);
		FontSettings->DrawText(DBOXGetKeyString(GlobalData::Player1KeyUp).c_str() , 297 ,  Menu::Up.GetPos().y + 11);
		FontSettings->DrawText(DBOXGetKeyString(GlobalData::Player1KeyDown).c_str() , 297 ,  Menu::Down.GetPos().y + 11);
		FontSettings->DrawText(DBOXGetKeyString(GlobalData::Player1KeyFire).c_str() , 297 ,  Menu::Fire.GetPos().y + 11);
		FontSettings->DrawText(DBOXGetKeyString(GlobalData::Player1KeyGrenade).c_str() , 297 ,  Menu::Grenade.GetPos().y + 11);
	}

	UINT NewKey = GlobalData::Input->GetKeyByState(DBOXKEY_CLICKED) ; 
	if(NewKey == DIK_LMB) NewKey = 0;
	if(Menu::Left.CheckState(DBOXKEY_RELEASED))
	{
		Activated = 1;	
	}
	else if(Activated == 1 && NewKey != 0 )
	{
			GlobalData::Player1KeyLeft = NewKey;
			Activated = 0;
	}
	/////////////////////////////////////////////
	if(Menu::Right.CheckState(DBOXKEY_RELEASED))
	{
		Activated = 2;	
	}
	else if(Activated == 2 && NewKey != 0 )
	{
			GlobalData::Player1KeyRight = NewKey;
			Activated = 0;
	}
	/////////////////////////////////////////////
	if(Menu::Up.CheckState(DBOXKEY_RELEASED))
	{
		Activated = 3;	
	}
	else if(Activated == 3 && NewKey != 0 )
	{
			GlobalData::Player1KeyUp = NewKey;
			Activated = 0;
	}
	/////////////////////////////////////////////
	if(Menu::Down.CheckState(DBOXKEY_RELEASED))
	{
		Activated = 4;	
	}
	else if(Activated == 4 && NewKey != 0 )
	{
			GlobalData::Player1KeyDown = NewKey;
			Activated = 0;
	}
	/////////////////////////////////////////////
	if(Menu::Fire.CheckState(DBOXKEY_RELEASED))
	{
		Activated = 5;	
	}
	else if(Activated == 5 && NewKey != 0 )
	{
			GlobalData::Player1KeyFire = NewKey;
			Activated = 0;
	}
	//////////////////////////////////////////////
	if(Menu::Grenade.CheckState(DBOXKEY_RELEASED))
	{
		Activated = 6;	
	}
	else if(Activated == 6 && NewKey != 0 )
	{
			GlobalData::Player1KeyGrenade = NewKey;
			Activated = 0;
	}

	if(Menu::Back.CheckState(DBOXUI_RELEASED))
		{
			Activated = 0;
			GlobalData::SaveDataToIni();
			Game::GameState = GAME_MENU;
		}
	return 0;
}