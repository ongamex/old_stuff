#include "DBOXUI.h"

//DBOXUIManager

void DBOXUIManager::Update()
{
	RECT ClRc;
	GetClientRect(this->hWnd , &ClRc);
	Resize = (ClRc.right != this->CurrentWidth || ClRc.bottom != this->CurrentHeight);
	if(Resize)MessageBeep(MB_OK);
	CurrentWidth = (float)ClRc.right;
	CurrentHeight = (float)ClRc.bottom;
	PrevMouseLeftButtonState = MouseLeftButtonState;
	PrevMouseRightButtonState = MouseRightButtonState;
	PrevMouseClientPos = MouseClientPos;

	MouseClientPos = Input->GetMousePosClient();
	MouseLeftButtonState = Input->GetKeyState(DIK_LMB);
	MouseRightButtonState = Input->GetKeyState(DIK_RMB);
	return ;
}
//DBOXUIObject

DBOXUIObject::DBOXUIObject()
{
	Hidden = Locked = false ; 
}

void DBOXUIObject::Update()
{
	return ;
}

void DBOXUIObject::AdaptSizeAndPosition()
{
	return ; 
}

/**************************************************************************************
	Button
***************************************************************************************/

void DBOXUIButton::Create(DBOXUIManager* Manager , RECT aFrames[3] , D3DXVECTOR3* aPosition , LPDIRECT3DTEXTURE9 aTexture)
{

	//Locked = Hidden = false;

	mngr = Manager; 
	Font = NULL;
	Text = "";

	Frames[0] = aFrames[0];
	Frames[1] = aFrames[1];
	Frames[2] = aFrames[2];

	Position = *aPosition ;
	Position.z = DBOXUI_LEVEL;//top layer
	Texture = aTexture;
	FontColor = 0xff222222;
	FontFace = std::string();
	AdaptSizeAndPosition();//font is created here

	//Null the State
	Prev = Now = 0;
	
	return ;
}


void DBOXUIButton::AdaptSizeAndPosition()
{
	float xScale = mngr->CurrentWidth/mngr->BaseWidth;
	float yScale=  mngr->CurrentHeight/mngr->BaseHeight;

	TransformedPosition.x = Position.x*xScale;
	TransformedPosition.y = Position.y*yScale;
	TransformedPosition.z = DBOXUI_LEVEL;

	for(size_t i = 0 ; i < 3 ; i++)
	{
		TransformedFrames[i].left =		(long)( (float)(Frames[i].left)		*xScale );
		TransformedFrames[i].right =	(long)( (float)(Frames[i].right)	*xScale );
		TransformedFrames[i].top =		(long)( (float)(Frames[i].top)		*yScale );
		TransformedFrames[i].bottom =	(long)( (float)(Frames[i].bottom)	*yScale );
	}

	int AlphaH = (int) ( TransformedFrames[0].bottom - TransformedFrames[0].top ) - 14;
	int AlphaW = (int) ( (float)AlphaH*0.4f) ;

	if(Font)
	{
		Font->Release();
		delete Font;
		Font = NULL;
	}

	if(!FontFace.empty())
	{
		LPDIRECT3DDEVICE9 pDevice;
		mngr->Engine2d->GetSprite()->GetDevice(&pDevice);
		Font = DBOXCreateFont( const_cast<CHAR*>(FontFace.c_str()) ,  pDevice , AlphaW , AlphaH  , 50 , FontColor);
	}

	return ; 
}

