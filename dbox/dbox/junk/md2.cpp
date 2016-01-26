/***************************************************************************/
/*                                                                         */
/* File: md2.cpp                                                           */
/* Author: bkenwright@screentoys.net                                       */
/* Date: 10-11-2002                                                        */
/*                                                                         */
/***************************************************************************/


#include "md2.h"


/***************************************************************************/
/*                                                                         */
/* The action implimentations of our CLoadMD2 class.                       */
/*                                                                         */
/***************************************************************************/
bool CMD2X::ImportMD2(char* szFileName, char* szTexName)
{
	m_fp = fopen(szFileName, "rb");

	ReadMD2Data();
	SetUpFrames();
	SetUpDX();

	fclose(m_fp);
	
	// Load or textures into our DX.
	 
	D3DXCreateTextureFromFile(g_pD3DDevice, szTexName, &m_pTexture);

	return true;
}

void CMD2X::ReadMD2Data()
{
	fread(&m_Md2Header, 1, sizeof(m_Md2Header), m_fp);	

	// Allocate memory for our data so we can read it in.
	m_pSkins		= new stMd2Skins	[ m_Md2Header.numSkins ];
	m_pTexCoords	= new stMd2TexCoords[ m_Md2Header.numTexCoords ];
	m_pTriangles	= new stMd2Triangles[ m_Md2Header.numTriangles ];
	m_pFrames		= new stMd2Frames	[ m_Md2Header.numFrames ];

	// -1- Seek to the start of our skins name data and read it in.
	fseek(m_fp, m_Md2Header.offsetSkins, SEEK_SET);
	fread(m_pSkins, sizeof(stMd2Skins), m_Md2Header.numSkins, m_fp);

	// -2- Seek to the start of our Texture Coord data and read it in.
	fseek(m_fp, m_Md2Header.offsetTexCoords, SEEK_SET);
	fread(m_pTexCoords, sizeof(stMd2TexCoords), m_Md2Header.numTexCoords, m_fp);

	// -3- Seek to the start of the Triangle(e.g. Faces) data and read that in.
	fseek(m_fp, m_Md2Header.offsetTriangles, SEEK_SET);
	fread(m_pTriangles, sizeof(stMd2Triangles), m_Md2Header.numTriangles, m_fp);

	// -4- Finally lets read in "one" of the frames, the first one.!
	
	struct stAliasVerts
	{
		byte vertex[3]; // an index reference into the location of our vertexs
		byte lightNormalIndex; // in index into which tex coords to use.
	};
	struct stAliasFrame
	{
		float scale[3];
		float translate[3];
		char name[16];
		stAliasVerts aliasVerts[1];
	};
	
	unsigned char largebuffer[50000];
	stAliasFrame* pTempFrame = (stAliasFrame*) largebuffer;

	fseek(m_fp, m_Md2Header.offsetFrames, SEEK_SET);

	for(int iFrame=0; iFrame< m_Md2Header.numFrames; iFrame++)
	{

		fread(pTempFrame, 1, m_Md2Header.frameSize, m_fp); // We have read in all the frame data here (into a temporyary!!..eeEKK)..

		m_pFrames[iFrame].pFinalVerts = new stMd2Vertices[ m_Md2Header.numVertices ];
		
		strcpy( m_pFrames[iFrame].name,  pTempFrame->name );
		// CONVERSION!  A few things before we can use our read in values,
		// for some reason the Z and Y need to be swapped, as Z is facing up
		// and Y is facing into the screen.
		// Also our texture coordinates values are between 0 and 256, we just
		// divide them all by 256 which makes them between 0 and 1.

		// Swap Z<->Y
		for(int i=0; i< m_Md2Header.numVertices; i++)
		{
			m_pFrames[iFrame].pFinalVerts[i].vertex[0] = pTempFrame->aliasVerts[i].vertex[0] * pTempFrame->scale[0] 
																		+ pTempFrame->translate[0];      // x
			m_pFrames[iFrame].pFinalVerts[i].vertex[2] = -1*(pTempFrame->aliasVerts[i].vertex[1] * pTempFrame->scale[1] 
																		+ pTempFrame->translate[1]); // z
			m_pFrames[iFrame].pFinalVerts[i].vertex[1] = pTempFrame->aliasVerts[i].vertex[2] * pTempFrame->scale[2] 
																		+ pTempFrame->translate[2];      // y
		}
	}
	// Scale Textures.
	for (int j=0; j< m_Md2Header.numTexCoords; j++)
	{
		// WARNING.. you can't put a decimal number into a short...e.g.
		// you can't put 0.1 into a unsigned short int, it will be changed to 0.
		/*
		m_pTexCoords[j].u = m_pTexCoords[j].u ;// 256; //float(m_Md2Header.skinWidth);
		m_pTexCoords[j].v = m_pTexCoords[j].v ;// 256; //float(m_Md2Header.skinHeight);
		*/
	}
	
}

