#pragma once

#include "DBOXMath.h"
#include "DBOX2D.h"
#include "DBOXInput.h"
#include "DBOXFont.h"

#include <string>
#include <map>
#include <vector>

#define IsKeyDownVK(vk_code) ((GetAsyncKeyState((int)(vk_code)) & 0x8000) ? 1 : 0)
#define IsKeyUpVK(vk_code) ((GetAsyncKeyState((int)(vk_code)) & 0x8000) ? 0 : 1)


//Collistion test and area classes



//UI ObjectState

struct DBOXUIState{
	DWORD KeyState ;
	float Amount ;//0 - 1
	bool Checked ;
	tstring strValue;//UNICODE adn ASCII

	bool Hidden , Locked;
	bool Active ;
	bool PasswordMode;
	int CharacterLimit;
};

struct DBOXUIObject;

struct DBOXUIPROPERTIES {
	HWND hWnd;
	LPDIRECT3DDEVICE9 pDevice;
	LPDBOX2D eng2d;
	LPDBOXINPUT inp;
	long BaseWidth , BaseHeight;
	tstring TextureName;

	lRect BtnRects[3];
	lRect CheckboxRects[2];
	lRect TextboxRect;
	
	lRect ProgressBar[2];

	lRect SliderRect[2];

	tstring FontFace;//ASCII !!!
	DWORD FontColor;
	DWORD TextureKeyColor;
};

struct UIPage{
	std::map<std::string , DBOXUIObject*> Page;
};

struct DBOXUIManager{
	
public :
	DBOXUIPROPERTIES props;

	std::map<std::string , std::map<std::string , DBOXUIObject*>> Pages;

	LPDBOXFONT ButtonFont;
	LPDBOXFONT TextboxFont;
	LPDBOXFONT CheckboxFont;
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
	void Create(DBOXUIManager* mngr , tstring Text , lPoint Position);
	void Update();
	void Set(DBOXUIState);
};

DBOXUIObject* DBOXUICreateButton(DBOXUIManager* mngr , tstring Text , lPoint Position);

///////////////////////////////////////////////////////////
//Textbox
///////////////////////////////////////////////////////////

struct DBOXUITextbox : public DBOXUIObject {
private : 
	DWORD backspaceTimer;

public : 
	void Create(DBOXUIManager* mngr , int limit , lPoint Position , tstring Text);
	void Update();
	void Set(DBOXUIState);
};

DBOXUIObject* DBOXUICreateTextbox(DBOXUIManager* mngr , int limit , lPoint Position , tstring Text = tstring(__TEXT("")));

///////////////////////////////////////////////////////////
//Checkbox
///////////////////////////////////////////////////////////

struct DBOXUICheckbox : public DBOXUIObject { 
private :
	bool prevState , currState;
public :
	void Create(DBOXUIManager* mngr, lPoint Position, tstring Text);
	void Update();
	void Set(DBOXUIState);
};

DBOXUIObject* DBOXUICreateCheckbox(DBOXUIManager* mngr, lPoint Position, tstring Text);

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