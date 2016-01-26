
#include "DBOXFont.h"


//Object creator
LPDBOXFONT DBOXCreateFont(tstring FontFace ,LPDIRECT3DDEVICE9 d3ddev, int Width , int Height , int Weight , DWORD TextColor)
{
	LPDBOXFONT retval = new DBOXFont;
	HRESULT hr = retval->Create(FontFace , d3ddev , Width , Height , Weight , TextColor);
	if(FAILED(hr)) return NULL;
	
	return retval;
	
}

//Class definition

DBOXFont::DBOXFont(){Font = NULL;}
DBOXFont::~DBOXFont(){if(Font)Font->Release();Font = NULL;}

LPD3DXFONT	DBOXFont::GetFont(){return this->Font;}
DWORD		DBOXFont::GetColor(){return Color;}

void		DBOXFont::SetFont(LPD3DXFONT pFont){Font = pFont;}
void		DBOXFont::SetColor(DWORD TextColor){Color = TextColor;}

HRESULT DBOXFont::Create(tstring FontFace ,LPDIRECT3DDEVICE9 d3ddev, int Width , int Height , int Weight , DWORD TextColor)
{
	if(d3ddev == NULL)return DBOXFONTE_;
	Color = TextColor;
	Font = NULL;
	D3DXFONT_DESC FontDesc;
	
	ZeroMemory(&FontDesc , sizeof(D3DXFONT_DESC));
	FontDesc.FaceName;

	memcpy(FontDesc.FaceName, FontFace.c_str(), FontFace.size());

	FontDesc.Width = Width;
	FontDesc.Height = Height;
	FontDesc.CharSet = DEFAULT_CHARSET;
	FontDesc.Weight = Weight;

	if(D3DXCreateFontIndirect(d3ddev , &FontDesc , &Font) == D3D_OK)	
	return  DBOXFONT_OK;

	return DBOXFONTE_;
}

void DBOXFont::Release()
{
	if(Font != NULL)Font->Release();
	Font = NULL;
	return;
}

HRESULT DBOXFont::DrawText(tstring Text , int x , int y)
{
 	UINT StrLenght = strlen(Text.c_str());
	return DrawText(Text, StrLenght, x, y);
}

HRESULT DBOXFont::DrawText(tstring Text , long TextLenght , int x , int y)
{	
	if(Font == NULL) return DBOXFONTE_;

	LPDIRECT3DDEVICE9 device;
	D3DVIEWPORT9		Vp;
	RECT Rect;

	Font->GetDevice(&device);
	device->GetViewport(&Vp);

	int MaxX = Vp.Width;
	int MaxY = Vp.Height;

	if((x > (int)(MaxX)) || (y > (int)(MaxY))) return DBOXFONTE_;

	Rect.left = x;
	Rect.right = MaxX;

	Rect.top = y;
	Rect.bottom = MaxY;

	HRESULT hr = Font->DrawText(NULL , Text.c_str() , TextLenght, &Rect , DT_TOP|DT_LEFT , Color);
	if(FAILED(hr))return DBOXFONTE_;
	return DBOXFONT_OK;
}

HRESULT DBOXFont::DrawTextEx(tstring Text ,DWORD Format ,const LPRECT Rect)
{
	if(((int)(Rect->top) > (int)(Rect->bottom)) || ((int)(Rect->left) > (int)(Rect->right)))
	return DBOXFONTE_;

	UINT StrLenght = strlen(Text.c_str());

	HRESULT hr = Font->DrawText(NULL , Text.c_str() , (int)(StrLenght) , Rect , Format , Color);
	if(FAILED(hr))return DBOXFONTE_;
	
	return DBOXFONT_OK;
}