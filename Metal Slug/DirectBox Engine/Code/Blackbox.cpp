#include "blackbox.h"

#define LoadTexture(filename) GlobalData::GFCtrl->CreateTextureFromFile(filename , D3DX_DEFAULT , D3DX_DEFAULT , D3DCOLOR_ARGB(255 , 64 , 128 , 0))

RECT* CreateHStrip(long sx ,long sy , long fw , long fh, UINT nOfFrames)
{
	RECT* retval = new RECT[nOfFrames];

	POINT Start = {sx , sy};
	POINT Frame = {fw , fh};
	for(UINT i = 0 ; i < nOfFrames ; i++)
	{
		RECT Data = { Start.x + i*Frame.x , Start.y   ,   Start.x + Frame.x*(i+1) , Start.y + Frame.y };
		retval[i] = Data;
	}

	return retval;
}

//data
namespace Resources
{
	//InGame
	namespace Textures
	{
		std::vector<LPDIRECT3DTEXTURE9> Data(0);
		DWORD Load(void*)
		{
			Data.resize(5);
			Data[0] = LoadTexture("Textures\\Obj\\player_1.bmp");	//player1
			Data[1] = LoadTexture("Textures\\Levels\\testX1.bmp");	//test background
			Data[2] = LoadTexture("Textures\\Obj\\bnm.bmp");
			Data[3] = LoadTexture("Textures\\Obj\\enemy.bmp"); // japan enemy
			Data[4] = LoadTexture("Textures\\Obj\\explosions.bmp"); //explosions
			return 0;
		}

		DWORD OnLostDevice(void*)
		{
			//textures are in MANAGED MEMORY !
			return 0;
		}

		DWORD OnResetDevice(void*)
		{
			//textures are in MANAGED MEMORY !
			return 0;
		}

		DWORD Release(void*)
		{
			long DataSize = Data.size();
			for(long i = 0 ; i < DataSize ; i++)
			{
				if(Data[i] != NULL)
				{
					Data[i]->Release();
					Data[i] = NULL ; 
				}	
			}

			return 0 ;
		}//end release
	}//end namespace Textures

	namespace Animations
	{
		std::vector<vGAnimation> Data;

