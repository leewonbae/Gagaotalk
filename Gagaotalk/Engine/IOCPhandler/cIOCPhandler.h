#pragma once
#include "Resource.h"
#include "cSession.h"
class cIOCPhandler : public cSingleton< cIOCPhandler>
{
	friend class  cSingleton< cIOCPhandler>;
public:
	void InitIOCPhandler();
	void MakeListenCompletionPort(SOCKET _sock );
	void MakeCompletionPort(SOCKET _sock, cSession* _session);

	HANDLE getIOCPhandle() { return mHCP; }
private:
	cIOCPhandler();
	~cIOCPhandler();

	HANDLE mHCP;


};

