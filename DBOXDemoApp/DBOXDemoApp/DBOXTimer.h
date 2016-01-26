#pragma once

#include <Windows.h>
#include <WindowsX.h>


class DBOXTimer
{
	__int64 Freq;
	__int64 LastTime;
	float SecondsPerTick;
public:

	__int64 iDelta;//read only
	float fDelta, fps;//read only

	DBOXTimer(void);

	void Initialize();
	void Update();

	~DBOXTimer(void);
};