		DWORD Load(void*)
		{
			Data.resize(10);
			
			{
				//Player1 Idle 
				GAnimation Idle;
				RECT* IdleRect = CreateHStrip(0 , 0 , 80 , 43 , 6);
				Idle.SetData(IdleRect , 6 , 1 ,  Resources::Textures::Data[0]);
				//Player1 Walk
				GAnimation Walk;
				RECT* WalkRect = CreateHStrip(0,43 , 80 , 43 , 12);
				Walk.SetData(WalkRect , 12 , 1 ,  Resources::Textures::Data[0]);
				//Player1 Fire
				GAnimation Fire;
				RECT* FireRect = CreateHStrip(0 , 86 , 80 , 43 , 18);
				Fire.SetData(FireRect , 18 , 1 , Resources::Textures::Data[0]);
				//Player1 Walk and Fire
				GAnimation WalkAndFire;
				RECT* WalkAndFireRect = CreateHStrip(0 , 129 , 80 , 43 , 12);
				WalkAndFire.SetData(WalkAndFireRect , 12 , 1 , Resources::Textures::Data[0]);
				//Player1 Die
				GAnimation Die;
				RECT* DieRect = CreateHStrip(0,213 , 52,43 ,42);
				Die.SetData(DieRect , 42 , -1.0f , Resources::Textures::Data[0]);
				//Put values in container
				Data[0].resize(5); // Player1
				Data[0][0] = Idle;
				Data[0][1] = Walk;
				Data[0][2] = Fire;
				Data[0][3] = WalkAndFire;
				Data[0][4] = Die;
			}//Player1 END

			//Test Background
			{
				GAnimation Idle ;
				RECT* IdleRect = CreateHStrip(0 , 0 ,  640 , 480 , 1);
				Idle.SetData(IdleRect ,1 , 1.0f , Resources::Textures::Data[1]);
				Data[1].resize(1);
				Data[1][0] = Idle;
			}//Background end

			{
				Data[2].resize(3);
				//Machinegun bullet
				{
					GAnimation Idle;
					RECT* IdleRect = CreateHStrip(0,0 , 32,5 , 1);
					Idle.SetData(IdleRect , 1 , 1.0f , Resources::Textures::Data[2]);

					Data[2][0] = Idle;
				}

				//Pmp  bullet
				{
					GAnimation Idle;
					RECT* IdleRect = new RECT[16];
					RECT* IdleRect1 = CreateHStrip(0,86 , 147,69 , 8);
					RECT* IdleRect2 = CreateHStrip(0,155 , 147,69 , 8);
					
					memset(IdleRect , 0 , sizeof(RECT)*16);
					memcpy(IdleRect , IdleRect1 , sizeof(RECT)*8);
					memcpy(IdleRect + 8 , IdleRect2 , sizeof(RECT)*8); 

					Idle.SetData(IdleRect , 16 , 1.0f , Resources::Textures::Data[4]);// this is in the exp cause of the animations

					Data[2][1] = (Idle);//Data[2][1]

					delete IdleRect1 ; 
					delete IdleRect2 ;
				}
				//Grenade - The Animation of the grenade is in the explosions texture
				{
					GAnimation Idle ; 

					RECT* IdleRect = new RECT[35];

					long yIndex = 237 ;

					RECT* Row1 = CreateHStrip(0 , yIndex , 161 , 113 , 7);yIndex += 113;
					RECT* Row2 = CreateHStrip(0 , yIndex , 161 , 113 , 7);yIndex += 113;
					RECT* Row3 = CreateHStrip(0 , yIndex , 161 , 113 , 7);yIndex += 113;
					RECT* Row4 = CreateHStrip(0 , yIndex , 161 , 113 , 7);yIndex += 113;
					RECT* Row5 = CreateHStrip(0 , yIndex , 161 , 113 , 7);

					memset(IdleRect , 0 , sizeof(RECT)*35);

					memcpy(&IdleRect[0] , Row1 , sizeof(RECT)*7);
					memcpy(&IdleRect[7] , Row2 , sizeof(RECT)*7);
					memcpy(&IdleRect[14] , Row3 , sizeof(RECT)*7);
					memcpy(&IdleRect[21] , Row4 , sizeof(RECT)*7);
					memcpy(&IdleRect[28] , Row5 , sizeof(RECT)*7);

					Idle.SetData(IdleRect , 25 , 1.0f , Resources::Textures::Data[4]);// this is in the exp cause of the animations

					Data[2][2] = (Idle);//Data[2][2]

					delete Row1;
					delete Row2;
					delete Row3;
					delete Row4;
					delete Row5;

				}
			}
			//Japan Kamikaze
			{
				GAnimation Walk ; 
				RECT* WalkRect = CreateHStrip(0,0 , 80,43 ,12);
				Walk.SetData(WalkRect , 12 , 1.0f , Resources::Textures::Data[3]);

				GAnimation Attack ;
				RECT* AttackRect = CreateHStrip(0,43 , 80,43 , 25);
				Attack.SetData(AttackRect , 25 , 1.0f , Resources::Textures::Data[3]);

				GAnimation Die ;
				RECT* DieRect = CreateHStrip(0,86 , 80,43 , 20);
				Die.SetData(DieRect , 20 , 1.0f , Resources::Textures::Data[3]);

				Data[3].resize(3);
				Data[3][0] = Walk;
				Data[3][1] = Attack;
				Data[3][2] = Die;
			}

			//Explosion small
			{
				Data[4].resize(3);

				//explosion small type 1
				{
					GAnimation Idle;
					RECT* IdleRect = CreateHStrip(0,0 , 80 , 43 , 8);
					Idle.SetData(IdleRect , 8 , 1.0f , Resources::Textures::Data[4]);

					
					Data[4][0] = Idle;
				}

				//explosion small type 2
				{
					GAnimation Idle;
					RECT* IdleRect = CreateHStrip(0,44 , 80 , 42 , 12);
					Idle.SetData(IdleRect , 12 , 1.0f , Resources::Textures::Data[4]);

					Data[4][1] = Idle;
				}
				//explosion medium
				{
					GAnimation Idle;
					RECT* IdleRect = CreateHStrip(0,847 , 48 , 112 , 21);
					Idle.SetData(IdleRect , 21 , 1.0f , Resources::Textures::Data[4]);
					Data[4][2] = Idle;
				}
			}

			//misc
			{
				Data[5].resize(6);
				//health pack
				{
					GAnimation Idle;
					RECT* IdleRect = CreateHStrip(0,482 ,  28,30 , 33);
					Idle.SetData(IdleRect , 33 , 1.0f , Resources::Textures::Data[2]);

					Data[5][0] = Idle;
				}

				//coin
				{
					GAnimation Idle ; 
					RECT* IdleRect = CreateHStrip(0,467 , 16,15 , 10);
					Idle.SetData(IdleRect , 10 , 1.0f , Resources::Textures::Data[2]);

					Data[5][1] = Idle;
				}

				//pmp Icon
				{
					GAnimation Idle;
					RECT* IdleRect = CreateHStrip(0,395 , 30,26 , 6);
					Idle.SetData(IdleRect , 6 , 1.0f  , Resources::Textures::Data[2]);

					Data[5][2] = Idle;
				}
				//chilli
				{
					GAnimation Idle;
					RECT* IdleRect = CreateHStrip(0,5  , 31,49 , 12);
					Idle.SetData(IdleRect , 12 , 1.f , Resources::Textures::Data[2]);

					Data[5][3] = Idle;
				}
				//ammo crate
				{
					GAnimation Idle;
					RECT* IdleRect = CreateHStrip(0,338  , 32,32 , 6);
					Idle.SetData(IdleRect , 6 , 1.f , Resources::Textures::Data[2]);

					Data[5][4] = Idle;
				}
				//rock
				{
					GAnimation Idle;
					RECT* IdleRect = CreateHStrip(0 , 66 , 68 , 48 , 1);
					Idle.SetData(IdleRect , 1 , 1.f , Resources::Textures::Data[2]);
					Data[5][5] = Idle;
				}
				//sniper
				{
					GAnimation Walk ; 
					RECT* WalkRect = CreateHStrip(0,129 , 80,43 ,6);
					Walk.SetData(WalkRect , 6 , -1.0f , Resources::Textures::Data[3]);

					GAnimation Attack ;
					RECT* AttackRect = CreateHStrip(0,172 , 80,43 , 18);
					Attack.SetData(AttackRect , 18 , -1.0f , Resources::Textures::Data[3]);

					GAnimation Die ;
					RECT* DieRect = CreateHStrip(0,215 , 80,43 , 17);
					Die.SetData(DieRect , 17 , -1.0f , Resources::Textures::Data[3]);

					Data[6].resize(3);
					Data[6][0] = Walk;
					Data[6][1] = Attack;
					Data[6][2] = Die;
				}
				//sniper bullet
				{
					GAnimation Idle;
					RECT* IdleRect = CreateHStrip(43,0  , 14,8 , 1);
					Idle.SetData(IdleRect , 1 , 1.f , Resources::Textures::Data[2]);
					
					Data[7].resize(1);
					Data[7][0] = Idle;
				}
				//nomad
				{
					GAnimation Walk ; 
					RECT* WalkRect = CreateHStrip(0,258 , 33,52 ,12);
					Walk.SetData(WalkRect , 12 , 1.0f , Resources::Textures::Data[3]);

					GAnimation Attack ;
					RECT* AttackRect = CreateHStrip(0,310 , 72,66 , 10);
					Attack.SetData(AttackRect , 10 , 1.0f , Resources::Textures::Data[3]);

					GAnimation Die ;
					RECT* DieRect = CreateHStrip(0,376 , 47,65 , 9);
					Die.SetData(DieRect , 8 , 1.0f , Resources::Textures::Data[3]);

					Data[8].resize(3);
					Data[8][0] = Walk;
					Data[8][1] = Attack;
					Data[8][2] = Die;
				}
				//kamokaze fix
				{
				GAnimation Walk ; 
				RECT* WalkRect = CreateHStrip(0,0 , 80,43 ,12);
				Walk.SetData(WalkRect , 12 , 1.0f , Resources::Textures::Data[3]);

				GAnimation Attack ;
				RECT* AttackRect = CreateHStrip(0,43 , 80,43 , 25);
				Attack.SetData(AttackRect , 25 , 1.0f , Resources::Textures::Data[3]);

				GAnimation Die ;
				RECT* DieRect = CreateHStrip(0,86 , 80,43 , 20);
				Die.SetData(DieRect , 20 , 1.0f , Resources::Textures::Data[3]);

				Data[9].resize(3);
				Data[9][0] = Walk;
				Data[9][1] = Attack;
				Data[9][2] = Die;
				}
			}
			return 0;
		}

