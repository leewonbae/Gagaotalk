#include "cUser.h"
#include "cDBmanager.h"
#include "cUserManager.h"

cUser::cUser()
{
	mMap.insert(make_pair(PROTOCOL_LOGIN_REQ, &cUser::LoginFunc));
	mMap.insert(make_pair(PROTOCOL_NEWSUER_REQ, &cUser::NewUserFunc));
	mMap.insert(make_pair(PROTOCOL_FRIEND_LIST_REQ, &cUser::FriendListFunc));
	mMap.insert(make_pair(PROTOCOL_ADD_FRIEND_REQ, &cUser::AddFriendFunc));
	mMap.insert(make_pair(PROTOCOL_FIND_FRIEND_REQ, &cUser::FindFriendFunc));
	mMap.insert(make_pair(PROTOCOL_OFFLINE_DATA_REQ, &cUser::OfflineDataFunc));
	mMap.insert(make_pair(PROTOCOL_ONLINE_DATA_REQ, &cUser::OnlineDataFunc));


}
cUser::~cUser()
{

}

void cUser::Parsing(PACKET_HEAD* _packet)
{
	fp	Temp;
	mIter = mMap.find(_packet->mCmd);

	if (mIter != mMap.end())
	{
		Temp = mIter->second;
		(this->*Temp)(_packet);
	}
	else
	{
		cout << _packet << endl;
	}
}
void cUser::OfflineDataFunc(PACKET_HEAD* _packet)
{
	cout << "OfflineDataFunc" << endl;
	PACKET_OFFLINE_DATA_ACK sendpacket;
	 
	vector<MSG_BOX> temp = SINGLETON(cDBmanager).SelectOfflineMsg(this->getUsernum());
	int total_size = temp.size();
	int idx = 0;
	
	while (1)
	{
		if (total_size / MAX_SEND_OFFLINE_MSG != 0)
		{
			total_size = total_size - MAX_SEND_OFFLINE_MSG;
			sendpacket.mMsgCount = MAX_SEND_OFFLINE_MSG;
			for (int i = 0;i < MAX_SEND_OFFLINE_MSG;i++)
			{
				sendpacket.mSendUserKey[i] = temp[idx].mSendUserKey;
				strcpy(sendpacket.mOfflineMsg[i], temp[idx].mMsg);
				idx++;
			}

			OnSend(&sendpacket, sendpacket.mPacketSize, this->getKey());
			memset(&sendpacket, 0, sizeof(PACKET_OFFLINE_DATA_ACK));
		}
		else
		{
			sendpacket.mMsgCount = total_size;
			for (int i = 0;i < total_size;i++)
			{
				sendpacket.mSendUserKey[i] = temp[idx].mSendUserKey;
				strcpy(sendpacket.mOfflineMsg[i], temp[idx].mMsg);
				idx++;
			}

			OnSend(&sendpacket, sendpacket.mPacketSize, this->getKey());
			break;
		}
		
	}

	SINGLETON(cDBmanager).DeleteOfflineMSG(this->getUsernum());
	
}
void cUser::OnlineDataFunc(PACKET_HEAD* _packet)
{
	cout << "OnlineDataFunc" << endl;
	PACKET_ONLINE_DATA_REQ* recvpacket = (PACKET_ONLINE_DATA_REQ*)_packet;
	PACKET_ONLINE_DATA_ACK sendpacket;
	sendpacket.mFromUserKey = this->getUsernum();
	strcpy(sendpacket.mOnlineMsg, recvpacket->mMsg);

	cUser* temp = SINGLETON(cUserManager).ConnectCheck(recvpacket->mToKey);
	if (temp != NULL)
	{
		cout << "온라인" << endl;
		temp->OnSend(&sendpacket, sendpacket.mPacketSize, temp->getKey());

	}
	else
	{
		cout << "오프라인" << endl;
		bool check =SINGLETON(cDBmanager).PushOffLineMsgBox(this->getUsernum(),
			recvpacket->mToKey, recvpacket->mMsg);
	
		
		if (check == true)
		{
			
			SINGLETON(cDBmanager).InsertOfflineBox();
		}
		// 오프라인 데이터로 보내야댐 
	}

}
void cUser::FindFriendFunc(PACKET_HEAD* _packet)
{
	cout << "FindFriendFunc" << endl;
	PACKET_FIND_FRIEND_REQ* recvpacket = (PACKET_FIND_FRIEND_REQ*)_packet;
	PACKET_FIND_FRIEND_ACK sendpacket;
	
	LOGIN_INFO result;
	memcpy(
		&result,
		&SINGLETON(cDBmanager).FindUserData(recvpacket->mFindUserKey),
		sizeof(LOGIN_INFO)
	);

	if (result.mkey == recvpacket->mFindUserKey)
	{
		sendpacket.mFindUserKey = recvpacket->mFindUserKey;
		strcpy(sendpacket.mNickName, result.mNickName);
	}
	 
	
	OnSend(&sendpacket, sendpacket.mPacketSize, this->getKey());
}
void cUser::AddFriendFunc(PACKET_HEAD* _packet)
{
	cout << "AddFriendFunc" << endl;
	PACKET_ADD_FRIEND_REQ* recvpacket = (PACKET_ADD_FRIEND_REQ*)_packet;
	PACKET_ADD_FRIEND_ACK sendpacket;

	if (SINGLETON(cDBmanager).CheckAddFriend(mLoginInfo.mkey)==true)
	{
		cout << "추가 가능" << endl;
		if (SINGLETON(cDBmanager).AddFriendList(
			mLoginInfo.mkey,
			recvpacket->mAddFriendKey,
			recvpacket->mFriendNickName	) == true)
		{
			cout << "추가 완료" << endl;
			sendpacket.result = true;
			sendpacket.mAddFriendKey = recvpacket->mAddFriendKey;
			strcpy(sendpacket.mFriendNickName, recvpacket->mFriendNickName);
		}
		else
		{
			cout << "친구 중복" << endl;
		}
	}
	


	OnSend(&sendpacket, sendpacket.mPacketSize, this->getKey());

}
void cUser::FriendListFunc(PACKET_HEAD* _packet)
{
	cout << "FriendListFunc" << endl;
 
	PACKET_FRIEND_LIST_ACK sendpacket;

	memcpy(&sendpacket.mFriendList,
		&SINGLETON(cDBmanager).GetFriendData(mLoginInfo.mkey),
		sizeof(FRIEND_LIST)
	);


	OnSend(&sendpacket, sendpacket.mPacketSize, this->getKey());

}
void cUser::LoginFunc(PACKET_HEAD* _packet)
{
	cout << "LoginFunc" << endl;
	PACKET_LOGIN_REQ* recvpacket = (PACKET_LOGIN_REQ*)_packet;
	PACKET_LOGIN_ACK sendpacket;
	mLoginInfo = SINGLETON(cDBmanager).LoginCheck(recvpacket->mUserId, recvpacket->mUserPassword);

	if (mLoginInfo.mkey != 0)
	{
		SINGLETON(cUserManager).InUser(this);

		sendpacket.mUserNum = mLoginInfo.mkey;
		strcpy(sendpacket.mNickName, mLoginInfo.mNickName);
	}
	
	OnSend(&sendpacket, sendpacket.mPacketSize, this->getKey());
}
void cUser::NewUserFunc(PACKET_HEAD* _packet)
{
	cout << "NewUserFunc" << endl;
	PACKET_NEWUSER_REQ* recvpacket = (PACKET_NEWUSER_REQ*)_packet;
	PACKET_NEWUSER_ACK sendpacket;
	bool flag = SINGLETON(cDBmanager).AddNewUser(
		recvpacket->mNewUserId,
		recvpacket->mNewUserPassword,
		recvpacket->mNewNickName
	);

	 
	sendpacket.mResult = flag;

	OnSend(&sendpacket, sendpacket.mPacketSize, this->getKey());


}
 