#include "cDBmanager.h"
cDBmanager::cDBmanager()
{

}
cDBmanager::~cDBmanager()
{

}
 

void cDBmanager::InitMysql()
{
	for (int i = 0;i < MAX_DB_CONNECTION_COUNT;i++)
	{

		MYSQL* temp = new MYSQL;
		mysql_init(temp);
		MYSQL* Connections = mysql_real_connect(temp, DB_HOST, DB_ID, DB_PASSWORD, DB_NAME, 3306, NULL, 0);

		if (Connections == NULL)
		{
			ShowError(temp);
		}
		else
		{
			mConnectionsCount++;
			mConnectionPool.push(temp);
		}

	}

	cout << "SHOW MYSQL SET OK " << endl;

}
void cDBmanager::ShowError(MYSQL* _connections)
{
	cout << "SHOW DATABASES ERROR = " << mysql_error(_connections) << endl;
}



MYSQL* cDBmanager::getConnection()
{
	MYSQL* temp = NULL;
	mCS_Mysql.LockCS();
	// 모두 사용하고 있다면, 추가 객체 생성 후 반환
	if (mConnectionPool.empty())
	{
		MYSQL* mOverConnection = new MYSQL;
		mysql_init(mOverConnection);
		mOverConnection = mysql_real_connect(temp, DB_HOST, DB_ID, DB_PASSWORD, DB_NAME, 3306, NULL, 0);
		temp = mOverConnection;
		mConnectionsCount++;
	}
	else
	{
		temp = mConnectionPool.front();
		mConnectionPool.pop();
	}


	mCS_Mysql.UnlockCS();

	return temp;

}

void cDBmanager::RecycleConnections(MYSQL* _connections)
{
	mCS_Mysql.LockCS();
	// 결과 메모리 해제 
	MYSQL_RES* res = mysql_store_result(_connections);
	mysql_free_result(res);
	mysql_close(_connections);

	// 추가 생성된 객체는 재활용 하기 전에 삭제 
	// 일정 갯수 유지 
	if (mConnectionsCount > MAX_DB_CONNECTION_COUNT)
	{
		
		delete _connections;
		mConnectionsCount--;
	}
	else
	{
		_connections = mysql_real_connect(_connections, DB_HOST, DB_ID, DB_PASSWORD, DB_NAME, 3306, NULL, 0);
		if (_connections == NULL)
		{
			ShowError(_connections);
		}
		else
		{
			mConnectionPool.push(_connections);
		}
		
	}

	mCS_Mysql.UnlockCS();
}
 
