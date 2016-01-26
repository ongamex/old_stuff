#include "DBOXUI_OLD.h"

//UI Controller class

LPDBOXUI DBOXCreateUI(LPD3DXSPRITE pSprite , HWND TargetWindow)
{
	LPDBOXUI retval = new DBOXUI;
	retval->EnableMouseClick = 1;
	retval->LeftMouseButtonState = 0;
	retval->Sprite = pSprite;
	retval->SetTargetWindow(TargetWindow);
	
	return retval;
}

//DBOXUI

DBOXUI::DBOXUI(){Sprite = NULL ; hWnd = NULL;}
DBOXUI::DBOXUI(LPD3DXSPRITE ParamSprite , HWND Target){Sprite = ParamSprite;hWnd = Target;}
DBOXUI::~DBOXUI(){Sprite = NULL ; hWnd = NULL;}

void DBOXUI::SetTargetWindow(HWND Target){hWnd = Target;return;}

HWND DBOXUI::GetTargetWindow() { return hWnd; }		
DBOXUICLIENTAREA DBOXUI::GetClientArea(){return ClientArea;}

void DBOXUI::Update()
{
	if(hWnd == NULL)return;
	ActiveWnd = GetActiveWindow();
	if(ActiveWnd != hWnd) return;
	POINT ClientPos = DBOXGetClientPos(hWnd);
	ClientArea.X = ClientPos.x;
	ClientArea.Y = ClientPos.y;

	RECT ClRc;GetClientRect(hWnd , &ClRc);

	ClientArea.X1 =ClientArea.X + ClRc.right;
	ClientArea.Y1 =ClientArea.Y + ClRc.bottom;

	PrevLeftMouseButtonState = LeftMouseButtonState;
	LeftMouseButtonState = WIN_KEY_DOWN(VK_LBUTTON);
	LMBClick = (LeftMouseButtonState == 1 && PrevLeftMouseButtonState == 0) ? 1  : 0;
	LMBClick &= EnableMouseClick;
	GetCursorPos(&CurrPos);

	return;
}

BOOL DBOXUI::IsApplicationActive(){return (ActiveWnd == hWnd);}

/* ********************************************************************************************************************************* */
//DBOXUIButton

void DBOXUIButton::SetUIController(LPDBOXUI Controller){pController = Controller;return;}
void DBOXUIButton::SetTexture(LPDIRECT3DTEXTURE9 Texture){Tex = Texture;return;}
void DBOXUIButton::SetPos(long x, long y){Pos.x = x;Pos.y = y;return;}
void DBOXUIButton::SetPos(POINT Position){Pos = Position;}
void DBOXUIButton::SetHotkey(DWORD VK_KeyCode){HotKey = VK_KeyCode;}
void DBOXUIButton::SetState(DWORD NewState){State = NewState;Prev = NewState & 0x10;Now = NewState & 0x01; }

LPDBOXUI DBOXUIButton::GetController(){return pController;}
LPDIRECT3DTEXTURE9 DBOXUIButton::GetTexture(){return Tex;}
POINT DBOXUIButton::GetPos(){return Pos;}
void DBOXUIButton::GetRect(void* Data){if(Data == NULL)return;memcpy(Data , StateRect , sizeof(RECT)*3);}
int DBOXUIButton::GetFrame(){return Frame;}
DWORD DBOXUIButton::GetState(){return State;}
DWORD DBOXUIButton::GetHotkey(){return HotKey;}


BOOL DBOXUIButton::CheckState(DWORD DBOXUIState)
{
	switch(DBOXUIState)
	{
	case DBOXUI_UP : return DBOXUIUp(State);
	case DBOXUI_CLICKED : return DBOXUIClicked(State);
	case DBOXUI_RELEASED : return DBOXUIReleased(State);
	case DBOXUI_DOWN : return DBOXUIDown(State);
	default : return 0;
	}
	return 0;
}
BOOL DBOXUIButton::IsHidden(){return Hidden;}
BOOL DBOXUIButton::IsLocked(){return Locked;}

void DBOXUIButton::Create(LPDBOXUI Controller , LPDIRECT3DTEXTURE9 Texture , RECT Areas[3])
{
	pController = Controller;
	Tex = Texture;
	State = 0;
	Locked = 0;
	Hidden = 0;
	Frame = 0;
	Pos.x = 0;Pos.y = 0;
	HotKey = 0;
	Activated = 0;
	memcpy(StateRect , Areas , sizeof(RECT)*3);
	return;
}

