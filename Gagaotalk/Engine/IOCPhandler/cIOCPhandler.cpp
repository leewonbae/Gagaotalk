#include "cIOCPhandler.h"
cIOCPhandler::cIOCPhandler()
{
	 
}
cIOCPhandler::~cIOCPhandler()
{

}
void cIOCPhandler::InitIOCPhandler()
{
	mHCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
}

void cIOCPhandler::MakeCompletionPort(SOCKET _sock, cSession* _session)
{
	CreateIoCompletionPort((HANDLE)_sock,mHCP,(ULONG_PTR)_session,0);

}
void cIOCPhandler::MakeListenCompletionPort(SOCKET _sock)
{
	 CreateIoCompletionPort((HANDLE)_sock, mHCP, (ULONG_PTR)NULL, 0);
}
