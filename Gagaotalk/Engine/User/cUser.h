#pragma once
#include "Resource.h"
#include "cSession.h"
class cUser : public cSession
{
public:
	cUser();
	~cUser();

	int getUsernum() { return mLoginInfo.mkey; }

	void Parsing(PACKET_HEAD* _packet);

	
	typedef void (cUser::* fp)(PACKET_HEAD* _packet);

private:
	void LoginFunc(PACKET_HEAD* _packet);
	void NewUserFunc(PACKET_HEAD* _packet);
	void FriendListFunc(PACKET_HEAD* _packet);
	void AddFriendFunc(PACKET_HEAD* _packet);
	void FindFriendFunc(PACKET_HEAD* _packet);
	void OfflineDataFunc(PACKET_HEAD* _packet);
	void OnlineDataFunc(PACKET_HEAD* _packet);

	map<int, fp> mMap;
	map<int, fp>::iterator mIter;

	LOGIN_INFO mLoginInfo;
};

