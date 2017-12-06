// CtkSizer.h: interface for the CCtkSizer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTKSIZER_H__1779D9B6_48CF_11D6_95FE_00022D122810__INCLUDED_)
#define AFX_CTKSIZER_H__1779D9B6_48CF_11D6_95FE_00022D122810__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxwin.h>


//===================================================================
//	Class Name:
//		CtkSizer
//	Description:
//		A class which encapsulate the functionality of controlling
//		the minimum size of a Window.
//	Usage:
//		1.  In OnCreate, call SetMinimumSize to set the minimum size
//		    of a resizable window.
//		2.  In OnSizing, call AdjustSize with the same parameters of 
//			OnSizing ().
//===================================================================

class CtkSizer  
{
public:
	CtkSizer();
	virtual ~CtkSizer();
	void AdjustSize (unsigned fwSide, LPRECT rect);
	void SetMinimumSize (int min_width, int min_height);

protected:
	int m_nMinClientWidth;
	int m_nMinClientHeight;
};

#endif // !defined(AFX_CTKSIZER_H__1779D9B6_48CF_11D6_95FE_00022D122810__INCLUDED_)
