#pragma once
#include "Resource.h"
class cInitServer : public cSingleton< cInitServer>
{
	friend class cSingleton< cInitServer>;
public:
	void SetInitServer();
	//void StartAccept();
	void CloseWDdata();

	SOCKET getListenSock() { return mListenSock; }
private:
	cInitServer();
	~cInitServer();

	SOCKET mListenSock;
};
