#pragma once

enum PACKET_PROTOCOL {
	PROTOCOL_HEAD = 0,
	PROTOCOL_ACCEPT_ACK,

	PROTOCOL_LOGIN_REQ,
	PROTOCOL_LOGIN_ACK,

	PROTOCOL_NEWSUER_REQ,
	PROTOCOL_NEWSUER_ACK,

	PROTOCOL_SEND_MSG_REQ,
	PROTOCOL_SEND_MSG_ACK,
	PROTOCOL_TRANS_MSG_ACK,

	PROTOCOL_FRIEND_LIST_REQ,
	PROTOCOL_FRIEND_LIST_ACK,

	PROTOCOL_FIND_FRIEND_REQ,
	PROTOCOL_FIND_FRIEND_ACK,

	PROTOCOL_ADD_FRIEND_REQ,
	PROTOCOL_ADD_FRIEND_ACK,

	PROTOCOL_OFFLINE_DATA_REQ,
	PROTOCOL_OFFLINE_DATA_ACK,

	PROTOCOL_ONLINE_DATA_REQ,
	PROTOCOL_ONLINE_DATA_ACK,

};


class PACKET_HEAD {
public:
	int mCmd = 0;
	int mPacketSize = 0;
};
/////////////////////////////////////////////////////////////////////
class PACKET_ONLINE_DATA_REQ : public PACKET_HEAD {
public:
	PACKET_ONLINE_DATA_REQ()
	{
		mCmd = PROTOCOL_ONLINE_DATA_REQ;
		mPacketSize = sizeof(PACKET_ONLINE_DATA_REQ);
	}
	int mToKey = 0;
	char mMsg[10] = { 0, };
};
class PACKET_ONLINE_DATA_ACK : public PACKET_HEAD {
public:
	PACKET_ONLINE_DATA_ACK()
	{
		mCmd = PROTOCOL_ONLINE_DATA_ACK;
		mPacketSize = sizeof(PACKET_ONLINE_DATA_ACK);
	}
	int mFromUserKey = 0;
	char mOnlineMsg[10] = { 0, }; 
};
/////////////////////////////////////////////////////////////////////
class PACKET_OFFLINE_DATA_REQ : public PACKET_HEAD {
public:
	PACKET_OFFLINE_DATA_REQ()
	{
		mCmd = PROTOCOL_OFFLINE_DATA_REQ;
		mPacketSize = sizeof(PACKET_OFFLINE_DATA_REQ);
	}

};
class PACKET_OFFLINE_DATA_ACK : public PACKET_HEAD {
public:
	PACKET_OFFLINE_DATA_ACK()
	{
		mCmd = PROTOCOL_OFFLINE_DATA_ACK;
		mPacketSize = sizeof(PACKET_OFFLINE_DATA_ACK);
	}
	// -1 == offline msg end;
	int mMsgCount = 0;
	int mSendUserKey[MAX_SEND_OFFLINE_MSG] = { 0, };
	char mOfflineMsg[MAX_SEND_OFFLINE_MSG][20] = { 0, }; 
};
/////////////////////////////////////////////////////////////////////
class PACKET_FIND_FRIEND_REQ : public PACKET_HEAD {
public:
	PACKET_FIND_FRIEND_REQ()
	{
		mCmd = PROTOCOL_FIND_FRIEND_REQ;
		mPacketSize = sizeof(PACKET_FIND_FRIEND_REQ);
	}
	int mFindUserKey = 0;

};
class PACKET_FIND_FRIEND_ACK : public PACKET_HEAD {
public:
	PACKET_FIND_FRIEND_ACK()
	{
		mCmd = PROTOCOL_FIND_FRIEND_ACK;
		mPacketSize = sizeof(PACKET_FIND_FRIEND_ACK);
	}
	int mFindUserKey = 0;
	char mNickName[20] = { 0, };

};
/////////////////////////////////////////////////////////////////////
class PACKET_FRIEND_LIST_REQ : public PACKET_HEAD {
public:
	PACKET_FRIEND_LIST_REQ()
	{
		mCmd = PROTOCOL_FRIEND_LIST_REQ;
		mPacketSize = sizeof(PACKET_FRIEND_LIST_REQ);
	}


};
class PACKET_FRIEND_LIST_ACK : public PACKET_HEAD {
public:
	PACKET_FRIEND_LIST_ACK()
	{
		mCmd = PROTOCOL_FRIEND_LIST_ACK;
		mPacketSize = sizeof(PACKET_FRIEND_LIST_ACK);
	}

