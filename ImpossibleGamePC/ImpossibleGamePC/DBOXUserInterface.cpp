#include "DBOXUserInterface.h"

#ifndef __assert
#define __assert(_x) { if(!_x) return false ; } 
#endif

#ifndef __safe_release
#define __safe_release(_p) { if(_p) { (_p) -> Release() ; ; _p = nullptr ; } }
#endif


bool Point2Rect(lPoint p , lRect r)
{
	return ((p.x >= r.left) && (p.x <= r.right) && (p.y >= r.top) && (p.y <= r.bottom));
}


//void DBOXUIObject::Update() {return ;}

bool DBOXUIManager::Create(DBOXUIPROPERTIES properties)
{
	__assert(properties.eng2d);
	__assert(properties.inp);

	memcpy(&this->props , &properties , sizeof(DBOXUIPROPERTIES));

	HRESULT hr = D3DXCreateTextureFromFileExA(props.pDevice , props.TextureName.c_str() , D3DX_DEFAULT , D3DX_DEFAULT , D3DX_DEFAULT , 0 , 
		D3DFMT_A8R8G8B8 , D3DPOOL_MANAGED , D3DX_DEFAULT , D3DX_DEFAULT , props.TextureKeyColor , 0  , 0 , &this->pTexture);

	if(FAILED(hr)) return false;

	this->UpdateContent();

	return true;
}


void DBOXUIManager::UpdateContent()
{
	__safe_release(this->ButtonFont);
	__safe_release(this->CheckboxFont);
	__safe_release(this->TextboxFont);
	__safe_release(this->LabelFont);

	scaleX =((float)(props.eng2d->ScreenWidth))/((float)(props.BaseWidth));
	scaleY =((float)(props.eng2d->ScreenHeight))/((float)(props.BaseHeight));

	//calculate scaled button rects
	for(int i = 0; i < 3 ; ++i)
	{
		transfBntRects[i].left = (long)( (float)(props.BtnRects[i].left)*scaleX );
		transfBntRects[i].right = (long)( (float)(props.BtnRects[i].right)*scaleX );
		transfBntRects[i].top = (long)( (float)(props.BtnRects[i].top)*scaleY );
		transfBntRects[i].bottom = (long)( (float)(props.BtnRects[i].bottom)*scaleY );
	}

	for(int i = 0; i < 2 ; ++i)
	{
		transfCheckboxRects[i].left = (long)( (float)(props.CheckboxRects[i].left)*scaleX );
		transfCheckboxRects[i].right = (long)( (float)(props.CheckboxRects[i].right)*scaleX );
		transfCheckboxRects[i].top = (long)( (float)(props.CheckboxRects[i].top)*scaleY );
		transfCheckboxRects[i].bottom = (long)( (float)(props.CheckboxRects[i].bottom)*scaleY );

		transfSliderRect[i].left = (long)( (float)(props.SliderRect[i].left)*scaleX );
		transfSliderRect[i].right = (long)( (float)(props.SliderRect[i].right)*scaleX );
		transfSliderRect[i].top = (long)( (float)(props.SliderRect[i].top)*scaleY );
		transfSliderRect[i].bottom = (long)( (float)(props.SliderRect[i].bottom)*scaleY );

		transfProgressBar[i].left = (long)( (float)(props.ProgressBar[i].left)*scaleX );
		transfProgressBar[i].right = (long)( (float)(props.ProgressBar[i].right)*scaleX );
		transfProgressBar[i].top = (long)( (float)(props.ProgressBar[i].top)*scaleY );
		transfProgressBar[i].bottom = (long)( (float)(props.ProgressBar[i].bottom)*scaleY );

	}

	transfTextboxRect.left = (long)( (float)(props.TextboxRect.left)*scaleX );
	transfTextboxRect.right = (long)( (float)(props.TextboxRect.right)*scaleX );
	transfTextboxRect.top = (long)( (float)(props.TextboxRect.top)*scaleY );
	transfTextboxRect.bottom = (long)( (float)(props.TextboxRect.bottom)*scaleY );
	//Button Alpha 
	
	int AlphaH = (int) ( transfBntRects[0].bottom - transfBntRects[0].top ) - 35;
	int AlphaW = (int) ( (float)AlphaH* 0.33f) ;

	ButtonFont = DBOXCreateFont(const_cast<char*>(props.FontFace.c_str()) , props.pDevice , AlphaW , AlphaH , 700 , props.FontColor);

	//CheckBox Alpha

	AlphaH = (int) ( transfCheckboxRects[0].bottom - transfCheckboxRects[0].top );
	AlphaW = (int) ( (float)AlphaH*0.33f) ;

	CheckboxFont = DBOXCreateFont(const_cast<char*>(props.FontFace.c_str()) , props.pDevice , AlphaW , AlphaH , 700 , props.FontColor);

	//Textbox Aplpha 

	AlphaH = (int) ( transfTextboxRect.bottom - transfTextboxRect.top ) - 35;
	AlphaW = (int) ( (float)AlphaH*0.33f) ;

	TextboxFont = DBOXCreateFont(const_cast<char*>(props.FontFace.c_str()) , props.pDevice , AlphaW , AlphaH , 700 , props.FontColor);
	
	AlphaH = (int) ( props.LabelWordSize )*scaleY;
	AlphaW = (int) ( (float)AlphaH*0.33f) ;

	LabelFont = DBOXCreateFont(const_cast<char*>(props.FontFace.c_str()) , props.pDevice , AlphaW , AlphaH , 700 , props.FontColor);
	
}

