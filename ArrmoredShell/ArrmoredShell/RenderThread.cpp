//Rendering thread - this is just a function that is in main thread cause d3ddevice lost 

#include "BlackBox.h"

void OnLostDevice();
void ResetDevice();

//Rendering thread 
void RenderThread(void* pArg) 
{
	System::RenderingTimer.Update();

	//Get All the data needed for the rendering
	WaitForSingleObject(System::DataMutex, 0);//waits the logic to release the mutex
	

	//free the mutex so the logic can go on
	ReleaseMutex(System::DataMutex);

	//IDirect3DDevice9 and ID3DXSprite are handled by that function 
	HRESULT hr = System::Ri3D->HandleLostDevice(200);
	if(hr == DBOX3D_DEVSTATE_LOST){
		//on lost device
		OnLostDevice();
		Sleep(10);
		return;
	}
	else if(hr == DBOX3D_DEVSTATE_RESETED){
		ResetDevice();
		//on resetDevice	
	}

	//Render like hell
	System::Ri3D->BeginScene(0xffff00ff);

	//DemoAnimCtrl.Animate(System::RenderingTimer.fDelta, &DBOXMatIdent);
	for(auto itr = pAnims.begin(); itr != pAnims.end(); itr++)
		(*itr)->Animate(System::RenderingTimer.fDelta, &DBOXMatIdent);


	System::Ri3D->EndScene();//present

}//end function

//Resource management
void OnLostDevice(){
	lDBOXFontManger.OnLostDevice();
	return;
}

void ResetDevice(){ 
	lDBOXFontManger.OnResetDevice();
	return;
}