void DBOXUIButton::Update()
{
	//Adapt
	if(mngr->Resize)
	this->AdaptSizeAndPosition();

	Prev = Now;
	Now = 0;

	float FrameW , FrameH;
	bool IsMouseInTheBox = false;
	bool Collision;
	size_t FrameToDraw = 0;

	if(Hidden)
	{
		return ; //
	}

	if( !Locked )
	{

		AdaptSizeAndPosition();

		FrameW = (float) (TransformedFrames[0].right - TransformedFrames[0].left);
		FrameH = (float) (TransformedFrames[0].bottom - TransformedFrames[0].top);

		if(mngr->MouseClientPos.x >= TransformedPosition.x && mngr->MouseClientPos.x <= TransformedPosition.x + FrameW)
		{if(mngr->MouseClientPos.y >= TransformedPosition.y && mngr->MouseClientPos.y <= TransformedPosition.y + FrameH)
			 IsMouseInTheBox = 1;
		}

		Collision = (IsMouseInTheBox && (mngr->MouseLeftButtonState == DBOXKEY_DOWN) );

		if(IsMouseInTheBox && mngr->MouseLeftButtonState == DBOXKEY_CLICKED) Activated = true;
		if(Activated && !IsMouseInTheBox)Activated = false;

		if(IsMouseInTheBox)
		{
			FrameToDraw = 1;
			if(Collision && Activated)
			{
				FrameToDraw = 2;
				Now = 1;
			}
		}
	}//if( !Locked )...end

	//draw the element
	D3DXMATRIX Translation ; 
	D3DXMatrixTranslation(&Translation , TransformedPosition.x , TransformedPosition.y , TransformedPosition.z);
	D3DXMATRIX Scaling ; 

	//adapt the picture for the resolution
	float sx , sy;
	sx = mngr->CurrentWidth/mngr->BaseWidth;
	sy = mngr->CurrentHeight/mngr->BaseHeight;

	D3DXMatrixScaling(&Scaling , sx , sy , 1.0f);

	mngr->Engine2d->DrawToScreen(Texture , &(Scaling*Translation) , &Frames[FrameToDraw] , DBOX_CENTER_LEFTTOP , 0);
	if(Font)
	{

		TCHAR Str[1000];
#ifdef UNICODE
		swprintf(Str , L"%s" , Text.c_str());
#else
		sprintf(Str , "%s" , Text.c_str());
#endif

		Font->DrawTextEx(Str , DT_VCENTER | DT_CENTER , &Rect((long)(this->TransformedPosition.x) , (long)(this->TransformedPosition.y) , (long)( this->TransformedPosition.x + this->TransformedFrames[FrameToDraw].right - this->TransformedFrames[FrameToDraw].left),(long)( this->TransformedPosition.y + this->TransformedFrames[FrameToDraw].bottom - this->TransformedFrames[FrameToDraw].top)));
	}

	return;
}

BOOL DBOXUIButton::Is(DWORD State)
{
	 DWORD thisState = 0x00;
	thisState += 0x10*Prev;
	thisState += 0x01*Now;
	return (BOOL)(thisState == State);
}

/**************************************************************************************
	ProgressBar
***************************************************************************************/
void DBOXUIProgressBar::Create(DBOXUIManager* Manager , RECT aFrames[2] , D3DXVECTOR3* aPosition , LPDIRECT3DTEXTURE9 aTexture)
{
	mngr = Manager; 
	Texture = aTexture;
	Inverted = false;

	memcpy(Frames , aFrames,  sizeof(RECT)*2);
	Position = *aPosition;
	Position.z = DBOXUI_LEVEL;
	
	this->AdaptSizeAndPosition();

	return ;
}

void DBOXUIProgressBar::AdaptSizeAndPosition()
{
	float xScale = mngr->CurrentWidth/mngr->BaseWidth;
	float yScale=  mngr->CurrentHeight/mngr->BaseHeight;

	TransformedPosition.x = Position.x*xScale;
	TransformedPosition.y = Position.y*yScale;
	TransformedPosition.z = DBOXUI_LEVEL;

	for(size_t i = 0 ; i < 2 ; i++)
	{
		TransformedFrames[i].left =		(long)( (float)(Frames[i].left)		*xScale );
		TransformedFrames[i].right =	(long)( (float)(Frames[i].right)	*xScale );
		TransformedFrames[i].top =		(long)( (float)(Frames[i].top)		*yScale );
		TransformedFrames[i].bottom =	(long)( (float)(Frames[i].bottom)	*yScale );
	}

	return ; 
}