void DBOXUIManager::Update()
{
	RECT rc;
	GetClientRect(this->props.hWnd , &rc);
	//if(rc.right != props.eng2d->ScreenWidth || rc.bottom != props.eng2d->ScreenHeight)
	UpdateContent();//if we have a resize of lost of device

	this->IsWindowActive = (props.hWnd == GetActiveWindow());
	this->LMBState = props.inp->IsKeyDown(DIK_LMB);
	this->RMBState = props.inp->IsKeyDown(DIK_RMB);
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(props.hWnd , &p);
	this->MousePosition.x = p.x;
	this->MousePosition.y = p.y;

	
		auto i = this->Pages[ActivePage];
		if( i.begin() != i.end() ){
		for(auto j = i.begin() ; j != i.end() ; ++j)
		{
			j->second->Update();
		}
		}
	
}

void DBOXUIManager::ChangeActivePage(std::string Page)
{
	this->Update();
	this->ActivePage = Page;
}

///////////////////////////////////////////////////////////
//Button Code
///////////////////////////////////////////////////////////

void DBOXUIButton::Create(DBOXUIManager* mngr , std::string Text , lPoint Position)
{
	this->pManager = mngr;
	this->InfoState.strValue = Text;
	this->position = Position;
	this->prevState = this->currState = 0;
	this->InfoState.Locked = this->InfoState.Hidden = 0;
}

void DBOXUIButton::Update()
{
	if(InfoState.Hidden){currState = prevState =0; return;}

	prevState = currState;
	currState = 0;

	lRect boundingRect ;

	boundingRect.left  = (float)(position.x)*pManager->scaleX;
	boundingRect.right = ((float)(position.x + pManager->props.BtnRects[0].right - pManager->props.BtnRects[0].left)) *pManager->scaleX;
	boundingRect.top = (float)(position.y)*pManager->scaleY;
	boundingRect.bottom = ((float)(position.y + pManager->props.BtnRects[0].bottom -  pManager->props.BtnRects[0].top))*pManager->scaleY;

	int frameToDraw = 0;

	bool Collision = Point2Rect(pManager->MousePosition , boundingRect);
	
	if(!InfoState.Locked && Collision && ! InfoState.Locked) frameToDraw = 1;

	if(frameToDraw && pManager->LMBState) frameToDraw = 2;

	currState = (frameToDraw == 2);
	
	InfoState.KeyState = 0;
	InfoState.KeyState = 0x10*(prevState!=0) + 0x01*(currState!=0);

	D3DXMATRIX transl,scal;
	D3DXMatrixTranslation(&transl , boundingRect.left , boundingRect.top , 0);
	D3DXMatrixScaling(&scal , pManager->scaleX , pManager->scaleY , 1.f);

	//if UI is locked, draw it transperant
	DWORD ColorFilter = (InfoState.Locked) ? 0x55FFFFFF : 0xffffffff;

	pManager->props.eng2d->DrawToScreen(pManager->pTexture , &(scal*transl) , (RECT*)&pManager->props.BtnRects[frameToDraw] , DBOX_CENTER_LEFTTOP , 0 , ColorFilter);
	pManager->ButtonFont->DrawTextEx(const_cast<char*>(this->InfoState.strValue.c_str() ), DT_VCENTER|DT_CENTER , (RECT*)(&boundingRect) );

}

