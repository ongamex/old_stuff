#include "Blackbox.h"
#include <fstream>
////////////////////////////////////////
//Constants
///////////////////////////////////////

const float CubeSpeed = 0.755f;
const float Gravity = 0.958f;
const float JumpSpeed = 0.96f;

//////////////////////////////////////////
//Enitity buffer commands
/////////////////////////////////////////////
void ClearEnitityBuffer()
{
	EnitityBuffer.clear();
	return ;
}

void DeleteElement(std::vector<Enitity*>::iterator i)
{
	EnitityBuffer.erase(i);
	return;
}

void SpawnEnitity(int Type,D3DXVECTOR3 Position,void* param)
{
	ECube* Cube ;
	EBlock* Block ;
	EEnd* End;
	switch(Type)
	{
	case ENITITY_CUBE : 
		Level::StartingLevelPos = Position;
		Cube = new ECube;
		Cube->Type = Type;
		Cube->Position = Position;
		Cube->Box = OBB3D(3.4f , 3.3f , 3.4f);
		Cube->zRotation = 0.f;
		Cube->Transform = DBOXMatIdent;
		D3DXMatrixTranslation(&Cube->Transform ,Position.x , Position.y , Position.z);
		EnitityBuffer.push_back(Cube);
		Level::Cube = Cube;
		Camera->SetEye(D3DXVECTOR3(Position.x - 10 , Position.y + 15 , 0));
		break;
	case ENITITY_CONE :
	case ENITITY_STAIR :
	case ENITITY_CREEP :
	case ENITITY_LINE :
	case ENITITY_STAR :
		Block = new EBlock;
		Block->Type = Type;
		Block->Position = Position;
		Block->Transform = DBOXMatIdent;
		D3DXMatrixTranslation( &Block->Transform , Position.x , Position.y , Position.z);
		if(Type != ENITITY_CONE)
		{	Block->Box = OBB3D(3.4f , 3.3f , 3.4f); }
		else
		{	Block->Box = OBB3D(0.8f , 2.4f , 3.4f);}
		
		EnitityBuffer.push_back(Block);
		break;
	case ENITITY_END :
		Level::EndingLevelPos = Position;
		End = new EEnd;
		End->Position = Position;
		End->Type = Type;
		EnitityBuffer.push_back(End);
		break;
	//default : exit(-1);
	}
	return ;
}
/////////////////////////////////////////////
//Read level Data and set level vars to default
//////////////////////////////////////////////////
void LoadLevel(std::string filename)
{
	Level::pFlagPos = 0;
	Level::CollectedStars = 0;
	Level::TotalStars = 0;
	if(Level::pFlagPos){delete Level::pFlagPos; Level::pFlagPos = nullptr;}

	std::ifstream in(filename.c_str());
	std::string command;
	ClearEnitityBuffer();
	while(!in.eof())
	{
		in >> command;
		if(command == "end")
		{
			break;
		}
		if(command == std::string("spwn"))
		{
			int Type;D3DXVECTOR3 Position(0,0,0);
			in >> Type;
			in >> Position.x;
			in >> Position.y;
			SpawnEnitity(Type , Position , nullptr);
		}
	}
}

void RenderEnitities()
{
	for(int i = 0; i < EnitityBuffer.size() ; ++i)
	{
		if(EnitityBuffer[i]->Type < 300)//everything over 300 does not have model
		{
			if(Vec3Dist(&Level::Cube->Position , &EnitityBuffer[i]->Position) < 110.f)
			Meshes[EnitityBuffer[i]->Type].Render(EnitityBuffer[i]->Transform);
		}
	}
	if(Level::pFlagPos)
	{
		D3DXMATRIX Transform;
		D3DXMatrixTranslation(&Transform , Level::pFlagPos->x , Level::pFlagPos->y , 0);
		Meshes[ENITITY_FLAG].Render(Transform);
	}
}
/////////////////////////////////////////////////////
///Other objects logic
/////////////////////////////////////////////////////
DWORD EBlock::Update()
{
	D3DXMatrixTranslation(&this->Transform , Position.x , Position.y , Position.z);
	Box.SetTransform(this->Transform);
	return 0;
}

DWORD EEnd::Update()
{
	if(Level::Cube->Position.x >= this->Position.x)
	{
		if(Profiles::ActiveProfile->UnlockedLevel == Level::LevelNumber)
		Profiles::ActiveProfile->UnlockedLevel = Level::LevelNumber + 1;
		Profiles::Save();
		UIm.ChangeActivePage("lcompleate");
	}
	return 0;
}

//Cube 

void Jump(ECube* pCube)
{
}
///////////////////////////////////////////////
//Cube logic
///////////////////////////////////////////////

