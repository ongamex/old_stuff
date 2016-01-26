#include "DBOXTimer.h"


DBOXTimer::DBOXTimer(void)
{
	Initialize();
}

void DBOXTimer::Initialize()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&Freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&LastTime);
	iDelta = 0;
	fDelta = 0.f;
	fps = 0.f;
	SecondsPerTick = 1.f/(float)Freq;
	//memset(&this->LastTime,0 ,sizeof(LastTime));
	QueryPerformanceCounter((LARGE_INTEGER*)&LastTime);

}

void DBOXTimer::Update()
{
	__int64 Cnt;
	//QueryPerformanceFrequency((LARGE_INTEGER*)&Freq);//freqency cannot be changed while the system is running
	QueryPerformanceCounter((LARGE_INTEGER*)&Cnt);
	
	iDelta = (Cnt - LastTime);
	fDelta = (float)(iDelta)*SecondsPerTick;
	fps = 1.f/fDelta;
	LastTime = Cnt;
	return;
}

DBOXTimer::~DBOXTimer(void)
{
}
