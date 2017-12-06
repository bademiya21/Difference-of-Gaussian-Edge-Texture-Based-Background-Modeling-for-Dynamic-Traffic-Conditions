#if !defined(AFX_CALIBRATION_H__5310E84A_17EA_4505_87EC_1A859AB1BC62__INCLUDED_)
#define AFX_CALIBRATION_H__5310E84A_17EA_4505_87EC_1A859AB1BC62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Calibration.h : header file
//
#include "..\ximage\ximage.h"

/////////////////////////////////////////////////////////////////////////////
// CCalibration dialog

class CCalibration : public CDialog
{
// Construction
public:
	CCalibration(CWnd* pParent = NULL);   // standard constructor
	~CCalibration();
	CxImage* gui;
	CPoint point1[4];

// Dialog Data
	//{{AFX_DATA(CCalibration)
	enum { IDD = IDD_CALIBRATION };
	CListCtrl	m_pointListCtrl;
	CButton	m_buttonOk;
	CButton	m_buttonCancel;
	CStatic	m_stInput1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalibration)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CCalibration)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int width;
	int height;
	CxImage *guiDraw;
	char coordinate[100];
	HDC	hdc;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALIBRATION_H__5310E84A_17EA_4505_87EC_1A859AB1BC62__INCLUDED_)
