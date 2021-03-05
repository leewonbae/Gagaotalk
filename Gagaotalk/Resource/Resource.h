#pragma once

#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <mswsock.h>
#include <ctime>
#include <mysql.h>
#include <string>
#include "cSingleton.h"
 
#pragma comment (lib,"ws2_32.lib")
#pragma comment (lib,"mswsock.lib")
#pragma comment (lib,"libmysql.lib")

using namespace std;
/////////////////////////////////////////////////////////////////////
#include <map>
#include <queue>
#include <vector>
#include <list>

/////////////////////////////////////////////////////////////////////

#pragma warning (disable : 4996)
#pragma warning (disable : 4805)

////////////////////////////////////////////////////////////////////
#define BUFFER_SIZE 1024
#define PORT 7296

#define WORKFLEG_ACCPET 0
#define WORKFLEG_SEND 1
#define WORKFLEG_RECV 2
#define WORKFLEG_DISCONNECT 3

#define DB_ID "root"
#define DB_HOST "127.0.0.1"
#define DB_NAME "scema_chatting_server"
#define DB_PASSWORD "814dnjsqo*"
#define DB_PORT 3306
 
#define MAX_DB_CONNECTION_COUNT 8
#define MAX_DB_MSG_SAVE_COUNT 50 
#define MAX_SESSION_COUNT 50
#define MAX_FRIEND_COUNT 20
#define MAX_OFFLINE_MSG_COUNT 1

#define MAX_SEND_OFFLINE_MSG 20
/////////////////////////////////////////////////////////////
struct OVERDATA {
	OVERLAPPED mOverlapped;
	WSABUF mWsabuf;
	int mWorkFleg;
	LPVOID	SelfPoint;
};
struct MSG_BOX {
	int mTime = 0;
	int mSendUserKey = 0;
	int mRecvUserKey = 0;
	char mMsg[20] = { 0, };
};

struct LOGIN_INFO {
	int mkey = 0;
	char mNickName[20] = { 0, };
};

struct FRIEND_LIST {
	int mFriendCount = 0;
	int mFriendKeyList[MAX_FRIEND_COUNT] = { 0, };
	char mFriendNickNameList[MAX_FRIEND_COUNT][20] = { 0, };

};
 
/////////////////////////////////////////////////////////////