		DWORD Release(void*)
		{
			Data.clear();
			return 0;
		}
	}
	//
	namespace Menu
	{
		LPDBOXFONT FontSettings;

		LPDIRECT3DTEXTURE9 Texture;
		DBOXUIButton Logo;
		DBOXUIButton NewGame , Continue;
		DBOXUIButton Settings; 
		DBOXUIButton Back , Quit;
		//for settings 
		DBOXUIButton Left , Right , Up , Down , Fire , Grenade;
		//ingame
		DBOXUIProgressBar HealthBar;
	}
}

namespace Game
{
	GAME_STATE	GameState = GAME_INIT ;
	int			Level = 0;
	BOOL		GoalReached = FALSE;
	D3DXVECTOR4	InGameArea ;
	GObject* Player = NULL;
	extern float Points = 0;
	extern LPDBOXFONT Font = NULL;

	extern int Grenades = 0; 
	extern int PmpAmmo = 0;
	int LastDamage = 0;

	namespace AnimationTicker
	{
		BOOL Tick = 0;
		DWORD LastTime = timeGetTime();
		DWORD Period = 1000/37;

		void Update()
		{
			if(timeGetTime() - LastTime > Period){Tick = 1 ; LastTime = timeGetTime();}
			else 
			{
				Tick = 0 ;
			}

			return ;
		}
	};

