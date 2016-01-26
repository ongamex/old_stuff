#ifndef DBOXMATH_H
#define DBOXMATH_H

#include <d3d9.h>
#include <d3dx9.h>

#ifndef sqr
	#define sqr(x) ( (x) * (x) )
#endif

#define signf(x) ( ( x != 0 ) ? x/fabsf(x) : 0 )

extern D3DXMATRIX DBOXMatIdent;

float DBOXNormalizeAngle(float Angle);//normalizes angle by setting it in range [0.0f ; D3DX_PI)

//Template functions

template <typename T> void Goto(T& Val , T Goal , T Step) 
{ 
	Step = fabsf(Step);
	if(Val == Goal) return;
	if(Val < Goal){Val += Step; if(Val > Goal)Val = Goal;}
	if(Val > Goal){Val -= Step; if(Val < Goal)Val = Goal;}
	return;
}

RECT Rect(long left , long top , long right , long bottom);

float lerp(float t , float a , float b);
float Vec3Dist(const D3DXVECTOR3* a ,const D3DXVECTOR3* b);

///

struct fRect {
	float left,top , right,bottom;

	fRect() : left(0),top(0) , right(0),bottom(0) {}
	fRect(float Left,float Top,float Right,float Bottom) : left(Left), top(Top), right(Right), bottom(Bottom) {}

};

struct lRect {
	long left,top , right,bottom;

	lRect() : left(0),top(0) , right(0),bottom(0) {}
	lRect(long Left,long Top,long Right,long Bottom) : left(Left), top(Top), right(Right), bottom(Bottom) {}

	RECT ToRect(){return Rect(left,top , right,bottom);}
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

fRect MovefRect(const fRect* pRect, const D3DXVECTOR3* pV);
bool OverlapfRect(fRect* r1, fRect* r2, D3DXVECTOR3* o = NULL); 

#endif