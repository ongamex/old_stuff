//Logic thread

#include "BlackBox.h"


void LogicThread(void* pArg)
{while(1){
	System::GameTimer.Update();
	System::Input->UpdateDevices();
	WaitForSingleObject(System::DataMutex, 0);
	//do logic here
	System::GameTimer.Update();

	if(System::Input->IsKeyDown(DIK_LEFT)){
		DemoAnimation->mOffset.x -= 25.f*System::GameTimer.fDelta;
	}
	if(System::Input->IsKeyDown(DIK_RIGHT)){
		DemoAnimation->mOffset.x += 25.f*System::GameTimer.fDelta;
	}
	ReleaseMutex(System::DataMutex);
}

return;
}