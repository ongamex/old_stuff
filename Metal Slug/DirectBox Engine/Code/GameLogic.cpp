#include "GameLogic.h"


DWORD LoadGame(int level)
{
	Game::ObjectBuffer::ClearAll();
	Game::LoadLevel((level));
	Game::ObjectBuffer::Render::SetCamEx();

	return 0;
}

DWORD ProcessGame(void* pData)
{
	if(GlobalData::Input->IsKeyClicked(DIK_ESCAPE))
	{
		Sleep(200);
		Game::GameState = GAME_MENU;
		return 0;
	}

	Game::ObjectBuffer::DoObjectsLogic();
	if(Game::GameState != GAME_PLAYING)
	{
		return 0 ;
	}
	Game::ObjectBuffer::Render::MoveCamera();
	Game::ObjectBuffer::Render::RenderScene();

	OPEN_RESOURCES ;

	Menu::HealthBar.SetProgress(Game::Player->Health / 255.0f);
	Menu::HealthBar.Update();

	Game::Font->DrawText(("x "+ToString((int)Game::Points)).c_str() , 31 , 45);
	Game::Font->DrawTextA(("x " + ToString((int)Game::PmpAmmo)).c_str() , 31 , 67);
	Game::Font->DrawTextA(("x " + ToString((int)Game::Grenades)).c_str() , 31 , 90);
	static int CoinFrame = 0 , PmpFrame = 0;
	if(CoinFrame == Resources::Animations::Data[5][1].FramesCnt)CoinFrame = 0;
	if(PmpFrame == Resources::Animations::Data[5][2].FramesCnt)PmpFrame = 0;

	GlobalData::Sprite->Begin(D3DXSPRITE_ALPHABLEND);
	GlobalData::Sprite->SetTransform(&DBOXMatIdent);
	GlobalData::Sprite->Draw(Resources::Animations::Data[5][1].Texture , &Resources::Animations::Data[5][1].Data[CoinFrame] , &D3DXVECTOR3(0 , 0 , 0) , &D3DXVECTOR3(8 , 45 , 0) , 0xffffffff);
	GlobalData::Sprite->Draw(Resources::Animations::Data[5][2].Texture , &Resources::Animations::Data[5][2].Data[PmpFrame] , &D3DXVECTOR3(0 , 0 , 0) , &D3DXVECTOR3(2 , 62 , 0) , 0xffffffff);
	GlobalData::Sprite->Draw(Resources::Animations::Data[2][2].Texture , &Resources::Animations::Data[2][2].Data[26] , &D3DXVECTOR3(0 , 0 , 0) , &D3DXVECTOR3(-94 , 21 , 0) , 0xffffffff);
	GlobalData::Sprite->End();

	CoinFrame += 1;
	PmpFrame += 1;
	return 0;
}