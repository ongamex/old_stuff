
#pragma once


// dxdebug.h

#include <stdio.h>

#pragma comment(lib, "D3d9.lib")
#pragma comment(lib, "D3dx9.lib")
#include <d3dx9.h>

#include "common.h"

//#define DBG_HALT __asm{ int 0x10 }
//#define DBG_ASSERT(exp) {if ( !(exp) ) {DBG_HALT;}}


/*
inline void DebugDrawBox2D(IDirect3DDevice9* pD3DDevice, float x0, float y0, float x1, float y1)
{
	struct TLVERTEX
	{
		float x,y,z,rhw;
		float tu,tv;
		enum { FVF_TLVERTEX = D3DFVF_XYZRHW | D3DFVF_TEX1 };
	};

	pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_SELECTARG1);
	pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_TEXTURE);
	float scale = 128.0f;
	TLVERTEX Vertex[4] = 
	{
		// x		y   z rhw tu tv
		{    0,    0,	0, 1, 0, 0,},
		{scale,    0,	0, 1, 1, 0,},
		{scale,scale,	0, 1, 1, 1,},
		{    0,scale,	0, 1, 0, 1,},
	};
	pD3DDevice->SetTexture( 0, NULL );
	pD3DDevice->SetVertexShader( TLVERTEX::FVF_TLVERTEX );
	pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( TLVERTEX ) );
}
*/

inline void DebugDrawLine3D(IDirect3DDevice9* pDevice, const D3DXVECTOR3& from, const D3DXVECTOR3& to, DWORD col=0xff00ffff)
{
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	pDevice->SetTransform( D3DTS_WORLD, &mat );

	struct VERTEX
	{
		D3DXVECTOR3 pos;
		unsigned int rgba;
		enum { FVF_VERTEX = D3DFVF_XYZ | D3DFVF_DIFFUSE };
	};

	pDevice->SetTextureStageState(0,D3DTSS_COLOROP,		D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_DIFFUSE);
	VERTEX Vertex[2] = 
	{
		// pos  colour
		{ from,	col},
		{ to,	col},

	};
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetTexture( 0, NULL );
	pDevice->SetFVF(VERTEX::FVF_VERTEX);
	pDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, Vertex, sizeof( VERTEX ) );
}


inline
void DrawDrawArrow3D(IDirect3DDevice9  * pDevice, 
					 const D3DXVECTOR3& from,
				     const D3DXVECTOR3& to,
				     unsigned int col/* = 0xffff00ff*/)
{
	// Draw Line From-To
	DebugDrawLine3D(pDevice, from, to, col);

	// Build the Arrow Head
	D3DXVECTOR3 N = from - to;

	D3DXVec3Normalize(&N, &N);


	D3DXVECTOR3 misc = N;
	misc.x += 20.0f;
	D3DXVec3Normalize(&misc, &misc);

	D3DXVECTOR3 perp;
	D3DXVec3Cross(&perp, &N, &misc);

	D3DXMATRIX matRot;
	D3DXMatrixRotationAxis(&matRot, &misc, 3.14f/5.0f);

	D3DXVECTOR3 N0, N1;
	D3DXVec3TransformCoord(&N0, &N, &matRot);

	D3DXMatrixRotationAxis(&matRot, &misc, -(3.14f/5.0f));
	D3DXVec3TransformCoord(&N1, &N, &matRot);


	DebugDrawLine3D(pDevice, to, to + N0*0.5f, col);
	DebugDrawLine3D(pDevice, to, to + N1*0.5f, col);
}// End DrawArrowLine(..)

inline void DebugDrawCross3D(IDirect3DDevice9* pDevice, D3DXVECTOR3& pos, float scale=1.0f, DWORD col=0xff00ffff)
{
	DebugDrawLine3D(pDevice, pos-D3DXVECTOR3(scale,0,0), pos+D3DXVECTOR3(scale,0,0), col);
	DebugDrawLine3D(pDevice, pos-D3DXVECTOR3(0,scale,0), pos+D3DXVECTOR3(0,scale,0), col);
	DebugDrawLine3D(pDevice, pos-D3DXVECTOR3(0,0,scale), pos+D3DXVECTOR3(0,0,scale), col);
}