bool  cDBmanager::AddNewUser(char* _id, int _password, char* _nickname)
{

	bool result = false;
	char sql_newuser[256] = { 0, };
	MYSQL* connection = getConnection();
	  
	 
	sprintf(sql_newuser, "CALL newuser_procedure('%s',%d,'%s')", _id, _password, _nickname);

	mCS_Newuser.LockCS();
	int query_state = mysql_query(connection, sql_newuser);
	if (query_state != 0)
	{
		ShowError(connection);

	}
	else
	{
		MYSQL_RES* res = mysql_store_result(connection);
		if (res->row_count <= 0)
		{

		}
		else
		{
			result = true;
		}
		
  
	}
 
	mCS_Newuser.UnlockCS();
	RecycleConnections(connection);

	return result;
 
}
LOGIN_INFO cDBmanager::LoginCheck(char* _id, int _password)
{
	LOGIN_INFO result;
	char sql_login[256] = { 0, };
	MYSQL* connection = getConnection();

	//login_procedure`(_id char(10), _password int)
	sprintf(sql_login, "CALL login_procedure('%s',%d)", _id, _password);

 
	int query_state = mysql_query(connection, sql_login);
	if (query_state != 0)
	{
		ShowError(connection);

	}
	else
	{
		MYSQL_RES* res = mysql_store_result(connection);
		if (res->row_count <= 0)
		{

		}
		else
		{
			MYSQL_ROW row = mysql_fetch_row(res);
			result.mkey = stoi(row[0]);
			strcpy(result.mNickName, row[1]);
		}
 
	}

	 
	RecycleConnections(connection);

	return result;
}
void cDBmanager::SaveMsgLog(int _senduser, int _recvuser, char* _msg)
{
	SYSTEMTIME systime;

	GetLocalTime(&systime);
	string  current_time = to_string(systime.wYear) + to_string(systime.wMonth) + to_string(systime.wDay);
	int time = stoi(current_time);
 
	
	MSG_BOX savebox;
	
	strcpy(savebox.mMsg, _msg);
	savebox.mSendUserKey = _senduser;
	savebox.mRecvUserKey = _recvuser;
	savebox.mTime = time;

	mCS_MsgBox.LockCS();
	
	mSaveMsgBox.push_back(savebox);
	 
	mCS_MsgBox.UnlockCS();
 
}
bool cDBmanager::PushOffLineMsgBox(int _senduser, int _recvuser, char* _msg)
{
	bool result = false;
	
	MSG_BOX temp;
	strcpy(temp.mMsg, _msg);
	temp.mSendUserKey = _senduser;
	temp.mRecvUserKey = _recvuser;
	mCS_Offline.LockCS();
	mOfflineMsgBox.push_back(temp);
	mCS_Offline.UnlockCS();
	
	if (mOfflineMsgBox.size() >= MAX_OFFLINE_MSG_COUNT)
	{
		result = true;
	}

	return result;
}
void cDBmanager::InsertOfflineBox()
{
	vector<MSG_BOX> temp = mOfflineMsgBox;
	mCS_Offline.LockCS();
	mOfflineMsgBox.clear();
	mCS_Offline.UnlockCS();

	MYSQL* connections = getConnection();
	//offline_msg_procedure
	char sql_offline[256] = { 0, };
	for (int i = 0;i < MAX_OFFLINE_MSG_COUNT;i++)
	{
		sprintf(sql_offline, "CALL offline_msg_procedure(%d,%d,'%s')",
			temp[i].mSendUserKey,temp[i].mRecvUserKey,temp[i].mMsg);
		int query_state = mysql_query(connections, sql_offline);
		if (query_state != 0)
		{
			ShowError(connections);
		}
		
	}

	RecycleConnections(connections);
	
}
void cDBmanager::CheckMsgBox()
{
	bool result = false;
	vector<MSG_BOX> TempMsgBox;
	mCS_MsgBox.LockCS();
	if (mSaveMsgBox.size() == MAX_DB_MSG_SAVE_COUNT)
	{
		TempMsgBox = mSaveMsgBox;
		mSaveMsgBox.clear();
		result = true;
	}
	mCS_MsgBox.UnlockCS();
 	
	if (result == true)
	{
		MYSQL* connection = getConnection();

		for (int i = 0;i < MAX_DB_MSG_SAVE_COUNT;i++)
		{
			char sql_log[256] = { 0, };
			sprintf(sql_log, "CALL msg_log_procedure(%d,%d,%d,'%s')",
				TempMsgBox[i].mTime,
				TempMsgBox[i].mSendUserKey,
				TempMsgBox[i].mRecvUserKey,
				TempMsgBox[i].mMsg
			);
			int query_state = mysql_query(connection, sql_log);
			if (query_state != 0)
			{
				ShowError(connection);

			}
		}

		RecycleConnections(connection);

	}
 
}

