#ifndef DBOXMATH_H
#define DBOXMATH_H

#include <d3d9.h>
#include <d3dx9.h>

#define DBOXToRadian(fDegree)		(     ( (float)(fDegree) )	 * (D3DX_PI / 180.0f)     )
#define DBOXToDegree(fRadian)		(     ( (float)(fRadian) )	 * (180.0f / D3DX_PI)     )

#define GotoVal(val , end_val  , step_size) (val > end_val) ? ((val + step_size < end_val) ? val += step_size : val = end_val) : ((val - step_size > end_val) ? val -= step_size : val = end_val)

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

#endif