	namespace ObjectBuffer
	{
		std::vector<GObject*> Buffer(0);

		void ClearAll()
		{
			for(size_t i = 0; i < Buffer.size() ; i++)
			{
				if(Buffer[i])
				delete Buffer[i];
			}
			Buffer.clear();
			return ;
		}

		void Push(GObject* pObj)
		{
			Buffer.push_back(pObj);
			return ;
		}

		void DestroyObject(long idx)
		{
			delete Buffer[idx];
			Buffer.erase(Buffer.begin()  + idx);
			return ;
		}

		//Do ObjectsLogic
		void DoObjectsLogic()
		{
	
			for(int i = 0 ; i < (int)Game::ObjectBuffer::Buffer.size() ; i++)
			{
				if(i == Game::ObjectBuffer::Buffer.size())break;

				DWORD RetVal = Game::ObjectBuffer::Buffer[i]->Update(i);
				
				switch(RetVal)
				{
				case OBJECT_IDLE : break ; 
				case OBJECT_DESTROY : Game::ObjectBuffer::DestroyObject(i); i-- ;break;
				default : break ;
				}
				
			}//end loop

			return ;
		}

/*************************************************************************
	Description : Spawn Objects
	params : object_id , object_position , void data
	return value : object_pointer ; 
**************************************************************************/
		GObject* SpawnObject(long id ,D3DXVECTOR3 Pos ,void* pData)
		{
			switch(id)
			{

			case OBJECT_PLAYER1 :
				{
					GObjectPlayer*  Obj = new GObjectPlayer ;

					Obj->Position = Pos;
					Obj->Facing = 1.0f;
					Obj->Health = *(float*)(pData);

					Game::Player = Obj;
					Push(Obj);
					return Obj;
				}
				break;

			case OBJECT_BACKGROUND_TEST :
				{
					GObjectBackground* Obj = new GObjectBackground;
					Obj->TopLeftPosition = Pos; 
					
					Push(Obj);
					return Obj;
				}
				break;

			case OBJECT_BULLET_MACHINEGUN :
				{
					GObjectBulletMachinegun* Obj = new GObjectBulletMachinegun();
					Obj->Facing = *(float*)(pData);
					Obj->Position = Pos;
					
					Push(Obj);
					return Obj;
				}
				break;
			case OBJECT_BULLET_SNIPER :
				{
					GObjectBulletSniper* Obj = new GObjectBulletSniper();
					Obj->Facing = *(float*)(pData);
					Obj->Position = Pos;

					Push(Obj);
					return Obj;
				}break;
			case OBJECT_ENEMY_KAMIKAZE_1 : 
				{
					GObjectEnemyKamikaze1* Obj = new GObjectEnemyKamikaze1();

					Obj->Position = Pos;
					Push(Obj);
					return Obj;
				}
				break ; 
			case OBJECT_ENEMY_SNIPER :
				{
					GObjectEnemySniper* Obj = new GObjectEnemySniper();
					Obj->Position = Pos;

					Push(Obj);
					return Obj;
				}
				break;
			case OBJECT_ENEMY_NOMAD : 
				{
					GObjectEnemyNomad* Obj =new GObjectEnemyNomad();
					Obj->Position = Pos;

					Push(Obj);
					return Obj;
				}
			case OBJECT_EXPLOSION_SMALL1 : 
				{
					GObjectExplosionSmall1* Obj = new GObjectExplosionSmall1();
					Obj->Position = Pos;

					Push(Obj);
					return Obj;
				}
				break ; 

			case OBJECT_EXPLOSION_SMALL2 : 
				{
					GObjectExplosionSmall2* Obj = new GObjectExplosionSmall2();
					Obj->Position = Pos;

					Push(Obj);
					return Obj;
				}
				break ; 
			case OBJECT_MISC_AID :
				{
					GObjectBonusAid* Obj = new GObjectBonusAid();
					Obj->Position = Pos;
					Push(Obj);
					return Obj;
				}
				break ; 
			case OBJECT_MISC_COIN : 
				{
					GObjectBonusCoin* Obj = new GObjectBonusCoin();
					Obj->Position = Pos;

					Push(Obj);
					return Obj;
				}
				break;
			case OBJECT_BULLET_PMP :
				{
					GObjectBulletPmp* Obj = new GObjectBulletPmp();
					Obj->Position = Pos;
					Obj->Facing = *(float*)(pData);

					Push(Obj);
					return Obj;
				}
				break;
			case OBJECT_BULLET_GRENADE : 
				{
					GObjectBulletGrenade* Obj = new GObjectBulletGrenade();
					Obj->Position = Pos; 
					Obj->Facing = *(float*)(pData);

					Push(Obj);
					return Obj;
				}
				break;
			case OBJECT_MISC_PMPICO :
				{
					GObjectBonusPmp* Obj = new GObjectBonusPmp();
					Obj->Position = Pos;

					Push(Obj);
					return Obj;
				}
				break;
			case OBJECT_MISC_CRATE :
				{
					GObjectBonusCrate* Obj = new GObjectBonusCrate();
					Obj->Position = Pos;

					Push(Obj);
					return Obj;
				}
				break;
			case OBJECT_STRUCT_ROCK :
				{
					GObjectDecoreRock* Obj = new GObjectDecoreRock();
					Obj->Position = Pos;

					Push(Obj);
					return Obj;
				}
				break;
			}//end switch

			return 0;
		}
		//end SpawnObject