inline void DebugWireDrawBox2D(IDirect3DDevice9* pD3DDevice, float x0, float y0, float w0, float h0, DWORD col=0xff00ffff)
{
	struct VERTEX
	{
		float x,y,z,rhw;
		unsigned int rgba;
		enum { FVF_VERTEX = D3DFVF_XYZRHW | D3DFVF_DIFFUSE };
	};

	pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_SELECTARG1);
	pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_DIFFUSE);
	VERTEX Vertex[5] = 
	{
		// x   y		z  rhw, colour
		{ x0,	y0,		0, 1,  col},
		{ x0+w0,y0,		0, 1,  col},
		{ x0+w0,y0+h0,	0, 1,  col},
		{ x0,	y0+h0,	0, 1,  col},
		{ x0,	y0,		0, 1,  col},
	};
	pD3DDevice->SetTexture( 0, NULL );
	//pD3DDevice->SetVertexShader( VERTEX::FVF_VERTEX );
	pD3DDevice->SetFVF(VERTEX::FVF_VERTEX);
	pD3DDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, 4, Vertex, sizeof( VERTEX ) );
}

inline void DebugDrawTexList2D(IDirect3DDevice9* pD3DDevice, 
							   IDirect3DTexture9* pTex,  
							   D3DXVECTOR2* pPoints, 
							   D3DXVECTOR2* pTexPoints, 
							   int NumPoints, 
							   int depth,
							   float mulr, float mulg, float mulb, float mula,
							   int   addr, int   addg, int   addb, int   adda)
{
	#pragma pack(1)
	struct TVERTEX
	{
		float x,y,z,rhw;
		DWORD colmul;
		DWORD coladd;
		float tu,tv;
		enum { FVF_TVERTEX = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1  };
	};
	#pragma pack()


	// stage 1 coloring
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP,  D3DTOP_ADD );
	pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG1,D3DTA_SPECULAR );
	pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG2,D3DTA_CURRENT );


	// 0
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,  D3DTOP_MODULATE );
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1,D3DTA_TEXTURE );
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2,D3DTA_DIFFUSE );

	// 1
	pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,  D3DTOP_ADD);
	pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1,D3DTA_CURRENT );
	pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2,D3DTA_SPECULAR);

	pD3DDevice->SetTextureStageState(2, D3DTSS_ALPHAOP,  D3DTOP_DISABLE );

	//             0xaarrggbb
	DWORD colmul = 0x00000000;
	DWORD coladd = 0xffff0000;
	

	float scale = 255;
	if (pTex)
		colmul = D3DCOLOR_ARGB((int)(mula*scale),(int)(mulr*scale),(int)(mulg*scale),(int)(mulb*scale));

	coladd = D3DCOLOR_ARGB(adda,addr,addg,addb);

	TVERTEX* Vertex = new TVERTEX[NumPoints + 5]; // +5 is safety buffer

	for (int i=0; i<NumPoints; i++)
	{
		Vertex[i].x=pPoints[i].x;
		Vertex[i].y=pPoints[i].y;
		Vertex[i].z=0.85f;  ////// temp ...to-do
		Vertex[i].rhw=1;
		Vertex[i].tu=pTexPoints[i].x;
		Vertex[i].tv=pTexPoints[i].y;

		Vertex[i].colmul = colmul;
		Vertex[i].coladd = coladd;

	}

	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	pD3DDevice->SetTexture( 0, pTex );
	//pD3DDevice->SetVertexShader( TVERTEX::FVF_TVERTEX );
	pD3DDevice->SetFVF( TVERTEX::FVF_TVERTEX );
	pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, NumPoints-2, Vertex, sizeof( TVERTEX ) );
	
	delete[] Vertex;

	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,  D3DTOP_DISABLE );
	pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,  D3DTOP_DISABLE );
	pD3DDevice->SetTextureStageState(2, D3DTSS_ALPHAOP,  D3DTOP_DISABLE );
	pD3DDevice->SetTexture(0, NULL);
}