void DBOXUIButton::Set(DBOXUIState state)
{

}

DBOXUIObject* DBOXUICreateButton(DBOXUIManager* mngr , std::string Text , lPoint Position)
{
	DBOXUIButton* retval = new DBOXUIButton;
	retval->Create(mngr , Text , Position);
	return retval;
}
///////////////////////////////////////////////////////////
//Texbox
///////////////////////////////////////////////////////////

void DBOXUITextbox::Create(DBOXUIManager* mngr , int limit  , lPoint Position , std::string Text)
{
	this->backspaceTimer = 0;
	this->InfoState.Active = false;
	this->InfoState.PasswordMode = false;
	this->InfoState.Locked = this->InfoState.Hidden = 0;

	this->pManager = mngr;
	this->position = Position;
	this->InfoState.strValue = Text;
	this->InfoState.CharacterLimit = limit;
}

void DBOXUITextbox::Update()
{
	if(InfoState.Hidden)return;

	lRect boundingRect;
	boundingRect.left = (float)(position.x)*pManager->scaleX;
	boundingRect.top = (float)(position.y)*pManager->scaleY;
	boundingRect.right = (float)(position.x + pManager->props.TextboxRect.right - pManager->props.TextboxRect.left)*pManager->scaleX;
	boundingRect.bottom = (float)(position.y + pManager->props.TextboxRect.bottom - pManager->props.TextboxRect.top)*pManager->scaleY;

	bool Collision = Point2Rect(pManager->MousePosition , boundingRect);
	if(Collision && pManager->LMBState)
	{
		this->InfoState.Active = true;
	}
	if(!Collision && pManager->LMBState)
	{
		this->InfoState.Active = false;
	}
	if(pManager->props.inp->IsKeyDown(DIK_RETURN) && this->InfoState.Active)
	{
		this->InfoState.Active  = false;
	}
	
	std::string textToDraw = this->InfoState.strValue;
	if(this->InfoState.PasswordMode)
	{
		for(int i = 0 ; i < textToDraw.length() ; ++i)
			textToDraw[i] = '*';
	}


	
	if(this->InfoState.Active && !InfoState.Locked)
	{
				if ( this->InfoState.strValue.length()  < this->InfoState.CharacterLimit )
				{
					DWORD Key = pManager->props.inp->GetKeyByState(DBOXKEY_CLICKED);
					char keyVal = pManager->props.inp->GetChar(Key);
					if(keyVal != '\0' && keyVal != '\b')
					{
						this->InfoState.strValue += keyVal;
					}
				}
					
				if(pManager->props.inp->IsKeyClicked(DIK_BACK))
				{
						if(this->InfoState.strValue.size() > 0)this->InfoState.strValue.pop_back();
						this->backspaceTimer = timeGetTime() + 750;
				}

				int BackDelta = (int)(timeGetTime()) - (int)this->backspaceTimer;

				if(pManager->props.inp->IsKeyDown(DIK_BACK) && BackDelta > 17)
				{
						if(this->InfoState.strValue.size() > 0) this->InfoState.strValue.pop_back();
							this->backspaceTimer = timeGetTime();
				}
	
				textToDraw += std::string("...");
			
	}
	
	D3DXMATRIX transl,scal;
	D3DXMatrixTranslation(&transl , boundingRect.left , boundingRect.top , 0);
	D3DXMatrixScaling(&scal , pManager->scaleX , pManager->scaleY , 1.f);

	//if UI is locked, draw it transperant
	DWORD ColorFilter = (InfoState.Locked) ? 0x55FFFFFF : 0xffffffff;
	
	pManager->props.eng2d->DrawToScreen(pManager->pTexture , &(scal*transl) , (RECT*)&pManager->props.TextboxRect , DBOX_CENTER_LEFTTOP , 0 ,ColorFilter);
	
	if(this->InfoState.Active && !InfoState.Locked)
		pManager->TextboxFont->DrawTextEx(const_cast<char*>(textToDraw.c_str()) , DT_RIGHT | DT_VCENTER , (RECT*)&boundingRect);
	else
		pManager->TextboxFont->DrawTextEx(const_cast<char*>(textToDraw.c_str()) , DT_CENTER | DT_VCENTER , (RECT*)&boundingRect);

	return ;
}