void CMD2X::SetUpFrames()
{
	//stand01, stand02... walk01 etc.
	//First lets see how many key frames there are.. e.g.  walk, run, stand etc.
	m_Keys.numKeys = 0;
	char strName[16] = {0};
	char strLastName[16] = {0};
	char strVeryFirstName[16] = {0};

	for(int iFrame=0; iFrame< m_Md2Header.numFrames; iFrame++)
	{
		strcpy(strName, m_pFrames[iFrame].name);
		int stringLength = strlen( strName );
		for(int i=0; i< stringLength; i++)
		{
		// a:97   A:65   z:122  Z:90
			if( (strName[i] < 'A') || (strName[i] > 'z' ) )
			{
				// Its an integer if we are here.
				strName[i] = '\0';
				if( strcmp( strLastName, strName ) )
				{
					strcpy(strLastName, strName);
					m_Keys.numKeys++;
				}
				if( !strcmp( strVeryFirstName, "" ) )
					strcpy( strVeryFirstName, strName );
				break;
			}
		}	
	}
	// Now that we are here, we know how many of each key frame there
	// are, e.g. there are 4 for stand, 6 for walk etc.
	//m_Keys.numKeys--;
	m_Keys.pKey = new stKeyFrame[ m_Keys.numKeys ];

	strcpy( strName, "");
	strcpy( strLastName, strVeryFirstName);
	//strcpy(strLastName, "");
	int curKey = 0;

	m_Keys.pKey[0].start = 0;
	strcpy( m_Keys.pKey[0].szName, strVeryFirstName );

	for(int iFrame=0; iFrame< m_Md2Header.numFrames; iFrame++)
	{
		strcpy(strName, m_pFrames[iFrame].name);
		int stringLength = strlen( strName );
		for(int i=0; i< stringLength; i++)
		{
			// a:97   A:65   z:122  Z:90
			if( (strName[i] < 'A') || (strName[i] > 'z' ) )
			{
				// Its an integer if we are here.
				strName[i] = '\0';
				if( strcmp( strLastName, strName ) )
				{
					m_Keys.pKey[curKey].end = iFrame-1;
					curKey++;
					m_Keys.pKey[curKey].start = iFrame;

					strcpy(strLastName, strName);
					strcpy(m_Keys.pKey[curKey].szName, strName);
				}
				else
				{
					m_Keys.pKey[curKey].end = iFrame;
				}
				break;
			}
		}	
	}


}

void CMD2X::SetUpDX()
{
	/***************************************************************************/
	/* Data is transfered into the directX vertex buffers etc.                 */
	/***************************************************************************/
	int numFaces = m_Md2Header.numTriangles;

	// Create a vertex buffer for a single model frame.. all frames are the same size.
	g_pD3DDevice->CreateVertexBuffer( numFaces * 3 * sizeof(my_vertex), 0, m_vertex_description, D3DPOOL_MANAGED, &m_vb , 0);


}

bool CMD2X::Timer(float* t)
{
	float delay = 0.2f; // 0.2 of a second between changes. ~Speed of your models animation~

	float time = GetTickCount();

	m_elapsedTime = time - m_lastTime;
	m_elapsedTime = m_elapsedTime /1000.0f; // convert to seconds.
	m_elapsedTime = m_elapsedTime / delay;  // make into a percentage of 100 e.g. 0->1

	*t = m_elapsedTime;

	if( m_elapsedTime > 1.0f )
	{
		m_lastTime = time;
		m_elapsedTime = 0.0f;

		*t = m_elapsedTime;
		return true;
	}
	return false;
}

stMd2AnimationData CMD2X::GetAnimationData()
{
	stMd2AnimationData retval;
	retval.Animation = m_curAnimKey;
	retval.CurrFrame = m_curFrame;
	retval.NextFrame = m_nextFrame;
	retval.LastTime = m_lastTime;
	return retval ;
}
void CMD2X::SetAnimationData(const stMd2AnimationData* data)
{
		m_curAnimKey = data->Animation;
		m_curFrame = data->CurrFrame ;
		m_nextFrame = data->NextFrame ;
		m_lastTime = data->LastTime;
}

