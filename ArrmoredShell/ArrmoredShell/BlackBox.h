#pragma once 

//used for externa vars
#ifdef BLACKBOX_DEFINE 
	#define blackbox_extern 
#else 
	#define blackbox_extern extern
#endif

#include <process.h>

#include "DBOXApp.h"
#include "DBOX2D.h"
#include "DBOX3D.h"
#include "DBOXFont.h" 
#include "DBOXInput.h"
#include "IniReader.h"
#include "IniWriter.h"
#include "DBOXTimer.h"
#include "AnimationManager.h"

#include <vector>
#include <map>

#define CONFING_FILENAME ".\\config.ini"

//prototypes of the logic and rendering thread.
void LogicThread(void* pArg);//
void RenderThread(void* pArg);//note : the logic is only a function that is called by the main thread

namespace Resources{
	namespace Textures{	
		blackbox_extern std::map<std::string,IDirect3DTexture9*> Buffer;
		
		IDirect3DTexture9* LoadTexture(const std::string& NameFile);//loads a texture into memory, if texture is already loaded returns the pointer to loaded the texture
		IDirect3DTexture9* GetTexture(const std::string& NameFile);
	};

	namespace Animations{
		blackbox_extern std::map<std::string, ASAnimation*> Buffer;

		ASAnimation* LoadAnimation(const std::string& FileName);
		ASAnimation* GetAnimation(const std::string& Name);
	};
};

namespace System{
	blackbox_extern LPDBOX3D			Ri3D;
	blackbox_extern LPDBOX2D			Ri;
	blackbox_extern LPDBOXINPUT			Input;
	blackbox_extern DBOXApplication*	pApp;

	blackbox_extern DBOXTimer			GameTimer;
	blackbox_extern DBOXTimer			RenderingTimer;
	blackbox_extern HANDLE DataMutex;//mutex used for locking the game object. RenderThread will wait for that mutex to draw 
};

namespace Settings{//holds settings form ini files
	blackbox_extern int Width;
	blackbox_extern int Height;
	blackbox_extern bool Windowed;
	blackbox_extern bool vSync;
	void Import();
	void Export();
};

blackbox_extern ASAnimation* DemoAnimation;
blackbox_extern ASAnimationInfo DemoAnimCtrl;
blackbox_extern std::vector<ASAnimationInfo*> pAnims;