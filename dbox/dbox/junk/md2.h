// md2.h

// bkenwright@xdev.net

// simple md2 loader / render


#pragma once


#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Winmm.lib")

#include <windows.h>
#include <stdio.h>

#define ULONG_PTR unsigned long

#pragma comment(lib, "D3d9.lib") //directX 9
#pragma comment(lib, "D3dx9.lib")
#include <d3dx9.h>

struct stMd2AnimationData{
int Animation;
int CurrFrame,NextFrame;
float LastTime;

};


struct stMd2Header
{
	int magic;			// The magic number used to identify the file.
	int version;		// The file version number (must be 8).
	int skinWidth;		// The width in pixels of our image.
	int skinHeight;		// The height in pixels of our image.
	int frameSize;		// The size in bytes the frames are.
	int numSkins;		// The number of skins associated with the model.
	int numVertices;	// The number of vertices.
	int numTexCoords;	// The number of texture coordinates.
	int numTriangles;	// The number of faces (polygons).
	int numGlCommands;	// The number of gl commands.
	int numFrames;		// The number of animated frames.
	int offsetSkins;	// The offset in the file for the skin data.
	int offsetTexCoords;// The offset in the file for the texture data.
	int offsetTriangles;// The offset in the file for the face data.
	int offsetFrames;	// The offset in the file for the frames data.
	int offsetGlCommands;// The offset in the file for the gl commands data.
	int offsetEnd;		// The end of the file offset.
};

// Some structures to hold or read in data in.
struct stMd2Skins
{
	char skinName[64];
};

struct stMd2TexCoords
{
	short u, v;
};

struct stMd2Triangles
{
	short vertexIndex[3];
	short texIndex[3];
};

struct stMd2Vertices
{
	float vertex[3];
	float normal[3];
};
struct stMd2Frames
{
	char name[16];
	stMd2Vertices* pFinalVerts;
};
///used in directX


extern LPDIRECT3D9 g_pD3D;
extern LPDIRECT3DDEVICE9 g_pD3DDevice;

struct stKeyFrame
{
	int start;
	int end;
	char szName[16];
};
struct stKey
{
	int numKeys;
	stKeyFrame *pKey;
};

struct my_vertex
{
      FLOAT x, y, z;  // D3DFVF_XYZ
      FLOAT tu, tv;   // D3DFVF_TEX1
};

/***************************************************************************/
/*                                                                         */
/* The CLoadMD2 class, yup its name speaks for itself, it loads the 3D     */
/* model data from the .md2 file and then we can access its public data    */
/* variables to use the data.                                              */
/*                                                                         */
/***************************************************************************/

class CMD2X
{
public:
	CMD2X(){ m_vertex_description = (D3DFVF_XYZ | D3DFVF_TEX1); };
	~CMD2X(){};
public:
	bool ImportMD2(char* szFileName, char* szTexName);
	void Release();
	
	void RenderFrame();

	void Animate(UINT iAnimKey);


	//multi instance
	stMd2AnimationData GetAnimationData();
	void SetAnimationData(const stMd2AnimationData*);

	void RenderWith(stMd2AnimationData*);

protected:
	void ReadMD2Data();
	void SetUpFrames();
	void SetUpDX();
	
	bool Timer(float* t);
	void SetDXVertices(UINT iAnimKey);

	FILE* m_fp;
protected:
	stMd2Header		m_Md2Header;

	stMd2Skins		*m_pSkins;
	stMd2Triangles	*m_pTriangles;
	stMd2TexCoords  *m_pTexCoords;
	stMd2Frames		*m_pFrames;
protected:
	stKey			m_Keys;
	int m_curFrame;
	int m_nextFrame; 
	int m_curAnimKey;
protected:
	float m_lastTime;
	float m_elapsedTime;
protected:// dx variables
	UINT m_vertex_description;
	IDirect3DTexture9* m_pTexture;
	IDirect3DVertexBuffer9 * m_vb; // vertex buffer
};