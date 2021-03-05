#pragma once
#include "Resource.h"

class cCriticalSection {
public:
	cCriticalSection()
	{
		InitializeCriticalSection(&mCS);
	}
	~cCriticalSection()
	{
		DeleteCriticalSection(&mCS);
	}
	
	void LockCS()
	{
		EnterCriticalSection(&mCS);
	}
	void UnlockCS()
	{
		LeaveCriticalSection(&mCS);
	}

private:

	CRITICAL_SECTION mCS;



};