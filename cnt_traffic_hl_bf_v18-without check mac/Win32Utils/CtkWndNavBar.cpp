// CtkWndNavBar.cpp : implementation file
//

#include "stdafx.h"
#include "CtkWndNavBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCtkWndNavBar

CCtkWndNavBar::CCtkWndNavBar()
{
}

CCtkWndNavBar::~CCtkWndNavBar()
{
}


BEGIN_MESSAGE_MAP(CCtkWndNavBar, CWnd)
	ON_BN_CLICKED (100, OnHome)
	ON_BN_CLICKED (101, OnPrev)
	ON_EN_CHANGE  (102, OnChangeEditPos)
	ON_BN_CLICKED (103, OnNext)
	ON_BN_CLICKED (104, OnEnd)
	//{{AFX_MSG_MAP(CCtkWndNavBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCtkWndNavBar message handlers

int CCtkWndNavBar::Create(CString title, DWORD nStyle, CRect &rect, DWORD dwMessage, CWnd *p, int nID, CWnd *notify)
{
	m_dwMessage = dwMessage;
	nStyle |= WS_CLIPSIBLINGS;
	if (notify==0) pWndNotify = p;
	else pWndNotify = notify;
	return CWnd::Create (AfxRegisterWndClass (CS_DBLCLKS, LoadIcon (AfxGetInstanceHandle(),IDC_ARROW), NULL, 0), 
		title, nStyle, rect, p, nID);
}

extern AFX_EXTENSION_MODULE CtkMfcDLL; 
int CCtkWndNavBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	/*
	m_bnHome.Create (_T(""), WS_CHILD|WS_VISIBLE|BS_ICON, CRect (0,0,0,0), this, 100);
	m_bnPrev.Create (_T(""), WS_CHILD|WS_VISIBLE|BS_ICON, CRect (0,0,0,0), this, 101);
	m_edPos.Create (WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_WANTRETURN|ES_CENTER|ES_AUTOVSCROLL, CRect (0,0,0,0), this, 102);
	m_bnNext.Create (_T(""), WS_CHILD|WS_VISIBLE|BS_ICON, CRect (0,0,0,0), this, 103);
	m_bnEnd.Create (_T(""), WS_CHILD|WS_VISIBLE|BS_ICON, CRect (0,0,0,0), this, 104);
	m_scNav.Create (WS_CHILD|WS_VISIBLE, CRect (0,0,0,0), this, 105);

	m_bnHome.SetIcon (LoadIcon ((HINSTANCE)CtkMfcDLL.hModule, MAKEINTRESOURCE(IDI_ICON_NAVHOME)));
	m_bnPrev.SetIcon (LoadIcon ((HINSTANCE)CtkMfcDLL.hModule, MAKEINTRESOURCE(IDI_ICON_NAVPREV)));
	m_bnNext.SetIcon (LoadIcon ((HINSTANCE)CtkMfcDLL.hModule, MAKEINTRESOURCE(IDI_ICON_NAVNEXT)));
	m_bnEnd.SetIcon (LoadIcon ((HINSTANCE)CtkMfcDLL.hModule, MAKEINTRESOURCE(IDI_ICON_NAVEND)));
	*/

	m_bnHome.Create (_T("|<"), WS_CHILD|WS_VISIBLE, CRect (0,0,0,0), this, 100);
	m_bnPrev.Create (_T("<"), WS_CHILD|WS_VISIBLE, CRect (0,0,0,0), this, 101);
	m_edPos.Create (WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_WANTRETURN|ES_CENTER|ES_AUTOVSCROLL, CRect (0,0,0,0), this, 102);
	m_bnNext.Create (_T(">"), WS_CHILD|WS_VISIBLE, CRect (0,0,0,0), this, 103);
	m_bnEnd.Create (_T(">|"), WS_CHILD|WS_VISIBLE, CRect (0,0,0,0), this, 104);
	m_scNav.Create (WS_CHILD|WS_VISIBLE, CRect (0,0,0,0), this, 105);
	m_scNav.SetRange (0,0);
	
	m_nCount = m_nPos = 0;
	UpdateControls ();

	return 0;
}

