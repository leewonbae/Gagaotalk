#pragma once
#include "Resource.h"
#include "cUser.h"
#include "cCriticalSection.h"
#include "Packet.h"

class cUserManager : public cSingleton< cUserManager>
{
	friend class cSingleton< cUserManager>;
public:
	void InUser(cUser* _user);
	void OutUser(cUser* _user);

	cUser* ConnectCheck(int _userkey);
	// Ű�� �������� ���̺� �ʿ� 

private:
	cUserManager();
	~cUserManager();
	
	cCriticalSection mCS_UserArra;
	map<int, cUser*> mUserArray;

};