inline void DebugDrawTexList2D_2(IDirect3DDevice9* pD3DDevice, 
							   IDirect3DTexture9* pTex,  
							   D3DXVECTOR2* pPoints, 
							   D3DXVECTOR2* pTexPoints, 
							   int NumPoints, 
							   int depth,
							   float mulr, float mulg, float mulb, float mula,
							   int   addr, int   addg, int   addb, int   adda)
{
	#pragma pack(1)
	struct TVERTEX
	{
		float x,y,z,rhw;
		DWORD colmul;
	//	DWORD coladd;
		float tu,tv;
		enum { FVF_TVERTEX = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1  };
	};
	#pragma pack()


	if (pTex)
	{
		pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

		pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
		pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	}
	else
	{
		pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
		pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );

		pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
		pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
	}


	//             0xaarrggbb
	DWORD colmul = 0x00000000;
	//DWORD colmul = 0x00000000;
	//DWORD coladd = 0xffff0000;
	

	//float scale = 255;
	//if (pTex)
	//colmul = D3DCOLOR_ARGB((int)(mula*scale),(int)(mulr*scale),(int)(mulg*scale),(int)(mulb*scale));

	//coladd = D3DCOLOR_ARGB(adda,addr,addg,addb);


	DWORD col = D3DCOLOR_ARGB((int)(adda*mula), (int)(addr*mulr), (int)(addg*mulg), (int)(addb*mulb));


	TVERTEX* Vertex = new TVERTEX[NumPoints + 5]; // +5 is safety buffer

	for (int i=0; i<NumPoints; i++)
	{
		Vertex[i].x=pPoints[i].x;
		Vertex[i].y=pPoints[i].y;
		Vertex[i].z=0.85f;  ////// temp ...to-do
		Vertex[i].rhw=1;
		Vertex[i].tu=pTexPoints[i].x;
		Vertex[i].tv=pTexPoints[i].y;

		Vertex[i].colmul = col;
	//	Vertex[i].coladd = coladd;

	}

	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	pD3DDevice->SetTexture( 0, pTex );
	//pD3DDevice->SetVertexShader( TVERTEX::FVF_TVERTEX );
	pD3DDevice->SetFVF( TVERTEX::FVF_TVERTEX );
	pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, NumPoints-2, Vertex, sizeof( TVERTEX ) );
	
	delete[] Vertex;

	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,  D3DTOP_DISABLE );
	pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,  D3DTOP_DISABLE );
	pD3DDevice->SetTextureStageState(2, D3DTSS_ALPHAOP,  D3DTOP_DISABLE );
	pD3DDevice->SetTexture(0, NULL);
}


inline void DebugDrawLineList2D(IDirect3DDevice9* pD3DDevice, D3DXVECTOR2* pPoints, int NumPoints)
{
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	pD3DDevice->SetTextureStageState(2, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	pD3DDevice->SetTexture(0, NULL);

	pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);

	struct VERTEX
	{
		float x,y,z,rhw;
		unsigned int rgba;
		enum { FVF_VERTEX = D3DFVF_XYZRHW | D3DFVF_DIFFUSE };
	};

	pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_SELECTARG1);
	pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_DIFFUSE);

	static float rhw = 1.0f;
	VERTEX* Vertex = new VERTEX[NumPoints];
	for (int i=0; i<NumPoints; i++)
	{
		Vertex[i].x=pPoints[i].x;
		Vertex[i].y=pPoints[i].y;
		Vertex[i].z=0;
		Vertex[i].rhw=1.0f;
		Vertex[i].rgba=0xff00ffff;
	}

	pD3DDevice->SetTexture( 0, NULL );
	//pD3DDevice->SetVertexShader( VERTEX::FVF_VERTEX );
	pD3DDevice->SetFVF( VERTEX::FVF_VERTEX );
	pD3DDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, NumPoints-1, Vertex, sizeof( VERTEX ) );
	
	delete[] Vertex;
}

