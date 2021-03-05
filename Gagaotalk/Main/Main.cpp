#include "cInitServer.h"
#include "cSessionManager.h"
#include "cIOCPhandler.h"
#include "cThreadManager.h"
#include "cDBmanager.h"
int main()
{
	SINGLETON(cIOCPhandler).InitIOCPhandler();
	SINGLETON(cInitServer).SetInitServer();
	SINGLETON(cThreadManager).MakeThreadPool();
	SINGLETON(cSessionManager).MakeSessionObject(MAX_SESSION_COUNT);
	SINGLETON(cDBmanager).InitMysql();
	while (1)
	{

	}
 
	SINGLETON(cInitServer).CloseWDdata();

	return 0;
}