// cnt_traffic.h : main header file for the CNT_TRAFFIC application
//

#if !defined(AFX_CNT_TRAFFIC_H__020BA7B7_E2F0_4440_B97F_37F4BB39EC89__INCLUDED_)
#define AFX_CNT_TRAFFIC_H__020BA7B7_E2F0_4440_B97F_37F4BB39EC89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCnt_trafficApp:
// See cnt_traffic.cpp for the implementation of this class
//

class CCnt_trafficApp : public CWinApp
{
public:
	CCnt_trafficApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCnt_trafficApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCnt_trafficApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNT_TRAFFIC_H__020BA7B7_E2F0_4440_B97F_37F4BB39EC89__INCLUDED_)