void DBOXUIProgressBar::Update()
{
	//Adapt
	if(mngr->Resize)
	this->AdaptSizeAndPosition();

	float Facing = 1.f ;
	if( Inverted )
	{
		Facing = -1.f;
	}
	this->AdaptSizeAndPosition();
	float Width = (float)(TransformedFrames[0].right - TransformedFrames[0].left);
	float Height = (float)(TransformedFrames[0].bottom - TransformedFrames[0].top);

	D3DXVECTOR3 Center(0,0,0);
	Center.x = (Frames[0].right - Frames[0].left) / 2.f;
	Center.y = (Frames[0].bottom - Frames[0].top) / 2.f;

		//adapt the picture for the resolution
	float sx , sy;
	sx = mngr->CurrentWidth/mngr->BaseWidth;
	sy = mngr->CurrentHeight/mngr->BaseHeight;

	RECT ProgressRect;

	if(Progress > 1.0f)Progress = 1.0f;
	if(Progress < 0.0f)Progress = 0.0f;

	ProgressRect.left = Frames[1].left;
	ProgressRect.top = Frames[1].top;
	ProgressRect.right = (long)(Progress*(float)Frames[1].right);
	ProgressRect.bottom = Frames[1].bottom;

	D3DXMATRIX Translation , Scaling;

	D3DXMatrixTranslation(&Translation ,TransformedPosition.x + (Width)/2,TransformedPosition.y + (Height)/2 , 0.0f);
	D3DXMatrixScaling(&Scaling , Facing*sx , sy , 1.0f);

	mngr->Engine2d->DrawToScreen(Texture , &(Scaling*Translation) , &Frames[0] , DBOX_CENTER_CUSTOM , &Center);
	mngr->Engine2d->DrawToScreen(Texture , &(Scaling*Translation) , &ProgressRect , DBOX_CENTER_CUSTOM , &Center);

	return ;
}

/**************************************************************************************
	Textbox
***************************************************************************************/

void DBOXUITextBox::Create(DBOXUIManager* Manager , LPDIRECT3DTEXTURE9 Texture , char* aFontName , D3DXVECTOR3* aPosition , RECT* pRect , long NumberOfSymb , long xTextOffset , long yTextOffset)
{
	mngr = Manager;
	Time = timeGetTime();

	Activated = 0;

	this->FontName = aFontName;
	this->Font = NULL;
	this->Texture = Texture;

	this->xOffset = xTextOffset;
	this->yOffset = yTextOffset;

	Position = *aPosition ; 
	Position.z = DBOXUI_LEVEL;
	PassMode = 0;
	TextureArea = *pRect;

	TextBoxSize.x = (float)(pRect->right - pRect->left) ; 
	TextBoxSize.y = (float)(pRect->bottom - pRect->top) ; 
	TextBoxSize.z = 0.0f;

	AlphaH = (int) ( TextBoxSize.y ) - (int)yOffset * 2;
	AlphaW = (int) ( (float)AlphaH*0.4f) ;

	Font = 0 ;
	MaxStrSize = NumberOfSymb;

	Text = "";

	this->AdaptSizeAndPosition();

	return ;
}

/*
 This Method resets the member font Font
*/
void DBOXUITextBox::AdaptSizeAndPosition()
{
	float xScale = mngr->CurrentWidth/mngr->BaseWidth;
	float yScale = mngr->CurrentHeight/mngr->BaseHeight;

	TransformedPosition.x = Position.x*xScale;
	TransformedPosition.y = Position.y*yScale;
	TransformedPosition.z = DBOXUI_LEVEL;

	TransformedAlphaW = (int)(((float)(AlphaW))*xScale);
	TransformedAlphaH = (int)(((float)(AlphaH))*yScale);

	this->TransformedXOffset = (long)(xScale*this->xOffset) ;
	this->TransformedYOffset = (long)(yScale*this->yOffset) ;

	TransformedTextBoxSize.x = TextBoxSize.x*xScale;
	TransformedTextBoxSize.y = TextBoxSize.y*yScale;

	if(Font)
	{
		Font->Release();
		Font = NULL;
	}

	LPDIRECT3DDEVICE9 pDevice;
	mngr->Engine2d->GetSprite()->GetDevice(&pDevice);
	Font = DBOXCreateFont(const_cast<char*>(FontName.c_str()) ,  pDevice , TransformedAlphaW , TransformedAlphaH  , 50 , 0xff222222);
	
	return ;
}

