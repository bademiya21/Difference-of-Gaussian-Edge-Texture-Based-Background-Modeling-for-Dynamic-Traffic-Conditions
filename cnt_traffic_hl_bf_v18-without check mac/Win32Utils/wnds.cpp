#include "stdafx.h"
#include "Wnds.h"

/////////////////////////////////////////////////////////////////////////////
// CWndDib

unsigned char CWndDib::clip [1024];
unsigned char * CWndDib::clp = 0;
long int CWndDib::crv_tab[256];
long int CWndDib::cbu_tab[256];
long int CWndDib::cgu_tab[256];

long int CWndDib::cgv_tab[256];
long int CWndDib::tab_76309[256];

void CWndDib::init_dither_tab ()
{
  long int crv, cbu, cgu, cgv;
  int i;

  crv = 104597;
  cbu = 132201;                 /* fra matrise i global.h */
  cgu = 25675;
  cgv = 53279;

  for (i = 0; i < 256; i++)
  {
    crv_tab[i] = (i - 128) * crv;
    cbu_tab[i] = (i - 128) * cbu;
    cgu_tab[i] = (i - 128) * cgu;
    cgv_tab[i] = (i - 128) * cgv;
    tab_76309[i] = 76309 * (i - 16);
  }
}

CWndDib::CWndDib()
{
	width = 0;
	height = 0;
	buffer = 0;
	handle = CreateEvent (NULL, FALSE, TRUE, NULL);
}

CWndDib::~CWndDib()
{
	if (buffer) delete [] buffer;
	CloseHandle (handle);
}


BEGIN_MESSAGE_MAP(CWndDib, CWnd)
	//{{AFX_MSG_MAP(CWndDib)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWndDib message handlers

void CWndDib::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect crect;
	CDC * pDC = GetDC ();
	CDC memDC;
	CBitmap bmp;
	GetClientRect (crect);
	WaitForSingleObject (handle, INFINITE);
//	WaitForSingleObject (handle, 15000);
	if (buffer)
	{
//		BITMAPINFOHEADER bmih;
		BOOL b = DrawDibBegin (hdibdraw, pDC->m_hDC, crect.Width(), crect.Height(), &bmih, width, height, DDF_UPDATE);
		DrawDibDraw (hdibdraw, pDC->m_hDC, 0, 0, crect.Width(), crect.Height(), &bmih,
			buffer, 0, 0, width, height, DDF_SAME_DRAW);
	}
	else
	{
		memDC.CreateCompatibleDC (pDC);
		bmp.CreateCompatibleBitmap (&memDC, crect.Width(), crect.Height());
		memDC.SelectObject (&bmp);
		memDC.FillSolidRect (0,0,crect.Width(),crect.Height(),RGB(0,0,0,));
		pDC->BitBlt (0, 0, crect.Width(), crect.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.DeleteDC();
		bmp.DeleteObject();
	}
	SetEvent (handle);
	ReleaseDC (pDC);

	// Do not call CWnd::OnPaint() for painting messages
}

void CWndDib::UpdateDisplay(int w, int h, char *p)
{
	UpdateDisplay (w, h, p, p+w*h, p+w*h*5/4);
}

void CWndDib::UpdateDisplayRGB (int w, int h, char *rgb)
{
	int a;
	a = w * h * 3;
	WaitForSingleObject (handle, INFINITE);
//	WaitForSingleObject (handle, 15000);
	if (width!=w || height!=h)
	{
		if (buffer) delete [] buffer;
		width = w;
		height = h;
		buffer = new char [a];
		memset (&bmih, 0, sizeof (BITMAPINFOHEADER));
		bmih.biWidth = width;
		bmih.biHeight = height;
		bmih.biSize = sizeof (BITMAPINFOHEADER);
//		bmih.biCompression = ('I'<<0)+('4'<<8)+('2'<<16)+('0'<<24);
		bmih.biCompression = BI_RGB;
		bmih.biPlanes = 1;
		bmih.biBitCount = 24;
		bmih.biSizeImage = width * height * 3;
	}
	memcpy (buffer, rgb, bmih.biSizeImage);
	SetEvent (handle);
	Invalidate();
}

void CWndDib::UpdateDisplay(int w, int h, char *y, char *u, char *v)
{
	int a;
	a = w * h;
	WaitForSingleObject (handle, INFINITE);
//	WaitForSingleObject (handle, 15000);
	if (width!=w || height!=h)
	{
		if (buffer) delete [] buffer;
		width = w;
		height = h;
		buffer = new char [a*3];
		memset (&bmih, 0, sizeof (BITMAPINFOHEADER));
		bmih.biWidth = width;
		bmih.biHeight = height;
		bmih.biSize = sizeof (BITMAPINFOHEADER);
//		bmih.biCompression = ('I'<<0)+('4'<<8)+('2'<<16)+('0'<<24);
		bmih.biCompression = BI_RGB;
		bmih.biPlanes = 1;
		bmih.biBitCount = 24;
		bmih.biSizeImage = width * height * 3;
	}
	ConvertYUVtoRGB ((unsigned char *)y, (unsigned char *)u, (unsigned char *)v, (unsigned char *)buffer, width, height);
	SetEvent (handle);
	Invalidate ();
}

