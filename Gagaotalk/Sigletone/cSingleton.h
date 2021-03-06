#pragma once

//--------------------------------------------------------------------------------------------------------------------------------------
//목  적 : 싱글턴 클래스 생성
//작성자 : 이원배
//일  시 : 2020. 02. 13
//--------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
#define	SINGLETON(x)	cSingleton<x>::GetInstance()

template<class T>
class cSingleton
{
public:
	static T& GetInstance()
	{
		static	T	Instance;
		return	Instance;
	}

protected:
	cSingleton() {};
	virtual ~cSingleton() {};
private:
};
