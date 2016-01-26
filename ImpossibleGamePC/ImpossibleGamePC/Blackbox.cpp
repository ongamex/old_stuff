#define BLACKBOX_UNEXTERN //create vars

#include "Blackbox.h" 

#include "IniReader.h"
#include "IniWriter.h"

//UI
void SetupUI()
{
	DBOXUIPROPERTIES props = {0};
	props.BaseWidth = 1280;
	props.BaseHeight = 1024;
	props.eng2d = Engine2D;
	props.FontColor = 0xffdddddd;
	props.hWnd = App->GetWindowHandle();
	props.pDevice = Engine3D->GetDevice();
	props.inp = Input;
	props.FontFace = "Arial Black";
	props.TextureKeyColor = 0xffff00ff;
	props.TextureName = "UI.png";
	props.LabelWordSize = 85;

	props.BtnRects[0] = lRect(0,0 , 386,139);
	props.BtnRects[1] = lRect(0,140 , 386,279);
	props.BtnRects[2] = lRect(0,280 , 386,419);

	props.TextboxRect = lRect(0,420 , 589,559);

	UIm.Create(props);
	//select profile
	UIm.Pages["prof"]["Hello Label"] = DBOXUICreateLabel(&UIm , lRect(0, 90,  1280 , 400) , "Impossible Game for PC(beta)\n\rOriginal game by : flukedude.com\n\rThe remake by K.Petkov\n\rSelect your profile...");
	UIm.Pages["prof"]["prof1"] = DBOXUICreateButton(&UIm , Profiles::profiles[0].Name , lPoint(1280/4 - 387/2 - 90, 500)); 
	UIm.Pages["prof"]["prof2"] = DBOXUICreateButton(&UIm , Profiles::profiles[1].Name , lPoint(1280/2 - 387/2 , 500)); 
	UIm.Pages["prof"]["prof3"] = DBOXUICreateButton(&UIm , Profiles::profiles[2].Name , lPoint(1920/2 - 387/2 + 90 , 500)); 
	UIm.Pages["prof"]["ok"] = DBOXUICreateButton(&UIm , "OK" , lPoint(740, 800)); 
	UIm.Pages["prof"]["tb"] = DBOXUICreateTextbox(&UIm , 9 , lPoint(120 , 800) , "profile");
	UIm.Pages["prof"]["Name Label"] = DBOXUICreateLabel(&UIm , lRect(0, 670,  1280 , 100) , "Enter your name ...");
	//main
	UIm.Pages["main"]["Hello Label"] = DBOXUICreateLabel(&UIm , lRect(0, 0,  1280 , 100) , "Hello ");
	UIm.Pages["main"]["Start"] = DBOXUICreateButton(&UIm , "Start" , lPoint(1280/2 - 387/2 , 350));
	UIm.Pages["main"]["Settings"] = DBOXUICreateButton(&UIm , "Settings" , lPoint(1280/2 - 387 - 1, 500));
	UIm.Pages["main"]["Controls"] = DBOXUICreateButton(&UIm , "Controls" , lPoint(1280/2 + 1, 500));
	UIm.Pages["main"]["Stats"] = DBOXUICreateButton(&UIm , "Stats" , lPoint(1280/2 - 387 - 1, 650));
	UIm.Pages["main"]["Profiles"] = DBOXUICreateButton(&UIm , "Profiles" , lPoint(1280/2 + 1 , 650));
	UIm.Pages["main"]["Exit"] = DBOXUICreateButton(&UIm , "Exit" , lPoint(1280/2 - 387/2  , 800));
	UIm.Pages["main"]["Controls"]->InfoState.Locked = UIm.Pages["main"]["Stats"]->InfoState.Locked = true;
	//settings
	UIm.Pages["settings"]["ResolutionSelect"] = DBOXUICreateLabel(&UIm , lRect(0, 100,  1280 , 100) , "Select your screen resolution ..");
	UIm.Pages["settings"]["640x480"] = DBOXUICreateButton(&UIm , "640x480" , lPoint(1280/2 - 387 - 10, 270)); 
	UIm.Pages["settings"]["800x600"] = DBOXUICreateButton(&UIm , "800x600", lPoint(1280/2  + 10 , 270)); 
	UIm.Pages["settings"]["1024x768"] = DBOXUICreateButton(&UIm , "1024x768" , lPoint(1280/2 - 387 - 10, 420)); 
	UIm.Pages["settings"]["1280x1024"] = DBOXUICreateButton(&UIm , "1280x1024" , lPoint(1280/2  + 10 , 420)); 
	UIm.Pages["settings"]["Toggle"] = DBOXUICreateButton(&UIm , "Fullscreen" , lPoint(1280/2 - 387 - 10 , 570)); 
	//
	UIm.Pages["settings"]["Toggle"]->InfoState.Locked = true;
	UIm.Pages["settings"]["Back"] = DBOXUICreateButton(&UIm , "Back" , lPoint(1280/2  + 10 , 570)); 
	//select level
	UIm.Pages["level"]["l1"] = DBOXUICreateButton(&UIm , "Level 1" , lPoint(1280/2 - 387/2 , 350)); 
	UIm.Pages["level"]["l2"] = DBOXUICreateButton(&UIm , "Level 2" , lPoint(1280/2 - 387/2 , 500)); 
	UIm.Pages["level"]["l3"] = DBOXUICreateButton(&UIm , "Level 3" , lPoint(1280/2 - 387/2 , 650)); 
	UIm.Pages["level"]["Back"] = DBOXUICreateButton(&UIm , "Menu" , lPoint(1920/2 - 387/2 + 90 , 800)); 

	UIm.Pages["ingame"]["Menu"] = DBOXUICreateButton(&UIm , "Menu" , lPoint(1920/2 - 387/2 + 110 , 830));
	UIm.Pages["paused"]["Resume"] = DBOXUICreateButton(&UIm , "Resume" , lPoint(1280/2 - 387/2 , 500)); 
	UIm.Pages["paused"]["Leave"] = DBOXUICreateButton(&UIm , "Leave" , lPoint(1280/2 - 387/2  , 650)); 
	UIm.Pages["lcompleate"]["Menu"] = DBOXUICreateButton(&UIm , "Menu" , lPoint(1280/2 - 387/2  , 650));
	UIm.Pages["lcompleate"]["label"] = DBOXUICreateLabel(&UIm , lRect(0 , 400 , 1280 , 400) , "Level clear !!!");
	UIm.ActivePage = "prof";
}
//Filter Texture

