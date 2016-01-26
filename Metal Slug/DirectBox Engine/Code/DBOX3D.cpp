#pragma once
#include "DBOX3D.h"

//Create Interface function

LPDBOX3D DBOXCreate3D()
{
	LPDBOX3D retval = new DBOX3D;
	retval->CreateInterface();
	return retval;
}

D3DPRESENT_PARAMETERS DBOXSetPresentParameters(HWND FocusWnd , UINT BackBufferWidth , UINT BackBufferHeight , D3DFORMAT BackBufferFormat , BOOL Windowed)
{
	D3DPRESENT_PARAMETERS retval;
	ZeroMemory(&retval , sizeof(D3DPRESENT_PARAMETERS));
	
	retval.hDeviceWindow = FocusWnd;
	retval.Windowed = Windowed;
	if(1)
	{
		retval.BackBufferFormat = BackBufferFormat;
		retval.BackBufferWidth = BackBufferWidth;
		retval.BackBufferHeight = BackBufferHeight;
		retval.BackBufferCount = 1;
	}
	retval.FullScreen_RefreshRateInHz = 0;
	retval.SwapEffect = D3DSWAPEFFECT_DISCARD;
	retval.EnableAutoDepthStencil = TRUE;
	retval.AutoDepthStencilFormat = D3DFMT_D16;

	return retval;
}

//DBOX3D

void DBOX3D::CreateInterface()
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	return;
}

HRESULT DBOX3D::CreateDevice(D3DPRESENT_PARAMETERS pp)
{
	d3dpp = pp;
	
	//GetDeviceCaps , sets HW or SW processing
	D3DCAPS9 DevCaps;
	if(FAILED(d3d->GetDeviceCaps(D3DADAPTER_DEFAULT , D3DDEVTYPE_HAL , &DevCaps)))return DBOX3DE_;
	
	DWORD VertexProcessingAndLight = (DevCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) ? D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	if(FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT , D3DDEVTYPE_HAL , pp.hDeviceWindow , VertexProcessingAndLight | D3DCREATE_PUREDEVICE , &pp , &d3ddev)))return DBOX3DE_DEVICE;
	
	if(FAILED(d3ddev->SetRenderState(D3DRS_ZENABLE , TRUE))) return DBOX3DE_ ;
	if(FAILED(d3ddev->SetRenderState(D3DRS_LIGHTING , FALSE))) return DBOX3DE_ ;
	
	if(FAILED(D3DXCreateSprite(d3ddev , &Sprite)))  return DBOX3DE_ ;

	return DBOX3D_OK;

}

void DBOX3D::Release()
{
	if(d3d){d3d->Release();d3d = NULL;}
	if(d3ddev){d3ddev->Release();d3ddev = NULL;}
	if(Sprite){Sprite->Release();Sprite = NULL;}

	return;
}
HRESULT DBOX3D::BeginScene(DWORD Color)
{
	if(FAILED(d3ddev->Clear(0 , NULL , D3DCLEAR_TARGET , Color , 1.0f , 1))) return DBOX3DE_;
	if(FAILED(d3ddev->Clear(0 , NULL , D3DCLEAR_ZBUFFER , Color , 1.0f , 1))) return DBOX3DE_;
	if(FAILED(d3ddev->BeginScene())) return DBOX3DE_;
	return DBOX3D_OK;
}

HRESULT DBOX3D::EndScene()
{
	if(FAILED(d3ddev->EndScene())) return DBOX3DE_;
	if(FAILED(d3ddev->Present(NULL , NULL , NULL , NULL))) return DBOX3DE_;
	return DBOX3D_OK;
}

HRESULT	DBOX3D::HandleLostDevice(DWORD SleepTime = 0)
{
	HRESULT hr = d3ddev->TestCooperativeLevel();
	if(hr == D3DERR_DEVICELOST)
		{
			Sleep(SleepTime);
			return DBOX3D_DEVSTATE_LOST;
		}
	if(hr == D3DERR_DEVICENOTRESET)
	{
		//reset device
		d3ddev->Reset(&d3dpp);
		return DBOX3D_DEVSTATE_RESETED;
	}

	return DBOX3D_DEVSTATE_GOOD ;

}

LPDIRECT3D9 DBOX3D::GetD3D()
{
	return d3d;
}

LPDIRECT3DDEVICE9 DBOX3D::GetDevice()
{
	return d3ddev;
}

D3DPRESENT_PARAMETERS DBOX3D::GetPresentParameters()
{
	return d3dpp;
}


HRESULT	DBOX3D::SetRenderState(D3DRENDERSTATETYPE State,DWORD Value)
{
	HRESULT hr = d3ddev->SetRenderState(State,Value);
	if(FAILED(hr))return DBOX3DE_;
	return DBOX3D_OK;
}

void DBOX3D::SetPresentParameters(D3DPRESENT_PARAMETERS pp)
{
	d3dpp = pp;
	return;
}

LPD3DXFONT DBOX3D::CreateSimpleFont(STRTYPE FontFace , UINT szWidth , UINT szHeight)
{
	LPD3DXFONT Font = NULL;
	D3DXFONT_DESC FontDesc;
	
	ZeroMemory(&FontDesc , sizeof(D3DXFONT_DESC));

	//strcpy_s(FontDesc.FaceName , FontFace);//Set font facename
	for(int i = 0; i < sizeof(FontFace)/sizeof(FontFace[0]) ; i++)
		FontDesc.FaceName[i] = FontFace[i];

	FontDesc.Width = szWidth;
	FontDesc.Height = szHeight;
	FontDesc.CharSet = DEFAULT_CHARSET;
	FontDesc.Weight = 800;

	if(D3DXCreateFontIndirect(d3ddev , &FontDesc , &Font) == D3D_OK)	
	return Font;
	return NULL;
}

bool DBOX3D::LimitFrames(DWORD Frames)
{
	static DWORD LastFrameTime = 0;
	DWORD CurrentTime = timeGetTime();

	if ((CurrentTime - LastFrameTime) > (1000 / Frames)) 
	{
		LastFrameTime = CurrentTime;
		return 0;
	}
	return 1;
}

/************************************************************************************************************************************************************
Sprite Methods:
************************************************************************************************************************************************************/

LPD3DXSPRITE DBOX3D::GetSprite()
{
	return Sprite;
}

HRESULT  DBOX3D::SpriteBegin(DWORD Flags)
{
	if(FAILED(Sprite->Begin(Flags))) return DBOX3DE_ ;
	return DBOX3D_OK;
}

HRESULT DBOX3D::SpriteEnd()
{
	if(FAILED(Sprite->End())) return DBOX3DE_ ; 
	return DBOX3DE_ ;
}
/************************************************************************************************************************************************************
Other Methods:
************************************************************************************************************************************************************/
LPDIRECT3DTEXTURE9 DBOX3D::CreateTextureFromFile(STRTYPE FileName , LONG Width , LONG Height , DWORD ColorKey)
{
	LPDIRECT3DTEXTURE9 retval;
	HRESULT hr = D3DXCreateTextureFromFileEx(d3ddev , FileName , Width , Height , 0 , 0 , D3DFMT_A8R8G8B8 , D3DPOOL_MANAGED , D3DX_DEFAULT , D3DX_DEFAULT , ColorKey , NULL , NULL , &retval);
	if(FAILED(hr)) return NULL;
	return retval;
}

/************************************************************************************************************************************************************
Afterclass
************************************************************************************************************************************************************/

RECT DBOXRect(LONG Left ,LONG Top ,LONG Right ,LONG Bottom)
{
	RECT retval = {Left , Top , Right , Bottom};
	return retval;
}