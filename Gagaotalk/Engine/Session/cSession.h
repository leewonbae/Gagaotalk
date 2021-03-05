#pragma once
#include "Resource.h"
#include "Packet.h"

//--------------------------------------------------------------------------------------------------------------------------------------
//목  적 : Session class 
//작성자 : 이원배
//일  시 : 2020. 02. 13
//--------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------

class cSession
{
public:
	cSession();
	~cSession();

	void setKey(int _sessionkey) { mSessionPK = _sessionkey; }
	int getKey() { return mSessionPK; }
 
	SOCKET getSock() { return mAcceptSocket; }

	void SetUserOrSession(bool _flag) { mUserOrSession = _flag; }
	bool GetUserOrSession() { return mUserOrSession; }

	void OnRecv();
	void OnSend(void* _packet, int _packetsize, int _safekey);
	void SetReadyAccept();
	void RecycleCloseSocket();
 
	void ShowClientInfo();

	 
	void Flag_Recv(OVERDATA* _over);
	void Flag_Send(OVERDATA* _over);

	
	virtual void Parsing(PACKET_HEAD* _packet) = 0;
private:
	OVERDATA mRecvOverdata, mSendOverdata, mAcceptOverdata, mDisconnectOverdata;
	SOCKET mAcceptSocket;
	char mRecvBuf[BUFFER_SIZE] = { 0, };
	char mSendBuf[BUFFER_SIZE] = { 0, };
	char mAcceptBuf[BUFFER_SIZE] = { 0, };

	bool mUsingFlag = false;
	bool mUserOrSession = false;
	int mSessionPK = -1;
};

