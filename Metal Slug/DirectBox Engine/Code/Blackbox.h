/****************************************************************************************************************************************
FileName  : Blackbox.h 
Copyright : Kostadin Petkov 2010
*****************************************************************************************************************************************/

#pragma once

#include <Windows.h>
#include <WindowsX.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "DBOXApp.h"
#include "DBOX3D.h"
#include "DBOXInput.h"
#include "DBOXUI.h"
#include "DBOXFont.h"
#include "DBOXMath.h"
#include "Ini\IniReader.h"
#include "ini\IniWriter.h"

#include "GameMainMenu.h"
#include "GameInit.h"
#include "GObject.h"

//ObjectID
#include "ObjectDefines.h"
#include "GameLogic.h"

//defines

#define EPSYLON			10

#define OPEN_GLOBALDATA using namespace GlobalData
#define OPEN_GAME		using namespace Game
#define OPEN_RESOURCES	using namespace Resources
#define COLOR_KEY		D3DCOLOR_ARGB(255 , 64 , 128 , 0) 
#define CONFIG_FILENAME TEXT(".\\config.ini")

//Player1 defines 

#define mPlayer1KeyDown		GlobalData::Player1KeyDown
#define mPlayer1KeyUp		GlobalData::Player1KeyUp
#define mPlayer1KeyLeft		GlobalData::Player1KeyLeft
#define mPlayer1KeyRight	GlobalData::Player1KeyRight
#define mPlayer1KeyFire		GlobalData::Player1KeyFire
#define mPlayer1KeyGrenade	GlobalData::Player1KeyGrenade

#define IS_KEY(state , key_id)  GlobalData::Input->GetKeyState(key_id) == state 

#define IS_KEY_DOWN(key_id)			GlobalData::Input->GetKeyState(key_id) == DBOXKEY_DOWN
#define IS_KEY_UP(key_id)			GlobalData::Input->GetKeyState(key_id) == DBOXKEY_UP
#define IS_KEY_CLICKED(key_id)		GlobalData::Input->GetKeyState(key_id) == DBOXKEY_CLICKED
#define IS_KEY_RELEASED(key_id)		GlobalData::Input->GetKeyState(key_id) == DBOXKEY_RELEASED
//types
typedef std::vector<GAnimation> vGAnimation;

//event control functions , threads
RECT* CreateHStrip(long sx ,long sy , long fw , long fh, UINT nOfFrames);

DWORD CheckDevice(void*); // retval : 1 - skip frame ,  0 proceed
DWORD OnLostDevice(void*);
DWORD OnResetDevice(void*);
DWORD InputThread(void*);
DWORD OnEnterLoop(void*);

DWORD SwitchDisplayMode(BOOL Windowed);

//game state 
DWORD GameProc(void*);

//types
typedef enum tagGAME_STATE
{
	GAME_NULL = 0 , 
	GAME_INIT , //useless
	GAME_MENU , 
	GAME_MENU_SETTINGS , 
	GAME_LOAD_NEXT_LEVEL ,
	GAME_TEST_IN ,
	GAME_FROZEN , //usless at the moment
	GAME_PLAYING , 
	GAME_END ,
	GAME_QUIT, 

}GAME_STATE;

//data

namespace Resources
{
	//InGame
	namespace Textures
	{
		extern std::vector<LPDIRECT3DTEXTURE9> Data;
		DWORD Load(void*);
		DWORD OnLostDevice(void*);
		DWORD OnResetDevice(void*);
		DWORD Release(void*);
	}

	namespace Animations
	{
		extern std::vector<vGAnimation> Data;
		DWORD Load(void*);
		DWORD Release(void*);
	}
	//
	namespace Menu
	{
		extern LPDBOXFONT FontSettings ;

		extern LPDIRECT3DTEXTURE9 Texture;
		extern DBOXUIButton Logo;
		extern DBOXUIButton NewGame , Continue ;
		extern DBOXUIButton Settings ; 
		extern DBOXUIButton Back , Quit;
		extern DBOXUIProgressBar HealthBar;
		//for settings 
		extern DBOXUIButton Left , Right , Up , Down , Fire , Grenade;
	}
}

namespace Game
{
	extern GAME_STATE	GameState ;
	extern int			Level ;
	extern BOOL			GoalReached ;
	extern D3DXVECTOR4	InGameArea;//( x y , w z )
	extern GObject* Player ;
	extern float Points ;
	extern LPDBOXFONT Font;

	extern int Grenades ; 
	extern int PmpAmmo ;
	extern int LastDamage;

	namespace AnimationTicker
	{
		extern BOOL Tick;
		extern DWORD LastTime;
		extern DWORD Period; 

		void Update();
	};

	namespace ObjectBuffer
	{
		extern std::vector<GObject*> Buffer;
		void ClearAll();
		void Push(GObject*);
		void DestroyObject(long idx);
		void DoObjectsLogic();

		GObject* SpawnObject(long id ,D3DXVECTOR3 Pos ,void* pData);
		

		namespace Render
		{
			extern D3DXVECTOR3 CameraPosition;//This Position is in the world space !!!
			extern std::vector<GObject*> Buffer ; 

			void SetCam(D3DXVECTOR3 Pos);
			void SetCamEx();
			void SortBufferByYPos();//Private use
			void MoveCamera();
			void RenderScene();//gets the objects in scene and render them
			
		}

	};

		void LoadLevel(long LevelID);
};

namespace GlobalData
{
	//Device Manager
	extern LPDBOXAPP		Application ; 
	extern LPDBOX3D			GFCtrl ;
	extern LPDBOXINPUT		Input ;
	extern LPD3DXSPRITE		Sprite ;
	extern LPDIRECT3DDEVICE9 d3ddev ;
	extern LPDBOXUI			UICtrl ;
	//Window Options
	extern long WindowWidth ; //from config.ini
	extern long WindowHeight ; //from config.ini
	extern BOOL Windowed ; //from config.ini

	extern HWND hWnd ;

	//Player Controls 
	extern int Player1KeyLeft ; //from config.ini 
	extern int Player1KeyRight ; //from config.ini 
	extern int Player1KeyUp ; //from config.ini
	extern int Player1KeyDown ; //from config.ini
	extern int Player1KeyFire ; //from config.ini
	extern int Player1KeyGrenade ; //from config.ini 

	void LoadDataFromIni();
	void SaveDataToIni();

};
