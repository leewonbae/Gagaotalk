#pragma once
#include "Resource.h"
#include "Packet.h"
#include "cCriticalSection.h"



enum SQL_MODE {
	SQL_LOGIN = 0,
	SQL_ADD_USER,
	SQL_SAVE_MSG_LOG,

};
class cDBmanager : public cSingleton< cDBmanager>
{
	friend class cSingleton< cDBmanager>;
public:
	
	MYSQL* getConnection();
	void InitMysql();
	void RecycleConnections(MYSQL* _connections);

	LOGIN_INFO LoginCheck(char* _id, int _password);//정상일 때 0 이상의 숫자 반한
	bool AddNewUser(char* _id , int _password, char* _nickname); // 
	void SaveMsgLog(int _senduser, int _recvuser , char* _msg);

	FRIEND_LIST GetFriendData(int _key);
	bool CheckAddFriend(int _key);
	bool AddFriendList(int _mykey, int _addkey, char* _nick);
	LOGIN_INFO FindUserData(int _finduserkey);
	void CheckMsgBox();
	
	bool PushOffLineMsgBox(int _senduser, int _recvuser, char* _msg);
	void InsertOfflineBox();
	
	vector<MSG_BOX> SelectOfflineMsg(int _requser);// OFFLINE MSG 가져오기
	void DeleteOfflineMSG(int _key);

private:
	cDBmanager();
	~cDBmanager();

	void ShowError(MYSQL* _connections);

	cCriticalSection mCS_Mysql,mCS_Newuser, mCS_MsgBox, mCS_Friend, mCS_Offline;
	queue<MYSQL*> mConnectionPool;
	int mConnectionsCount = 0;

	vector<MSG_BOX>  mSaveMsgBox;
	bool mMsgSaveFlag = false;

	vector<MSG_BOX> mOfflineMsgBox;
	
};

