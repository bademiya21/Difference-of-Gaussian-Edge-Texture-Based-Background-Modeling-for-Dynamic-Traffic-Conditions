// CtkSizer.cpp: implementation of the CCtkSizer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CtkSizer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CtkSizer::CtkSizer()
{
	SetMinimumSize (320, 240);
}

CtkSizer::~CtkSizer()
{
}

void CtkSizer::SetMinimumSize(int min_width, int min_height)
{
	m_nMinClientWidth = min_width;
	m_nMinClientHeight = min_height;
}

void CtkSizer::AdjustSize (UINT fwSide, LPRECT rect)
{
	if ((rect->right-rect->left) < m_nMinClientWidth)
	{
		if (fwSide == WMSZ_LEFT || fwSide == WMSZ_TOPLEFT || fwSide == WMSZ_BOTTOMLEFT) rect->left = rect->right - m_nMinClientWidth;
		else if (fwSide == WMSZ_RIGHT || fwSide == WMSZ_TOPRIGHT || fwSide == WMSZ_BOTTOMRIGHT) rect->right = rect->left + m_nMinClientWidth;
		else
		{
			rect->left += ((m_nMinClientWidth-rect->right+rect->left)/2);
			rect->right = rect->left + m_nMinClientWidth;
		}
	}
	if ((rect->bottom-rect->top) < m_nMinClientHeight)
	{
		if (fwSide == WMSZ_TOP || fwSide == WMSZ_TOPLEFT || fwSide == WMSZ_TOPRIGHT) rect->top = rect->bottom - m_nMinClientHeight;
		else if (fwSide == WMSZ_BOTTOM || fwSide == WMSZ_BOTTOMLEFT || fwSide == WMSZ_BOTTOMRIGHT) rect->bottom = rect->top + m_nMinClientHeight;
		else
		{
			rect->top += ((m_nMinClientHeight-rect->bottom+rect->top)/2);
			rect->bottom = rect->top + m_nMinClientHeight;
		}
	}
}