		namespace Render
		{
			D3DXVECTOR3 CameraPosition(0,0,0);
			std::vector<GObject*> Buffer ; 
			
			void SetCam(D3DXVECTOR3 Pos)
			{
				Render::CameraPosition = Pos;
				return ;
			}

			void SetCamEx()
			{
				CameraPosition = D3DXVECTOR3(Game::Player->Position.x , (float)GlobalData::WindowHeight/2.0f , 0);
				return ;
			}

			void SortBufferByYPos()
			{
				GObject* _buffer;
				long BufferSize = Render::Buffer.size();

				for(long i = 0 ; i < BufferSize ; i++)
					for(long j = i ; j < BufferSize ; j++)
					{
						if(Render::Buffer[i]->Position.y > Render::Buffer[j]->Position.y)
						{
							_buffer = Render::Buffer[i];
							Render::Buffer[i] = Render::Buffer[j];
							Render::Buffer[j] = _buffer;
						}
					}
				return; 
			}//end


			void MoveCamera()
			{
				float delta = (CameraPosition.x - Game::Player->Position.x);
				float distance_eps = 65;

				if(fabs(delta) > distance_eps)
				{
					
					//if delta > 0 player is on the right
					if(delta > 0)
					{
						CameraPosition.x = Game::Player->Position.x + distance_eps;
					}
					if(delta < 0)
					{
						CameraPosition.x = Game::Player->Position.x - distance_eps;
					}

				}


				return;
			}//end SetUp