	FRIEND_LIST mFriendList;
};
/////////////////////////////////////////////////////////////////////
class PACKET_ADD_FRIEND_REQ : public PACKET_HEAD {
public:
	PACKET_ADD_FRIEND_REQ()
	{
		mCmd = PROTOCOL_ADD_FRIEND_REQ;
		mPacketSize = sizeof(PACKET_ADD_FRIEND_REQ);
	}
	char mFriendNickName[20] = { 0, };
	int mAddFriendKey = 0;
};
class PACKET_ADD_FRIEND_ACK : public PACKET_HEAD {
public:
	PACKET_ADD_FRIEND_ACK()
	{
		mCmd = PROTOCOL_ADD_FRIEND_ACK;
		mPacketSize = sizeof(PACKET_ADD_FRIEND_ACK);
	}
	bool result = false;
	char mFriendNickName[20] = { 0, };
	int mAddFriendKey = 0;

};

/////////////////////////////////////////////////////////////////////

class PACKET_SEND_MSG_REQ : public PACKET_HEAD {
public:
	PACKET_SEND_MSG_REQ()
	{
		mCmd = PROTOCOL_SEND_MSG_REQ;
		mPacketSize = sizeof(PACKET_SEND_MSG_REQ);
	}

	char mMsg[20] = { 0, };

};
class PACKET_SEND_MSG_ACK : public PACKET_HEAD {
public:
	PACKET_SEND_MSG_ACK()
	{
		mCmd = PROTOCOL_SEND_MSG_ACK;
		mPacketSize = sizeof(PACKET_SEND_MSG_ACK);
	}

};

class PACKET_TRANS_MSG_ACK : public PACKET_HEAD {
public:
	PACKET_TRANS_MSG_ACK()
	{
		mCmd = PROTOCOL_TRANS_MSG_ACK;
		mPacketSize = sizeof(PACKET_TRANS_MSG_ACK);
	}
	int mSendKey = 0;
	int mRecvKey = 0;
	char mSendUserNick[10] = { 0, };
	char mSendMsg[20] = { 0, };

};
/////////////////////////////////////////////////////////////////////
class PACKET_NEWUSER_REQ : public PACKET_HEAD {
public:
	PACKET_NEWUSER_REQ()
	{
		mCmd = PROTOCOL_NEWSUER_REQ;
		mPacketSize = sizeof(PACKET_NEWUSER_REQ);
	}

	char mNewUserId[10] = { 0, };
	int mNewUserPassword = 0;
	char mNewNickName[10] = { 0, };

};
class PACKET_NEWUSER_ACK : public PACKET_HEAD {
public:
	PACKET_NEWUSER_ACK()
	{
		mCmd = PROTOCOL_NEWSUER_ACK;
		mPacketSize = sizeof(PACKET_NEWUSER_ACK);
	}

	bool mResult = false;

};

/////////////////////////////////////////////////////////////////////
class PACKET_LOGIN_REQ : public PACKET_HEAD {
public:
	PACKET_LOGIN_REQ()
	{
		mCmd = PROTOCOL_LOGIN_REQ;
		mPacketSize = sizeof(PACKET_LOGIN_REQ);
	}

	char mUserId[10] = { 0, };
	int mUserPassword = 0;
};

class PACKET_LOGIN_ACK : public PACKET_HEAD {
public:
	PACKET_LOGIN_ACK()
	{
		mCmd = PROTOCOL_LOGIN_ACK;
		mPacketSize = sizeof(PACKET_LOGIN_ACK);
	}

	int mUserNum = 0;
	char mNickName[20] = { 0, };
};
/////////////////////////////////////////////////////////////////////
class PACKET_ACCEPT_ACK : public PACKET_HEAD {
public:
	PACKET_ACCEPT_ACK()
	{
		mCmd = PROTOCOL_ACCEPT_ACK;
		mPacketSize = sizeof(PACKET_ACCEPT_ACK);
	}
	int mSafeKey = -1;

};