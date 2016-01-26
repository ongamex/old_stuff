/******************************************************************************/
/*
	common.h

	Desc: Some simple quick and very useful functions for testing and debug
	...slow but dirty!

*/
/******************************************************************************/

#pragma once

#include <windows.h>
#include <time.h>
#include <math.h>

#include <d3dx9.h>


#define DBG_HALT __asm{ int 0x10 }

#define DBG_ASSERT(exp) {if ( !(exp) ) {DBG_HALT;}}

//#define THROW(exp) { DBG_HALT; }

#define THROW(exp)	{ throw(exp);	}

/******************************************************************************/


#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

/******************************************************************************/

// Helper function to stuff a FLOAT into a DWORD argument
inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }


/******************************************************************************/


//Saving debug information to a log file
__forceinline void dprintf(const char *fmt, ...) 
{
	/*
	va_list parms;
	char buf[256];

	// Try to print in the allocated space.
	va_start(parms, fmt);
	vsprintf (buf, fmt, parms);
	va_end(parms);

	// Write the information out to a txt file
	FILE *fp = fopen("output.txt", "a+");
	fprintf(fp, "%s",  buf);
	fclose(fp);
	*/
}// End dprintf(..)


/******************************************************************************/

inline 
float RandNum(float rrmax)
{
	float ranNum = (float(rand()+1.0f))/RAND_MAX;
	ranNum *= rrmax;
	return (ranNum);

}

/******************************************************************************/

// returns a random number between min and max            
inline int RandomNumber(int iMin, int iMax)
{
  if (iMin == iMax) return(iMin);
  return((rand() % (abs(iMax-iMin)+1))+iMin);
}

inline float RandomNumber(float fMin, float fMax)
{
  if (fMin == fMax) return(fMin);
  float fRandom = (float)rand() / (float)RAND_MAX;
  return((fRandom * (float)fabs(fMax-fMin))+fMin);
}

inline D3DXVECTOR3 RandomNumber(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax)
{
  float x = RandomNumber(vMin.x, vMax.x);
  float y = RandomNumber(vMin.y, vMax.y);
  float z = RandomNumber(vMin.z, vMax.z);
  return(D3DXVECTOR3(x,y,z));
}

inline D3DXCOLOR RandomNumber(D3DXCOLOR Min, D3DXCOLOR Max)
{
  float r = RandomNumber(Min.r, Max.r);
  float g = RandomNumber(Min.g, Max.g);
  float b = RandomNumber(Min.b, Max.b);
  float a = RandomNumber(Min.a, Max.a);
  return(D3DXCOLOR(r,g,b,a));
}

/******************************************************************************/


/*
inline float Noise2D(int x, int y)
{
  int n = x + y * 57;
  n = (n<<13) ^ n;
  return ( 1.0f - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);    
}
 
inline float Noise3D(int x, int y, int z)
{
  int n = x + y * 57 + z * 131;
  n = (n<<13) ^ n;
  return ( 1.0f - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) 
	* 0.000000000931322574615478515625f);    
}
*/




