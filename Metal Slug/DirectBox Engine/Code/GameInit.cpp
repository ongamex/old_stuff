#include "GameInit.h"

DWORD GameInit(void*)
{
	OPEN_GLOBALDATA ;
	OPEN_GAME ;
	OPEN_RESOURCES ; 
	float scale = 0.8;
	//set game 
	Game::Font = DBOXCreateFont("Impact" , GlobalData::d3ddev , 12 , 15 , 1000 , D3DCOLOR_XRGB(0 , 0 , 0));
	//set menu

	Menu::FontSettings = DBOXCreateFont("Impact"  , GlobalData::d3ddev , 29 , 33 , 1000 , D3DCOLOR_XRGB(201 , 201 , 201));

	Menu::Texture = GFCtrl->CreateTextureFromFile("Textures\\MenuX1.bmp" , D3DX_DEFAULT , D3DX_DEFAULT , COLOR_KEY);
	//Create the logo button
	RECT LogoRect[] = {{0,0 , 399*scale,199*scale},{0,0 , 399*0.8,199*scale},{0,0 , 399*0.8,199*scale}};

	RECT* NewGameRect	= CreateHStrip(0 , 200*scale , 300*scale , 66*scale , 3);
	RECT* ContinueRect	= CreateHStrip(0 , 266*scale , 300*scale , 66*scale , 3);
	RECT* SettingsRect	= CreateHStrip(0 , 332*scale , 300*scale , 66*scale , 3);
	RECT* BackRect		= CreateHStrip(0 , 398*scale , 300*scale , 66*scale , 3);
	RECT* QuitRect		= CreateHStrip(0 , 464*scale , 300*scale , 66*scale , 3);
	RECT HealthBarRect[] = {{869*scale , 0 , 1024*scale , 34*scale} , {869*scale , 34*scale , 1024*scale , 66*scale} };

	Menu::Logo.Create(UICtrl , Menu::Texture , LogoRect);
	Menu::Logo.SetPos(200*scale , 50*scale);

	Menu::NewGame.Create(UICtrl , Menu::Texture , NewGameRect);
	Menu::NewGame.SetPos(250*scale , 350*scale);
	Menu::NewGame.SetHotkey(VK_RETURN);

	Menu::Continue.Create(UICtrl , Menu::Texture , ContinueRect);
	Menu::Continue.SetPos(250*scale , 350*scale);
	Menu::Continue.SetHotkey(VK_ESCAPE);

	Menu::Settings.Create(UICtrl , Menu::Texture , SettingsRect);
	Menu::Settings.SetPos(250*scale , 417*scale);

	Menu::Back.Create(UICtrl , Menu::Texture , BackRect);
	Menu::Back.SetPos(0,0);
	Menu::Back.SetHotkey(VK_ESCAPE);

	Menu::Quit.Create(UICtrl , Menu::Texture , QuitRect);
	Menu::Quit.SetPos(250*scale , 484*scale);
	
	Menu::HealthBar.Create(UICtrl , Menu::Texture , HealthBarRect);
	Menu::HealthBar.SetPos(10*scale , 10*scale);
	Menu::HealthBar.SetProgress(1.0f);
	//for settings
	// Left , Right , Up , Down , Fire , Granade;
	
	RECT* LeftRect		= CreateHStrip(0 , 530*scale , 300*scale , 60*scale , 3);
	RECT* RightRect		= CreateHStrip(0 , 590*scale , 300*scale , 60*scale , 3);
	RECT* UpRect		= CreateHStrip(0 , 650*scale , 300*scale , 60*scale , 3);
	RECT* DownRect		= CreateHStrip(0 , 710*scale , 300*scale , 60*scale , 3);
	RECT* FireRect		= CreateHStrip(0 , 770*scale , 300*scale , 60*scale , 3);
	RECT* GrenadeRect	= CreateHStrip(0 , 830*scale , 300*scale , 60*scale , 3);

	Menu::Left.Create(UICtrl , Menu::Texture , LeftRect);
	Menu::Left.SetPos(50*scale , 60*scale);

	Menu::Right.Create(UICtrl , Menu::Texture , RightRect);
	Menu::Right.SetPos(50*scale , 130*scale);

	Menu::Up.Create(UICtrl , Menu::Texture , UpRect);
	Menu::Up.SetPos(50*scale , 200*scale);

	Menu::Down.Create(UICtrl , Menu::Texture , DownRect);
	Menu::Down.SetPos(50*scale , 270*scale);

	Menu::Fire.Create(UICtrl , Menu::Texture , FireRect);
	Menu::Fire.SetPos(50*scale , 340*scale);

	Menu::Grenade.Create(UICtrl , Menu::Texture , GrenadeRect);
	Menu::Grenade.SetPos(50*scale , 410*scale);

	GameState = GAME_MENU;
	return 0;
}