static 
void DrawCircleXZ(IDirect3DDevice9* pDevice, const D3DXVECTOR3& pos, const float radius, const unsigned int colour=0xffff0000, const int res=15)
{
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	pDevice->SetTransform( D3DTS_WORLD, &mat );

	struct VERTEX
	{
		D3DXVECTOR3 pos;
		unsigned int rgba;
		enum { FVF_VERTEX = D3DFVF_XYZ | D3DFVF_DIFFUSE };
	};

	pDevice->SetTextureStageState(0,D3DTSS_COLOROP,		D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_DIFFUSE);

	VERTEX* Vertex = new VERTEX[res*3];

	const float PI2 = (2.0f*3.14f);
	float step = PI2/(float)res;
	for (int i=0; i<res+1; i++)
	{
		float f0 = i*step;
		Vertex[i].pos  = pos + radius*D3DXVECTOR3(sinf(f0), 0, cosf(f0));
		Vertex[i].rgba = colour;
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetTexture( 0, NULL );
	pDevice->SetFVF(VERTEX::FVF_VERTEX);
	pDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, res, Vertex, sizeof( VERTEX ) );

	delete[] Vertex;

}//End DrawCirlceXZ(..)

static void DrawCube(IDirect3DDevice9* pDevice, const D3DXVECTOR3& pos, const D3DXVECTOR3& dim)
{

  	struct stVERTEX
	{
		float x,y,z;
		unsigned int colour;
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
	};

	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_DIFFUSE);
	float w = dim.x/2.0f;
	float h = dim.y/2.0f;
	float d = dim.z/2.0f;
	DWORD col = 0xffff00ff;
	float x = pos.x;
	float y = pos.y;
	float z = pos.z;

	stVERTEX Vertex[36] = 
	{
		//   x     y      z       colour
		// Front
		{   x-w,   y-h,	z-d,    0xffff0000},
		{   x+w,   y+h,	z-d,    0xffff0000},
		{   x-w,   y+h,	z-d,    0xffff0000},

		{   x-w,   y-h,	z-d,    0xffff0000},
		{   x+w,   y-h,	z-d,    0xffff0000},
		{   x+w,   y+h,	z-d,    0xffff0000},

		// Back
		{   x-w,   y-h,	z+d,    0xff00ff00},
		{   x-w,   y+h,	z+d,    0xff00ff00},
		{   x+w,   y+h,	z+d,    0xff00ff00},

		{   x-w,   y-h,	z+d,    0xff00ff00},
		{   x+w,   y+h,	z+d,    0xff00ff00},
		{   x+w,   y-h,	z+d,    0xff00ff00},

		// Top
		{   x-w,   y+h,	z-d,    0xff0000ff},
		{   x+w,   y+h,	z+d,    0xff0000ff},
		{   x-w,   y+h,	z+d,    0xff0000ff},
					  
		{   x-w,   y+h,	z-d,    0xff0000ff},
		{   x+w,   y+h,	z-d,    0xff0000ff},
		{   x+w,   y+h,	z+d,    0xff0000ff},

		// Bottom
		{   x-w,   y-h,	z-d,    0xffffff0f},
		{   x-w,   y-h,	z+d,    0xffffff0f},
		{   x+w,   y-h,	z+d,    0xffffff0f},
					  
		{   x-w,   y-h,	z-d,    0xffffff00},
		{   x+w,   y-h,	z+d,    0xffffff00},
		{   x+w,   y-h,	z-d,    0xffffff00},

		// Left
		{   x-w,   y-h,	z-d,    0xffffffff},
		{   x-w,   y+h,	z-d,    0xffffffff},
		{   x-w,   y+h,	z+d,    0xffffffff},
				  
		{   x-w,   y-h,	z-d,    0xffffffff},
		{   x-w,   y+h,	z+d,    0xffffffff},
		{   x-w,   y-h,	z+d,    0xffffffff},

		// Right
		{   x+w,   y-h,	z-d,    0xff000000},
		{   x+w,   y+h,	z+d,    0xff000000},
		{   x+w,   y+h,	z-d,    0xff000000},
			  
		{   x+w,   y-h,	z-d,    0xff000000},
		{   x+w,   y-h,	z+d,    0xff000000},
		{   x+w,   y+h,	z+d,    0xff000000},
	};
	pDevice->SetTexture( 0, NULL);

		
	#if(DIRECT3D_VERSION >= 0x0900)
		pDevice->SetFVF( stVERTEX::FVF );
	#else
		pDevice->SetVertexShader( stVERTEX::FVF );
	#endif // DIRECT3D_VERSION


	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	pDevice->SetTransform(D3DTS_WORLD, &mat);

	pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

	HRESULT hr = 
	pDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 12, Vertex, sizeof( stVERTEX ) );

}//End DrawCube(..)

