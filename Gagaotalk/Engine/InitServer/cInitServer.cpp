#include "cInitServer.h"
#include "cIOCPhandler.h"

cInitServer::cInitServer()
{

}
cInitServer::~cInitServer()
{

}
void cInitServer::SetInitServer()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);


	SOCKADDR_IN serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(PORT);
	serveraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

 
	mListenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (mListenSock == INVALID_SOCKET)
	{
		cout << "socket error" << endl;
	}
	bool optval = 1;
	int	checkval = 0;
 
	checkval = setsockopt(mListenSock, SOL_SOCKET, SO_REUSEADDR,(char*)&optval, sizeof(optval));
	if (checkval == SOCKET_ERROR)
		printf("Socket Option Change(SO_REUSEADDR)\n");

	checkval = setsockopt(mListenSock, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, (char*)&optval, sizeof(optval));
	if (checkval == SOCKET_ERROR)
		printf("Socket Option Chang(SO_CONDITIONAL_ACCEPT)\n");

	 

	if (bind(mListenSock, (SOCKADDR*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR)
	{
		printf("bind error");
		return;
	}

	


	if (listen(mListenSock, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("listen error");
		return;
	}
	// ¸®½¼ ¼ÒÄÏ ¿¬°á 0 ÀÌ¶û ¿¬°á 
	// ¸®½¼¼ÒÄÏÀÌ¶û, accept socket ÀÌ¶û ¿¬°á 
	SINGLETON(cIOCPhandler).MakeListenCompletionPort(mListenSock);

}
void cInitServer::CloseWDdata()
{

	closesocket(mListenSock);
	WSACleanup();
}
 