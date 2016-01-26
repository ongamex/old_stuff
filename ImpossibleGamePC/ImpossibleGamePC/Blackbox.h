//major program resources

#pragma once 

#include <Windows.h>
#include <WindowsX.h>
//#include <irrKlang.h>
#include <string>

#include "DBOX3D.h"
#include "DBOX2D.h"
#include "DBOXApp.h"
#include "DBOXInput.h"
#include "DBOXUserInterface.h" 
#include "OBB3D.h"
#include "DBOXCameraTP.h"

#ifdef BLACKBOX_UNEXTERN 
#define externBL  
#else
#define externBL extern
#endif

#define CFG_FILENAME ".\\config.ini"
#define PROFILES_FILENAME ".\\profiles.ini"

#define ENITITY_CUBE 0 
#define ENITITY_CONE 1
#define ENITITY_STAIR 2
#define ENITITY_CREEP 3
#define ENITITY_LINE 4
#define ENITITY_STAR 6
#define ENITITY_FLAG 7
#define ENITITY_END 400

externBL LPDBOX3D Engine3D;
externBL LPDBOX2D Engine2D;
externBL LPDBOXINPUT Input;
externBL LPDBOXAPP App;
externBL DBOXUIManager UIm;
//externBL irrklang::ISoundEngine* SndEngine; 
externBL DWORD TimeDelta; //used for rendering
externBL float Delta;
externBL LPDBOXCAMERATP Camera;

externBL XMesh Meshes[8];
externBL LPDIRECT3DTEXTURE9 FilterTexture;
//Level varibles

class Enitity;
namespace Level{
	externBL int LevelNumber;
	externBL std::string LevelToLoad;
	externBL DWORD ClearColor;
	externBL D3DXVECTOR3 StartingLevelPos;
	externBL D3DXVECTOR3 EndingLevelPos;
	externBL int TotalStars;
	externBL int CollectedStars;
	externBL D3DXVECTOR3* pFlagPos;
	externBL Enitity* Cube;
};

void SetupUI();

//GameState class

class CGameState{
public : 
	virtual void OnEnter() = 0;
	virtual void Update() = 0;
	virtual void OnLeave() = 0;
};

class CGameSateLoad : public CGameState{
	void OnEnter();
	void Update();
	void OnLeave();
};

class CGameSateProfileSelect : public CGameState{
	int i;//profile index
	void OnEnter();
	void Update();
	void OnLeave();
};

class CGameSateMainMenu : public CGameState{
	void OnEnter();
	void Update();
	void OnLeave();
};

class CGameStateLevel : public CGameState{
	void OnEnter();
	void Update();
	void OnLeave();
};

bool ChangeState(CGameState* pState);
externBL CGameSateLoad gsLoad;
externBL CGameSateProfileSelect gsProfile;
externBL CGameSateMainMenu gsMainMenu;
externBL CGameStateLevel gsLevel;
//main gameSateVarible
//it is defined in Gamesate.cpp
externBL CGameState* GameState;

//game settings
namespace GameProperties{

	void Load();
	void Save();
	void Default();

	externBL int Width , Height; 
	externBL int Windowed;

};

//profile settings
namespace Profiles
{
	struct Profile
	{
		std::string Name;
		int UnlockedLevel, SettedUp;
		DWORD Jump, Flag, Remove;
		int Attempts;
		int Jumps;
	};
	void Load();
	void Save();

	externBL Profile profiles[3];
	externBL Profile* ActiveProfile;
};

//Game enitity class - interface class
//used for game objects and events(probably)
class Enitity
{
public :
	D3DXVECTOR3 Position;
	OBB3D Box;
	int Type;//id of the enitity
	D3DXMATRIX Transform;//transform of the object, used for rendering

	virtual DWORD Update() = 0;
};
//object buffer 
externBL std::vector<Enitity*> EnitityBuffer;

void DrawFilterTexture();

void ClearEnitityBuffer();
void DeleteElement(std::vector<Enitity*>::iterator i);
void SpawnEnitity(int Type);

void LoadLevel(std::string filename);

void RenderEnitities();
//player class
class ECube : public Enitity
{
public :
	
	float zRotation ;//used for effect
	DWORD Update();
};

class EBlock : public Enitity
{
public :
	DWORD Update();
};

class EEnd : public Enitity
{
public : 
	DWORD Update();
};

//////////////////////////////////
//Particle systems

namespace Particles
{
	namespace Stars
	{

		externBL LPDIRECT3DTEXTURE9 Star;
		externBL DWORD Timer;

		struct Particle
		{
			DWORD Color;
			D3DXVECTOR3 Position;
			DWORD Born;
			float Rotation;
		};
		externBL std::vector<Particle> Buffer;

		void AddNew();
		void RemoveAll();
		void Process();

	};
}