void CMD2X::RenderWith(stMd2AnimationData* a)
{
		this->SetAnimationData(a);
		this->Animate(a->Animation);
		this->RenderFrame();
		*a = this->GetAnimationData();
}

void CMD2X::SetDXVertices(UINT iAnimKey)
{
	// First Create the position of our new verts using
	// p(t) = p0 + t(p1 - p0);

	// For those who are sharp to the eye, well its arranged
	// this way, as if you change from a walk to a death, or run
	// etc, it will change gradually... not jump... creates a really
	// smooth effect.

	float t;

	if( Timer(&t) )
	{
		m_curFrame = m_nextFrame;
		m_nextFrame = (m_nextFrame + 1);

		if( m_curAnimKey != iAnimKey )
		{
			m_nextFrame = m_Keys.pKey[ iAnimKey ].start; // (curFrame + 1);
			m_curAnimKey = iAnimKey;
		}
	}


	if( m_curFrame == m_nextFrame )
		m_nextFrame++;

	if( m_nextFrame > m_Keys.pKey[ iAnimKey ].end)
		m_nextFrame = m_Keys.pKey[ iAnimKey ].start;
	


	// Copy our array which is in computer memory over to the directX memory.. using that pointer we
    // just created etc.
    unsigned char *temp_pointer_vb;
    m_vb->Lock(0,0, (void**)&temp_pointer_vb, 0);
	my_vertex* p_mem = (my_vertex*)temp_pointer_vb;

	int numFaces = m_Md2Header.numTriangles;

	stMd2Vertices*  pCurFrame =  m_pFrames[m_curFrame].pFinalVerts;
	stMd2Vertices*  pNexFrame =  m_pFrames[m_nextFrame].pFinalVerts;

	for(int p=0; p< numFaces; p++)
	{
		for(int c=0; c<3; c++) // 3 sides to each face (e.g. triangle).
		{
			int index = m_pTriangles[p].vertexIndex[c];
			float x1 = pCurFrame[index].vertex[0];//x
			float y1 = pCurFrame[index].vertex[1];//y
			float z1 = pCurFrame[index].vertex[2];//z

			float x2 = pNexFrame[index].vertex[0];//x
			float y2 = pNexFrame[index].vertex[1];//y
			float z2 = pNexFrame[index].vertex[2];//z

			p_mem[p*3+c].x = x1 + t * (x2 - x1);
			p_mem[p*3+c].y = y1 + t * (y2 - y1);
			p_mem[p*3+c].z = z1 + t * (z2 - z1);
				
			int texIndex = m_pTriangles[p].texIndex[c];
			p_mem[p*3+c].tu = m_pTexCoords[ texIndex ].u / 256.0f;
			p_mem[p*3+c].tv = m_pTexCoords[ texIndex ].v / 256.0f;
		}
	}
	m_vb->Unlock();
	
	
}

void CMD2X::Animate(UINT iAnimKey)
{
	// Check to make sure that the iAnimKey is within range.
	iAnimKey = iAnimKey % m_Keys.numKeys;

	SetDXVertices(iAnimKey);
}

void CMD2X::RenderFrame()
{
	unsigned int numFaces = m_Md2Header.numTriangles;	

    // Okay at this point... our graphics card has our vertices stored in it... we've just copied
    // them over :) Some stuff to setup or graphics card!
    // Turn off lighting becuase we are specifying that our vertices have colour
    g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	g_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1, D3DTA_TEXTURE);

	

	// Draw our triangle.
	g_pD3DDevice->SetStreamSource(0, m_vb, 0, sizeof(my_vertex));
	g_pD3DDevice->SetFVF(m_vertex_description);
	g_pD3DDevice->SetTexture(0,m_pTexture);

	//                                  number of primitives
	//                                starting vertex      |
	//                                              |      |
	//                                             \|/    \|/
	//                                              |      |
	g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, numFaces);

}

void CMD2X::Release()
{
	m_vb->Release();

	m_pTexture->Release();

	// We can delete all our allocated memory as its now been transfered over
	// to the directX vertex buffer.
	for(int iFrame=0; iFrame< m_Md2Header.numFrames; iFrame++)
	{
		delete[] m_pFrames[iFrame].pFinalVerts;
	}

	delete[] m_pSkins;
	delete[] m_pTexCoords;
	delete[] m_pTriangles;
	delete[] m_pFrames;

	delete[] m_Keys.pKey;
}
