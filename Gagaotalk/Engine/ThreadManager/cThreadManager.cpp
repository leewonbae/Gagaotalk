#include "cThreadManager.h"
#include "cIOCPhandler.h"
#include "cSessionManager.h"
#include "cUserManager.h"
cThreadManager::cThreadManager()
{

}
cThreadManager::~cThreadManager()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------
//��  �� : ���μ��� ���� *2 ��ŭ ������ ����
//�ۼ��� : �̿���
//��  �� : 2020. 02. 15
//--------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------

void cThreadManager::MakeThreadPool()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	mMakeThreadEvent = CreateEvent(NULL, false, false, 0);
	for (int i = 0;i < (int)si.dwNumberOfProcessors * 2;i++)
	{
		HANDLE thread = CreateThread(NULL, 0, WorkerThread, this, 0, NULL);
		mThreadArray.push_back(thread);
		 
		HANDLE hevent = CreateEvent(NULL, true, false, 0);
		mEventArray[i] = hevent;

		WaitForSingleObject(mMakeThreadEvent, INFINITE);
		mThreadCount++;
	}
	cout << "[thread pool ok]" << endl;
}


//--------------------------------------------------------------------------------------------------------------------------------------
//��  �� : �۾�  ������ 
//�ۼ��� : �̿���
//��  �� : 2020. 02. 15
//--------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------
DWORD WINAPI cThreadManager::WorkerThread(LPVOID arg)
{
	cThreadManager* manager = (cThreadManager*)arg;
	//������ ������ �̺�Ʈ 
	SetEvent(manager->mMakeThreadEvent);
	
	DWORD transbytes, retval;
	cSession* ptr_session =	NULL;
	OVERDATA* ptr_over = NULL;
 
	while (1)
	{
		retval = GetQueuedCompletionStatus(SINGLETON(cIOCPhandler).getIOCPhandle(), &transbytes,
			(PULONG_PTR)&ptr_session, (LPOVERLAPPED*)&ptr_over,INFINITE);
 
		int last_error = WSAGetLastError();
		clock_t time_start = clock();
		 
		if (retval == true && transbytes == 0 && ptr_session == NULL)// Ŭ���̾�Ʈ ���� ó�� 
		{
			 
			if (ptr_over->mWorkFleg == WORKFLEG_ACCPET)
			{
				ptr_session = (cSession*)ptr_over->SelfPoint;
				SINGLETON(cSessionManager).InputNewSession(ptr_session);
				ptr_session->ShowClientInfo();
			}
			
		}
		else if (retval == true && transbytes != 0)//�۾� �з�üũ 
		{
			
			switch (ptr_over->mWorkFleg)
			{
			case WORKFLEG_RECV:
				printf("[ OVERDATA  ] = RECV ,  ");
				ptr_session->Flag_Recv(ptr_over);
				break;

			case WORKFLEG_SEND:

				ptr_session->Flag_Send(ptr_over);
				break;
			}

			if (ptr_over->mWorkFleg == WORKFLEG_RECV)
			{
				clock_t time_end = clock();
				clock_t elapsed = time_end - time_start;
				printf("[  Elapsed ]  =  %d ms \n", elapsed);
			}

			continue;
		}
		 
		else if (retval == false && transbytes == 0 && ptr_session != NULL)// ���� ����
		{
			ptr_session->RecycleCloseSocket();
			ptr_session->ShowClientInfo();
			SINGLETON(cSessionManager).OutSession();
		}
		else if (retval == true && transbytes == 0 && ptr_session != NULL)// �Ϲ� ����
		{
			if (ptr_session->GetUserOrSession() == true)
			{
				SINGLETON(cUserManager).OutUser((cUser*)ptr_session);
			}
			ptr_session->RecycleCloseSocket();
			ptr_session->ShowClientInfo();
			SINGLETON(cSessionManager).OutSession();
		}
		
		 

		
	
		  
	}

	
	return 0;
}