DWORD ECube::Update()
{
	static int Jumping = 0;
	static DWORD JumpTimer = 0;

	D3DXMatrixTranslation(&this->Transform ,this->Position.x , this->Position.y , this->Position.z);
	this->Box.SetTransform(this->Transform);
	D3DXMATRIX Rotation;
	
	D3DXMatrixRotationZ(&Rotation , this->zRotation);
	Transform = Rotation*Transform;
	

	Transform = Rotation*Transform;
	enum {
		COLLISION_NONE = 1,
		COLLISION_FATAL ,
		COLLISION_SLIDING
	};
	int CollisionSituation = COLLISION_NONE;

	this->Position.x += CubeSpeed;

	for(int i = 0;i < EnitityBuffer.size();i++)
	{
		if(EnitityBuffer[i]->Type == ENITITY_CUBE)continue;
		if(EnitityBuffer[i]->Type > 300)continue;
		if(OBB3DCollide(this->Box , EnitityBuffer[i]->Box ))
		{
			if(EnitityBuffer[i]->Type == ENITITY_STAR)
			{
				Level::CollectedStars++;
				EnitityBuffer.erase(EnitityBuffer.begin() + i);
				i--;
				continue;
			}

			if(EnitityBuffer[i]->Type == ENITITY_CONE || EnitityBuffer[i]->Type == ENITITY_CREEP)
			{
				CollisionSituation = COLLISION_FATAL;
				break;
			}
			else if (EnitityBuffer[i]->Type == ENITITY_STAIR || EnitityBuffer[i]->Type == ENITITY_LINE)
			{
				if(this->Position.y - EnitityBuffer[i]->Position.y >= 1.5f){this->Position.y += (3.2 - fabsf(this->Position.y - EnitityBuffer[i]->Position.y));CollisionSituation = COLLISION_SLIDING; }
				else { CollisionSituation = COLLISION_FATAL; break;}
			}	
		}//end collision check
	}//end loop

	//Check jumping
	if(Input->IsKeyDown(Profiles::ActiveProfile->Jump) && CollisionSituation == COLLISION_SLIDING)
	{
		Jumping = true;
		JumpTimer = timeGetTime();
	}

	//Process jumping
	if(Jumping)
	{
		if(JumpTimer + 140 > timeGetTime())
		{
			this->Position.y += JumpSpeed;
		}
		else if(JumpTimer + 210 > timeGetTime())//hold in the air
		{

		}
		else
		{
			Jumping = false;
		}
	}

	//Get the current position height
	BOOL Hit = true;
	float Distance = FLT_MAX, MinDist = FLT_MAX;
	for(int i = 0;i < EnitityBuffer.size();i++)
	{
		if(EnitityBuffer[i]->Type == ENITITY_CUBE)continue;
		float tDist;BOOL tHit;
		D3DXVECTOR3 p = Position - EnitityBuffer[i]->Position;
		p.x +=1.7;
		if(fabsf(this->Position.x - EnitityBuffer[i]->Position.x) < 6.9f){
		D3DXIntersect(Meshes[3].pMesh , &p , &(D3DXVECTOR3(0,-1.f,0)) , &tHit , 0 , 0 , 0 ,&tDist , 0 , 0);
		if(tHit){Hit = 1;Distance = tDist;if(Distance < MinDist)MinDist = Distance;}}
	}

/*	for(int i = 0;i < EnitityBuffer.size();i++)
	{
		if(EnitityBuffer[i]->Type == ENITITY_CUBE)continue;
		float tDist;BOOL tHit;
		D3DXVECTOR3 p = Position - EnitityBuffer[i]->Position;
		p.x -=1.7;
		if(fabsf(this->Position.x - EnitityBuffer[i]->Position.x) < 7.9f){
		D3DXIntersect(Meshes[3].pMesh , &p , &(D3DXVECTOR3(0,-1.f,0)) , &tHit , 0 , 0 , 0 ,&tDist , 0 , 0);
		if(tHit){Hit = 1;Distance = tDist;if(Distance < MinDist)MinDist = Distance;}}
	}*/
	
		for(int i = 0;i < EnitityBuffer.size();i++)
	{
		if(EnitityBuffer[i]->Type == ENITITY_CUBE || EnitityBuffer[i]->Type == ENITITY_END)continue;
		float tDist;BOOL tHit;
		D3DXVECTOR3 p = Position - EnitityBuffer[i]->Position;
		if(fabsf(this->Position.x - EnitityBuffer[i]->Position.x) < 7.9f){	
		D3DXIntersect(Meshes[3].pMesh , &p , &(D3DXVECTOR3(0,-1.f,0)) , &tHit , 0 , 0 , 0 ,&tDist , 0 , 0);
		if(tHit){Hit = 1;Distance = tDist;if(Distance < MinDist)MinDist = Distance;}}
	}
	Distance = MinDist;

	//animate
	if(Distance > 1.8f)
	{
		zRotation -= D3DX_PI/37.f;
	}
	else
		zRotation = 0.0f;


	//act the gravity
	if(CollisionSituation == COLLISION_NONE && !Jumping)
	{
		if(Distance > Gravity)
		{
			this->Position.y -= Gravity;
		}
		else
		{
			this->Position.y = this->Position.y - Distance + 1.5f;
		}
	}

	//dead
	if(CollisionSituation == COLLISION_FATAL)
	{
		Particles::Stars::RemoveAll();
		Sleep(250);
		if(Level::pFlagPos)
			this->Position = *Level::pFlagPos;
		else
		this->Position = Level::StartingLevelPos;
		Camera->SetEye(D3DXVECTOR3(Position.x - 10 , Position.y + 10 , 0));
		Jumping = false;
	}

	//create flag
	if(Input->IsKeyClicked(Profiles::ActiveProfile->Flag))
	{
		delete Level::pFlagPos; Level::pFlagPos = nullptr;
		if(!Level::pFlagPos)
		{
			Level::pFlagPos = new D3DXVECTOR3(this->Position);
		}
	}

	if(Input->IsKeyClicked(DIK_E))
	{
		delete Level::pFlagPos; Level::pFlagPos = nullptr;
	}

	//move camera
	Camera->SetEye(D3DXVECTOR3(Position.x , Camera->GetEye().y , 0));

	float CameraSpeed = 0.33f;
	if(fabsf(Camera->GetEye().y - this->Position.y) > 9.f)
	{
		CameraSpeed = Gravity;
	}

	if(Position.y - 1.f > Camera->GetEye().y)Camera->MoveInDirection(D3DXVECTOR3(0,1,0) , CameraSpeed);
	
	if(Position.y + 1.f < Camera->GetEye().y) Camera->MoveInDirection(D3DXVECTOR3(0,-1,0) , CameraSpeed);
	return 0;
}