void DBOXUIButton::Hide(BOOL bHide){Hidden = bHide;return;}
void DBOXUIButton::Lock(BOOL bLock){Locked = bLock;return;}

HRESULT DBOXUIButton::Update()
{
	if(pController->IsApplicationActive() == FALSE) {Prev = Now;Now = 0x00;return DBOXUI_OK;}
	
	if(Hidden)
	{
		Frame = 0;
		Prev = 0;
		Now = 0;
		State = 0;

		return DBOXUI_OK;
	}

	if(Locked)
	{
		Frame = 0;
		Prev = 0;
		Now = 0;
		State = 0;
	}
else
{
	DBOXUICLIENTAREA Cla = pController->GetClientArea();
	POINT MousePos = pController->CurrPos;//Get mouse position on the screen !

	BOOL Collision = 0 ;
	BOOL MouseClick = pController->LMBClick;
	BOOL InteractEnable = 0;

	int FrameWidth , FrameHeight;
	FrameWidth = StateRect[Frame].right - StateRect[Frame].left;
	FrameHeight = StateRect[Frame].bottom - StateRect[Frame].top;

	//Collision detection
	if(MousePos.x > Pos.x + Cla.X && MousePos.x < Pos.x + Cla.X + FrameWidth)
	{if(MousePos.y > Pos.y + Cla.Y && MousePos.y < Pos.y + Cla.Y +FrameHeight )Collision = 1;}
	
	if(Activated && !Collision)Activated = 0;
	if(Activated && pController->LeftMouseButtonState)InteractEnable = 1;
	if(Collision && MouseClick)Activated = 1;
	if(!pController->LeftMouseButtonState)InteractEnable = 0;
	//Buffer moment button state

	Prev = Now;
	Now = Collision && InteractEnable;

	//Calculate button state

	State = 0x00;
	State += 0x10*Prev;
	State += 0x01*Now;

	//Chose wich frame is needed to be drawn
	if(!Collision)
	{
		Frame = 0;
	}
	else
	{
		if(Collision && !InteractEnable)Frame = 1;
		if(Collision && InteractEnable)Frame = 2;
	}
}//end if
	
	if(HotKey != 0)
		if(WIN_KEY_DOWN(HotKey))
		{
			Prev = Now;
			Now = 0x01;
			State = 0x00;
			State += 0x10*Prev;
			State += 0x01*Now;
			Frame = 2;
		}


	pController->Sprite->Begin(D3DXSPRITE_ALPHABLEND);
	pController->Sprite->Draw(Tex , &StateRect[Frame] , &D3DXVECTOR3(0,0,0) , &D3DXVECTOR3((float)Pos.x , (float)Pos.y , 0.0f) , 0xffffffff);	
	pController->Sprite->End();
	
	return DBOXUI_OK;
}

void DBOXUIButton::Release()
{
	Tex->Release();
	return;
}

/* ************************************************************************************************************************* */
//ProgressBar

void DBOXUIProgressBar::Create(LPDBOXUI Controller,LPDIRECT3DTEXTURE9 Texture, RECT Areas[2])
{
	pController = Controller;
	Tex = Texture;
	if(Areas != NULL)memcpy(Area , Areas , sizeof(RECT)*2);
	Width = Areas[1].right - Areas[1].left;
	Pos.x = 0;
	Pos.y = 0;
	Hidden = 0;
	return;
}

void DBOXUIProgressBar::Release(){Tex->Release();return;}

void DBOXUIProgressBar::SetUIController(LPDBOXUI Controller){pController = Controller;return;}
void DBOXUIProgressBar::SetTexture(LPDIRECT3DTEXTURE9 Texture){Tex = Texture;return;}
LPDBOXUI DBOXUIProgressBar::GetController(){return pController;}
LPDIRECT3DTEXTURE9 DBOXUIProgressBar::GetTexture(){return Tex;}

void DBOXUIProgressBar::SetPos(long x, long y){Pos.x = x;Pos.y = y;return;}
void DBOXUIProgressBar::SetPos(POINT Position){Pos = Position;return;}
POINT DBOXUIProgressBar::GetPos(){return Pos;}

float DBOXUIProgressBar::GetProgress(){return Progress;}
void DBOXUIProgressBar::SetProgress(float fProgress){Progress = fProgress;return;}
void DBOXUIProgressBar::AddProgress(float f){Progress+=f;return;}