void DBOXUITextBox::Update()
{
	//Adapt
	if(mngr->Resize)
	this->AdaptSizeAndPosition();

	if(Hidden)return;

	bool IsMouseInTheBox = false;
	DWORD Delta = timeGetTime() - Time;
	static DWORD BackspaceTime = timeGetTime();//for all objects
	
	if(mngr->MouseClientPos.x >= TransformedPosition.x && mngr->MouseClientPos.x <= TransformedPosition.x + TransformedTextBoxSize.x)
	{if(mngr->MouseClientPos.y >= TransformedPosition.y && mngr->MouseClientPos.y <= TransformedPosition.y + TransformedTextBoxSize.y)
	  IsMouseInTheBox = 1;
	}

	if(IsMouseInTheBox && (mngr->MouseLeftButtonState == DBOXKEY_RELEASED) )
	{
		this->Activated = true;
	}

	if( !IsMouseInTheBox && (mngr->MouseLeftButtonState == DBOXKEY_RELEASED) )
	{
		this->Activated = false;
	}

	if(mngr->Input->IsKeyDown(DIK_RETURN))
	{
		Activated = false;
	}

	Activated &= !Locked;
	
	if(Activated)
	{//Is textbox in use
		
		DWORD Key = mngr->Input->GetKeyByState(DBOXKEY_CLICKED);
		if(Key != 0)
		{
			char CharVal = mngr->Input->GetChar(Key);
			if(CharVal != '\0' && (Text.size() < (unsigned int)((int)this->MaxStrSize)) )
			{
				if(CharVal != '\b')Text+=CharVal;
			}
	
		}

		if(mngr->Input->IsKeyClicked(DIK_BACK))
		{
					if(Text.size() > 0)Text.pop_back();
					BackspaceTime = timeGetTime() + 750;
		}

		int BackDelta = (int)(timeGetTime()) - (int)BackspaceTime;

		if(mngr->Input->IsKeyDown(DIK_BACK) && BackDelta > 17)
		{
				if(Text.size() > 0)Text.pop_back();
					BackspaceTime = timeGetTime();
		}
	}//end if Ativated

	std::string TextToDraw;	

	if(!PassMode)	
	{
		TextToDraw = Text;
	}
	else for(int i = 0; i < (int)Text.size() ; i++)TextToDraw += "*";
	if(Activated) TextToDraw += "...";

	//adapt the picture for the resolution
	float sx , sy;
	sx = mngr->CurrentWidth/mngr->BaseWidth;
	sy = mngr->CurrentHeight/mngr->BaseHeight;
	D3DXMATRIX Scaling;
	D3DXMatrixScaling(&Scaling , sx , sy , 1.0f);

	if(this->Texture)
	{
		D3DXMATRIX Transl;
		D3DXMatrixTranslation(&Transl , TransformedPosition.x , TransformedPosition.y , DBOXUI_LEVEL);

		mngr->Engine2d->DrawToScreen(this->Texture , &(Scaling*Transl) , &TextureArea , DBOX_CENTER_LEFTTOP , NULL);
	}

	DWORD Flags = (Activated) ? DT_RIGHT : DT_CENTER;
	TCHAR Str[1000];
#ifdef UNICODE 
	swprintf_s(Str , L"%s" , TextToDraw.c_str());
#else
	sprintf_s(Str , "%s" , TextToDraw.c_str());
#endif
	Font->DrawTextEx( Str , Flags | DT_VCENTER , &Rect((long)( this->TransformedPosition.x + this->TransformedXOffset ),(long)( this->TransformedPosition.y ),(long) ( this->TransformedPosition.x + this->TransformedTextBoxSize.x - this->TransformedXOffset ),(long)( this->TransformedPosition.y + this->TransformedTextBoxSize.y)) );
	
	return;
}