			void RenderScene()
			{
				Render::MoveCamera();

				Render::Buffer.clear();

				size_t ObjectBufferSize = Game::ObjectBuffer::Buffer.size();

				float Distance = 0.0f;
				float tmp_x = 0.0f;

				//get the objects that are in the scene
				for(size_t i = 0 ; i < ObjectBufferSize ; i++)
				{

					if(!IsBackground( Game::ObjectBuffer::Buffer[i]->ObjectType ))
					Render::Buffer.push_back(Game::ObjectBuffer::Buffer[i]);
	
				}//end
				
				Render::SortBufferByYPos();

				size_t RenderBufferSize = Render::Buffer.size();
				
				D3DXMATRIX CameraTransformInverse;
				D3DXMATRIX ObjTransl , ObjScale;
				float AbsRight;

				D3DXMatrixTranslation(&CameraTransformInverse , -(CameraPosition.x - (float)GlobalData::WindowWidth/2.0f), -(CameraPosition.y - (float)GlobalData::WindowHeight/2.0f) , 0.0f);

				
				GObject* pCurrObj = 0;
				for(size_t i = 0; i < RenderBufferSize ; i++)
				{
					GlobalData::Sprite->Begin(D3DXSPRITE_ALPHABLEND);
					pCurrObj = Game::ObjectBuffer::Render::Buffer[i];
					AbsRight = pCurrObj->Animations[pCurrObj->State].Right;//set Animation Right
					AbsRight *= pCurrObj->Facing;
					if(fabsf(AbsRight) > 1.f)
					{
						AbsRight = AbsRight / fabsf(AbsRight);
					}
					
					D3DXMatrixScaling(&ObjScale , AbsRight , 1 , 1);
					D3DXMatrixTranslation(&ObjTransl , pCurrObj->Position.x , pCurrObj->Position.y , 0.0f);
					
					GlobalData::Sprite->SetTransform(&(ObjScale*ObjTransl*CameraTransformInverse)); 
					GlobalData::Sprite->Draw(pCurrObj->Animations[pCurrObj->State].Texture , &pCurrObj->Animations[pCurrObj->State].Data[pCurrObj->Animations[pCurrObj->State].CurrentFrame] , &pCurrObj->Animations[pCurrObj->State].Center , &D3DXVECTOR3(0,0,0) , 0xffffffff);
					GlobalData::Sprite->SetTransform(&DBOXMatIdent);
					GlobalData::Sprite->End();
				}
				
			
				Render::Buffer.clear();

				return ;
			}//end RenderScene
		}//end namespace Render
	};//end ObjectBuffer

	/*************************************************************************
	Description : Load level form file , filling Game::ObjectBuffer:Buffer
	with object data . 
	params : level id ; file name == "'level' + level_id + '.lev'"
	return value : void ; 
	**************************************************************************/

