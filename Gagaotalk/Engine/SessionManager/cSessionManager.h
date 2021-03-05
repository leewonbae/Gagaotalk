#pragma once
#include "Resource.h"
#include "cUser.h"
#include "cCriticalSection.h"
class cSessionManager : public cSingleton< cSessionManager>
{
	friend class cSingleton< cSessionManager>;
public:

	void MakeSessionObject(int _max_session_count);

	void InputNewSession(cSession* _session);
	void OutSession();
	
 
	 
private:
	cSessionManager();
	~cSessionManager();

	void ShowSessionCount();
	cSession* mSessionArray[MAX_SESSION_COUNT] = {NULL,} ;
	 
	cCriticalSection mCS_SessionManager;
	int mUsingSessionCount = 0;

 

	 

};

