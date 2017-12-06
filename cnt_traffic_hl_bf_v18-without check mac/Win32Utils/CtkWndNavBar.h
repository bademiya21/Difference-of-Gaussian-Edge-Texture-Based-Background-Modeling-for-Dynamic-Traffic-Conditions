#if !defined(AFX_CTKWNDNAVBAR_H__5A98F944_9862_11D6_95FE_000039C0922E__INCLUDED_)
#define AFX_CTKWNDNAVBAR_H__5A98F944_9862_11D6_95FE_000039C0922E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CtkWndNavBar.h : header file
//

//#include "ctkmfc\resource.h"

/////////////////////////////////////////////////////////////////////////////
// CCtkWndNavBar window

class CCtkWndNavBar : public CWnd
{
// Construction
public:
	CCtkWndNavBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCtkWndNavBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdatePos (int pos);
	BOOL EnableWindow (BOOL enabled=TRUE);
	void SetPos (int pos);
	int GetPos () {return m_nPos;}
	void SetCount (int count);
	int GetCount () const { return m_nCount; }
	int Create(CString title, DWORD nStyle, CRect &rect, DWORD dwMessage, CWnd *p, int nID, CWnd * notify=0);
	virtual ~CCtkWndNavBar();

	CButton m_bnHome;
	CButton m_bnPrev;
	CEdit m_edPos;
	CButton m_bnNext;
	CButton m_bnEnd;
	CSliderCtrl m_scNav;
	CWnd * pWndNotify;

	// Generated message map functions
protected:
	DWORD m_dwMessage;
	int m_nCount;
	int m_nPos;

	afx_msg void OnHome (void);
	afx_msg void OnPrev (void);
	afx_msg void OnNext (void);
	afx_msg void OnEnd (void);
	afx_msg void OnChangeEditPos();
	void UpdateControls (void);

	//{{AFX_MSG(CCtkWndNavBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTKWNDNAVBAR_H__5A98F944_9862_11D6_95FE_000039C0922E__INCLUDED_)
