// Calibration.cpp : implementation file
//

#include "stdafx.h"
#include "cnt_traffic.h"
#include "Calibration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalibration dialog


CCalibration::CCalibration(CWnd* pParent /*=NULL*/)
	: CDialog(CCalibration::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalibration)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	guiDraw = new CxImage;
}

CCalibration::~CCalibration()
{
	if (guiDraw) delete guiDraw;
}

void CCalibration::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalibration)
	DDX_Control(pDX, IDC_LIST_POINT, m_pointListCtrl);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDC_STATIC_INPUT1, m_stInput1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalibration, CDialog)
	//{{AFX_MSG_MAP(CCalibration)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalibration message handlers

BOOL CCalibration::OnInitDialog() 
{
	CDialog::OnInitDialog();

	for (int i=0;i<4;i++)
	{
		point1[i]=CPoint(0,0);
	}
	
	// TODO: Add extra initialization here
	m_pointListCtrl.InsertColumn  (0, "Corner");
	m_pointListCtrl.SetColumnWidth(0, 70);
	m_pointListCtrl.InsertColumn  (1, "Point");
	m_pointListCtrl.SetColumnWidth(1, 70);
	m_pointListCtrl.InsertItem(1,"Bottom-left");
	m_pointListCtrl.InsertItem(2,"Top-left");
	m_pointListCtrl.InsertItem(3,"Top-right");
	m_pointListCtrl.InsertItem(4,"Bottom-right");

	width=gui->GetWidth();
	height=gui->GetHeight();

	guiDraw->Create(width,height,8,0);
	hdc=m_stInput1.GetDC()->m_hDC;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCalibration::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int selectedItem=m_pointListCtrl.GetSelectionMark();

	if (selectedItem<0 || selectedItem>4)
		return;
	point1[selectedItem].x = (point.x - 12)*2;
	point1[selectedItem].y = height-((point.y - 12)*2);
	
	sprintf(coordinate, "%s%d%s%d%s", "(",point1[selectedItem].x,",",point1[selectedItem].y,")");
	m_pointListCtrl.SetItemText(m_pointListCtrl.GetSelectionMark(),1,coordinate);
	
	if ((point1[selectedItem].x<0) || (point1[selectedItem].x>width) || (point1[selectedItem].y<0) || (point1[selectedItem].y>height))
	{
		MessageBox("Point is out of boundary!");
		return;
	}
	guiDraw->Copy(*gui,true,true,true);
	for(int i=0;i<4;i++)
	{
		guiDraw->DrawLine(point1[i].x-10,point1[i].x+10,point1[i].y-10,point1[i].y+10,RGB(255,0,0));
		guiDraw->DrawLine(point1[i].x-10,point1[i].x+10,point1[i].y+10,point1[i].y-10,RGB(255,0,0));
	}

	guiDraw->Draw(hdc,0,0,width/2,height/2,0,0);

	CDialog::OnLButtonDown(nFlags, point);
}


void CCalibration::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	gui->Draw(hdc,0,0,width/2,height/2,0,0);
	
	// Do not call CDialog::OnPaint() for painting messages
}