void DBOXUIProgressBar::Hide(BOOL Val)
{
	Hidden = Val;
	return ;
}
BOOL DBOXUIProgressBar::IsHidden()
{
	return Hidden;
}

void DBOXUIProgressBar::Update()
{
	if(Hidden) return;
	if(!pController->IsApplicationActive())return;
	
	RECT ProgRect ;
	ProgRect.left  = Area[1].left;
	ProgRect.top   = Area[1].top;
	ProgRect.bottom = Area[1].bottom;
	if(Progress < 0.0f)Progress = 0.0f;
	if(Progress > 1.0f)Progress = 1.0f;
	ProgRect.right = Area[1].left + (long)(Progress*(float)Width);

	pController->Sprite->Begin(D3DXSPRITE_ALPHABLEND);
	pController->Sprite->Draw(Tex, &Area[0] , &D3DXVECTOR3(0,0,0) , &D3DXVECTOR3((float)Pos.x , (float)Pos.y , 0.0f) , 0xffffffff);
	pController->Sprite->Draw(Tex, &ProgRect , &D3DXVECTOR3(0,0,0) , &D3DXVECTOR3((float)Pos.x , (float)Pos.y , 0.0f) , 0xffffffff);
	pController->Sprite->End();
	return;

}

/* ************************************************************************************************************************* */
//Slider

void DBOXUISlider::Create(LPDBOXUI Controller, LPDIRECT3DTEXTURE9 Texture, RECT Areas[], long OffsetX)
{
	Pos.x = 0;
	Pos.y = 0;
	Tex = Texture;
	pController = Controller;
	memcpy(Parts , Areas , sizeof(RECT)*2);

	Offset.x = OffsetX;
	Offset.y = 0;//unused

	SliderWidth = Parts[0].right - Parts[0].left;
	ButtonWidth = Parts[1].right - Parts[1].left;
	/*used for collision detection*/
	ButtonHeight = Parts[1].bottom - Parts[1].top;

	OMinPos = Offset.x;
	OMaxPos = SliderWidth - Offset.x - ButtonWidth; 
	
	OffsetWidth = OMaxPos - OMinPos;

	MinPos = OMinPos + Pos.x;
	MaxPos = OMaxPos + Pos.x;
	ButtonPos.x = Pos.x + OMinPos;
	ButtonPos.y = Pos.y;
	
	Activated = 0;
	PrevMousePos.x = 0;
	PrevMousePos.y = 0;

	Locked = 0;
	Hidden = 0;

	return;
}

void DBOXUISlider::SetPos(long x, long y)
{
	Pos.x = x;
	Pos.y = y;
	
	ButtonPos.x = x + Offset.x;
	ButtonPos.y = Pos.y;

	MinPos = OMinPos + Pos.x;
	MaxPos = OMaxPos + Pos.x;	

	return;
}

void DBOXUISlider::SetPos(POINT Position){
	
	Pos = Position;

	ButtonPos.x += Position.x;
	ButtonPos.y = Pos.y;

	MinPos = OMinPos + Pos.x;
	MaxPos = OMaxPos + Pos.x;

	return;
}

void DBOXUISlider::Hide(BOOL Val)
{Hidden = Val;}
void DBOXUISlider::Lock(BOOL Val)
{Locked = Val;}

BOOL DBOXUISlider::IsHidden()
{return Hidden;}
BOOL DBOXUISlider::IsLocked()
{return Locked;}