FRIEND_LIST cDBmanager::GetFriendData(int _key)
{
	FRIEND_LIST result;
	char sql_friend[256] = { 0, };
	MYSQL* connection = getConnection();

	 
	sprintf(sql_friend, "CALL call_my_friend_list_procedure(%d)", _key);

	mCS_Friend.LockCS();
	int query_state = mysql_query(connection, sql_friend);
	mCS_Friend.UnlockCS();

	if (query_state != 0)
	{
		ShowError(connection);

	}
	else
	{
		MYSQL_RES* res = mysql_store_result(connection);
		if (res->row_count <= 0)
		{

		}
		else
		{
			MYSQL_ROW row;
			while ((row = mysql_fetch_row(res)) != NULL)
			{
				if (result.mFriendCount >= MAX_FRIEND_COUNT)
				{
					break;
				}
				result.mFriendKeyList[result.mFriendCount] = stoi(row[0]);
				strcpy(result.mFriendNickNameList[result.mFriendCount], row[1]);
				result.mFriendCount++;
			}
			 
		}

	}


	RecycleConnections(connection);

	return result;
}
bool cDBmanager::CheckAddFriend(int _key)
{
	bool result = false;
	char sql_check[256] = { 0, };
	MYSQL* connection = getConnection();

	 
	sprintf(sql_check, "CALL check_friend_count_procedure(%d)", _key);


	int query_state = mysql_query(connection, sql_check);
	if (query_state != 0)
	{
		ShowError(connection);

	}
	else
	{
		MYSQL_RES* res = mysql_store_result(connection);
		if (res->row_count <= 0)
		{

		}
		else
		{
			MYSQL_ROW row;
			if((row = mysql_fetch_row(res)) != NULL )
			{
				if (stoi(row[0]) < MAX_FRIEND_COUNT)
				{
					result = true;
				}
				else
				{
					result = false;
				}
			}

		}

	}


	RecycleConnections(connection);

	return result;
}
bool cDBmanager::AddFriendList(int _mykey, int _addkey, char* _nick)
{
	bool result = false;
	char sql_add[256] = { 0, };
	MYSQL* connection = getConnection();

	//add_friend_procedure`(_my_key int, _add_key int, _nick char(20))
	sprintf(sql_add, "CALL add_friend_procedure(%d,%d,'%s')",
		_mykey, _addkey, _nick);
	mCS_Friend.LockCS();

	int query_state = mysql_query(connection, sql_add);
	mCS_Friend.UnlockCS();

	if (query_state != 0)
	{
		ShowError(connection);

	}
	else
	{
		MYSQL_RES* res = mysql_store_result(connection);
		if (res == NULL)
		{
			
		}
		else
		{
			if (res->row_count <= 0)
			{

			}
			else
			{

			}
			result = true;
		}

	}


	RecycleConnections(connection);
	return result;
}
LOGIN_INFO cDBmanager::FindUserData(int _finduserkey)
{
	LOGIN_INFO result;
	char sql_find[256] = { 0, };
	MYSQL* connection = getConnection();

	//find_user_procedure
	sprintf(sql_find, "CALL find_user_procedure(%d)",
		_finduserkey);
	mCS_Friend.LockCS();

	int query_state = mysql_query(connection, sql_find);
	mCS_Friend.UnlockCS();
	if (query_state != 0)
	{
		ShowError(connection);

	}
	else
	{
		MYSQL_RES* res = mysql_store_result(connection);
		 
		MYSQL_ROW row;

		if ((row = mysql_fetch_row(res)) != NULL)
		{
			// 유저키, 닉네임
			result.mkey = stoi(row[0]);
			strcpy(result.mNickName, row[1]);
		}

	}


	RecycleConnections(connection);

	return result;
}

vector<MSG_BOX> cDBmanager::SelectOfflineMsg(int _requser)// OFFLINE MSG 가져오기
{
	vector<MSG_BOX> result;
	MYSQL* connections = getConnection();
	char sql_selectoff[256] = { 0, };

	sprintf(sql_selectoff, "CALL select_offline_procedure(%d)",
		_requser);
	mCS_Offline.LockCS();

	int query_state = mysql_query(connections, sql_selectoff);
	if (query_state != 0)
	{
		ShowError(connections);
	}
	else
	{

		MYSQL_RES* res = mysql_store_result(connections);
		MYSQL_ROW row;
		MSG_BOX temp;
		while ((row = mysql_fetch_row(res)) != NULL)
		{
			temp.mSendUserKey = stoi(row[0]);
			temp.mRecvUserKey = stoi(row[1]);
			strcpy(temp.mMsg, row[2]);

			result.push_back(temp);
		}
	}

	mCS_Offline.UnlockCS();
	RecycleConnections(connections);

	return result;
}
void cDBmanager::DeleteOfflineMSG(int _key)
{
	MYSQL* connections = getConnection();

	char sql_delete[256] = { 0, };

	sprintf(sql_delete, "CALL delete_offline_procedure(%d)", _key);
	mCS_Offline.LockCS();

	int query_state = mysql_query(connections, sql_delete);
	if (query_state != 0)
	{
		ShowError(connections);
	}
	else
	{
 
	}

	mCS_Offline.UnlockCS();

	RecycleConnections(connections);
}