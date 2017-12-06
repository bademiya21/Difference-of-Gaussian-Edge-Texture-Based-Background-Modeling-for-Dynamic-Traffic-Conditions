#include "CtkProcesses.h"


//=======================================================
//	CtkThread
//=======================================================

void CtkThread::Stop (void)
{
	if (IsRunning())
	{
		PostThreadMessage (m_dwThread, WM_QUIT, 0, 0);
//		WaitForSingleObject (m_hThread, INFINITE);
		WaitForSingleObject (m_hThread, 15000);
		CloseHandle (m_hThread);
		m_hThread = NULL;
	}
}


//=======================================================
//	CtkWorkerThread
//=======================================================

DWORD CtkWorkerThread::FunctionLoopThread (void * dummy)
{
	MSG msg;
	CtkWorkerThread * p = (CtkWorkerThread *) dummy;
	for (;;)
	{
		if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
			if (msg.message==WM_QUIT) 
				break;
		if (!p->OnLoop ())
		{
			CloseHandle (p->m_hThread);
			p->m_hThread = NULL;
			break;
		}
	}
	return 0;
}

bool CtkWorkerThread::Start (void * dummy)
{
	if (IsRunning ()) return false;
	if (!OnBegin (dummy)) return false;
	m_hThread = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) FunctionLoopThread, 
			this, NULL, &m_dwThread);
	return m_hThread!=NULL;
}


//=======================================================
//	CtkMessageThread
//=======================================================

DWORD CtkMessageThread::FunctionMsgThread (void * dummy)
{
	MSG msg;
	CtkMessageThread * p = (CtkMessageThread *) dummy;
	while (GetMessage (&msg, NULL, 0, 0xffffffff))
	{
		p->OnMessage (msg.message, msg.wParam, msg.lParam);
	}
	return 0;
}

bool CtkMessageThread::Start (void * dummy)
{
	if (IsRunning ()) return false;
	if (!OnBegin (dummy)) return false;
	m_hThread = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) FunctionMsgThread, 
		this, NULL, &m_dwThread);
	return m_hThread!=NULL;
}

void CtkMessageThread::Message (UINT message, WPARAM wParam, LPARAM lParam)
{
	PostThreadMessage (m_dwThread, message, wParam, lParam);
}

