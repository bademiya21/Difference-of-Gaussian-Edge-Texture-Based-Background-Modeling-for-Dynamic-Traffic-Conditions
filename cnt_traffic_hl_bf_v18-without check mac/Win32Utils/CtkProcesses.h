#ifndef __CTK_PROCESSES_H__
#define __CTK_PROCESSES_H__

#include "stdafx.h"


//===================================================================
//	Abstract Class Name:
//		CtkThread
//	Description:
//		A class which encapsualte a thread in multithreading 
//		environment.
//	Note:
//		Class is not used by programmer, used to derive:
//		classes CtkWorkerThread and CtkMessageThread only
//===================================================================

class CtkThread
{
public:
	CtkThread () { m_hThread = NULL; }
	virtual ~CtkThread () { Stop (); }
	void Stop (void);
	bool IsRunning (void) const { return m_hThread!=NULL; }
	virtual bool Start (void * params) = 0;
	virtual bool OnBegin (void * params) = 0;

protected:
	static DWORD FunctionLoopThread (void * dummy);
	static DWORD FunctionMsgThread (void * dummy);
	DWORD  m_dwThread;
	HANDLE m_hThread;
};


//===================================================================
//	Abstract Class Name:
//		CtkWorkerThread
//	Bases Class(es):
//		CtkThread
//	Description:
//		A class which encapsualte a worker thread in multithreading 
//		environment. The threads keeps looping and calling OnLoop.
//	Derivation Steps:
//		1.  Override the OnLoop function with the loop function.
//		2.  Override the OnBegin function for initialization.
//	Usage:
//		1.  Call Start to start the thread.
//		2.  The OnLoop function will be run continuously.
//		3.  Call Stop to stop the thread.
//===================================================================

class CtkWorkerThread : public CtkThread
{
public:
	virtual bool Start (void * params);
	virtual bool OnLoop (void) = 0;

protected:
	static DWORD FunctionLoopThread (void * dummy);
};


//===================================================================
//	Abstract Class Name:
//		CtkMessageThread
//	Bases Class(es):
//		CtkThread
//	Description:
//		A class which encapsualte a thread with message pump
//		in multithreading environment and stalls until a message
//		is received.
//	Derivation Steps:
//		1.  Override the OnMessage function to perform desired
//		    message-driven tasks.
//		2.  Override the OnBegin function for initialization.
//	Usage:
//		1.  Call Start to start the thread.
//		2.  Call Message to send some messages to the thread.
//			Usage values between 100 and 1000 as first parameter.
//		3.  Call Stop to stop the thread.
//===================================================================

class CtkMessageThread : public CtkThread
{
public:
	virtual bool Start (void * params);
	virtual bool OnMessage (UINT message, WPARAM wParam, LPARAM lParam) = 0;
	virtual void Message (UINT message, WPARAM wParam = 0, LPARAM lParam = 0);

protected:
	static DWORD FunctionMsgThread (void * dummy);
};


//===================================================================
//	Class Name:
//		CtkSync
//	Description:
//		A class allowing two or more threads to synchronize operations
//		like accessing common hardware.
//	Usage:
//		1.  In a thread call Wait to wait for sync signal to be free.
//		    The thread will stall until the signal is free.
//		2.  On Wait return, do something and REMEMBER to call Release.
//===================================================================

class CtkSync
{
public:
	CtkSync() 
	{ 
		hMutex = CreateMutex(NULL,              // no security attributes
							 FALSE,				// initially not owned
							 NULL);				// name of mutex
	}
	virtual ~CtkSync(void) 
	{ 
		CloseHandle (hMutex); 
	}
	void Wait(void) 
	{ 
		WaitForSingleObject (hMutex, INFINITE); 
//		WaitForSingleObject (hMutex, 15000); 
	}
	void Release(void)
	{ 
		ReleaseMutex(hMutex);
	}

protected:
	HANDLE hMutex; 
};

/*
class CtkSync
{
public:
	CtkSync () { m_hEvent = CreateEvent (NULL, FALSE, TRUE, NULL); }
	virtual ~CtkSync (void) { CloseHandle (m_hEvent); }
	void Wait (void) 
	{ 
		WaitForSingleObject (m_hEvent, INFINITE); 
	}
	void Release (void) { SetEvent (m_hEvent); }

protected:
	HANDLE m_hEvent;
};
*/

//===================================================================
//	Class Name:
//		CtkSignal
//	Description:
//		A class allowing two or more threads to signal to another.
//	Usage:
//		1.  In a thread call Set to set the event.
//		2.  In another thread, call IsSet to wait for signal (with optional 
//			time-out).
//		3.  If IsSet returns true, do something desired.
//		4.  Call Reset when the second thread finished the job.
//		5.  Optionally, the first thread can call IsSet to determine
//			the signal have already been set.
//===================================================================

class CtkSignal
{
public:
	CtkSignal () { m_hEvent = CreateEvent (NULL, TRUE, FALSE, NULL); }
	virtual ~CtkSignal () { CloseHandle (m_hEvent); }
	void Set (void) { SetEvent (m_hEvent); }
	bool IsSet (unsigned int wait_ms) 
	{ return WaitForSingleObject (m_hEvent, wait_ms) == WAIT_OBJECT_0; }
	void Reset (void) { ResetEvent (m_hEvent); }

protected:
	HANDLE m_hEvent;
};

#endif//__CTK_PROCESSES_H__