#ifndef __WNDS_H__
#define __WNDS_H__

#include <afxwin.h>
#include <vfw.h>
#pragma comment(lib,"vfw32.lib")

#define VU_DLL_EXPORT __declspec(dllexport)

//===================================================================
//	Class Name:
//		CWndDib
//	Base Class(es):
//		CWnd
//	Description:
//		A class to display a raw bitmap in RGB24 or YCbCr formats.
//	Usage:
//		1.  In OnCreate, call Create to create the window and set 
//		    initial size and parent.
//		2.  Call UpdateDisplayRGB to set RGB24 frame.
//		3.  Call UpdateDisplay to set YCbCr frame.
//===================================================================

class CWndDib : public CWnd
{
// Construction
public:
	CWndDib();

// Attributes
public:

// Operations
public:
	BITMAPINFOHEADER bmih;
	HDRAWDIB hdibdraw;
	int width;
	int height;
	char * buffer;
	HANDLE handle;

	static unsigned char * clp;
	static unsigned char clip[1024];
	static long int crv_tab[256];
	static long int cbu_tab[256];
	static long int cgu_tab[256];

	static long int cgv_tab[256];
	static long int tab_76309[256];

	static void init_dither_tab ();
	static void ConvertYUVtoRGB (unsigned char *src0, unsigned char *src1, unsigned char *src2,
		unsigned char *dst_ori, int width, int height);

	virtual BOOL Create (LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL)
	{
		return CWnd::Create (AfxRegisterWndClass(CS_DBLCLKS, LoadCursor(0, IDC_CROSS), 0, 0), lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWndDib)
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateDisplayRGB (int w, int h, char *rgb);
	void UpdateDisplay (int w, int h, char * yuv);
	void UpdateDisplay(int w, int h, char *y, char *u, char *v);
	virtual ~CWndDib();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWndDib)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //__WNDS_H__ 