void DBOXUITextbox::Set(DBOXUIState)
{

}

DBOXUIObject* DBOXUICreateTextbox(DBOXUIManager* mngr , int limit , lPoint Position , std::string Text)
{
	DBOXUITextbox* retval = new DBOXUITextbox;
	retval->Create(mngr , limit , Position , Text);
	return retval;
}

//////////////////////////////////////////////////////
//Checkbox
//////////////////////////////////////////////////////

void DBOXUICheckbox::Create(DBOXUIManager* mngr, lPoint Position, std::string Text)
{
	this->pManager = mngr;

	this->InfoState.Checked = false;
	this->InfoState.strValue = Text;
	this->InfoState.Locked = this->InfoState.Hidden = 0;

	this->position = Position;

	prevState = currState = false;
}

void DBOXUICheckbox::Update()
{
	if(this->InfoState.Hidden){ prevState = currState = 0 ; return ;}

	prevState = currState;
	currState = 0;

	lRect boundingRect ;

	boundingRect.left  = (float)(position.x)*pManager->scaleX;
	boundingRect.right = ((float)(position.x + pManager->props.CheckboxRects[0].right - pManager->props.CheckboxRects[0].left)) *pManager->scaleX;
	boundingRect.top = (float)(position.y)*pManager->scaleY;
	boundingRect.bottom = ((float)(position.y + pManager->props.CheckboxRects[0].bottom -  pManager->props.CheckboxRects[0].top))*pManager->scaleY;

	bool Collision = Point2Rect(pManager->MousePosition , boundingRect);

	if(Collision && pManager->props.inp->IsKeyReleased(DIK_LMB) && ! InfoState.Locked)
	{
		this->InfoState.Checked = ! (this->InfoState.Checked);
		currState = 1;
	}

	this->InfoState.KeyState = 0;
	this->InfoState.KeyState = 0x10*prevState + 0x01*currState;

	D3DXMATRIX transl,scal;
	D3DXMatrixTranslation(&transl , boundingRect.left , boundingRect.top , 0);
	D3DXMatrixScaling(&scal , pManager->scaleX , pManager->scaleY , 1.f);

	DWORD ColorFilter = (InfoState.Locked) ? 0x55FFFFFF : 0xffffffff;

	int frameToDraw = 0;
	if(InfoState.Checked) frameToDraw = 1;

	this->pManager->props.eng2d->DrawToScreen(pManager->pTexture , &(scal*transl) , (RECT*)&pManager->props.CheckboxRects[frameToDraw] , DBOX_CENTER_LEFTTOP , 0 , ColorFilter);
	
	boundingRect.left = boundingRect.right + 5.f*pManager->scaleX;
	boundingRect.right = (float)(pManager->props.eng2d->ScreenWidth)*1.33f;
	
	this->pManager->CheckboxFont->DrawTextEx(const_cast<char*>(this->InfoState.strValue.c_str()) , DT_VCENTER | DT_LEFT , (RECT*)&boundingRect);

	return ;
}

void DBOXUICheckbox::Set(DBOXUIState)
{

}

DBOXUIObject* DBOXUICreateCheckbox(DBOXUIManager* mngr, lPoint Position, std::string Text)
{
	DBOXUICheckbox* retval = new DBOXUICheckbox;

	retval->Create(mngr , Position , Text);

	return retval;
}

//////////////////////////////////////////////////////////
//Progressbar
/////////////////////////////////////////////////////////