void CCtkWndNavBar::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CRect crect;
	int w;
	int ww;
	
	GetClientRect (crect);
	w = crect.Height();
	ww = 0;	m_bnHome.MoveWindow (ww, 0, w, w, FALSE); 
	ww += w; m_bnPrev.MoveWindow (ww, 0, w, w, FALSE);
	ww += w; m_edPos.MoveWindow (ww, 0, 80, w, FALSE);
	ww += 80; m_bnNext.MoveWindow (ww, 0, w, w, FALSE);
	ww += w; m_bnEnd.MoveWindow (ww, 0, w, w, FALSE);
	ww += w; m_scNav.MoveWindow (ww, 0, crect.Width()-ww, w, TRUE);
}

void CCtkWndNavBar::SetCount(int count)
{
	m_nCount = count;
	m_nPos = -1;
	m_scNav.SetRange (0, m_nCount-1);
	SetPos (0);
}

void CCtkWndNavBar::SetPos(int pos)
{
	if (pos==m_nPos) return;
	if (pos>=m_nCount) m_nPos = m_nCount - 1;
	else if (pos<0) m_nPos = 0;
	else m_nPos = pos;
	UpdateControls ();
}

void CCtkWndNavBar::UpdateControls()
{
	char str[256];

	m_scNav.EnableWindow (m_nCount>0 && IsWindowEnabled ());
	m_edPos.EnableWindow (m_nCount>0 && IsWindowEnabled ());
	m_bnHome.EnableWindow (m_nPos>0 && m_nCount>0 && IsWindowEnabled ());
	m_bnPrev.EnableWindow (m_nPos>0 && m_nCount>0 && IsWindowEnabled ());
	m_bnNext.EnableWindow (m_nPos<m_nCount-1 && m_nCount>0 && IsWindowEnabled ());
	m_bnEnd.EnableWindow (m_nPos<m_nCount-1 && m_nCount>0 && IsWindowEnabled ());
	if (m_nCount>0) 
	{
		sprintf(str, "%d/%d", (m_nPos + 1), m_nCount);
		m_scNav.SetPos (m_nPos);
	}
	else
	{
		strcpy(str, "");
	}
	m_edPos.SetWindowText (str);
}

void CCtkWndNavBar::OnHome (void)
{
	SetPos (0);
	UpdateControls ();
	pWndNotify->PostMessage (m_dwMessage, m_nPos, 0);
}

void CCtkWndNavBar::OnPrev (void)
{
	SetPos (m_nPos-1);
	UpdateControls ();
	pWndNotify->PostMessage (m_dwMessage, m_nPos, 1);
}

void CCtkWndNavBar::OnNext (void)
{
	SetPos (m_nPos+1);
	UpdateControls ();
	pWndNotify->PostMessage (m_dwMessage, m_nPos, 3);
}

void CCtkWndNavBar::OnEnd (void)
{
 	SetPos (m_nCount-1);
 	UpdateControls ();
 	pWndNotify->PostMessage (m_dwMessage, m_nPos, 4);
}

void CCtkWndNavBar::OnChangeEditPos() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString str;
	int nPos;
	char Esc[] = {'\27', 0};
	m_edPos.GetWindowText (str);
	int count = m_edPos.GetLineCount ();
	if (str.Find (Esc)!=-1)
	{
		m_edPos.SetWindowText (str);
		return;
	}
	if (str.FindOneOf ("\n\t")==-1) return;
	str.Remove ('\n');
	str.Remove ('\t');
	nPos = atoi (str) - 1;
	SetPos (nPos);
	UpdateControls ();
	pWndNotify->PostMessage (m_dwMessage, m_nPos, 2);
}

void CCtkWndNavBar::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if ((CScrollBar *)&m_scNav == pScrollBar)
	{
		int pos = m_scNav.GetPos ();
		if (pos!=m_nPos)
		{
			SetPos (pos);
			UpdateControls ();
			pWndNotify->PostMessage (m_dwMessage, m_nPos, 5);
		}
		return;
	}

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CCtkWndNavBar::EnableWindow(BOOL enabled)
{
	BOOL b;
	b = CWnd::EnableWindow (enabled);
	UpdateControls ();
	return b;
}

void CCtkWndNavBar::UpdatePos(int pos)
{
//	m_scNav.SetPos (pos);
	SetPos (pos);
	UpdateControls ();
}