static 
void RenderText(	IDirect3DDevice9 * pDevice,
				char * str, 
				int xpos /*= 10*/, 
				int ypos /*= 10*/, 
				unsigned int rgb /*= 0xffffff*/,
				int size /*=15*/)
{
	static ID3DXFont* pFont;

	#if(DIRECT3D_VERSION >= 0x0900)
	D3DXCreateFont(pDevice,  // LPDIRECT3DDEVICE9
				size,        // Width
				0,           // Height
				FW_NORMAL,   // Weight
				1,           // MipLevels
				false,       // Italic
				DEFAULT_CHARSET,     // CharSet
				OUT_DEFAULT_PRECIS,  // OutputPrecision
				ANTIALIASED_QUALITY, // Quality
				FIXED_PITCH | FF_DONTCARE, // PitchAndFamily
				"Arial",     // pFacename
				&pFont);     // LPD3DXFONT
	

	/*
	static LOGFONT lfont={
				15,						// height
				0,						// width; 
				0,						// lfEscapement; 
				0,						// lfOrientation; 
				FW_BOLD,				// lfWeight; 
				FALSE,					// lfItalic; 
				FALSE,					// lfUnderline; 
				FALSE,					// lfStrikeOut; 
				DEFAULT_CHARSET,		// lfCharSet; 
				OUT_DEFAULT_PRECIS,		// lfOutPrecision; 
				CLIP_DEFAULT_PRECIS,	// lfClipPrecision; 
				ANTIALIASED_QUALITY,	// lfQuality; 
				FIXED_PITCH,			// lfPitchAndFamily; 
				"Arial"					// lfFaceName[LF_FACESIZE]; 
				};

	lfont.lfHeight = 15;

	//This is all there is to creating a D3DXFont.  Hmmmm....
	D3DXCreateFontIndirect( pDevice ,&lfont,&pFont );
	*/
	#else
	static LOGFONT lfont={
			size, //height
			0,  //width; 
			0,  // lfEscapement; 
			0,  //lfOrientation; 
			FW_BOLD, // lfWeight; 
			FALSE, // lfItalic; 
			FALSE, // lfUnderline; 
			FALSE, // lfStrikeOut; 
			DEFAULT_CHARSET, // lfCharSet; 
			OUT_DEFAULT_PRECIS, //lfOutPrecision; 
			CLIP_DEFAULT_PRECIS, // lfClipPrecision; 
			ANTIALIASED_QUALITY,// lfQuality; 
			FIXED_PITCH,// lfPitchAndFamily; 
			"Arial"// lfFaceName[LF_FACESIZE]; 
			};

	lfont.lfHeight = size;

   //This is all there is to creating a D3DXFont.  Hmmmm....
   D3DXCreateFontIndirect( pDevice ,&lfont,&pFont );

	#endif // DIRECT3D_VERSION



   static char szbuff[900];
   sprintf(szbuff, "%s", str );
   RECT rect = {xpos,ypos, ((int)xpos)+800,ypos+800};


	#if(DIRECT3D_VERSION >= 0x0900)
   pFont->DrawTextA(NULL,
						szbuff,
						-1,
						&rect,
						DT_LEFT,
						0xFFBBBBFF);
	#else
   pFont->DrawTextA(szbuff,
						-1,
						&rect,
						DT_LEFT,
						0xFFBBBBFF);
	#endif // DIRECT3D_VERSION


	pFont->Release();
}