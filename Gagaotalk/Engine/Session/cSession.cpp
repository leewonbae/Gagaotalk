#include "cSession.h"
  
#include "cInitServer.h"
#include "cDBmanager.h"
cSession::cSession()
{
	mDisconnectOverdata.mWorkFleg = WORKFLEG_DISCONNECT;

}
cSession::~cSession()
{
	 
}
void  cSession::ShowClientInfo()
{
	SOCKADDR_IN clientaddr;
	int len = sizeof(clientaddr);

	getpeername(mAcceptSocket, (SOCKADDR*)&clientaddr, &len);

	printf("[   IP   ] =  %s , \t", inet_ntoa(clientaddr.sin_addr));
	printf("[  PORT  ] =  %d \n", ntohs(clientaddr.sin_port));
	 

}


//--------------------------------------------------------------------------------------------------------------------------------------
//��  �� : ���� Ǯ�� �غ� 
//�ۼ��� : �̿���
//��  �� : 2020. 02. 13
//--------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------
void cSession::SetReadyAccept()
{
	 
	ZeroMemory(&mAcceptOverdata.mOverlapped, sizeof(mAcceptOverdata.mOverlapped));
	mAcceptOverdata.mWorkFleg = WORKFLEG_ACCPET;
	mAcceptOverdata.mWsabuf.buf = mAcceptBuf;
	mAcceptOverdata.SelfPoint = this;

	SOCKET ListenSock =  SINGLETON(cInitServer).getListenSock();
	if (ListenSock == INVALID_SOCKET)
	{
		cout << "session get sock error" << endl;
	}
	mAcceptSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mAcceptSocket == INVALID_SOCKET)
	{
		cout << "session get sock error" << endl;
	}
	
	DWORD recvbytes = 0;
	
	
	setsockopt(mAcceptSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&ListenSock, sizeof(ListenSock));
	 
	DWORD retval  = AcceptEx(ListenSock, mAcceptSocket, mAcceptOverdata.mWsabuf.buf, 0, sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16, &recvbytes, (LPOVERLAPPED)&mAcceptOverdata.mOverlapped);


	if (retval == false && WSAGetLastError() != WSA_IO_PENDING)
	{
		 
		printf("AcceptEx Fail\n");
		 
	}
	
	 
}

//--------------------------------------------------------------------------------------------------------------------------------------
//��  �� : ���� ��Ȱ��
//�ۼ��� : �̿���
//��  �� : 2020. 02. 13
//--------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------

void cSession::RecycleCloseSocket()
{ 
	TransmitFile(mAcceptSocket, NULL, NULL, NULL, (LPOVERLAPPED)&mDisconnectOverdata, NULL, TF_DISCONNECT | TF_REUSE_SOCKET);
	SetReadyAccept();


}

 
//--------------------------------------------------------------------------------------------------------------------------------------
//��  �� : �۾� Recv  
//�ۼ��� : �̿���
//��  �� : 2020. 02. 13
//--------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------

void cSession::Flag_Recv(OVERDATA* _over)
{

	PACKET_HEAD* recvpacket = (PACKET_HEAD*)mRecvBuf;
	Parsing(recvpacket);
	
	OnRecv();
}


//--------------------------------------------------------------------------------------------------------------------------------------
//��  �� : �۾� Send
//�ۼ��� : �̿���
//��  �� : 2020. 02. 13
//--------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------
void cSession::Flag_Send(OVERDATA* _over)
{
	PACKET_HEAD* sendpacket = (PACKET_HEAD*)mSendBuf;
	if (sendpacket->mCmd == PROTOCOL_TRANS_MSG_ACK)
	{
		PACKET_TRANS_MSG_ACK* packet = (PACKET_TRANS_MSG_ACK*)sendpacket;
		SINGLETON(cDBmanager).SaveMsgLog(
			packet->mSendKey,
			packet->mRecvKey,
			packet->mSendMsg
		);
		SINGLETON(cDBmanager).CheckMsgBox();
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------
//��  �� : RECV
//�ۼ��� : �̿���
//��  �� : 2020. 02. 13
//--------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------


void cSession::OnRecv()
{

	DWORD recvbytes, flag = 0;
	

	ZeroMemory(&mRecvOverdata.mOverlapped, sizeof(mRecvOverdata.mOverlapped));
	ZeroMemory(mRecvBuf, BUFFER_SIZE);

	mRecvOverdata.mWsabuf.buf = mRecvBuf;
	mRecvOverdata.mWsabuf.len = BUFFER_SIZE;

	mRecvOverdata.mWorkFleg = WORKFLEG_RECV;
	WSARecv(mAcceptSocket, &mRecvOverdata.mWsabuf, 1, &recvbytes, &flag, &mRecvOverdata.mOverlapped, NULL);

	 

}
//--------------------------------------------------------------------------------------------------------------------------------------
//��  �� : SEND
//�ۼ��� : �̿���
//��  �� : 2020. 02. 13
//--------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------

void cSession::OnSend(void* _packet, int _packetsize, int _safekey)
{
	DWORD sendbytes;
	mSendOverdata.mWorkFleg = WORKFLEG_SEND;

	ZeroMemory(&mSendOverdata.mOverlapped, sizeof(mSendOverdata.mOverlapped));
	ZeroMemory(mSendBuf, BUFFER_SIZE);

	if (_safekey == 0)// �ʱ� safekey ���� �� ,
	{
		memcpy(mSendBuf, _packet, _packetsize);
	}
	else  // �� ���� _safekey ���� �̿��Ͽ� xor �������� ��Ŷ ��ȣȭ 
	{
		char temp[BUFFER_SIZE] = { 0, };
		memcpy(temp, _packet, _packetsize);
		
		for (int i = 0; i < _packetsize;i++)
		{
			mSendBuf[i] = temp[i] ^ _safekey;
		}
 
	}
 
	mSendOverdata.mWsabuf.buf = mSendBuf;
	mSendOverdata.mWsabuf.len = _packetsize;
	WSASend(mAcceptSocket, &mSendOverdata.mWsabuf, 1, &sendbytes, 0, &mSendOverdata.mOverlapped, NULL);

}