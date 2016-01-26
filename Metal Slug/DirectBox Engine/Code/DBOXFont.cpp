
#include "DBOXFont.h"


//Object creator
LPDBOXFONT DBOXCreateFont(STRTYPE FontFace ,LPDIRECT3DDEVICE9 d3ddev, int Width , int Height , int Weight , DWORD TextColor)
{
	LPDBOXFONT retval = new DBOXFont;
	HRESULT hr = retval->Create(FontFace , d3ddev , Width , Height , Weight , TextColor);
	if(FAILED(hr)) return NULL;
	
	return retval;
	
}

//Class definition

DBOXFont::DBOXFont(){Font = NULL;}
DBOXFont::~DBOXFont(){Font = NULL;}

LPD3DXFONT	DBOXFont::GetFont(){return Font;}
DWORD		DBOXFont::GetColor(){return Color;}

void		DBOXFont::SetFont(LPD3DXFONT pFont){Font = pFont;}
void		DBOXFont::SetColor(DWORD TextColor){Color = TextColor;}

HRESULT DBOXFont::Create(STRTYPE FontFace ,LPDIRECT3DDEVICE9 d3ddev, int Width , int Height , int Weight , DWORD TextColor)
{
	if(d3ddev == NULL)return DBOXFONTE_;
	Color = TextColor;
	Font = NULL;
	D3DXFONT_DESC FontDesc;
	
	ZeroMemory(&FontDesc , sizeof(D3DXFONT_DESC));

	//strcpy_s(FontDesc.FaceName , FontFace);//Set font facename
	for(int i = 0; i < sizeof(FontFace)/sizeof(FontFace[0]) ; i++)
		FontDesc.FaceName[i] = FontFace[i];

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

HRESULT DBOXFont::DrawText(const STRTYPE Text , int x , int y)
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
	
	UINT StrLenght = 0;
	for(int i = 0;;i++)
	{
		if(Text[i] != L'\0')StrLenght += 1;
		else break;
	}
	HRESULT hr = Font->DrawText(NULL , Text , (int)(StrLenght) , &Rect , DT_TOP|DT_LEFT , Color);
	if(FAILED(hr))return DBOXFONTE_;
	return DBOXFONT_OK;

}

HRESULT DBOXFont::DrawText(const STRTYPE Text , long TextLenght , int x , int y)
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

	HRESULT hr = Font->DrawText(NULL , Text , TextLenght, &Rect , DT_TOP|DT_LEFT , Color);
	if(FAILED(hr))return DBOXFONTE_;
	return DBOXFONT_OK;
}

HRESULT DBOXFont::DrawTextEx(const STRTYPE Text ,DWORD Format ,const LPRECT Rect)
{
	if(((int)(Rect->top) > (int)(Rect->bottom)) || ((int)(Rect->left) > (int)(Rect->right)))
	return DBOXFONTE_;

	UINT StrLenght = 0;
	for(int i = 0;;i++)
	{
		if(Text[i] != L'\0')StrLenght += 1;
		else break;
	}
	HRESULT hr = Font->DrawText(NULL , Text , (int)(StrLenght) , Rect , Format , Color);
	if(FAILED(hr))return DBOXFONTE_;
	
	return DBOXFONT_OK;
}