	void LoadLevel(long LevelID)
	{
		using namespace std;

		Game::ObjectBuffer::ClearAll();

		string FileName = "Levels\\level" + ToString(LevelID) + ".lev" ; 

		 ifstream InputFile ; 
		 InputFile.open(FileName.c_str() , ios::in);

		 if( !InputFile ) 
		 {
			 string ErrorMsg = "Error while loading " + ToString(FileName) + " file !\nApplication will now close .";
			 MessageBox(GlobalData::hWnd , ErrorMsg.c_str() , "Error !" , MB_OK | MB_ICONERROR | MB_DEFBUTTON1);
			 exit(-1);
		 }

		 //set world size
		 InputFile >> Game::InGameArea.x ; 
		 InputFile >> Game::InGameArea.y ; 
		 InputFile >> Game::InGameArea.z ;
		 InputFile >> Game::InGameArea.w ; 

		 string Command = "" ;

		 while(1)
		 {
			 //get command
			 InputFile >> Command ; 
	
			 if(Command == "spawn") 
			 {
				 long id  ; 
				 D3DXVECTOR3 Position ; 
				 float Data ; 

				 InputFile >> id ; //object id
				 InputFile >> Position.x >> Position.y ; // world position
				 Position.z = 0.0f; //default

				 InputFile >> Data ; //float

				 Game::ObjectBuffer::SpawnObject(id , Position ,(void*)&Data);
			 }

			 if(Command == "grenades")
			 {
				 int Data;
				 InputFile >> Data;
				 Game::Grenades = Data;
			 }

			 if(Command == "end") break ;
			 
		 }//end loop

		 InputFile.close();
		 Game::Level = LevelID;//update level id
		return ;
	}//end LoadLevel

};//end Game

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// namespace GlobalData ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace GlobalData
{

	 //Device Manager
	 LPDBOXAPP		Application = 0 ; 
	 LPDBOX3D			GFCtrl = 0 ;
	 LPDBOXINPUT		Input = 0 ;
	 LPD3DXSPRITE		Sprite = 0;
	 LPDIRECT3DDEVICE9 d3ddev = 0;
	 LPDBOXUI			UICtrl = 0;
	 //Window Options
	 long WindowWidth ;
	 long WindowHeight ;
	 BOOL Windowed ;
	 HWND hWnd ; 

	 //Player Controls 
	 int Player1KeyLeft ; 
	 int Player1KeyRight ; 
	 int Player1KeyUp ;
	 int Player1KeyDown ;
	 int Player1KeyFire ;
	 int Player1KeyGrenade ; 

	 /*Read and write data with config file*/

	 void LoadDataFromIni()
	{
		CIniReader CfgFile(CONFIG_FILENAME);

		WindowWidth = (long)CfgFile.ReadInteger("settings" , "window_width" , 800);
		WindowHeight = (long)CfgFile.ReadInteger("settings" , "window_height" ,600);
		Windowed = CfgFile.ReadInteger("settings" , "windowed" , 1);

		Player1KeyLeft = CfgFile.ReadInteger("player1_controls" , "left" , DIK_LEFT);
		Player1KeyRight = CfgFile.ReadInteger("player1_controls" , "right" , DIK_RIGHT);
		Player1KeyUp = CfgFile.ReadInteger("player1_controls" , "up" , DIK_UP);
		Player1KeyDown = CfgFile.ReadInteger("player1_controls" , "down" , DIK_DOWN);
		Player1KeyFire = CfgFile.ReadInteger("player1_controls" , "fire" , DIK_Z);
		Player1KeyGrenade = CfgFile.ReadInteger("player1_controls" , "grenade" , DIK_X);
	}

	void SaveDataToIni()
	{
		CIniWriter CfgFile(CONFIG_FILENAME);
		
		CfgFile.WriteInteger("settings" , "window_width" , WindowWidth);
		CfgFile.WriteInteger("settings" , "window_height" , WindowHeight);
		CfgFile.WriteInteger("settings" , "windowed" , Windowed);

		CfgFile.WriteInteger("player1_controls" , "left" , Player1KeyLeft);
		CfgFile.WriteInteger("player1_controls" , "right" , Player1KeyRight);
		CfgFile.WriteInteger("player1_controls" , "up" , Player1KeyUp);
		CfgFile.WriteInteger("player1_controls" , "down" , Player1KeyDown);
		CfgFile.WriteInteger("player1_controls" , "fire" , Player1KeyFire);
		CfgFile.WriteInteger("player1_controls" , "grenade" , Player1KeyGrenade);
	}

};

