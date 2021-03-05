#include "cUserManager.h"
cUserManager::cUserManager()
{

}
cUserManager::~cUserManager()
{

}
void cUserManager::InUser(cUser* _user)
{
	mCS_UserArra.LockCS();

	mUserArray.insert(make_pair(_user->getUsernum(), _user));
	_user->SetUserOrSession(true);
	mCS_UserArra.UnlockCS();
}
void cUserManager::OutUser(cUser* _user)
{
	map<int, cUser*>::iterator iter;
	iter = mUserArray.find(_user->getUsernum());
	_user->SetUserOrSession(false);

	mCS_UserArra.LockCS();

	mUserArray.erase(iter);

	mCS_UserArra.UnlockCS();
}

cUser* cUserManager::ConnectCheck(int _userkey)
{
	map<int, cUser*>::iterator iter;
	iter = mUserArray.find(_userkey);

	if (iter == mUserArray.end())
	{
		return NULL;
	}
	else
	{
		cUser* temp = iter->second;
		return temp;
	}
 
}