void DBOXUISlider::Update()
{
	if(!pController->IsApplicationActive())return;
	if(Hidden)
	{
		Activated = 0;
		return ;
	}
	BOOL Collision = 0;
	POINT CursorPos = pController->CurrPos;
	DBOXUICLIENTAREA Cla = pController->GetClientArea();//client area on the screen
	
	if(Locked)
	{
		pController->Sprite->Begin(D3DXSPRITE_ALPHABLEND);
		pController->Sprite->Draw(Tex , &Parts[0] , &D3DXVECTOR3(0,0,0) , &D3DXVECTOR3((float)Pos.x , (float)Pos.y , 0.0f) , 0xffffffff);
		pController->Sprite->Draw(Tex , &Parts[1] , &D3DXVECTOR3(0,0,0) , &D3DXVECTOR3((float)ButtonPos.x , (float)ButtonPos.y , 0.0f) , 0xffffffff);
		pController->Sprite->End();
		return;
	}

	if(CursorPos.x > Cla.X + ButtonPos.x && CursorPos.x < Cla.X + ButtonPos.x + ButtonWidth)
	{
		if(CursorPos.y > Cla.Y + ButtonPos.y && CursorPos.y < Cla.Y + ButtonPos.y + ButtonHeight)
			Collision = 1;
	}

	if(pController->LeftMouseButtonState == 0)Activated = 0;
	
	if(!Activated)
	{
		if(pController->LMBClick && Collision)
		{
			Activated = 1;
			PrevMousePos.x = pController->CurrPos.x;
			PrevMousePos.y = pController->CurrPos.y;
		}
	}
	
	if(Activated)
	{
		int Delta = CursorPos.x - PrevMousePos.x;
		ButtonPos.x += Delta;
		if(ButtonPos.x < MinPos)ButtonPos.x = MinPos;
		if(ButtonPos.x > MaxPos)ButtonPos.x = MaxPos;
		PrevMousePos = CursorPos;
	}
	
	pController->Sprite->Begin(D3DXSPRITE_ALPHABLEND);
	pController->Sprite->Draw(Tex , &Parts[0] , &D3DXVECTOR3(0,0,0) , &D3DXVECTOR3((float)Pos.x , (float)Pos.y , 0.0f) , 0xffffffff);
	pController->Sprite->Draw(Tex , &Parts[1] , &D3DXVECTOR3(0,0,0) , &D3DXVECTOR3((float)ButtonPos.x , (float)ButtonPos.y , 0.0f) , 0xffffffff);
	pController->Sprite->End();

	return;
}

float DBOXUISlider::GetVal()
{
	float DeltaBM = (float)(ButtonPos.x - MinPos);
	float DeltaMM = (float)(MaxPos - MinPos);
	return DeltaBM/DeltaMM;
}

void DBOXUISlider::SetVal(float Value)
{
	float Delta = (float)(OMaxPos - OMinPos); 	
	long OPos = (long)(Delta*Value);
	ButtonPos.x = Pos.x + OMinPos + OPos;
	return;
}

/* ********************************************************************************************************************************* */
//Checkbox

void DBOXUICheckbox::Create(LPDBOXUI Controller, LPDIRECT3DTEXTURE9 Tex ,RECT pFrameRect[3])
{
	pController = Controller; 
	Texture = Tex;
	memcpy(Frames , pFrameRect , sizeof(RECT)*3);
	Position.x = 0;
	Position.y = 0;
	State = 0 ;

	return ;
}

void DBOXUICheckbox::SetPos(long x, long y)
{
	Position.x = x;
	Position.y = y;

	return;
}

void DBOXUICheckbox::SetPos(POINT Pos)
{	
	Position = Pos;
	return;
}

BOOL DBOXUICheckbox::IsChecked()
{
	return (State == 1);
}

DWORD DBOXUICheckbox::GetState()
{
	return State;
}

void DBOXUICheckbox::SetState(DWORD NewState)
{
	State = NewState;
}

BOOL DBOXUICheckbox::Check(BOOL)
{
		switch(State)
		{
			case 0 : State = 1 ; break ;
			case 1 : State = 0 ; break ;
			default : break ; 
		}

		return State;
}

void DBOXUICheckbox::Update()
{
	if (! pController->IsApplicationActive() ) 
	{

		return ;
	}

	BOOL MouseClick =  pController->LMBClick;
	POINT MousePos = pController->CurrPos;
	DBOXUICLIENTAREA Cla = pController->GetClientArea();

	BOOL Collision =  0 ;
	
	long Width =  Frames[State].right - Frames[State].left;
	long Height = Frames[State].bottom - Frames[State].top;

	if ( MousePos.x > Cla.X + Position.x && MousePos.x < Cla.X + Position.x + Width )
	{
		if ( MousePos.y > Cla.Y + Position.y && MousePos.y < Cla.Y + Position.y + Height)
			Collision = 1 ;
	}
	
	if (Collision && MouseClick) 
	{
		switch(State)
		{
			case 0 : State = 1 ; break ;
			case 1 : State = 0 ; break ;
			default : break ; 
		}
	}

	pController->Sprite->Begin(D3DXSPRITE_ALPHABLEND);
	pController->Sprite->Draw(Texture , &Frames[State] , &D3DXVECTOR3(0,0,0) , &D3DXVECTOR3((float)Position.x  , (float)Position.y , 0.0f) , 0xffffffff);
	pController->Sprite->End();
	return ;
}

