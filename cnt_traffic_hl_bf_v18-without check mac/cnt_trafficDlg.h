// cnt_trafficDlg.h : header file
//
//{{AFX_INCLUDES()
#include "axismediacontrol.h"
//}}AFX_INCLUDES

#if !defined(AFX_CNT_TRAFFICDLG_H__708002B8_2AEA_44D8_8156_88A1335C2F42__INCLUDED_)
#define AFX_CNT_TRAFFICDLG_H__708002B8_2AEA_44D8_8156_88A1335C2F42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCnt_trafficDlg dialog
//#include "win32utils\wnds.h"	

#include "stdafx.h"
#include "resource.h"
#include "Calibration.h"
#include "time.h"
#include <iostream>
#include <iomanip>
#include "tracking.h"
//#include "AuthorizeMgr.h"
//#include "axismediacontrol.h"
#include <urlmon.h>
#include <Wininet.h>
#include <limits.h>
#include <float.h>
#include <math.h>

#define maskSize 7
#define sigma 1.5
using namespace std;

#define MIN_DIALOG_HEIGHT	500
#define MIN_DIALOG_WIDTH	700
#define MAX_MESSAGE_SIZE	1000
#define MB_SIZE						(1024 * 1024)
#define	DELETE_COUNT				999

/* Compare a value pointed to by 'ptr' to the 'centre' value and
 * increment pointer. Comparison is made by masking the most
 * significant bit of an integer (the sign) and shifting it to an
 * appropriate position. */
#define compab_mask_inc(ptr,shift) { value |= ((unsigned int)(*centre - *ptr - 1) & 0x80000000) >> (31-shift);ptr++; }
/* Compare a value 'val' to the 'centre' value. */
//#define compab_mask(val,shift) { value |= ((unsigned int)(*centre - (val) - 1) & 0x80000000) >> (31-shift); }
/* Predicate 2 for the 5x5 neighborhood */
#define predicate 2
/* The number of bits */
#define bits 6

typedef struct
{
	CDialog	*pDlg;
} THREAD_PARAM;

/*typedef struct
{
	int x,y;
} integerpoint;

typedef struct
{
	float x,y;
} floatpoint;*/

typedef BOOL (CALLBACK* LPFNGETSPACEEX) (LPCTSTR,
										 PULARGE_INTEGER,
										 PULARGE_INTEGER,
										 PULARGE_INTEGER);

typedef BOOL (CALLBACK* LPFNGETSPACE) (LPCTSTR,
									   LPDWORD,
									   LPDWORD,
									   LPDWORD,
									   LPDWORD);


class CCnt_trafficDlg : public CDialog
{
// Construction
public:
	CCnt_trafficDlg(CWnd* pParent = NULL);	// standard constructor
	~CCnt_trafficDlg();
	int len;
	int N_CLUSTER;
	THREAD_PARAM m_threadParam;
	bool m_threadrun;

// Dialog Data
	//{{AFX_DATA(CCnt_trafficDlg)
	enum { IDD = IDD_CNT_TRAFFIC_DIALOG };
	CStatic	m_curFrame;
	CEdit	m_edit;
	CButton	m_buttonChange;
	CButton	m_buttonStop;
	CButton	m_buttonLive;
	CListCtrl	m_countListCtrl;
	CButton	m_buttonSelectPoint;
	CStatic	m_stInput;
	CButton	m_buttonrun;
	CButton	m_buttonopen;
	CAxisMediaControl	m_AMC;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCnt_trafficDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCnt_trafficDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonOpen();
	afx_msg void OnButtonRun();
	afx_msg void OnButtonSelectPoint();
	afx_msg void OnButtonLive();
	afx_msg void OnButtonStop();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	
	bool		SelectPath();
	//void		gaussian_smoothing(BYTE *pixelIndex);
	//void		gradient(BYTE *pixelIndex);
	bool		cca();
	void		split_blob();
	void		hkmean(CvPoint* C, CvPoint *newC);
	bool		addNewC(CvPoint* prevC, CvPoint *C);
	void		update_hk_C(CvPoint *C, CvPoint *newC);
	float		comp_hk_error(CvPoint *C, CvPoint *newC);
	void		fit_kernel(CvPoint *C);
	void		drawBoundingBox();
	void		findCand_vehicles();
	void		get_kernel(int ID, int *bbox_kernel);
	void		update_kernel(int *bbox_kernel, int *new_bbox_kernel);
	int			get_overlap_area(int *bbox_ii,int *bbox_k);
	void		drawBox(int *Rect);
	bool		chk_err(int *bbox1, int *bbox2);
	void		merge_bbox(int *bbox_kernel, int *new_bbox);
	void		save_new_record(int **new_bbox);
	void		drawBoundingBox4();
	void		cleanUp();
	void		merge_process2();
	int			get_bbox_size(int *bbox_ii);
	void		determineType();
	void		delete_blobs();
	void		readROIfile();
	void		getParams();
	void		CreateFolder();
	void		WriteLog(const char *message);
	void		WriteFile(const char *filename, const char *timeStamp, const char *message);
	void		setCapturePath();
	void		updateUpTime();
	void		WriteStats(const char *message);
	BOOL		getFreeDiskSpace(const CString& sPath, DWORDLONG& rdwFreeSpace);
	void		deleteFirstFolder(char *currDir);
	void		InitializeBackground();
	//void		Gaussian_Filter(float stddev, int n, CvMat *g);
	//void		Calculate_Points(void);
	//float		Interpolate_At_Ptr(int* upperLeft, int i, int columns);
	//void		LBP(int* data, unsigned int rows, unsigned int columns, unsigned int* result, unsigned int x);
	void		Q_Sort(float* numbers, int left, int right);