void DBOXUIProgressbar::Create(DBOXUIManager* mngr , lPoint Position , float Value){

	this->InfoState.Locked = this->InfoState.Hidden = 0;
	this->InfoState.Checked = false;
	this->InfoState.Amount = Value;

	this->pManager = mngr;
	this->position = Position;
}

void DBOXUIProgressbar::Update()
{

	if(InfoState.Amount < 0.f)InfoState.Amount = 0.f;
	if(InfoState.Amount > 1.f)InfoState.Amount = 1.f;

	if(this->InfoState.Hidden){return;}


	lPoint TransfPos;
	float transfW = pManager->transfProgressBar[0].right - pManager->transfProgressBar[0].left;
	float transfH = pManager->transfProgressBar[0].bottom - pManager->transfProgressBar[0].top;

	
	TransfPos.x  = (float)(position.x)*pManager->scaleX + transfW/2.f;
	TransfPos.y  = (float)(position.y)*pManager->scaleY + transfH/2.f;

	D3DXVECTOR3 Center;
	Center.x  = float(pManager->props.ProgressBar[0].right - pManager->props.ProgressBar[0].left) / 2.f;
	Center.y  = float(pManager->props.ProgressBar[0].bottom - pManager->props.ProgressBar[0].top) / 2.f;

	lRect ProgressRect = pManager->props.ProgressBar[1];
	ProgressRect.right = ProgressRect.left + float(ProgressRect.right - ProgressRect.left)*InfoState.Amount;


	float ScalingX = pManager->scaleX * ( (InfoState.Checked) ? -1.f : 1.f );


	D3DXMATRIX transl,scal;
	D3DXMatrixTranslation(&transl , TransfPos.x  ,TransfPos.y  , 0);
	D3DXMatrixScaling(&scal , ScalingX , pManager->scaleY , 1.f);

	//if UI is locked, draw it transperant
	DWORD ColorFilter = (InfoState.Locked) ? 0x55FFFFFF : 0xffffffff;

	pManager->props.eng2d->DrawToScreen(pManager->pTexture , &(scal*transl) , (RECT*)&pManager->props.ProgressBar[0] , DBOX_CENTER_CUSTOM , &Center , ColorFilter);

	pManager->props.eng2d->DrawToScreen(pManager->pTexture , &(scal*transl) , (RECT*)&ProgressRect , DBOX_CENTER_CUSTOM , &Center , ColorFilter);

}

void DBOXUIProgressbar::Set(DBOXUIState)
{

}

DBOXUIObject* DBOXUICreateProgressbar(DBOXUIManager* mngr, lPoint Position , float Value)
{
	DBOXUIProgressbar* retval = new DBOXUIProgressbar;

	retval->Create(mngr , Position , Value);

	return retval;
}

///////////////////////////////////////////////////////////////////
//Slider
///////////////////////////////////////////////////////////////////


void DBOXUISlider::Create(DBOXUIManager* mngr, lPoint Position , float Value)
{
	this->pManager = mngr;
	this->InfoState.Active = false;
	this->InfoState.Amount = Value;
	this->position = Position;
	this->InfoState.Hidden = this->InfoState.Locked = false;
}