void DrawFilterTexture()
{
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale , UIm.scaleX , UIm.scaleY , 1.f);

	Engine2D->DrawToScreen(FilterTexture , &(scale) , 0 , DBOX_CENTER_LEFTTOP , 0 ,0xffffffff);
}

//Load and save gamesettings
void GameProperties::Load()
{
	CIniReader Reader(CFG_FILENAME);

	GameProperties::Width = Reader.ReadInteger("settings" , "app_width" , 1280);
	GameProperties::Height = Reader.ReadInteger("settings" , "app_height" , 1024);
	GameProperties::Windowed = Reader.ReadInteger("settings" , "app_windowed" , 1);

}

void GameProperties::Save()
{
	CIniWriter Writer(CFG_FILENAME);
	Writer.WriteInteger("settings" , "app_width" , GameProperties::Width);
	Writer.WriteInteger("settings" , "app_height" , GameProperties::Height);
	Writer.WriteInteger("settings" , "app_windowed" , GameProperties::Windowed);

}

//Load and save profiles

void Profiles::Load()
{
	ActiveProfile = 0 ;
	CIniReader Reader(PROFILES_FILENAME);

	for(int i = 0; i < 3; ++i)
	{
		std::string Section = const_cast<char*>((std::string("prof")+ToString(i)).c_str()) ;

		Profiles::profiles[i].Name = std::string(Reader.ReadString(const_cast<char*>(Section.c_str()) , "name" , "ErrorName"));
		Profiles::profiles[i].Jump = Reader.ReadInteger(const_cast<char*>(Section.c_str()) , "jump" , DIK_Q);
		Profiles::profiles[i].Flag = Reader.ReadInteger(const_cast<char*>(Section.c_str())  , "flag" , DIK_SPACE);
		Profiles::profiles[i].Remove = Reader.ReadInteger(const_cast<char*>(Section.c_str())  , "remove" , DIK_E);
		Profiles::profiles[i].UnlockedLevel = Reader.ReadInteger(const_cast<char*>(Section.c_str()) , "unlocked_level" , 1);
		Profiles::profiles[i].SettedUp = Reader.ReadInteger(const_cast<char*>(Section.c_str()) , "settedup" , 0);
		Profiles::profiles[i].Jumps = Reader.ReadInteger(const_cast<char*>(Section.c_str())  , "jumps" , 0);
		Profiles::profiles[i].Attempts = Reader.ReadInteger(const_cast<char*>(Section.c_str())  , "Attempts" , 0);
	}
}

void Profiles::Save()
{
	CIniWriter Writer(PROFILES_FILENAME);
	
	for(int i = 0; i < 3; i++)
	{
		std::string Section = const_cast<char*>((std::string("prof")+ToString(i)).c_str()) ;

		Writer.WriteString(const_cast<char*>(Section.c_str()) , "name" , const_cast<char*>(Profiles::profiles[i].Name.c_str()));
		Writer.WriteInteger(const_cast<char*>(Section.c_str()) , "jump" , Profiles::profiles[i].Jump);
		Writer.WriteInteger(const_cast<char*>(Section.c_str()) , "flag" , Profiles::profiles[i].Flag);
		Writer.WriteInteger(const_cast<char*>(Section.c_str()) , "unlocked_level" , Profiles::profiles[i].UnlockedLevel);
		Writer.WriteInteger(const_cast<char*>(Section.c_str()) , "settedup" , Profiles::profiles[i].SettedUp);
	}
}