	CxImage		*m_cxImage;
	CxImage		*m_cxImage0;
	CxImage		*m_mask;
	CxImage		*m_map_grad;
	CxImage		*m_bg;
	CxImage		*m_check;
	float		*H;
	structBlob	*cca_blob, *Blob;
	CvPoint		*C, *newC;
	CvPoint		*data;
	CvPoint		*hkC;
	int			**D;
	int			*Ind;
	int			*dist_nearestC,*maxdistD_C;
	CvPoint		*cand_newC;
	int			*minD;
	int			*compD;
	int			**buff_new_bbox;
	int			*bbox_kernel, *new_bbox_kernel;
	int			**recordBuf;
	int			count;
	CxImage		*m_gui;
	bool		drawAll;
	IplImage	*cca_in;
	IplImage	*cca_out,*cca_mask;
	IplImage	*background, *background_color[50];
	IplImage	*gaussian1, *gaussian2, *gaussian3, *gaussian4, *gaussian5, *gaussian6, *gaussian7;
	IplImage	*current_image_gray;
	int			size_bbox_ov;
	CvPoint		*merge_list;
	FILE		*froi;
	int			start, stop;
	float		lanewidth;
	float		m1,m2,c1,c2;
	float		C_mtb_length, C_car_length, C_light_length,C_half_width,C_half_height,C_maxdistD_C;
	int			cca_size1,cca_size2, T_Fdiff, T_edge, deltaT, MAX_CLUSTER, MAX_CCA_BLOB, m_top;
	HINTERNET	Internet, InternetURL;
	DWORD		Read;
	BYTE		Buffer[307200];
	FILE		*plik;
	bool		m_liveFlag;
	CString		m_captureDrive;
	char		m_logDir[MAX_PATH];
	char		m_statsDir[MAX_PATH];
	char		m_captureDir[MAX_PATH];
	char		m_captureFullDir[MAX_PATH];
	char		m_statsFilename[MAX_PATH];
	char		m_roiImage[MAX_PATH];
	char		m_roiText[MAX_PATH];
	char		m_liveAdd[MAX_PATH];
	char		m_outbox[MAX_PATH];		
	CFile		m_statsFile;
	CTime		m_programStartTime;
//	char		m_currDate[MAX_MESSAGE_SIZE];
	int			m_previousMinute;
	int			m_previousHour;
	int			m_previousDay;
//	CString		m_upTime;
//	CTimeSpan	m_programRunningTime;
	int			m_diskSpaceLeft;
	int			m_deleteCount;
	bool		firstFrame;
	RGBQUAD		fuchsia;
	char		tmpStr[MAX_MESSAGE_SIZE];
	bool		m_flagNextMinute;
	bool		m_flagNextHour;
	bool		m_flagNextDay;
	int			T_edge_count,max_edge_count;
	HDC			hdc;
	VARIANT		m_AMCbuffer;
	long		bufSize;
	BYTE		*aBuffer;
	bool		m_flagDisconnect;
	bool		saveImage;
	int			stFrame;
	int			TFrames;
	//double		*sqrt_table;
	//integerpoint	points[bits];
	//floatpoint		offsets[bits];
	//int			*lbpblock;
	unsigned int	*b, *index, *lbpresultimage,*lbpresultimage2, *lbpcode, *result;
	float		*max_foreground, *hist, *histweight, *temp1, **temp2, *foreground_lbp_hist;
	int		*gaussian_diff;
	IplImage	*lbpresultimage1, *gaussian_diff1;
	IplConvKernel	*strel1, *strel2;
		
public:
	
	void		Run();
	void		init();
	void		maintainDiskSpace();

	char		m_szPath[MAX_PATH];
	int			imgSize;
	int			width;
	int			height;
	int			no_blob;
	CxImage		*m_fig1;
	CxImage		*m_fig2;
	CxImage		*m_fig3;
	float		cx;
	float		cy;
	CTracking*	m_tracker;
	int			num_record;
	bool		m_garbageCollectionFlag;
	bool		m_stopFlag;
	int			curFrame;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNT_TRAFFICDLG_H__708002B8_2AEA_44D8_8156_88A1335C2F42__INCLUDED_)