void DBOXUISlider::Update()
{
	
	if(this->InfoState.Hidden){this->InfoState.KeyState = 0x00;return;}

	lPoint TransformedPosition ; 

	TransformedPosition.x = float(position.x)*pManager->scaleX;
	TransformedPosition.y = float(position.y)*pManager->scaleY;

	float transfSliderWidth = float(pManager->transfSliderRect[0].right - pManager->transfSliderRect[0].left);
	float transfSliderHeight = float(pManager->transfSliderRect[0].bottom - pManager->transfSliderRect[0].top);
	float transfButtonWidth = (pManager->transfSliderRect[1].right - pManager->transfSliderRect[1].left);
	float slidingWidth = transfSliderWidth - transfButtonWidth ;

	lRect btnBoundingRect;

	btnBoundingRect.left = (float)(position.x)*pManager->scaleX  + slidingWidth*InfoState.Amount;
	btnBoundingRect.top = (float)(position.y)*pManager->scaleY;
	btnBoundingRect.right = (float)(position.x + pManager->props.SliderRect[1].right - pManager->props.SliderRect[1].left)*pManager->scaleX + slidingWidth*InfoState.Amount;
	btnBoundingRect.bottom = (float)(position.y + pManager->props.SliderRect[1].bottom - pManager->props.SliderRect[1].top)*pManager->scaleY;

	bool Collision = Point2Rect(pManager->MousePosition , btnBoundingRect);
	
	if(Collision && pManager->props.inp->IsKeyClicked(DIK_LMB) && ! this->InfoState.Locked)
	{
		this->InfoState.Active = true;
	}
	if(!pManager->LMBState)this->InfoState.Active = false;

	if(this->InfoState.Active)
	{
		float px = pManager->MousePosition.x;
		//float percent = px/(slidingWidth);
		//InfoState.Amount += percent;

		float minX = position.x*pManager->scaleX + transfButtonWidth/2.f;
		float maxX = position.x*pManager->scaleX + slidingWidth + transfButtonWidth/2.f;
		
		float xPos =  px ;

		if(xPos < minX)xPos = minX;
		if(xPos > maxX)xPos = maxX;

		InfoState.Amount = (xPos - minX)/slidingWidth;

	}

	if(InfoState.Amount < 0.f)InfoState.Amount = 0.f;
	if(InfoState.Amount > 1.f)InfoState.Amount = 1.f;

	D3DXMATRIX translBnt , translSlider ,scal;
	D3DXMatrixScaling(&scal , pManager->scaleX ,pManager->scaleY ,1.f);

	D3DXMatrixTranslation(&translSlider , position.x*pManager->scaleX , position.y*pManager->scaleY , 0.f);
	D3DXMatrixTranslation(&translBnt , position.x*pManager->scaleX + slidingWidth*InfoState.Amount + transfButtonWidth/2.f , position.y*pManager->scaleY + transfSliderHeight/2 , 0.f);

	DWORD ColorFilter = (InfoState.Locked) ? 0x55FFFFFF : 0xffffffff;


	pManager->props.eng2d->DrawToScreen(pManager->pTexture , &(scal*translSlider) ,(RECT*)&pManager->props.SliderRect[0] , DBOX_CENTER_LEFTTOP , 0 , ColorFilter);
	pManager->props.eng2d->DrawToScreen(pManager->pTexture , &(scal*translBnt) ,(RECT*)&pManager->props.SliderRect[1] , DBOX_CENTER_CENTER , 0 , ColorFilter);

}

void DBOXUISlider::Set(DBOXUIState)
{

}

DBOXUIObject* DBOXUICreateSlider(DBOXUIManager* mngr, lPoint Position , float Value)
{
	DBOXUISlider* retval = new DBOXUISlider;
	retval->Create(mngr ,Position ,Value);
	return retval;
}

////////////////////////////////////////////////////////////
//Label
////////////////////////////////////////////////////////////

void DBOXUILabel::Create(DBOXUIManager* mngr , lRect Position , std::string Value)
{
	this->pManager = mngr;
	this->position = lPoint(Position.left , Position.top);
	this->InfoState.strValue = Value;
	this->InfoState.Hidden = false;
	w = Position.right;
	h = Position.bottom;
}
 

void DBOXUILabel::Update()
{
	if(InfoState.Hidden)return;
	
	lRect rectT;
	rectT.left = (float)(this->position.x) * pManager->scaleX ;
	rectT.top = (float)(this->position.y)* pManager->scaleY;
	rectT.right = rectT.left + w*pManager->scaleX;
	rectT.bottom = rectT.top + h*pManager->scaleY;
	pManager->LabelFont->DrawTextEx(const_cast<char*>(this->InfoState.strValue.c_str()) , DT_VCENTER | DT_CENTER  , (RECT*)&rectT); 

}

void DBOXUILabel::Set(DBOXUIState)
{

}

DBOXUIObject* DBOXUICreateLabel(DBOXUIManager* mngr , lRect Position , std::string Value)
{
	DBOXUILabel* retval = new DBOXUILabel;
	retval->Create(mngr ,Position ,Value);
	return retval;
}