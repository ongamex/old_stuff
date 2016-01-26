#pragma once

#include "DBOX2D.h"
#include "DBOXInput.h"
#include "DBOXFont.h"
#include <string>

#include <map>
#include <vector>

#define IsKeyDownVK(vk_code) ((GetAsyncKeyState((int)(vk_code)) & 0x8000) ? 1 : 0)
#define IsKeyUpVK(vk_code) ((GetAsyncKeyState((int)(vk_code)) & 0x8000) ? 0 : 1)


//Collistion test and area classes
struct fRect {
	float left,top , right,bottom;

	fRect() : left(0),top(0) , right(0),bottom(0) {}
	fRect(float Left,float Top,float Right,float Bottom) : left(Left), top(Top), right(Right), bottom(Bottom) {}
};

struct lRect {
	long left,top , right,bottom;

	lRect() : left(0),top(0) , right(0),bottom(0) {}
	lRect(long Left,long Top,long Right,long Bottom) : left(Left), top(Top), right(Right), bottom(Bottom) {}
};

struct fPoint{
	float x,y;

	fPoint() : x(0) , y(0) {}
	fPoint(float X , float Y) : x(X) , y(Y) {}
};

struct lPoint{
	long x,y;

	lPoint() : x(0) , y(0) {}
	lPoint(long X , long Y) : x(X) , y(Y) {}
};


//UI ObjectState

struct DBOXUIState{
	DWORD KeyState ;
	float Amount ;//0 - 1
	bool Checked ;
	std::string strValue;

	bool Hidden , Locked;
	bool Active ;
	bool PasswordMode;
	int CharacterLimit;
};


//////////////////////////////////////////////////////////////////////////
//beneath class decl
struct DBOXUIObject;

struct DBOXUIPROPERTIES {
	HWND hWnd;
	LPDIRECT3DDEVICE9 pDevice;
	LPDBOX2D eng2d;
	LPDBOXINPUT inp;
	long BaseWidth , BaseHeight;
	std::string TextureName;

	lRect BtnRects[3];
	lRect CheckboxRects[2];
	lRect TextboxRect;
	
	lRect ProgressBar[2];

	lRect SliderRect[2];

	std::string FontFace;
	DWORD FontColor;
	DWORD TextureKeyColor;

	int LabelWordSize;

	
};

struct UIPage{
	std::map<std::string , DBOXUIObject*> Page;
};

struct DBOXUIManager{
	
public :

	std::string ActivePage;

	DBOXUIPROPERTIES props;

	std::map<std::string , std::map<std::string , DBOXUIObject*>> Pages;

	LPDBOXFONT ButtonFont;
	LPDBOXFONT TextboxFont;
	LPDBOXFONT CheckboxFont;
	LPDBOXFONT LabelFont;
	LPDIRECT3DTEXTURE9 pTexture;

	//

	lRect transfBntRects[3];
	lRect transfCheckboxRects[2];
	lRect transfSliderRect[2];
	lRect transfProgressBar[2];

	lRect transfTextboxRect;
	//
	float scaleX, scaleY;
	lPoint MousePosition;

	bool LMBState , RMBState; 
	bool IsWindowActive;

	void UpdateContent();
	
	bool Create(DBOXUIPROPERTIES properties);

	void Update();
	void ChangeActivePage(std::string);
	
};

///////////////////////////////////////////////////////////
//Base Object
///////////////////////////////////////////////////////////

struct DBOXUIObject { 

	DBOXUIManager* pManager;
	DBOXUIState InfoState;
	lPoint position;

	virtual void Update() = 0;

	DBOXUIState GetState() {return this->InfoState;}
	virtual void Set(DBOXUIState) = 0;
};

///////////////////////////////////////////////////////////
//Buttons
///////////////////////////////////////////////////////////

struct DBOXUIButton : public DBOXUIObject {

private : 
	int prevState , currState;
	
public : 
	void Create(DBOXUIManager* mngr , std::string Text , lPoint Position);
	void Update();
	void Set(DBOXUIState);
};

DBOXUIObject* DBOXUICreateButton(DBOXUIManager* mngr , std::string Text , lPoint Position);

///////////////////////////////////////////////////////////
//Textbox
///////////////////////////////////////////////////////////

struct DBOXUITextbox : public DBOXUIObject {
private : 
	DWORD backspaceTimer;

public : 
	void Create(DBOXUIManager* mngr , int limit , lPoint Position , std::string Text);
	void Update();
	void Set(DBOXUIState);
};

DBOXUIObject* DBOXUICreateTextbox(DBOXUIManager* mngr , int limit , lPoint Position , std::string Text = std::string(""));

///////////////////////////////////////////////////////////
//Checkbox
///////////////////////////////////////////////////////////

struct DBOXUICheckbox : public DBOXUIObject { 
private :
	bool prevState , currState;
public :
	void Create(DBOXUIManager* mngr, lPoint Position, std::string Text);
	void Update();
	void Set(DBOXUIState);
};

DBOXUIObject* DBOXUICreateCheckbox(DBOXUIManager* mngr, lPoint Position, std::string Text);

//////////////////////////////////////////////////////////
//Progressbar
//////////////////////////////////////////////////////////

struct DBOXUIProgressbar : public DBOXUIObject {
private :

public : 
	void Create(DBOXUIManager* mngr , lPoint Position , float Value);
	void Update();
	void Set(DBOXUIState);

};

DBOXUIObject* DBOXUICreateProgressbar(DBOXUIManager* mngr, lPoint Position , float Value = 1.f);

/////////////////////////////////////////////////////////////
//Slider
/////////////////////////////////////////////////////////////

struct DBOXUISlider : public DBOXUIObject {
private :
public : 
	void Create(DBOXUIManager* mngr, lPoint Position , float Value);
	void Update();
	void Set(DBOXUIState);

};


DBOXUIObject* DBOXUICreateSlider(DBOXUIManager* mngr, lPoint Position , float Value = 0.5f);

/////////////////////////////////////////////////////////////
//Label
/////////////////////////////////////////////////////////////

struct DBOXUILabel : public DBOXUIObject {
private:
	int w,h;
public :
	void Create(DBOXUIManager* mngr , lRect Position , std::string Value);
	void Update();
	void Set(DBOXUIState);
};

DBOXUIObject* DBOXUICreateLabel(DBOXUIManager* mngr , lRect Position , std::string Value);