#pragma once
#include "Resource.h"
#include "Packet.h"
class cThreadManager : public cSingleton<cThreadManager>
{
	friend class  cSingleton<cThreadManager>;
public:
	void MakeThreadPool();
	void Gotoxy(int _x, int _y);
	void ShowThreadState();
	// 각 스레드별 상태 출력, 이벤트 발생시 , true  // 끝나면 false 로 전환 




	static DWORD WINAPI WorkerThread(LPVOID arg);
private:
	cThreadManager();
	~cThreadManager();
	HANDLE mMakeThreadEvent;
	vector<HANDLE> mThreadArray;
	
	
	HANDLE mEventArray[10] = { 0, };
	bool mStateArray[10] = { false, };
	
	int mThreadCount = 0;
};