void CWndDib::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	DrawDibClose(hdibdraw);
}

int CWndDib::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	hdibdraw = DrawDibOpen ();
	
	return 0;
}

void CWndDib::ConvertYUVtoRGB (
  unsigned char *src0, 
  unsigned char *src1,
  unsigned char *src2,
  unsigned char *dst_ori,
  int width, 
  int height)
{

	if (!clp)
	{
		clp = clip + 384;
		int i;
		for (i = -384; i < 640; i++)
		    clp[i] = (i < 0) ? 0 : ((i > 255) ? 255 : i);
		init_dither_tab ();
	}

  int y11, y21;
  int y12, y22;
  int y13, y23;
  int y14, y24;
  int u, v;
  int i, j;
  int c11, c21, c31, c41;
  int c12, c22, c32, c42;
  unsigned int DW;
  unsigned int *id1, *id2;
  unsigned char *py1, *py2, *pu, *pv;
  unsigned char *d1, *d2;

  d1 = dst_ori;
  d1 += width * height * 3 - width * 3;
  d2 = d1 - width * 3;

  py1 = src0;
  pu = src1;
  pv = src2;
  py2 = py1 + width;

  id1 = (unsigned int *) d1;
  id2 = (unsigned int *) d2;

  for (j = 0; j < height; j += 2)
  {
    /* line j + 0 */
    for (i = 0; i < width; i += 4)
    {
      u = *pu++;
      v = *pv++;
      c11 = crv_tab[v];
      c21 = cgu_tab[u];
      c31 = cgv_tab[v];
      c41 = cbu_tab[u];
      u = *pu++;
      v = *pv++;
      c12 = crv_tab[v];
      c22 = cgu_tab[u];
      c32 = cgv_tab[v];
      c42 = cbu_tab[u];

      y11 = tab_76309[*py1++];  /* (255/219)*65536 */
      y12 = tab_76309[*py1++];
      y13 = tab_76309[*py1++];  /* (255/219)*65536 */
      y14 = tab_76309[*py1++];

      y21 = tab_76309[*py2++];
      y22 = tab_76309[*py2++];
      y23 = tab_76309[*py2++];
      y24 = tab_76309[*py2++];

      /* RGBR */
      DW = ((clp[(y11 + c41) >> 16])) |
        ((clp[(y11 - c21 - c31) >> 16]) << 8) |
        ((clp[(y11 + c11) >> 16]) << 16) |
        ((clp[(y12 + c41) >> 16]) << 24);
      *id1++ = DW;

      /* GBRG */
      DW = ((clp[(y12 - c21 - c31) >> 16])) |
        ((clp[(y12 + c11) >> 16]) << 8) |
        ((clp[(y13 + c42) >> 16]) << 16) |
        ((clp[(y13 - c22 - c32) >> 16]) << 24);
      *id1++ = DW;

      /* BRGB */
      DW = ((clp[(y13 + c12) >> 16])) |
        ((clp[(y14 + c42) >> 16]) << 8) |
        ((clp[(y14 - c22 - c32) >> 16]) << 16) |
        ((clp[(y14 + c12) >> 16]) << 24);
      *id1++ = DW;

      /* RGBR */
      DW = ((clp[(y21 + c41) >> 16])) |
        ((clp[(y21 - c21 - c31) >> 16]) << 8) |
        ((clp[(y21 + c11) >> 16]) << 16) |
        ((clp[(y22 + c41) >> 16]) << 24);
      *id2++ = DW;

      /* GBRG */
      DW = ((clp[(y22 - c21 - c31) >> 16])) |
        ((clp[(y22 + c11) >> 16]) << 8) |
        ((clp[(y23 + c42) >> 16]) << 16) |
        ((clp[(y23 - c22 - c32) >> 16]) << 24);
      *id2++ = DW;

      /* BRGB */
      DW = ((clp[(y23 + c12) >> 16])) |
        ((clp[(y24 + c42) >> 16]) << 8) |
        ((clp[(y24 - c22 - c32) >> 16]) << 16) |
        ((clp[(y24 + c12) >> 16]) << 24);
      *id2++ = DW;
    }
    id1 -= (9 * width) >> 2;
    id2 -= (9 * width) >> 2;
    py1 += width;
    py2 += width;
  }
}

