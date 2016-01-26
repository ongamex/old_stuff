#include "Blackbox.h"

void Particles::Stars::AddNew()
{
	Particles::Stars::Particle Star;
	Star.Color = D3DCOLOR_ARGB(0xff,rand()%255,rand()%255,rand()%255);
	Star.Born = timeGetTime();
	Star.Rotation = (float(rand()%100)/100)*D3DX_PI*2.f;
	Star.Position = Level::Cube->Position;
	Star.Position.y -= 3.0f;
	Star.Position.x -= 3.7f;
	Star.Position.y += (float(rand()%100)/100)*5.f;
	Particles::Stars::Buffer.push_back(Star);
}

void Particles::Stars::RemoveAll()
{
	Particles::Stars::Buffer.clear();
}

void Particles::Stars::Process()
{
	const DWORD lifetime = 1800;


	DWORD time = timeGetTime();
	for(int i = 0; i < Particles::Stars::Buffer.size() ;i++)
	{
		if(time - Particles::Stars::Buffer[i].Born > lifetime)
		{
			//particle is dead
			Particles::Stars::Buffer.erase(Particles::Stars::Buffer.begin() + i);
			i--;
			continue;
		}
		Particles::Stars::Buffer[i].Rotation -= 0.03f;
		D3DXMATRIX world , view , proj , transl , scaling , rotation;
		D3DVIEWPORT9 vp;

		Engine3D->GetDevice()->GetTransform(D3DTS_WORLD , &world);
		Engine3D->GetDevice()->GetTransform(D3DTS_VIEW , &view);
		Engine3D->GetDevice()->GetTransform(D3DTS_PROJECTION ,&proj);
		Engine3D->GetDevice()->GetViewport(&vp);

		D3DXVECTOR3 CubePosition , Result;

		D3DXVec3Project(&Result , &Particles::Stars::Buffer[i].Position , &vp , &proj , &view , &DBOXMatIdent);
		
		Result.z = 0.f;
		float ScaleFactor = (float)((float)(time) - Particles::Stars::Buffer[i].Born + lifetime/10)/float(lifetime/4);
		
		D3DXMatrixTranslation(&transl , Result.x ,Result.y , 0);
		D3DXMatrixScaling(&scaling , ScaleFactor*UIm.scaleX ,ScaleFactor*UIm.scaleY , 1.f);
		D3DXMatrixRotationZ(&rotation , Particles::Stars::Buffer[i].Rotation);
		
		Engine2D->DrawToScreen(Particles::Stars::Star , &(scaling*rotation*transl) , 0 , DBOX_CENTER_CENTER , 0 , Particles::Stars::Buffer[i].Color );
	}
	return;
}