//event control functions

DWORD CheckDevice(void*)
{
	HRESULT CooperativeLevelTestResult = GlobalData::GFCtrl->HandleLostDevice(100);

	if(CooperativeLevelTestResult == DBOX3D_DEVSTATE_LOST){OnLostDevice(0);return 1 ;}
	if(CooperativeLevelTestResult == DBOX3D_DEVSTATE_RESETED){OnResetDevice(0);}

	return 0;
}

DWORD OnLostDevice(void*)
{
	GlobalData::Sprite->OnLostDevice();
	Resources::Menu::FontSettings->GetFont()->OnLostDevice();
	Game::Font->GetFont()->OnLostDevice();
	return 0;
}
DWORD OnResetDevice(void*)
{
	GlobalData::Sprite->OnResetDevice();
	Resources::Menu::FontSettings->GetFont()->OnResetDevice();
	Game::Font->GetFont()->OnResetDevice();
	return 0;
}

DWORD InputThread(void*)
{
	GlobalData::Input->UpdateDevices();
	return 0;
}

DWORD OnEnterLoop(void*)
{
		GlobalData::UICtrl->Update();
	return 0;
}
//

DWORD SwitchDisplayMode(BOOL bWindowed)
{

	DWORD WindowStyle = (bWindowed) ? WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_CLIPCHILDREN : WS_POPUP | WS_CLIPCHILDREN | WS_VISIBLE ; 
	SetWindowLong(GlobalData::hWnd, GWL_STYLE , WindowStyle);
	SetMenu(GlobalData::hWnd , 0);

	D3DPRESENT_PARAMETERS d3dpp = GlobalData::GFCtrl->GetPresentParameters();
	
	d3dpp.Windowed = bWindowed;
	if(d3dpp.Windowed)
	{
		d3dpp.BackBufferCount = 0;
		d3dpp.BackBufferWidth = 0;
		d3dpp.BackBufferWidth = 0;
		d3dpp.hDeviceWindow = GlobalData::hWnd;
	}
	else
	{
		d3dpp.BackBufferCount = 1;
		d3dpp.BackBufferWidth = GlobalData::WindowWidth;
		d3dpp.BackBufferHeight = GlobalData::WindowHeight;
		d3dpp.hDeviceWindow = GlobalData::hWnd;
	}
	
	GlobalData::GFCtrl->SetPresentParameters(d3dpp);

	//reset device
	OnLostDevice(0);
	GlobalData::d3ddev->Reset(&d3dpp);
	OnResetDevice(0);
	
	
	return 0;
}

/*****************************************************************************************

retval : const 0 ;

*****************************************************************************************/
DWORD GameProc(void*)
{
	OnEnterLoop(0);
	switch(Game::GameState)
	{
		case GAME_NULL	:					break;
		case GAME_INIT	: GameInit(0) ;		break;
		case GAME_MENU	: GameMainMenu(0) ; break;
		case GAME_MENU_SETTINGS : 
			{
				if(GlobalData::WindowWidth == 640)
					Resources::Menu::Back.SetPos(50*0.8 , 524*0.8);
				else Resources::Menu::Back.SetPos(50 , 524);
				GameSettingsMenu(0);
				break;
			}
		case GAME_LOAD_NEXT_LEVEL :
			{
				Game::GoalReached = 0;//reset the goal
				Game::Level += 1; 
				LoadGame(Game::Level);
				Game::GameState = GAME_PLAYING;
				break;
			}
		case GAME_PLAYING : 
			{
				ProcessGame(0);
				break;
			}
		case GAME_END :
			{
				//NULL All data
				Game::ObjectBuffer::ClearAll();
				Game::Points = 0;
				Game::Player = 0;
				Game::GoalReached = 0;
				Game::Level = 0;
				Game::InGameArea = D3DXVECTOR4(0,0,0,0);
				Game::Grenades = 0;
				Game::PmpAmmo = 0 ;
				//resetGameState
				Game::GameState = GAME_MENU;
	
				break; 
			}
		default : break ;
			
	}
	
	return 0 ;
}