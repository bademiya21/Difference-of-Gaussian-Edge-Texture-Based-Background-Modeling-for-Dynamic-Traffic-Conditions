// cnt_trafficDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cnt_traffic.h"
#include "cnt_trafficDlg.h"
#include "direct.h"
#include <iostream>
#include <fstream>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT MainThread(LPVOID pParam)
{
	CCnt_trafficDlg			   *pDlg;
	THREAD_PARAM			   *pPar;

	pPar = (THREAD_PARAM *)pParam;
	pDlg = (CCnt_trafficDlg *)(pPar->pDlg);

	while(pDlg->m_threadrun)
	{
		if (pDlg->m_stopFlag)
			goto END_OF_MAIN_THREAD;
		pDlg->Run();
	}

END_OF_MAIN_THREAD:
	pDlg->curFrame=-1;
	pDlg->m_threadrun=false;
	pDlg->m_buttonopen.EnableWindow(true);
	pDlg->m_buttonLive.EnableWindow(true);
	// end thread
	AfxEndThread(0);
	return 1;
}

UINT GarbageCollectionThread(LPVOID pParam)
{
	CCnt_trafficDlg			*pDlg;
	THREAD_PARAM			*pPar;
	int						i;

	pPar = (THREAD_PARAM *)pParam;
	pDlg = (CCnt_trafficDlg *)(pPar->pDlg);

	while(1)
	{
		if (!pDlg->m_garbageCollectionFlag)
		{
			goto END_OF_GARBAGE_COLLECTION_THREAD;
		}
		pDlg->maintainDiskSpace();

		for(i=0; i < 60; i++)
		{
			if (!pDlg->m_garbageCollectionFlag) break;
			Sleep(1000);
		}
	}

END_OF_GARBAGE_COLLECTION_THREAD:
	AfxEndThread(0);
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// CCnt_trafficDlg dialog

CCnt_trafficDlg::CCnt_trafficDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCnt_trafficDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCnt_trafficDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//check authorization
	//AS server mac:00-14-5E-FB-34-84
	//test pc mac:00-14-22-4e-70-8b
//	const BYTE macAddr[6] = { 0x00, 0x14, 0x5e, 0xfb, 0x34, 0x84 };
//	const BYTE macAddr[6] = { 0x00, 0x14, 0x22, 0x4e, 0x70, 0x8b };
//	AuthorizeMgr auth("myApp", CTime(2007, 8, 15, 0, 0, 0), CTime(2007, 10, 25, 0, 0, 0), macAddr);
//	if(!auth.IsAuthorized())
//	{
//		MessageBox("Program is unable to run on this machine.");
//		exit(0);
//	}
	init();

}

CCnt_trafficDlg::~CCnt_trafficDlg()
{
}

void CCnt_trafficDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCnt_trafficDlg)
	DDX_Control(pDX, IDC_STATIC_CURRENT_FRAME, m_curFrame);
	DDX_Control(pDX, IDC_EDIT_EDGETHRESHOLD, m_edit);
	DDX_Control(pDX, IDC_BUTTON6, m_buttonChange);
	DDX_Control(pDX, IDC_BUTTON5, m_buttonStop);
	DDX_Control(pDX, IDC_BUTTON4, m_buttonLive);
	DDX_Control(pDX, IDC_LIST_COUNT, m_countListCtrl);
	DDX_Control(pDX, IDC_BUTTON3, m_buttonSelectPoint);
	DDX_Control(pDX, IDC_STATIC_INPUT, m_stInput);
	DDX_Control(pDX, IDC_BUTTON2, m_buttonrun);
	DDX_Control(pDX, IDC_BUTTON1, m_buttonopen);
	DDX_Control(pDX, IDC_AXISMEDIACONTROL1, m_AMC);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCnt_trafficDlg, CDialog)
	//{{AFX_MSG_MAP(CCnt_trafficDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON2, OnButtonRun)
	ON_BN_CLICKED(IDC_BUTTON3, OnButtonSelectPoint)
	ON_BN_CLICKED(IDC_BUTTON4, OnButtonLive)
	ON_BN_CLICKED(IDC_BUTTON5, OnButtonStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON6, OnButtonChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCnt_trafficDlg message handlers

BOOL CCnt_trafficDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
		
	// TODO: Add extra initialization here	
	m_countListCtrl.InsertColumn(0, "Type");
	m_countListCtrl.SetColumnWidth(0,64);
	m_countListCtrl.InsertColumn(1,"Count", LVCFMT_CENTER, 60,-1);
	m_countListCtrl.InsertItem(1, "Motorbike");	
	m_countListCtrl.InsertItem(2, "Car");
	m_countListCtrl.InsertItem(3, "Light");
	m_countListCtrl.InsertItem(4, "Heavy/Bus");
//	m_countListCtrl.InsertItem(5, "Bus");
	m_countListCtrl.InsertItem(5, "Total");
	m_countListCtrl.SetItemText(0,1,"0");
	m_countListCtrl.SetItemText(1,1,"0");
	m_countListCtrl.SetItemText(2,1,"0");
	m_countListCtrl.SetItemText(3,1,"0");
//	m_countListCtrl.SetItemText(4,1,"0");
	m_countListCtrl.SetItemText(4,1,"0");

		
	WriteLog("");
	WriteStats("");

	strcpy(tmpStr, "Program starts");
	WriteLog(tmpStr);

	sprintf(tmpStr, "Log Dir: %s", m_logDir);
	WriteLog(tmpStr);

	sprintf(tmpStr, "Stats Dir: %s", m_statsDir);
	WriteLog(tmpStr);
	
	if (saveImage)
	{
		sprintf(tmpStr, "Capture Dir: %s", m_captureDir);
		WriteLog(tmpStr);
	}

	sprintf(tmpStr, "Garbage collection trigger: %d GB", m_diskSpaceLeft);
	WriteLog(tmpStr);
	
	m_buttonrun.EnableWindow(false);

	hdc=m_stInput.GetDC()->m_hDC;

	sprintf(tmpStr, "%d", T_edge);
	m_edit.SetWindowText(tmpStr);

	m_curFrame.SetWindowText("");

	m_garbageCollectionFlag  = true;
	m_threadParam.pDlg = (CCnt_trafficDlg *)(this);

	SetTimer(1, 1000, 0);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CCnt_trafficDlg::init()
{
	CFileException		ex;
	getParams();
	readROIfile();
	drawAll=true;
	saveImage=true;
	if (saveImage)
		setCapturePath();
	m_programStartTime  = CTime::GetCurrentTime();
	width=640;
	height=480;
	imgSize=width*height;
	m_threadrun = false;
	start=0;
	stop=0;
	m_liveFlag=false;
	m_stopFlag=false;
	m_garbageCollectionFlag	 = false;
	m_flagNextMinute=false;
	m_flagNextHour=false;
	m_flagNextDay=false;
	m_flagDisconnect=false;
	stFrame=0;
	curFrame =-1;
	num_record=0;
	bufSize=0;
	count=0;
	//FILE* sqfile;
	
	T_edge_count=100;
	max_edge_count=50;

	m_tracker				= new CTracking();
	m_mask					= new CxImage();
	m_cxImage				= new CxImage();
	m_bg					= new CxImage();
	m_check					= new CxImage();
	m_fig1					= new CxImage();
	m_fig2					= new CxImage();
	m_fig3					= new CxImage();
	m_gui					= new CxImage();
	cca_blob				= new structBlob[MAX_CCA_BLOB];
	Blob					= new structBlob[MAX_CLUSTER];
	H						= new float[maskSize];
	D						= new int*[MAX_CLUSTER];
	Ind						= new int[imgSize];
	dist_nearestC			= new int[imgSize];	
	maxdistD_C				= new int[MAX_CLUSTER];	
	minD					= new int[imgSize];	
	compD					= new int[MAX_CLUSTER];	
	buff_new_bbox			= new int*[MAX_CLUSTER];
	bbox_kernel				= new int[4];
	new_bbox_kernel			= new int[4];
	recordBuf				= new int*[MAX_CLUSTER];

	lbpresultimage			= new unsigned int[imgSize];
	lbpresultimage2			= new unsigned int[imgSize];
	result					= new unsigned int[16];
	//lbpblock				= new int[361];

	for (int i=MAX_CLUSTER;i--; )
	{
		D[i]				= new int[imgSize];
		buff_new_bbox[i]	= new int[4];
		recordBuf[i]		= new int[4];
	}

	data					= new CvPoint[imgSize];
	C						= new CvPoint[MAX_CLUSTER];
	newC					= new CvPoint[MAX_CLUSTER+1];
	hkC						= new CvPoint[MAX_CLUSTER];
	cand_newC				= new CvPoint[MAX_CLUSTER];
	merge_list				= new CvPoint[1000];

	lbpcode					= new unsigned int[(width-4)*(height-4)];
	b						= new unsigned int[(width-10)*(height-10)];
	index					= new unsigned int[(width-10)*(height-10)];
	max_foreground			= new float[(width-10)*(height-10)];
	hist					= new float[(width-10)*(height-10)*80];
	histweight				= new float[(width-10)*(height-10)*5];
	foreground_lbp_hist		= new float[(width-10)*(height-10)*16];
	
	temp1					= new float[5];
	temp2					= new float*[5];
	for (i=5;i--; )
		temp2[i]			= new float[16];
	gaussian_diff			= new int[imgSize];
	/*sqrt_table				= new double[100001];
	i=0;
	float temp6;
	char line[50];
	sqfile = fopen("sqrttable.dat", "r" );
	while (fgets(line,20,sqfile)!=NULL){
			
		sscanf(line,"%f", &temp6);
		sqrt_table[i] = temp6;
		i++;
	}
	fclose(sqfile);*/
	
		
	cca_in = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U, 1);
	cca_in->origin = 1;
	cca_out = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U, 1);
	cca_out->origin = 1;

	background = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U, 1);
	background->origin = 1;
	/*gaussian1 = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U, 1);
	gaussian1->origin = 1;*/
	gaussian2 = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U, 1);
	gaussian2->origin = 1;
	gaussian3 = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U, 1);
	gaussian3->origin = 1;
	gaussian4 = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U, 1);
	gaussian4->origin = 1;
	gaussian5 = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U, 1);
	gaussian5->origin = 1;
	gaussian6 = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U, 1);
	gaussian6->origin = 1;
	gaussian7 = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U, 1);
	gaussian7->origin = 1;
	//gaussian_diff1 = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U, 1);
	//gaussian_diff1->origin = 1;
	current_image_gray = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U, 1);
	current_image_gray->origin = 1;
	
	for (int p=50;p--; )
	{
		background_color[p] = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U, 3);
		background_color[p]->origin = 1;
	}

	m_bg->Create(width,height,8,0);
	m_fig1->Create(width,height,8,0);
	m_fig2->Create(width,height,8,0);
	m_fig3->Create(width,height,8,0);
	m_gui->Create(width,height,8,0);

	lbpresultimage1 = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U, 1);
	lbpresultimage1->origin = 1;

	strel1 = cvCreateStructuringElementEx(5, 5, 0, 0,CV_SHAPE_RECT, NULL);
	strel2 = cvCreateStructuringElementEx(9, 9, 0, 0,CV_SHAPE_RECT, NULL);


	m_previousMinute = ceil(m_programStartTime.GetHour()/10)*10;
	m_previousHour = m_programStartTime.GetHour();
	m_previousDay = m_programStartTime.GetDay();
	
	int minute=ceil(m_programStartTime.GetMinute()/10)*10;
	sprintf(m_statsFilename, "%s\\TMS-OB-OVC-01_aeslog_%d%02d%02d%02d%02d.txt", m_statsDir, m_programStartTime.GetYear(), m_programStartTime.GetMonth(), m_programStartTime.GetDay(), m_programStartTime.GetHour(),minute);
	if (!m_statsFile.Open(m_statsFilename, CFile::shareDenyWrite | CFile::modeWrite, &ex))
	{
		if (!m_statsFile.Open(m_statsFilename, CFile::modeCreate | CFile::modeWrite, &ex))
		{
			// error
		}
	}
	m_statsFile.SeekToEnd(); // append new entry at the end of file.
		
	//Load mask
	m_mask->Load(m_roiImage, CXIMAGE_FORMAT_JPG);
	//cca_mask = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U, 1);
	//cca_mask->origin = 1;
	//cca_mask=cvLoadImage(m_roiImage,-1);
	//cca_mask->origin = 1;
	//cvFlip(cca_mask, NULL, 0);
	
	if (!m_mask->IsValid())
		MessageBox("Mask not available");
	
	fuchsia.rgbBlue=255;
	fuchsia.rgbGreen=0;
	fuchsia.rgbRed=255;

	m_tracker->init();

	//cvNamedWindow("LBPResult",CV_WINDOW_AUTOSIZE);
	//cvShowImage("LBPResult", cca_mask);
	//cvNamedWindow("CCAResult",CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("Current Image",CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("Current Image Grayscale",CV_WINDOW_AUTOSIZE);

}

void CCnt_trafficDlg::cleanUp()
{
	if (Blob)			delete [] Blob;
	if (m_fig3)			delete m_fig3;
	if (m_fig2)			delete m_fig2;
	if (m_fig1)			delete m_fig1;
	if (cca_blob)		delete cca_blob;
	if (m_bg)			delete m_bg;
	if (m_check)		delete m_check;

	if (m_cxImage)		delete m_cxImage;
	if (m_mask)			delete m_mask;
	if (H)				delete [] H;
	if (m_tracker)		delete m_tracker;
	
	if (newC)			delete [] newC;
	if (C)				delete [] C;
	if (data)			delete [] data;
	if (hkC)			delete [] hkC;
	
	if (cand_newC)		delete [] cand_newC;
	if (maxdistD_C)		delete [] maxdistD_C;
	if (dist_nearestC)	delete [] dist_nearestC;
	if (Ind)			delete [] Ind;
	
    if (minD)			delete [] minD;
	
	if (compD)			delete [] compD;
	
	if (new_bbox_kernel) delete [] new_bbox_kernel;
	if (bbox_kernel)	delete [] bbox_kernel;
	if (m_gui)			delete m_gui;

	for (int i=MAX_CLUSTER; i--; )
	{
		if (D[i])				delete [] D[i];
		if (buff_new_bbox[i])	delete [] buff_new_bbox[i];
		if (recordBuf[i])		delete [] recordBuf[i];
	}
	if (D)				delete [] D;
	if (buff_new_bbox)	delete [] buff_new_bbox;
	if (recordBuf)		delete [] recordBuf;
	if (merge_list)		delete []merge_list;

	if (result)				delete [] result;
	//if (lbpblock)			delete [] lbpblock;
	if (lbpresultimage)		delete [] lbpresultimage;
	if (lbpresultimage2)	delete [] lbpresultimage2;
	if (lbpcode)			delete [] lbpcode;

	if (b)				delete [] b;
	if (index)			delete [] index;
	if (max_foreground)	delete [] max_foreground;

	//if (gaussian13)				delete [] gaussian13;

	//for (i=0;i<imgSize;i++)
	//	if(gaussian13background[i])	delete [] gaussian13background[i];
	//if (gaussian13background)	delete [] gaussian13background;
	
	if(hist)				delete [] hist;
	if(histweight)			delete [] histweight;
	if(foreground_lbp_hist)	delete [] foreground_lbp_hist;

	if(temp1)			delete [] temp1;
	for (i=5;i--; )
		if(temp2[i])			delete [] temp2[i];
	if(temp2)			delete [] temp2;
	if(gaussian_diff)	delete [] gaussian_diff;
	//if(sqrt_table)		delete [] sqrt_table;

	//cvReleaseMat(&g1);
	//cvReleaseMat(&g2);
	//cvReleaseMat(&g3);
	//cvReleaseMat(&histweight);
	//cvReleaseMat(&hist);
	//cvReleaseMat(&sRect);
	//cvReleaseMat(&lbpblock);

	//cvReleaseData(&rect);

	//cvDestroyWindow("LBPResult");
	//cvDestroyWindow("CCAResult");
	//cvDestroyWindow("Current Image");
	//cvDestroyWindow("Current Image Grayscale");

	cvReleaseImage(&cca_in);
	cvReleaseImage(&cca_out);
	//cvReleaseImage(&cca_mask);
	cvReleaseImage(&lbpresultimage1);
	/*cvReleaseImage(&gaussian1);*/
	cvReleaseImage(&gaussian2);
	cvReleaseImage(&gaussian3);
	cvReleaseImage(&gaussian4);
	cvReleaseImage(&gaussian5);
	cvReleaseImage(&gaussian6);
	cvReleaseImage(&gaussian7);
	//cvReleaseImage(&gaussian_diff1);
	cvReleaseImage(&current_image_gray);

	cvReleaseStructuringElement(&strel1);
	cvReleaseStructuringElement(&strel2);

	strcpy(tmpStr, "Program stops");
	WriteLog(tmpStr);
	m_garbageCollectionFlag = false;
	
	
}

void CCnt_trafficDlg::OnButtonOpen() 
{
	// TODO: Add your control notification handler code here
	if (!SelectPath())
	{
		MessageBox("Cannot open directory!",0,0);
		return;
	}
	m_buttonrun.EnableWindow(true);
	CreateFolder();
	m_liveFlag=false;
}

void CCnt_trafficDlg::OnButtonRun() 
{
	m_threadrun = true;
	m_stopFlag = false;
	firstFrame=true;
	m_buttonopen.EnableWindow(false);
	m_buttonLive.EnableWindow(false);
	InitializeBackground();
	curFrame=-1;
	AfxBeginThread(MainThread, (LPVOID)(&m_threadParam), THREAD_PRIORITY_NORMAL);
	m_garbageCollectionFlag  = true;
	AfxBeginThread(GarbageCollectionThread, (LPVOID)(&m_threadParam), THREAD_PRIORITY_NORMAL);
}

bool CCnt_trafficDlg::SelectPath()
{
	bool returnFlag;

	// Select the path to the example
	BROWSEINFO bi;
	LPITEMIDLIST pidl;
	LPMALLOC pMalloc = NULL;

	returnFlag = true;
    if (SUCCEEDED(SHGetMalloc(&pMalloc))) 
	{
        ZeroMemory(&bi,sizeof(bi));
		bi.hwndOwner = NULL;
        bi.pszDisplayName = 0;
        bi.pidlRoot = 0;
		bi.lpszTitle = TEXT("Select folder for sequences");         
		bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
//      bi.lpfn = BrowseCallbackProc;
        pidl = SHBrowseForFolder(&bi);
        if (pidl) 
		{
			if (!SHGetPathFromIDList(pidl, m_szPath))
			{
				AfxMessageBox("Error in selecting a path!",MB_OK);
				returnFlag = false;
			}
            pMalloc->Free(pidl); 
			pMalloc->Release();
        }
		else
		{
			returnFlag = false;
		}
    }
	else
	{
		returnFlag = false;
	}
	return returnFlag;
}

void CCnt_trafficDlg::InitializeBackground()
{
	//***********************************************
	//* Codes added by How Lung

	// Form reference edge map
	

	//***************************************************
	int r, c, k, x=1;

	BYTE***		bg_red;
	BYTE***		bg_green;
	BYTE***		bg_blue;

	bg_red		= new BYTE**[height];
	bg_green	= new BYTE**[height];
	bg_blue		= new BYTE**[height];
	
	
	for (r=height; r--; )
	{
		bg_red[r]=new BYTE*[width];
		bg_green[r]=new BYTE*[width];
		bg_blue[r]=new BYTE*[width];
		
		for (c=width;c--; )
		{
			bg_red[r][c]=new BYTE [50];
			bg_green[r][c]=new BYTE [50];
			bg_blue[r][c]=new BYTE [50];
		}
	}	

	sprintf(tmpStr, "Learning background...");
	m_curFrame.SetWindowText(tmpStr);
	WriteLog(tmpStr);

	for (curFrame=stFrame;curFrame<stFrame+max_edge_count;curFrame++)
	{	
		getParams();
		//Load frames
		if (m_liveFlag)
		{	
			//AMC method
			
			try
			{
				m_AMC.GetCurrentImage(0,&m_AMCbuffer,&bufSize);
				
				if (bufSize<=0)
				{
					curFrame--;
					return;
				}

					
				SafeArrayAccessData(m_AMCbuffer.parray, (void**)&aBuffer); 
				m_cxImage->Decode(aBuffer,m_AMCbuffer.parray->rgsabound[0].cElements,CXIMAGE_FORMAT_JPG);
				SafeArrayUnaccessData(m_AMCbuffer.parray);
			}
			catch (COleDispatchException *e)
			{
				if (e->m_scError == E_INVALIDARG)
				{
				  MessageBox("Invalid parameters.");
				}
				else
				{
				  MessageBox("Failure.");
				}
			}
			
			if (saveImage)
			{
				sprintf(tmpStr, "%s\\%0.5d.jpg",m_szPath,curFrame);
				m_cxImage->Save(tmpStr,CXIMAGE_FORMAT_JPG);
			}
		}		
		else
		{			
			sprintf(tmpStr, "%s\\(%d).jpg",m_szPath,curFrame);
			m_cxImage->Load(tmpStr,CXIMAGE_FORMAT_JPG);
		}
		switch(x)
		{
			case 0:	for (r=height; r--; )
					{
						for (c=width; c--; )
						{
							bg_red[r][c][curFrame]=m_cxImage->GetPixelColor(c,r).rgbRed;
							bg_green[r][c][curFrame]=m_cxImage->GetPixelColor(c,r).rgbGreen;
							bg_blue[r][c][curFrame]=m_cxImage->GetPixelColor(c,r).rgbBlue;
						}
					}
					break;

			default:	for (r=height; r--; )
						{
							for (c=width; c--; )
							{
								((uchar *)(background_color[curFrame]->imageData + r*background_color[curFrame]->widthStep))[c*background_color[curFrame]->nChannels + 0]=m_cxImage->GetPixelColor(c,r).rgbBlue;
								((uchar *)(background_color[curFrame]->imageData + r*background_color[curFrame]->widthStep))[c*background_color[curFrame]->nChannels + 1]=m_cxImage->GetPixelColor(c,r).rgbGreen;
								((uchar *)(background_color[curFrame]->imageData + r*background_color[curFrame]->widthStep))[c*background_color[curFrame]->nChannels + 2]=m_cxImage->GetPixelColor(c,r).rgbRed;
							}
						}
		}
	}

	// The number of filter taps is calculated as follows: n = 2*round(3.5*stddev)+1
	//Gaussian_Filter(2, 15, g3);
	//Gaussian_Filter(2*sqrt(2), 21, g2);
	//Gaussian_Filter(4, 29, g1);
	
	//Calculate_Points();/*Set Up The neighbourhood sampling points for the LBP*/

	unsigned int value;
	int pred2 = predicate <<1;
	float max, min, weight,temp;
	int ind_min, ind_max;

	switch(x)
	{
		case 0:	RGBQUAD median; //Median Filtering
				median.rgbRed=0;
				median.rgbGreen=0;
				median.rgbBlue=0;
	
				for (k=151;k--; )
				{
					for (r=height; r--; )
					{
						for (c=width; c--; )
						{
							/*if (r>height-6)
							{
								((uchar *)(background_color[k]->imageData + r*background_color[k]->widthStep))[c*background_color[k]->nChannels + 0]=0; // B
								((uchar *)(background_color[k]->imageData + r*background_color[k]->widthStep))[c*background_color[k]->nChannels + 1]=0; // G
								((uchar *)(background_color[k]->imageData + r*background_color[k]->widthStep))[c*background_color[k]->nChannels + 2]=0; // R
								continue;
							}*/
							int min_delta=20000;
							for(int i=k; i>k-25;i--)
							{
								int delta=0;
								for (int j=k;j>k-25;j--)
								{
									delta+=abs(bg_red[r][c][i]-bg_red[r][c][j]);
								}
								if (delta<min_delta)
								{
									median.rgbRed=bg_red[r][c][i];
									min_delta=delta;
								}
							}
							min_delta=20000;
							for(i=k; i>k-25;i--)
							{
								int delta=0;
								for (int j=k;j>k-25;j--)
								{
									delta+=abs(bg_green[r][c][i]-bg_green[r][c][j]);
								}
								if (delta<min_delta)
								{
									median.rgbGreen=bg_green[r][c][i];
									min_delta=delta;
								}
							}
							min_delta=20000;
							for(i=k; i>k-25;i--)
							{
								int delta=0;
								for (int j=k;j>k-25;j--)
								{
									delta+=abs(bg_blue[r][c][i]-bg_blue[r][c][j]);
								}
								if (delta<min_delta)
								{
									median.rgbBlue=bg_blue[r][c][i];
									min_delta=delta;
								}
							}
							((uchar *)(background_color[k]->imageData + r*background_color[k]->widthStep))[c*background_color[k]->nChannels + 0]=median.rgbBlue; // B
							((uchar *)(background_color[k]->imageData + r*background_color[k]->widthStep))[c*background_color[k]->nChannels + 1]=median.rgbGreen; // G
							((uchar *)(background_color[k]->imageData + r*background_color[k]->widthStep))[c*background_color[k]->nChannels + 2]=median.rgbRed; // R
						}
					}
				}

				for (r=height; r--; )
				{
					for (c=width; c--; )
					{
						if (bg_red[r][c]) delete [] bg_red[r][c];
						if (bg_green[r][c]) delete [] bg_green[r][c];
						if (bg_blue[r][c]) delete [] bg_blue[r][c];
					}
					if (bg_red[r]) delete [] bg_red[r];
					if (bg_green[r]) delete [] bg_green[r];
					if (bg_blue[r]) delete [] bg_blue[r];
				}

				if (bg_red)	delete [] bg_red;
				if (bg_green) delete [] bg_green;
				if (bg_blue) delete [] bg_blue;

				for (k=5;k--; )//Obtain 5 preset histograms for each pixel from 5 diff images in the background image set.
				{
					cvCvtColor(background_color[10*k],background,CV_BGR2GRAY);
					cvReleaseImage(&background_color[10*k]);

					cvSmooth(background, gaussian7, CV_GAUSSIAN,3, 3, 1);
					cvSmooth(background, gaussian6, CV_GAUSSIAN,7, 7, 1.6);
					cvSmooth(background, gaussian5, CV_GAUSSIAN,13, 13, 2.56);
					cvSmooth(background, gaussian4, CV_GAUSSIAN,23, 23, 4.096);
					cvSmooth(background, gaussian3, CV_GAUSSIAN,41, 41, 6.5536);
					cvSmooth(background, gaussian2, CV_GAUSSIAN,67, 67, 10.48576);
					/*cvSmooth(background, gaussian1, CV_GAUSSIAN,423, 423, 64);*/
					
					for (r=height;r--; )
					{
						for (c=width;c--; )
						{
							// Obtain the Added DoG
							gaussian_diff[r*width+c] =	-0.4*(((uchar *)(gaussian7->imageData + r*gaussian7->widthStep))[c]) + 0.1*(((uchar *)(gaussian6->imageData + r*gaussian6->widthStep))[c])
														+ 0.15*(((uchar *)(gaussian5->imageData + r*gaussian5->widthStep))[c]) + 0.05*(((uchar *)(gaussian4->imageData + r*gaussian4->widthStep))[c])
														+ 0.05*(((uchar *)(gaussian3->imageData + r*gaussian3->widthStep))[c]) + 0.05*(((uchar *)(gaussian2->imageData + r*gaussian2->widthStep))[c]);

						}
					}

					for (r=height;r--; )
					{
						for (c=width;c--; )
						{
							if ((c<5) || (c>width-6) || (r>height-6) || (r<5)) 
								continue;
							// Perform a 11 x 11 block 4 Bit LBP with pointers pointing to the centre and the sampling points in a 5 x 5 overlapping block
							// within the 11 x 11 block. A histogram of 16 bins with cumulative value of 49 is obtained.
							int	*p0 = &gaussian_diff[(r-5)*width+c-5],
								*p1 = p0 + 2*predicate,
								*p2 = p1 + 2*width*predicate,
								*p3 = p2 - 2*predicate,
								//*p4 = p3 - 2*predicate,
								//*p5 = p4 - width*predicate,
								*centre = p3 + predicate - width*predicate;
													
							memset(result,0,sizeof(unsigned int)*16); //Clear result histogram

							// LBP
							for (int l=11-pred2;l--; )
							{
								for (int m=11-pred2;m--; )
								{
									value = 0;
									
									//Unrolled loop 
									compab_mask_inc(p0,0);
									compab_mask_inc(p1,1);
									compab_mask_inc(p2,2);
									compab_mask_inc(p3,3);
									//compab_mask_inc(p4,4);
									//compab_mask_inc(p5,5);
									centre++;

									result[value]++; //Increase histogram bin value
								}
								p0 += width-7;
								p1 += width-7;
								p2 += width-7;
								p3 += width-7;
								//p4 += width-7;
								//p5 += width-7;
								centre += width-7;
							}
							
							for (int j=16;j--; )
								hist[((r-5)*(width-10)+(c-5))*80+(k*16)+j] = (float)(result[j])/49; // Normalize the histogram for storage
							histweight[((r-5)*(width-10)+(c-5))*5+k] = (float)0.2; // Assign an initial and equal weight to each of the 5 histograms for each pixel.
						}
					}
				}

				for (k=25;k--; )
				{
					if (k==0 || k==30 || k==60 || k==90 || k==120)
							continue;/* Skip remaining statements in the loop and proceed with the next iteration */

					cvCvtColor(background_color[k],background,CV_BGR2GRAY);
					cvReleaseImage(&background_color[k]);

					cvSmooth(background, gaussian7, CV_GAUSSIAN,3, 3, 1);
					cvSmooth(background, gaussian6, CV_GAUSSIAN,7, 7, 1.6);
					cvSmooth(background, gaussian5, CV_GAUSSIAN,13, 13, 2.56);
					cvSmooth(background, gaussian4, CV_GAUSSIAN,23, 23, 4.096);
					cvSmooth(background, gaussian3, CV_GAUSSIAN,41, 41, 6.5536);
					cvSmooth(background, gaussian2, CV_GAUSSIAN,67, 67, 10.48576);
					/*cvSmooth(background, gaussian1, CV_GAUSSIAN,423, 423, 64);*/
					
					for (r=height;r--; )
					{
						for (c=width;c--; )
						{
							gaussian_diff[r*width+c] =	-0.4*(((uchar *)(gaussian7->imageData + r*gaussian7->widthStep))[c]) + 0.1*(((uchar *)(gaussian6->imageData + r*gaussian6->widthStep))[c])
														+ 0.15*(((uchar *)(gaussian5->imageData + r*gaussian5->widthStep))[c]) + 0.05*(((uchar *)(gaussian4->imageData + r*gaussian4->widthStep))[c])
														+ 0.05*(((uchar *)(gaussian3->imageData + r*gaussian3->widthStep))[c]) + 0.05*(((uchar *)(gaussian2->imageData + r*gaussian2->widthStep))[c]);

						}
					}

					for (r=height;r--; )
					{
						for (c=width;c--; )
						{
							if ((c<5) || (c>width-6) || (r>height-6) || (r<5)) 
								continue;

							int	*p0 = &gaussian_diff[(r-5)*width+c-5],
								*p1 = p0 + 2*predicate,
								*p2 = p1 + 2*width*predicate,
								*p3 = p2 - 2*predicate,
								//*p4 = p3 - 2*predicate,
								//*p5 = p4 - width*predicate,
								*centre = p3 + predicate - width*predicate;
													
							memset(result,0,sizeof(unsigned int)*16); //Clear result histogram

							for (int l=11-pred2;l--; )
							{
								for (int m=11-pred2;m--; )
								{
									value = 0;
									
									//Unrolled loop 
									compab_mask_inc(p0,0);
									compab_mask_inc(p1,1);
									compab_mask_inc(p2,2);
									compab_mask_inc(p3,3);
									//compab_mask_inc(p4,4);
									//compab_mask_inc(p5,5);
									centre++;

									result[value]++; //Increase histogram bin value
								}
								p0 += width-7;
								p1 += width-7;
								p2 += width-7;
								p3 += width-7;
								//p4 += width-7;
								//p5 += width-7;
								centre += width-7;
							}
							// Perform Histogram matching using Bhattacharya Distance measure for Histograms
							float H[5] = {0};
							for (int j=5;j--; )
							{
								for (int l=16;l--; )
									H[j] += sqrt((float)result[l]/49*hist[((r-5)*(width-10)+(c-5))*80+(j*16)+l]);
							}
							max = H[0];
							ind_max = 0;
							for (int i = 1; i < 5; i++) // Find the histogram that best matches the current one
							{
								if (H[i] > max)
								{
									max = H[i];
									ind_max = i;
								}
							}
							if (max < 0.65)// No matching (below threshold)
							{
								continue;
								/*min = histweight[((r-5)*(width-10)+(c-5))*5+0];
								ind_min = 0;
								for (int i = 1; i < 5; i++)// Find the histogram with the least weight
								{
									if (histweight[(r-5)*(width-10)+(c-5)*5+i] < min)
									{
										min = histweight[(r-5)*(width-10)+(c-5)*5+i];
										ind_min = i;
									}
								}
								for (int l=64;l--; )// Replace least weighted histogram with the new histogram obtained from the current background image
									hist[((r-5)*(width-10)+(c-5))*320+(ind_min*64)+l] = result[l]/49;
								histweight[((r-5)*(width-10)+(c-5))*5+ind_min] = (float)0.01;// Reassign the weight
								weight = 0;
								//Normalize the weights
								for (int j=5;j--; )
									weight += histweight[((r-5)*(width-10)+(c-5))*5+j];
								for (j=5;j--; )
									histweight[((r-5)*(width-10)+(c-5))*5+j] = histweight[((r-5)*(width-10)+(c-5))*5+j]/weight;*/
							}
							else // Histogram matches
							{
								for (int l=16;l--; )// Update the bins of the best matched histogram
								{
									temp = 0.015*(float)result[l]/49+ 0.985*hist[((r-5)*(width-10)+(c-5))*80+(ind_max)*16+l];
									hist[((r-5)*(width-10)+(c-5))*80+(ind_max)*16+l] = temp;
								}
								for (int j=5;j--; )// Update the weights of the best matched histogram and decrease the weights of the histograms that do not match
									histweight[((r-5)*(width-10)+(c-5))*5+j] = 0.985*histweight[((r-5)*(width-10)+(c-5))*5+j];
								histweight[((r-5)*(width-10)+(c-5))*5+ind_max] = histweight[((r-5)*(width-10)+(c-5))*5+ind_max]+0.015;
							}
						}
					}
				}

				break;

			default:	//Without median filtering especially for dynamic scenarios with little light information or a lot of noise (like rain)
						for (k=5;k--; ) //Obtain 5 preset histograms for each pixel from 5 diff images in the background image set.
						{
							cvCvtColor(background_color[10*k],background,CV_BGR2GRAY);
							cvReleaseImage(&background_color[10*k]);

							cvSmooth(background, gaussian7, CV_GAUSSIAN,3, 3, 1);
							cvSmooth(background, gaussian6, CV_GAUSSIAN,7, 7, 1.6);
							cvSmooth(background, gaussian5, CV_GAUSSIAN,13, 13, 2.56);
							cvSmooth(background, gaussian4, CV_GAUSSIAN,23, 23, 4.096);
							cvSmooth(background, gaussian3, CV_GAUSSIAN,41, 41, 6.5536);
							cvSmooth(background, gaussian2, CV_GAUSSIAN,67, 67, 10.48576);
							/*cvSmooth(background, gaussian1, CV_GAUSSIAN,423, 423, 64);*/
							
							for (r=height;r--; )
							{
								for (c=width;c--; )
								{
									// Obtain the Added DoG
									gaussian_diff[r*width+c] =	-0.4*(((uchar *)(gaussian7->imageData + r*gaussian7->widthStep))[c]) + 0.1*(((uchar *)(gaussian6->imageData + r*gaussian6->widthStep))[c])
																+ 0.15*(((uchar *)(gaussian5->imageData + r*gaussian5->widthStep))[c]) + 0.05*(((uchar *)(gaussian4->imageData + r*gaussian4->widthStep))[c])
																+ 0.05*(((uchar *)(gaussian3->imageData + r*gaussian3->widthStep))[c]) + 0.05*(((uchar *)(gaussian2->imageData + r*gaussian2->widthStep))[c]);

								}
							}
							
							for (r=height;r--; )
							{
								for (c=width;c--; )
								{
									if ((c<5) || (c>width-6) || (r>height-6) || (r<5)) 
										continue;
									// Perform a 11 x 11 block 4 Bit LBP with pointers pointing to the centre and the sampling points in a 5 x 5 overlapping block
									// within the 11 x 11 block. A histogram of 16 bins with cumulative value of 49 is obtained.
									int	*p0 = &gaussian_diff[(r-5)*width+c-5],
										*p1 = p0 + 2*predicate,
										*p2 = p1 + 2*width*predicate,
										*p3 = p2 - 2*predicate,
										//*p4 = p3 - 2*predicate,
										//*p5 = p4 - width*predicate,
										*centre = p3 + predicate - width*predicate;
															
									memset(result,0,sizeof(unsigned int)*16); //Clear result histogram
									
									// LBP
									for (int l=11-pred2;l--; )
									{
										for (int m=11-pred2;m--; )
										{
											value = 0;
											
											//Unrolled loop 
											compab_mask_inc(p0,0);
											compab_mask_inc(p1,1);
											compab_mask_inc(p2,2);
											compab_mask_inc(p3,3);
											//compab_mask_inc(p4,4);
											//compab_mask_inc(p5,5);
											centre++;

											result[value]++; //Increase histogram bin value
										}
										p0 += width-7;
										p1 += width-7;
										p2 += width-7;
										p3 += width-7;
										//p4 += width-7;
										//p5 += width-7;
										centre += width-7;
									}
									for (int j=16;j--; )
										hist[((r-5)*(width-10)+(c-5))*80+(k*16)+j] = (float)result[j]/49; // Normalize the histogram for storage
									histweight[((r-5)*(width-10)+(c-5))*5+k] = (float)0.2; // Assign an initial and equal weight to each of the 5 histograms for each pixel.
								}
							}
						}

						for (k=50;k--; )
						{
							if (k==0 || k==10 || k==20 || k==30 || k==40)
									continue;/* Skip remaining statements in the loop and proceed with the next iteration */

							cvCvtColor(background_color[k],background,CV_BGR2GRAY);
							cvReleaseImage(&background_color[k]);

							cvSmooth(background, gaussian7, CV_GAUSSIAN,3, 3, 1);
							cvSmooth(background, gaussian6, CV_GAUSSIAN,7, 7, 1.6);
							cvSmooth(background, gaussian5, CV_GAUSSIAN,13, 13, 2.56);
							cvSmooth(background, gaussian4, CV_GAUSSIAN,23, 23, 4.096);
							cvSmooth(background, gaussian3, CV_GAUSSIAN,41, 41, 6.5536);
							cvSmooth(background, gaussian2, CV_GAUSSIAN,67, 67, 10.48576);
							/*cvSmooth(background, gaussian1, CV_GAUSSIAN,423, 423, 64);*/

							for (r=height;r--; )
							{
								for (c=width;c--; )
								{
									gaussian_diff[r*width+c] =	-0.4*(((uchar *)(gaussian7->imageData + r*gaussian7->widthStep))[c]) + 0.1*(((uchar *)(gaussian6->imageData + r*gaussian6->widthStep))[c])
																+ 0.15*(((uchar *)(gaussian5->imageData + r*gaussian5->widthStep))[c]) + 0.05*(((uchar *)(gaussian4->imageData + r*gaussian4->widthStep))[c])
																+ 0.05*(((uchar *)(gaussian3->imageData + r*gaussian3->widthStep))[c]) + 0.05*(((uchar *)(gaussian2->imageData + r*gaussian2->widthStep))[c]);

								}
							}

							for (r=height;r--; )
							{
								for (c=width;c--; )
								{
									if ((c<5) || (c>width-6) || (r>height-6) || (r<5)) 
										continue;

									int	*p0 = &gaussian_diff[(r-5)*width+c-5],
										*p1 = p0 + 2*predicate,
										*p2 = p1 + 2*width*predicate,
										*p3 = p2 - 2*predicate,
										//*p4 = p3 - 2*predicate,
										//*p5 = p4 - width*predicate,
										*centre = p3 + predicate - width*predicate;
															
									memset(result,0,sizeof(unsigned int)*16); //Clear result histogram

									for (int l=11-pred2;l--; )
									{
										for (int m=11-pred2;m--; )
										{
											value = 0;
											
											//Unrolled loop 
											compab_mask_inc(p0,0);
											compab_mask_inc(p1,1);
											compab_mask_inc(p2,2);
											compab_mask_inc(p3,3);
											//compab_mask_inc(p4,4);
											//compab_mask_inc(p5,5);
											centre++;

											result[value]++; //Increase histogram bin value
										}
										p0 += width-7;
										p1 += width-7;
										p2 += width-7;
										p3 += width-7;
										//p4 += width-7;
										//p5 += width-7;
										centre += width-7;
									}
									// Perform Histogram matching using Bhattacharya Distance measure for Histograms
									float H[5] = {0};
									for (int j=5;j--; )
									{
										for (int l=16;l--; )
											H[j] += sqrt((float)result[l]/49*hist[((r-5)*(width-10)+(c-5))*80+(j*16)+l]);
									}
									max = H[0];
									ind_max = 0;
									for (int i = 1; i < 5; i++) // Find the histogram that best matches the current one
									{
										if (H[i] > max)
										{
											max = H[i];
											ind_max = i;
										}
									}
									if (max < 0.65) // No matching (below threshold)
									{
										continue;
										/*min = histweight[((r-5)*(width-10)+(c-5))*5+0];
										ind_min = 0;
										for (int i = 1; i < 5; i++)// Find the histogram with the least weight
										{
											if (histweight[(r-5)*(width-10)+(c-5)*5+i] < min)
											{
												min = histweight[(r-5)*(width-10)+(c-5)*5+i];
												ind_min = i;
											}
										}
										for (int l=64;l--; )// Replace least weighted histogram with the new histogram obtained from the current background image
											hist[((r-5)*(width-10)+(c-5))*320+(ind_min*64)+l] = result[l]/49;
										histweight[((r-5)*(width-10)+(c-5))*5+ind_min] = (float)0.01;// Reassign the weight
										weight = 0;
										//Normalize the weights
										for (int j=5;j--; )
											weight += histweight[((r-5)*(width-10)+(c-5))*5+j];
										for (j=5;j--; )
											histweight[((r-5)*(width-10)+(c-5))*5+j] = histweight[((r-5)*(width-10)+(c-5))*5+j]/weight;*/
									}
									else // Histogram matches
									{
										for (int l=16;l--; )// Update the bins of the best matched histogram 
										{
											temp = 0.015*(float)result[l]/49+ 0.985*hist[((r-5)*(width-10)+(c-5))*80+(ind_max)*16+l];
											hist[((r-5)*(width-10)+(c-5))*80+(ind_max)*16+l] = temp;
										}
										for (int j=5;j--; )// Update the weights of the best matched histogram and decrease the weights of the histograms that do not match
											histweight[((r-5)*(width-10)+(c-5))*5+j] = 0.985*histweight[((r-5)*(width-10)+(c-5))*5+j];
										histweight[((r-5)*(width-10)+(c-5))*5+ind_max] = histweight[((r-5)*(width-10)+(c-5))*5+ind_max]+0.015;
									}
								}
							}
						}
	}

	cvReleaseImage(&background);

	for (r=height;r--; )// Sort the histograms according to the weight from heaviest to the least
	{
		for (c=width;c--; )
		{
			if ((c<5) || (c>width-6) || (r>height-6) || (r<5)) 
					continue;

			float temp_descend;

			for (int i=5;i--; )
				temp1[i] = histweight[((r-5)*(width-10)+(c-5))*5+i];
			Q_Sort(temp1,0,4); // Returns sorted array in ascending order
			// Rearrange the array so that it is in the descending order
			temp_descend = temp1[4];
			temp1[4] = temp1[0];
			temp1[0] = temp_descend;
			temp_descend = temp1[3];
			temp1[3] = temp1[1];
			temp1[1] = temp_descend;
			
			float sum1=0;
			// Add the weights to the point where its sum is equal to or greater than the selection threshold
			for (i=0;i<5;i++)
			{
				sum1 += temp1[i];
				if (sum1 > 0.9) // defined threshold for selection
				{
					b[(r-5)*(width-10)+(c-5)] = i; // Value of the index of the weight where the threshold is met
					break;
				}
			}
			// Rearrange the histograms in descending order of the weights up to the index found above
			for (i=b[(r-5)*(width-10)+(c-5)]+1;i--; )
			{
				for (int j=5;j--; )
				{
					if (temp1[i] == histweight[((r-5)*(width-10)+(c-5))*5+j])
					{
						for (int l=16;l--; )
							temp2[i][l] = hist[((r-5)*(width-10)+(c-5))*80+(j*16)+l];
						break;
					}
				}
			}

			for (int j=b[(r-5)*(width-10)+(c-5)]+1;j--; )
			{
				for (int l=16;l--; )
					hist[((r-5)*(width-10)+(c-5))*80+(j*16)+l] = temp2[j][l];
				histweight[((r-5)*(width-10)+(c-5))*5+j] = temp1[j];
			}

			// Set the histograms and weights of the indexes beyond that found above to zero.
			if (b[(r-5)*(width-10)+(c-5)]+1<5)
			{
				for (j=b[(r-5)*(width-10)+(c-5)]+1;j<5;j++)
				{
					for (int l=16;l--; )
						hist[((r-5)*(width-10)+(c-5))*80+(j*16)+l] = 0.0;
					histweight[((r-5)*(width-10)+(c-5))*5+j] = 0.0;
				}
			}
		}
	}
}

void CCnt_trafficDlg::Run() 
{
	// TODO: Add your control notification handler code here
	int	r,c,i,j,l,m, value;
	int pred2 = predicate <<1;
	bool flag_continue=false;
	bool flag_update=false;
	//RGBQUAD bg_color;

	if (m_flagNextHour) //create new folder
	{			
		curFrame=-1;
		if (saveImage)
			setCapturePath();

		m_flagNextHour=false;

		if (m_liveFlag && saveImage)
			strcpy(m_szPath, m_captureFullDir);

		if (m_flagNextDay) //reset vehicle count
		{
			m_tracker->init(); 
			m_flagNextDay=false;
		}
	}
	if (m_flagNextMinute)
	{
		WriteStats("");
		m_flagNextMinute=false;
	}

	curFrame++;	
	getParams();
		
	if (m_liveFlag)
	{
		//AMC method	
		try
		{
			m_AMC.GetCurrentImage(0,&m_AMCbuffer,&bufSize);
			
			if (bufSize<=0)
			{
				if (!m_flagDisconnect)
					WriteLog("Camera disconnected");
				curFrame--;
				m_flagDisconnect=true;
				return;
			}
			if (m_flagDisconnect)
			{
				WriteLog("Camera reconnected");
				m_flagDisconnect=false;
			}

			SafeArrayAccessData(m_AMCbuffer.parray, (void**)&aBuffer); 
			m_cxImage->Decode(aBuffer,m_AMCbuffer.parray->rgsabound[0].cElements,CXIMAGE_FORMAT_JPG);	
			SafeArrayUnaccessData(m_AMCbuffer.parray);
			
		}
		catch (COleDispatchException *e)
		{
			if (e->m_scError == E_INVALIDARG)
			{
				MessageBox("Invalid parameters.");
			}
			else
			{
				MessageBox("Failure.");
			}
		}
		if (saveImage)
		{
			sprintf(tmpStr, "%s\\%0.5d.jpg",m_szPath,curFrame);
			m_cxImage->Save(tmpStr,CXIMAGE_FORMAT_JPG);
		}
	}
	else
	{
		sprintf(tmpStr, "%s\\(%d).jpg",m_szPath,curFrame);
		m_cxImage->Load(tmpStr,CXIMAGE_FORMAT_JPG);
		current_image_gray = cvLoadImage(tmpStr,0);
		if (current_image_gray == 0)
		{
			OnButtonStop();
			exit(0);
		}
		else
		{
			current_image_gray->origin = 1;
			cvFlip(current_image_gray, NULL, 0);
		}
	}
	
	sprintf(tmpStr, "Frame = %0.5d", curFrame);
	m_curFrame.SetWindowText(tmpStr);
	
	cvSmooth(current_image_gray, gaussian7, CV_GAUSSIAN,3, 3, 1);
	cvSmooth(current_image_gray, gaussian6, CV_GAUSSIAN,7, 7, 1.6);
	cvSmooth(current_image_gray, gaussian5, CV_GAUSSIAN,13, 13, 2.56);
	cvSmooth(current_image_gray, gaussian4, CV_GAUSSIAN,23, 23, 4.096);
	cvSmooth(current_image_gray, gaussian3, CV_GAUSSIAN,41, 41, 6.5536);
	cvSmooth(current_image_gray, gaussian2, CV_GAUSSIAN,67, 67, 10.48576);
	/*cvSmooth(current_image_gray, gaussian1, CV_GAUSSIAN,423, 423, 64);*/

	for (r=height;r--; )
	{
		for (c=width;c--; )
		{
			// Added DoG Image
			gaussian_diff[r*width+c] =	-0.4*(((uchar *)(gaussian7->imageData + r*gaussian7->widthStep))[c]) + 0.1*(((uchar *)(gaussian6->imageData + r*gaussian6->widthStep))[c])
										+ 0.15*(((uchar *)(gaussian5->imageData + r*gaussian5->widthStep))[c]) + 0.05*(((uchar *)(gaussian4->imageData + r*gaussian4->widthStep))[c])
										+ 0.05*(((uchar *)(gaussian3->imageData + r*gaussian3->widthStep))[c]) + 0.05*(((uchar *)(gaussian2->imageData + r*gaussian2->widthStep))[c]);

			
			if ((c>width-5) || (r>height-5)) 
				continue;
			// Obtain the 5 x 5 overlapping block 4 Bit LBP codes for each pixel for the Added DoG Image
			lbpcode[r*(width-4)+c]= (gaussian_diff[r*width+c] >= gaussian_diff[(r+2)*width+c+2])+((gaussian_diff[r*width+c+4] >= gaussian_diff[(r+2)*width+c+2])<<1) 
									+((gaussian_diff[(r+4)*width+c+4] >= gaussian_diff[(r+2)*width+c+2])<<2)+((gaussian_diff[(r+4)*width+c] >= gaussian_diff[(r+2)*width+c+2])<<3);
		}
	}

	float temp;
	
	for (r=height;r--; )
	{
		for (c=width;c--; )
		{
			if ((c<5) || (c>width-6) || (r>height-6) || (r<5)) 
			{
				//lbpresultimage[(r*width)+c] = 0;
				((uchar *)(lbpresultimage1->imageData + r*lbpresultimage1->widthStep))[c] = 0;
				continue;
			}
					
			memset(result,0,sizeof(unsigned int)*16); //Clear result histogram

			for ( l=r-5;l<r+2;l++)
			{
				for (m=c-5;m<c+2;m++)
					result[lbpcode[l*(width-4)+m]]++;// Compute the histogram
			}

			// Perform Histogram matching using Bhattacharya Distance measure for Histograms (Percentage Matching--> the higher the value, the more the histogram matches the current one)
			float H[5] = {0};
			for (j=b[(r-5)*(width-10)+(c-5)]+1;j--; )
			{
				for (l=16;l--; )
				{
					H[j] += sqrt((float)result[l]/49*hist[((r-5)*(width-10)+(c-5))*80+(j*16)+l]);
					if (j==b[(r-5)*(width-10)+(c-5)])
						foreground_lbp_hist[((r-5)*(width-10)+(c-5))*16+l] = (float)result[l]/49;//Store the current histogram for updating later 
				}
			}
			// Find the histogram that best matches the current histogram
			max_foreground[(r-5)*(width-10)+(c-5)] = H[0];
			index[(r-5)*(width-10)+(c-5)] = 0;
			for ( i = 1; i <= b[(r-5)*(width-10)+(c-5)]; i++)
			{
				if (H[i] > max_foreground[(r-5)*(width-10)+(c-5)])
				{
					max_foreground[(r-5)*(width-10)+(c-5)] = H[i];// Store the maximum of the distance for use later
					index[(r-5)*(width-10)+(c-5)] = i;// Store the index of the histogram that best matches the current one
				}
			}
			// Above threshold --> Classify Background pixel
			if (max_foreground[(r-5)*(width-10)+(c-5)]>=0.77)
			{
				//lbpresultimage[(r*width)+c]=0;
				((uchar *)(lbpresultimage1->imageData + r*lbpresultimage1->widthStep))[c] = 0;
			}
			else // Below Threshold --> Classify Foreground Pixel
			{
				//lbpresultimage[(r*width)+c]=255;
				((uchar *)(lbpresultimage1->imageData + r*lbpresultimage1->widthStep))[c] = 255;
			}
		}
	}	

	//cvMorphologyEx(lbpresultimage1,lbpresultimage1, gaussian_diff1,strel1, CV_MOP_OPEN, 1);
	//cvMorphologyEx(lbpresultimage1,lbpresultimage1, gaussian_diff1,strel1, CV_MOP_CLOSE, 1);
	//cvMorphologyEx(lbpresultimage1,lbpresultimage1, gaussian_diff1,strel2, CV_MOP_OPEN, 1);
	//cvMorphologyEx(lbpresultimage1,lbpresultimage1, gaussian_diff1,strel2, CV_MOP_CLOSE, 1);
	//cvSaveImage("LBPResult.jpeg",lbpresultimage1);

	flag_continue=cca();
	//flag_continue=true;
	
	for (r=height;r--; )
	{
		for (c=width;c--; )
		{
			if ((c<5) || (c>width-6) || (r>height-6) || (r<5)) 
				continue;

			flag_update=true;
			
			//if (((uchar *)(lbpresultimage1->imageData + r*lbpresultimage1->widthStep))[c] >= 100)
				//continue;
			for (i=no_blob;i--; )
			{
				if (c>=cca_blob[i].bbox[0] && c<=cca_blob[i].bbox[1] && r>=cca_blob[i].bbox[2] && r<=cca_blob[i].bbox[3])
				{
					flag_update=false; 
					break;
				}
				else flag_update=true;
			}
			if (flag_update) 
			{
				for (l=16;l--; ) // Update the bins of the histogram that best matches the current one as long as the pixel is within the ROI and is not classified as a background pixel
				{
					temp = 0.015*foreground_lbp_hist[((r-5)*(width-10)+(c-5))*16+l]+ 0.985*hist[((r-5)*(width-10)+(c-5))*80+(index[(r-5)*(width-10)+(c-5)]*16)+l];
					hist[((r-5)*(width-10)+(c-5))*80+(index[(r-5)*(width-10)+(c-5)]*16)+l] = temp;
				}
				//for (j=b[(r-5)*(width-10)+(c-5)]+1;j--; ) // Update the weights of the matched histogram and decrease the weights of the corresponding unmatched histograms.
					//histweight[((r-5)*(width-10)+(c-5))*5+j] = 0.985*histweight[((r-5)*(width-10)+(c-5))*5+j];
				//histweight[((r-5)*(width-10)+(c-5))*5+index[(r-5)*(width-10)+(c-5)]] = histweight[((r-5)*(width-10)+(c-5))*5+index[(r-5)*(width-10)+(c-5)]]+0.015;
			}
		}
	}

	if (flag_continue)
	{
		if (drawAll)
		{
			m_fig1->Copy(*m_cxImage,true,false,false);
			m_fig2->Copy(*m_cxImage,true,false,false);
		}
		m_fig3->Copy(*m_cxImage,true,false,false);

		split_blob();

		merge_process2();

		delete_blobs();

		if (drawAll)
			drawBoundingBox();

		findCand_vehicles();

		if (drawAll)
			drawBoundingBox4();

		determineType();

		m_tracker->width=width;
		m_tracker->height=height;
		m_tracker->m_figT=m_fig3;//m_fig3
		m_tracker->drawHnd=hdc;
		m_tracker->no_blob=num_record;
		m_tracker->frameNo=curFrame;
		m_tracker->m_top=m_top;
		m_tracker->m1=m1;
		m_tracker->m2=m2;
		m_tracker->c1=c1;
		m_tracker->c2=c2;
		m_tracker->T= TFrames;
			
		m_tracker->run(Blob,firstFrame);

		firstFrame=false;
	
		sprintf(tmpStr,"%d",m_tracker->mtb_count);
		m_countListCtrl.SetItemText(0,1,tmpStr);
		sprintf(tmpStr,"%d",m_tracker->car_count);
		m_countListCtrl.SetItemText(1,1,tmpStr);
		sprintf(tmpStr,"%d",m_tracker->light_count);
		m_countListCtrl.SetItemText(2,1,tmpStr);
		sprintf(tmpStr,"%d",m_tracker->heavy_count+m_tracker->bus_count);
		m_countListCtrl.SetItemText(3,1,tmpStr);
	//	sprintf(tmpStr,"%d",m_tracker->bus_count);
	//	m_countListCtrl.SetItemText(4,1,tmpStr);
		sprintf(tmpStr,"%d",m_tracker->v_count);
		m_countListCtrl.SetItemText(4,1,tmpStr);
			
		if (strcmp(m_tracker->stats,"")!=0)
		{
			sprintf(tmpStr,"%s",m_tracker->stats);
			WriteStats(tmpStr);
		}

		if (drawAll)
			m_tracker->m_figT->Draw(hdc,width/2,height/2,width/2,height/2,0,0);
		else
			m_tracker->m_figT->Draw(hdc,0,0,width,height,0,0);

		if (saveImage && m_tracker->m_figT->IsValid())
		{
			sprintf(tmpStr, "%s\\..\\v_count\\%0.5d.jpg", m_captureFullDir,curFrame);
			m_tracker->m_figT->Save(tmpStr, CXIMAGE_FORMAT_JPG);
		}
	}
	else 
	{
		m_fig3->Copy(*m_cxImage,true,false,false);

		sprintf(tmpStr,"v_count=%d", m_tracker->v_count);
		m_fig3->DrawString(hdc,width-220, 70,tmpStr,fuchsia,"Courier",30,400,0,0,false);
		m_fig3->Draw(hdc,0,0,width,height,0,0);
		
		if (saveImage)
		{
			sprintf(tmpStr, "%s\\..\\v_count\\%0.5d.jpg", m_captureFullDir,curFrame);
			m_fig3->Save(tmpStr, CXIMAGE_FORMAT_JPG);
		}
	}
}

bool CCnt_trafficDlg::cca()
{
	CBlobResult blobs;
	CBlob ccablob;

	bool flag_continue=false;
	int min_x,max_x,min_y,max_y, r, c;

	cvZero(cca_out);
	
	for (r=height;r--; )
	{
		for (c=width;c--; )
			//((uchar *)(cca_in->imageData + r*cca_in->widthStep))[c]=lbpresultimage[(r*width)+c];
			((uchar *)(cca_in->imageData + r*cca_in->widthStep))[c]=((uchar *)(lbpresultimage1->imageData + r*lbpresultimage1->widthStep))[c];
	}

	blobs = CBlobResult(cca_in, NULL, 0, false );
	blobs.Filter( blobs, B_INCLUDE,CBlobGetMean(), B_GREATER, 0 ); 
	blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_GREATER, cca_size1);
	
	int deleted=0;
	int numBlobs = blobs.GetNumBlobs();
	for (int i=0;i<numBlobs;i++)
	{	
		blobs.GetNthBlob(CBlobGetArea(),i, ccablob);
		min_x=ccablob.MinX();
		max_x=ccablob.MaxX();
		min_y=ccablob.MinY();
		max_y=ccablob.MaxY();

		if ((height-min_y>height/2) && (ccablob.Area()<cca_size2))
		{
			deleted++;
			continue;// Skip the remaining statements in the loop and start the next iteration
		}
		ccablob.FillBlob(cca_out, CV_RGB( 255, 255, 255 ));
		cca_blob[i-deleted].bbox[0]=min_x;
		cca_blob[i-deleted].bbox[1]=max_x;
		cca_blob[i-deleted].bbox[2]=min_y;
		cca_blob[i-deleted].bbox[3]=max_y;
		cca_blob[i-deleted].mu.x=(min_x+max_x)/2;
		cca_blob[i-deleted].mu.y = (min_y+max_y)/2;
	}
	no_blob=numBlobs-deleted;
	if (no_blob==0)
		flag_continue=false;
	else
		flag_continue=true;

	//if (curFrame == 1152){
	//cvSaveImage("LBPResult1.jpeg",cca_out);
		//exit(0);
	//}
	
	for (r=height;r--; )
	{
		for (c=width;c--; )
		{
			lbpresultimage2[(r*width)+c]=((uchar *)(cca_out->imageData + r*cca_out->widthStep))[c];
		}
	}	
	if (drawAll && saveImage)
	{
		cvFlip(cca_in,cca_in,0);
		sprintf(tmpStr, "%s\\..\\cca_in\\%0.5d.jpg", m_captureFullDir,curFrame);
		cvSaveImage(tmpStr, cca_in);
		cvFlip(cca_out,cca_out,0);
		sprintf(tmpStr, "%s\\..\\cca\\%0.5d.jpg", m_captureFullDir,curFrame);
		cvSaveImage(tmpStr, cca_out);
	}
	return flag_continue;
	
}

void CCnt_trafficDlg::split_blob()
{
	float sumX=0;
	float sumY=0;
	int blobID;
	bool f_next_iter;
	float minvalue=0;
	float mindistC=0;
	float maxValue=0;
	int index=0;
	float var1=0;

	int r, c;
	len =0;
	for (c = 0; c < width; c++)
	{
		for (r = 0; r<height; r++)
		{
			if (lbpresultimage2[(r*width)+c] > 0)
			{
				data[len]=cvPoint(c,r);

				sumX+=c;
				sumY+=r;

				len++;
			}
		}
	}
	blobID=1;
	N_CLUSTER=1;

	C[0].x = sumX/len;
	C[0].y = sumY/len;


	fit_kernel(C);

	f_next_iter=false;
	f_next_iter=addNewC(C, newC);
	while (N_CLUSTER<MAX_CLUSTER && f_next_iter)
	{
		N_CLUSTER++;
		hkmean(newC,C);
		f_next_iter=addNewC(C, newC);
	}

	fit_kernel(C);
	no_blob=N_CLUSTER;
	sprintf(tmpStr, "Frame= %d	N_CLUSTER= %d", curFrame, N_CLUSTER);
	WriteLog(tmpStr);
}


void CCnt_trafficDlg::fit_kernel(CvPoint *C)
{
	float D=0;
	float minD;
	int pt=0;	
	
	//Initialization
	for (int i=0; i<N_CLUSTER; i++)
	{ 
		Blob[i].mu=cvPoint(0,0);
		Blob[i].bbox[0]= Blob[i].bbox[2]=10000;
		Blob[i].bbox[1]= Blob[i].bbox[3]=0;
		Blob[i].present=false;
		Blob[i].type=0;
	}
		
	//1: Calculate distance
	
	for (i=0; i<no_blob; i++)
	{
		minD=10000;
		pt=0;
		for (int j=0;j<N_CLUSTER;j++)
		{
            D= abs(cca_blob[i].mu.x-C[j].x) + abs(cca_blob[i].mu.y-C[j].y);
			if (D<minD)
			{
				minD=D;
				pt=j;
			}
		}

		//Update bbox
		Blob[pt].present=true;
		if (cca_blob[i].bbox[0] < Blob[pt].bbox[0])
			Blob[pt].bbox[0]=cca_blob[i].bbox[0];
		
		if (cca_blob[i].bbox[1] > Blob[pt].bbox[1])  
			Blob[pt].bbox[1]=cca_blob[i].bbox[1];
		
		if (cca_blob[i].bbox[2] < Blob[pt].bbox[2])
			Blob[pt].bbox[2]=cca_blob[i].bbox[2];
         
		if (cca_blob[i].bbox[3] > Blob[pt].bbox[3])
			Blob[pt].bbox[3]=cca_blob[i].bbox[3];
	}

	for (i=0;i<N_CLUSTER;i++)
	{
		if (Blob[i].present)
		{
			cx=(Blob[i].bbox[1]-Blob[i].bbox[0])/2;
			cy=(Blob[i].bbox[3]-Blob[i].bbox[2])/2;

			Blob[i].mu=cvPoint(cx,cy);
		}
	}

}

void CCnt_trafficDlg::hkmean(CvPoint* C1, CvPoint *newC)
{
	int pass, iter;
	float error_hk;

	pass =1;
	iter=0;
	for (int i=0; i<N_CLUSTER;i++)
	{
		hkC[i]=C1[i];
	}

    while (pass)
	{
		update_hk_C(hkC, newC); 
		error_hk=comp_hk_error(hkC,newC);
		if (error_hk>50)
		{
			for (i=0;i<N_CLUSTER;i++)
			{
				hkC[i]=newC[i];
			}
			iter=iter+1;
		}
		
		else	pass=0;
	}
}

bool CCnt_trafficDlg::addNewC(CvPoint* prevC, CvPoint *C)
{
	int numC = N_CLUSTER;
	int tmpMinD, tmpInd;
	int maxvalue, ind;
	bool f_newC;

	for (int i=0; i<numC; i++)
	{
		for (int j=0; j<len;j++)
		{
			D[i][j]=0;
		}
	}
        
	for (i=0; i<numC; i++)
	{
		for (int j=0; j<len;j++)
		{
			D[i][j]=D[i][j]+abs(data[j].x-prevC[i].x);
			D[i][j]=D[i][j]+abs(data[j].y-prevC[i].y);
		}
	}
	for (int j=0; j<len;j++)
	{
		tmpMinD=10000;
		for (int i=0; i<numC; i++)
		{	
			if (D[i][j]<tmpMinD)
			{
				tmpMinD=D[i][j];
				tmpInd=i;
			}
			dist_nearestC[j]=tmpMinD;
			Ind[j]=tmpInd;
		}
	}

	for (i=0; i<numC;i++)
	{
		cand_newC[i]=cvPoint(0,0);
		maxdistD_C[i]=0;
	}

	for (i=0;i<len;i++)
	{
		if (dist_nearestC[i]>maxdistD_C[Ind[i]])
		{
			maxdistD_C[Ind[i]]=dist_nearestC[i];
			cand_newC[Ind[i]] = data[i];
		}
	}
        
	f_newC=false;
	maxvalue=0;
	ind=-1;

	for (i=0;i<numC;i++)
	{
		lanewidth = abs(((float)(prevC[i].y-c1)/m1)- ((float)(prevC[i].y-c2)/m2));
		if (maxdistD_C[i]>C_maxdistD_C*lanewidth)
		{
			f_newC=true;
			if (maxvalue<maxdistD_C[i])
			{
				maxvalue=maxdistD_C[i];
				ind=i;
			}
		}
	}
	
	if (f_newC)
	{
		for (i=0;i<numC;i++)
		{
			C[i]=prevC[i];
		}
		
		C[numC]= cand_newC[ind];
	}
	else
	{
		for (i=0;i<numC;i++)
		{
			C[i]=prevC[i];
		}
	}
		
	return f_newC;
}

void CCnt_trafficDlg::update_hk_C(CvPoint* C, CvPoint* newC)
{ 
	int numC = N_CLUSTER;
	int tmpMinD, tmpInd;


	for (int i=0; i<numC; i++)
	{
		for (int j=0; j<len;j++)
		{
			D[i][j]=0;
			D[i][j]=D[i][j]+abs(data[j].x-C[i].x);
			D[i][j]=D[i][j]+abs(data[j].y-C[i].y);
		}
	}
        
	for (int j=0; j<len;j++)
	{
		tmpMinD=1000;
		for (int i=0; i<numC; i++)
		{	
			if (D[i][j]<tmpMinD)
			{
				tmpMinD=D[i][j];
				tmpInd=i;
			}
			minD[j]=tmpMinD;
			Ind[j]=tmpInd;
		}
	}

	for (i=0; i<numC;i++)
	{
		float sumX = 0;
		float sumY = 0;
		int count = 0;
		for (j=0;j<len;j++)
		{
			if(Ind[j]==i)
			{
				sumX+=data[j].x;
				sumY+=data[j].y;
				count++;
			}
		}
		newC[i]=cvPoint(sumX/count, sumY/count);
	}
}

float CCnt_trafficDlg::comp_hk_error(CvPoint* C, CvPoint* newC)
{
	float error_hk=0;
	int numC = N_CLUSTER;

	for (int i=0;i<numC;i++)
	{
		compD[i]=0;
		compD[i]=compD[i]+abs(C[i].x-newC[i].x);
		compD[i]=compD[i]+abs(C[i].y-newC[i].y);
		error_hk+=compD[i];
	}
	
	return error_hk;
}


void CCnt_trafficDlg::drawBoundingBox()
{	
	int top,bottom,left,right;
	for (int i=0;i<no_blob;i++)
	{
		top=Blob[i].bbox[3];
		if (top%2!=0) top=top+1;

		bottom=Blob[i].bbox[2];
		if (bottom%2!=0) bottom=bottom+1;
		
		left=Blob[i].bbox[0];
		if (left%2!=0) left=left+1;
		
		right=Blob[i].bbox[1];
		if (right%2!=0) right=right+1;

		//draw in aqua
		m_fig1->DrawLine(left, right, bottom,bottom,RGB( 0, 255, 255));
		m_fig1->DrawLine(left, right, top, top,RGB( 0, 255, 255));
		m_fig1->DrawLine(left, left, bottom, top,RGB( 0, 255, 255));
		m_fig1->DrawLine(right, right, bottom, top,RGB( 0, 255, 255));
	
	}
	if (m_fig1->IsValid() && saveImage)
	{
		sprintf(tmpStr, "%s\\..\\fig1\\%0.5d.jpg", m_captureFullDir,curFrame);
		m_fig1->Save(tmpStr, CXIMAGE_FORMAT_JPG);
	}
	
	m_fig1->Draw(hdc,0,0,width/2,height/2,0,0);
}

void CCnt_trafficDlg::findCand_vehicles()
{


	for (int i=0;i<no_blob;i++)
	{
		get_kernel(i,bbox_kernel);
		update_kernel(bbox_kernel, new_bbox_kernel);

		while (chk_err(bbox_kernel, new_bbox_kernel))
		{
			for (int j=0;j<4;j++)
			{
				bbox_kernel[j]=new_bbox_kernel[j];
			}
			update_kernel(bbox_kernel, new_bbox_kernel);
		}

       if(drawAll) 	drawBox(bbox_kernel);
        merge_bbox(bbox_kernel, buff_new_bbox[i]);
	}
	save_new_record(buff_new_bbox);

}

void CCnt_trafficDlg::get_kernel(int ID, int *bbox_kernel)
{
	int y_bbox_bottom;
	float cX;
	float cY;

	int half_width, half_height;

    y_bbox_bottom=Blob[ID].bbox[2];
    
	lanewidth = abs(((float)(y_bbox_bottom-c1)/m1)- ((float)(y_bbox_bottom-c2)/m2));
	half_width=C_half_width*lanewidth;
	half_height=C_half_height*half_width;
    
    cX=(Blob[ID].bbox[0]+Blob[ID].bbox[1])/2;
    cY=(Blob[ID].bbox[2]+Blob[ID].bbox[3])/2;

	bbox_kernel[0]=cX-half_width;
	bbox_kernel[1]=cX+half_width;
	bbox_kernel[2]=cY-half_height;
	bbox_kernel[3]=cY+half_height;
	
}

void CCnt_trafficDlg::update_kernel(int *bbox_kernel, int *new_bbox_kernel)
{
	float kernel_halfwidth, kernel_halfheight, den, num_x, num_y;
	int area;

	kernel_halfwidth=(bbox_kernel[1]-bbox_kernel[0])/2;
	kernel_halfheight=(bbox_kernel[3]-bbox_kernel[2])/2;

    den=0;
	num_x=0;   
	num_y=0;
	
	for (int i=0; i<no_blob;i++)
	{
		area=get_overlap_area(Blob[i].bbox,bbox_kernel);
		num_x=num_x+cx*area;
		num_y=num_y+cy*area;
		den=den+area;
	}
	
	if (den>0)
	{
		cx=num_x/den;
		cy=num_y/den;

		new_bbox_kernel[0]=(cx - kernel_halfwidth);
		new_bbox_kernel[1]=(cx + kernel_halfwidth);
		new_bbox_kernel[2]=(cy - kernel_halfheight);
		new_bbox_kernel[3]=(cy + kernel_halfheight);
	}
}

int CCnt_trafficDlg::get_overlap_area(int *bbox_ii,int *bbox_k)
{
	float x1=0;
	float x2=0;
	float y1=0;
	float y2=0;
	int area;

	if(bbox_k[0]>bbox_ii[1] || bbox_k[1]<bbox_ii[0] || bbox_k[2]>bbox_ii[3] || bbox_k[3]<bbox_ii[2])
		area=0;
	else
	{
		x1=max(bbox_k[0],bbox_ii[0]);
		x2=min(bbox_k[1],bbox_ii[1]);
		y1=max(bbox_k[2],bbox_ii[2]);
		y2=min(bbox_k[3],bbox_ii[3]);
		area = (x2-x1+1)*(y2-y1+1);
	}

    if (area>0)
	{
		cx=((x2*(x2+1))-((x1-1)*x1))/(2*(x2-x1+1));
        cy=((y2*(y2+1))-((y1-1)*y1))/(2*(y2-y1+1));
	}
	return area;
	
}

bool CCnt_trafficDlg::chk_err(int *bbox1, int *bbox2)
{
	int cx1, cy1, cx2, cy2, D;
	
	cx1=(bbox1[0]+bbox1[1])/2;
	cy1=(bbox1[2]+bbox1[3])/2;
	
	cx2=(bbox2[0]+bbox2[1])/2;
	cy2=(bbox2[2]+bbox2[3])/2;

	D=abs(cx1-cx2)+abs(cy1-cy2);
	
	if (D<10)	return false;
	else	return true;
}

void CCnt_trafficDlg::drawBox(int *Rect)
{
	int top,bottom,left,right;

	for (int i=0;i<N_CLUSTER;i++)
	{		
		top=Rect[3];
		if (top%2!=0) top=top+1;

		bottom=Rect[2];
		if (bottom%2!=0) bottom=bottom+1;
		
		left=Rect[0];
		if (left%2!=0) left=left+1;
		
		right=Rect[1];
		if (right%2!=0) right=right+1;

		//draw in fuchsia
		m_fig1->DrawLine(left, right, bottom,bottom,RGB(255, 0, 255));
		m_fig1->DrawLine(left, right, top, top,RGB(255, 0, 255));
		m_fig1->DrawLine(left, left, bottom, top,RGB(255, 0, 255));
		m_fig1->DrawLine(right, right, bottom, top,RGB(255, 0, 255));
	
	}
	if (m_fig1->IsValid() && saveImage)
	{
		sprintf(tmpStr, "%s\\..\\fig1\\%0.5d.jpg", m_captureFullDir,curFrame);
		m_fig1->Save(tmpStr, CXIMAGE_FORMAT_JPG);
	}
	
	m_fig1->Draw(hdc,0,0,width/2,height/2,0,0);
}

void CCnt_trafficDlg::merge_bbox(int *bbox_kernel, int *new_bbox)
{
	int area_ii, area;

	new_bbox[0]=new_bbox[2]=10000;
	new_bbox[1]=new_bbox[3]=0;

	for (int i=0; i<no_blob; i++)
	{
        area_ii=(Blob[i].bbox[1]-Blob[i].bbox[0])*(Blob[i].bbox[3]-Blob[i].bbox[2]);

		area=get_overlap_area(Blob[i].bbox,bbox_kernel);
		if (((float)area/(float)area_ii>0.25)||(Blob[i].bbox[0]<bbox_kernel[0] && Blob[i].bbox[1]>bbox_kernel[1] && Blob[i].bbox[2]<bbox_kernel[2] && Blob[i].bbox[3]>bbox_kernel[3]))
		{
			if (Blob[i].bbox[0]<new_bbox[0])
				new_bbox[0]=Blob[i].bbox[0];

			if (Blob[i].bbox[1]>new_bbox[1])
				new_bbox[1]=Blob[i].bbox[1];
			
			if (Blob[i].bbox[2]<new_bbox[2])
				new_bbox[2]=Blob[i].bbox[2];
			
			if (Blob[i].bbox[3]>new_bbox[3])
				new_bbox[3]=Blob[i].bbox[3];
			
		}
	}
}

void CCnt_trafficDlg::save_new_record(int **new_bbox)
{
	int blobID;
	bool flag_yesinrecord, flag_inside;

	num_record=0;
	for (int i=0; i<no_blob; i++)
	{
		recordBuf[i][0]=0;
		recordBuf[i][1]=0;
		recordBuf[i][2]=0;
		recordBuf[i][3]=0;
	}

	for (i=0; i<no_blob; i++)
	{
		if (i==0)
		{
			num_record++;
			for (int j=0;j<4;j++)
			{
				recordBuf[i][j]=new_bbox[i][j];
			}
		}
		else
		{
			flag_yesinrecord=false;
			for (int j=0;j<num_record;j++)
			{
				if (new_bbox[i][0]>=recordBuf[j][0] && new_bbox[i][1]<=recordBuf[j][1] && new_bbox[i][2]>=recordBuf[j][2] && new_bbox[i][3]<=recordBuf[j][3])
					flag_yesinrecord=true;
			}

			if (!flag_yesinrecord)
			{
				num_record++;
				for (int j=0;j<4;j++)
				{
					recordBuf[num_record-1][j]=new_bbox[i][j];
				}
			}
		}
	}

	for(i=0;i<num_record;i++)
	{
		for(int j=0;j<4;j++)
		{
			new_bbox[i][j]=recordBuf[i][j];
		}
	}


	blobID=-1;
	for(i=0;i<num_record;i++)
	{
		for(int j=0;j<4;j++)
		{
			recordBuf[i][j]=0;
		}
		
		flag_inside=false;
		for(j=0;j<num_record;j++)
		{
			if(j!=i)
			{
				if (new_bbox[i][0]>=new_bbox[j][0] && new_bbox[i][1]<=new_bbox[j][1] && new_bbox[i][2]>=new_bbox[j][2] && new_bbox[i][3]<=new_bbox[j][3])
					flag_inside=true;
				else if (get_overlap_area(new_bbox[i],new_bbox[j])>0.4*min((new_bbox[i][1]-new_bbox[i][0])*(new_bbox[i][3]-new_bbox[i][2]),(new_bbox[j][1]-new_bbox[j][0])*(new_bbox[j][3]-new_bbox[j][2])))
				{
					if (new_bbox[j][0]<new_bbox[i][0])
						new_bbox[i][0]=new_bbox[j][0];
					if (new_bbox[j][1]>new_bbox[i][1])
						new_bbox[i][1]=new_bbox[j][1];
					if (new_bbox[j][2]<new_bbox[i][2])
						new_bbox[i][2]=new_bbox[j][2];
					if (new_bbox[j][3]>new_bbox[i][3])
						new_bbox[i][3]=new_bbox[j][3];
				}
			}
		}
		if (!flag_inside)
		{
			blobID++;
			for (j=0;j<4;j++)
			{
				Blob[blobID].bbox[j]=new_bbox[i][j];
				Blob[blobID].mu= cvPoint((new_bbox[i][0]+new_bbox[i][1])/2,(new_bbox[i][2]+new_bbox[i][3])/2);
			}
		}
	}
	num_record=blobID+1;

}

void CCnt_trafficDlg::drawBoundingBox4()
{
	int top,bottom,left,right;
	for (int i=0;i<no_blob;i++)
	{
		top=Blob[i].bbox[3];
		if (top%2!=0) top=top+1;

		bottom=Blob[i].bbox[2];
		if (bottom%2!=0) bottom=bottom+1;
		
		left=Blob[i].bbox[0];
		if (left%2!=0) left=left+1;
		
		right=Blob[i].bbox[1];
		if (right%2!=0) right=right+1;

		//draw in fuchsia
		m_fig2->DrawLine(left, right, bottom,bottom,RGB(255, 0, 255));
		m_fig2->DrawLine(left, right, top, top,RGB(255, 0, 255));
		m_fig2->DrawLine(left, left, bottom, top,RGB(255, 0, 255));
		m_fig2->DrawLine(right, right, bottom, top,RGB(255, 0, 255));
	
	}
	if (m_fig2->IsValid() && saveImage)
	{
		sprintf(tmpStr, "%s\\..\\fig2\\%0.5d.jpg", m_captureFullDir,curFrame);
		m_fig2->Save(tmpStr, CXIMAGE_FORMAT_JPG);
	}
	
	m_fig2->Draw(hdc,width/2,0,width/2,height/2,0,0);
}


void CCnt_trafficDlg::OnButtonSelectPoint() 
{
	// TODO: Add your control notification handler code here
	CCalibration m_selectPoint;
	if (m_liveFlag)	
	{
		m_gui->Copy(*m_cxImage,true,false,false);
	}
	else
	{
		for(int i=0;i<20;i++)
		{
			sprintf(tmpStr, "%s\\%0.5d.jpg",m_szPath, i);
			if (m_gui->Load(tmpStr,CXIMAGE_FORMAT_JPG))
				break;
		}
	}
	
	m_selectPoint.gui=m_gui;		
	m_selectPoint.DoModal();

	froi=fopen(m_roiText,"w");
	fprintf(froi,"%d\t%d\n%d\t%d\n%d\t%d\n%d\t%d\n",
		m_selectPoint.point1[0].x,m_selectPoint.point1[0].y,
		m_selectPoint.point1[1].x,m_selectPoint.point1[1].y,
		m_selectPoint.point1[2].x,m_selectPoint.point1[2].y,
		m_selectPoint.point1[3].x,m_selectPoint.point1[3].y);

	fclose(froi);
	m1=(float)(m_selectPoint.point1[1].y-m_selectPoint.point1[0].y)/(m_selectPoint.point1[1].x-m_selectPoint.point1[0].x);
	c1=m_selectPoint.point1[1].y-(m1*m_selectPoint.point1[1].x);
	m2=(float)(m_selectPoint.point1[2].y-m_selectPoint.point1[3].y)/(m_selectPoint.point1[2].x-m_selectPoint.point1[3].x);
	c2=m_selectPoint.point1[2].y-(m2*m_selectPoint.point1[2].x);

	m_buttonrun.EnableWindow(true);
}


void CCnt_trafficDlg::merge_process2()
{
	int pt, size_bbox_ii, size_bbox_jj, ID_ii, ID_jj,size_bbox_ov;
	bool pass;
	float ratio_ov;
	for (int i=0;i<1000;i++)
	{
		merge_list[i]=cvPoint(0,0);
	}

	pt=0;
	pass=true;
	while (pass)
	{
		pass=false;      
        for (i=0;i<no_blob-1;i++)
		{
			if (Blob[i].present)
			{
				size_bbox_ii=get_bbox_size(Blob[i].bbox);
               
               for (int j=i+1;j<no_blob;j++)
			   {
                   if (Blob[j].present)
				   {
					   size_bbox_jj=get_bbox_size(Blob[j].bbox);
					   size_bbox_ov=get_overlap_area(Blob[i].bbox,Blob[j].bbox);

					   if (size_bbox_ii<size_bbox_jj)
						   ratio_ov=(float)size_bbox_ov/size_bbox_ii;
					   else
						   ratio_ov=(float)size_bbox_ov/size_bbox_jj;
                   
					   if (ratio_ov>0.5)
					   {
						   pt++;
						   merge_list[pt-1]=cvPoint(i,j);
						   pass=true;
					   }
				   }
			   }
			}
		}

        // Merge blob
        for (i=0; i<pt; i++)
		{
           ID_ii=merge_list[i].x;
           ID_jj=merge_list[i].y;
           
           if (Blob[ID_ii].bbox[0]>Blob[ID_jj].bbox[0])
			   Blob[ID_ii].bbox[0]=Blob[ID_jj].bbox[0];
           
           if (Blob[ID_ii].bbox[1]<Blob[ID_jj].bbox[1])
			   Blob[ID_ii].bbox[1]=Blob[ID_jj].bbox[1];

           if (Blob[ID_ii].bbox[2]>Blob[ID_jj].bbox[2])
			   Blob[ID_ii].bbox[2]=Blob[ID_jj].bbox[2];

		   if(Blob[ID_ii].bbox[3]<Blob[ID_jj].bbox[3])
			   Blob[ID_ii].bbox[3]=Blob[ID_jj].bbox[3];

           Blob[ID_ii].present=true;
           Blob[ID_jj].present=false;
		}
	}
}


void CCnt_trafficDlg::delete_blobs()
{
	int ID=-1;
	for (int i=0; i<no_blob;i++)
	{
		int pixelcount=0;
		int bgcount=0;
		for (int r=Blob[i].bbox[2];r<=Blob[i].bbox[3];r++)
		{
			for (int c=Blob[i].bbox[0]; c<=Blob[i].bbox[1];c++)
			{
				if (((uchar *)(lbpresultimage1->imageData + r*lbpresultimage1->widthStep))[c]==255)
				{
					pixelcount++;
					if (max_foreground[(r-5)*(width-10)+(c-5)]>=0.77)
						bgcount++;
				}
			}
		}
		if ((float)bgcount/pixelcount>0.5)
			continue;

		if (Blob[i].bbox[1]>((Blob[i].bbox[2]-c1)/m1)+10 && Blob[i].bbox[0]<((Blob[i].bbox[2]-c2)/m2)-10 && Blob[i].bbox[2]<m_top)
		{
			ID++;
			Blob[ID].bbox[0]=Blob[i].bbox[0];
			Blob[ID].bbox[1]=Blob[i].bbox[1];
			Blob[ID].bbox[2]=Blob[i].bbox[2];
			Blob[ID].bbox[3]=Blob[i].bbox[3];
			Blob[ID].present=Blob[i].present;
		}
	}
	no_blob=ID+1;
}




int CCnt_trafficDlg::get_bbox_size(int *bbox_ii)
{
	int deltaX=bbox_ii[1]-bbox_ii[0];
	int deltaY=bbox_ii[3]-bbox_ii[2];

	int size_bbox=0;

	if (deltaX!=0 && deltaY!=0)
		size_bbox=(deltaX+1)*(deltaY+1);

	return size_bbox;
}

void CCnt_trafficDlg::determineType()
{
	int box_length;
	box_length=0;
	for (int i=0;i<num_record;i++)
	{
		box_length=Blob[i].bbox[3]-Blob[i].bbox[2];
		//heavy
		if (box_length>250)
			Blob[i].type=4;
		else
		{
			if (Blob[i].bbox[2]<5||Blob[i].bbox[3]>m_top-30)
				continue;
			
			lanewidth = abs(((float)(Blob[i].bbox[2]-c1)/m1)- ((float)(Blob[i].bbox[2]-c2)/m2));
			
			if (box_length<C_mtb_length*lanewidth)
				Blob[i].type=1;
			else if (box_length<C_car_length*lanewidth)
				Blob[i].type=2;
			else if (box_length<C_light_length*lanewidth)
				Blob[i].type=3;
			else //heavy
				Blob[i].type=4;
		}
	}
}

void CCnt_trafficDlg::readROIfile()
{
	char buf[100];
	CvPoint point[4];
	int pos;
	
	froi=fopen(m_roiText,"r");
	if (!froi)
	{
		MessageBox("Please select ROI.");
		m_buttonrun.EnableWindow(false);
	}
	else 
	{
		fread(buf, sizeof(char), 100, froi);
		CString str = CString(buf);

		for (int i=0; i<4; i++)
		{
			pos = str.Find("\t");
			point[i].x = atoi(str.Left(pos));
			str = str.Mid(pos+1,str.GetLength()-pos);
			pos = str.Find('\n');
			point[i].y = atoi(str.Mid(0, pos));
			str = str.Mid(pos+1,str.GetLength()-pos);
		}
		fclose(froi);
		
		m1=(float)(point[1].y-point[0].y)/(point[1].x-point[0].x);
		c1=point[1].y-(m1*point[1].x);
		m2=(float)(point[3].y-point[2].y)/(point[3].x-point[2].x);
		c2=point[3].y-(m2*point[3].x);
	
	}
}

void CCnt_trafficDlg::getParams()
{
	FILE* paramFile;
	char param1[100];
	char param2[100];

	paramFile = fopen("parameters.txt", "r" );
	if (!paramFile)
	{
		MessageBox("No parameter file found!");
		return;
	}
	else
	{
		while(!feof(paramFile))
		{
			fscanf(paramFile, "%s %s", param1, param2);

			if (_stricmp(param1, "TM_LOG_DIR") == 0)
			{
				sprintf(m_logDir, "%c:\\TM_Log", param2[0]);
				continue;
			}
			
			if (_stricmp(param1, "TM_STATS_DIR") == 0)
			{
				sprintf(m_statsDir, "%c:\\TM_Stats", param2[0]);
				continue;
			}
				
			if (_stricmp(param1, "TM_CAPTURE_DIR") == 0)
			{			
				sprintf(m_captureDir, "%c:\\TM_Capture", param2[0]);
				continue;
			}

			if (_stricmp(param1, "MTB_LENGTH_COEFF") == 0)
			{
				C_mtb_length = atof(param2);
				continue;
			}

			if (_stricmp(param1, "CAR_LENGTH_COEFF") == 0)
			{
				C_car_length = atof(param2);
				continue;
			}

			if (_stricmp(param1, "LIGHT_LENGTH_COEFF") == 0)
			{
				C_light_length = atof(param2);
				continue;
			}
				
			if (_stricmp(param1, "KERNEL_HALF_WIDTH_COEFF") == 0)
			{
				C_half_width = atof(param2);
				continue;
			}

			if (_stricmp(param1, "KERNEL_HALF_HEIGHT_COEFF") == 0)
			{
				C_half_height = atof(param2);
				continue;
			}

			if (_stricmp(param1, "CCA_SIZE1") == 0)
			{
				cca_size1 = atoi(param2);
				continue;
			}

			if (_stricmp(param1, "CCA_SIZE2") == 0)
			{
				cca_size2 = atoi(param2);
				continue;
			}

			if (_stricmp(param1, "MAXDISTD_C_COEFF") == 0)
			{
				C_maxdistD_C = atof(param2);
				continue;
			}

			if (_stricmp(param1, "FRAME_DIFF_THRESHOLD") == 0)
			{
				T_Fdiff = atoi(param2);
				continue;
			}

			if (_stricmp(param1, "EDGE_THRESHOLD") == 0)
			{
				T_edge = atoi(param2);
				continue;
			}

			if (_stricmp(param1, "DELTA_THRESHOLD") == 0)
			{
				deltaT = atoi(param2);
				continue;
			}
				
			if (_stricmp(param1, "MAX_CLUSTER") == 0)
			{
				MAX_CLUSTER = atoi(param2);
				continue;
			}
			
			if (_stricmp(param1, "MAX_CCA_BLOB") == 0)
			{
				MAX_CCA_BLOB = atoi(param2);
				continue;
			}
				
			if (_stricmp(param1, "DISK_SPACE_LEFT") == 0)
			{
				m_diskSpaceLeft = atoi(param2);
				if (m_diskSpaceLeft < 5)
				{
					m_diskSpaceLeft = 3;
				}
			}
			
			if (_stricmp(param1, "IP_ADDRESS") == 0)
			{
				sprintf(m_liveAdd, "http://%s/axis-cgi/mjpg/video.cgi",param2);
				continue;
			}

			if (_stricmp(param1, "ROI_IMAGE") == 0)
			{
				sprintf(m_roiImage, "%s", param2);
				continue;
			}
			if (_stricmp(param1, "ROI_TEXT") == 0)
			{
				sprintf(m_roiText, "%s", param2);
				continue;
			}
			
			if (_stricmp(param1, "TOP") == 0)
			{
				m_top=atoi(param2);
				continue;
			}
			if (_stricmp(param1, "TFRAMES") == 0)
			{
				TFrames = atoi(param2);
				continue;
			}

			if (_stricmp(param1, "OUTBOX") == 0)
			{
				sprintf(m_outbox, "%s", param2);
				continue;
			}
			else 
				sprintf(m_outbox,"");
		}
		fclose(paramFile);
	}
	
	_mkdir(m_logDir);
	_mkdir(m_statsDir);
	_mkdir(m_captureDir);
}

void CCnt_trafficDlg::OnButtonLive() 
{
	// TODO: Add your control notification handler code here
	CreateFolder();
	
	//AMC method
	this->UpdateData();

	// Set the media URL and the media type
	m_AMC.SetMediaURL(m_liveAdd);
	m_AMC.SetMediaType("mjpeg-unicast");
	m_AMC.SetEnableReconnect(true);
	
	try
	{
		// Starts the download of the mjpeg stream from the Axis camera/video server
		m_AMC.Play(); 
	}
	catch (COleDispatchException *e)
	{
		if (e->m_scError == E_INVALIDARG)
		{
		  MessageBox("Invalid parameters.");
		}
		else if (e->m_scError == E_ACCESSDENIED)
		{
		  MessageBox("Access denied.");
		}
		else
		{
		  MessageBox("Failure.");
		}
	}
	//Sleep(1000);
	try
	{
		// Save the image currently being displayed to the specified file.
		m_AMC.GetCurrentImage(0,&m_AMCbuffer,&bufSize);
		if (bufSize<=0)
		{
			MessageBox("No camera found. Please check IP address");
			WriteLog("Connection to IP camera fails");
			return;
		}
		SafeArrayAccessData(m_AMCbuffer.parray, (void**)&aBuffer); 
		m_cxImage->Decode(aBuffer,m_AMCbuffer.parray->rgsabound[0].cElements,CXIMAGE_FORMAT_JPG);
		SafeArrayUnaccessData(m_AMCbuffer.parray);
	}
	catch (COleDispatchException *e)
	{
		if (e->m_scError == E_INVALIDARG)
		{
		  MessageBox("Invalid parameters.");
		}
		else
		{
		  MessageBox("Failure.");
		}
	}
	
	m_bg->Copy(*m_cxImage,true,false,false);		

	if (saveImage)
	{
		sprintf(tmpStr, "%s\\init.jpg",m_captureFullDir);
		m_cxImage->Save(tmpStr,CXIMAGE_FORMAT_JPG);
	}	
		m_cxImage->Draw(hdc,0,0,width,height,0,0);
	
	m_liveFlag = true;
	m_buttonrun.EnableWindow(true);
	strcpy(m_szPath,m_captureFullDir);
}

void CCnt_trafficDlg::CreateFolder() 
{
	if (drawAll==true)
	{
		sprintf(tmpStr,"%s\\..\\fig1",m_captureFullDir);
		_mkdir(tmpStr);
		sprintf(tmpStr,"%s\\..\\fig2",m_captureFullDir);
		_mkdir(tmpStr);
		sprintf(tmpStr,"%s\\..\\cca",m_captureFullDir);
		_mkdir(tmpStr);
		sprintf(tmpStr,"%s\\..\\cca_in",m_captureFullDir);
		_mkdir(tmpStr);
	}
	sprintf(tmpStr,"%s\\..\\v_count",m_captureFullDir);
	_mkdir(tmpStr);
	
}

void CCnt_trafficDlg::WriteLog(const char *message)
{
	// This routine should only be called after m_logDir is declared
	// Checking is not done so as to speed up the routine.

	CFileException	ex;
	CFileException	fileException;
	char			exceptionMessage[MAX_PATH];
	char			messageStr[MAX_PATH];
	char			filename[MAX_PATH];
	char			timeStamp[MAX_PATH];

	try
	{
		CTime currentTime = CTime::GetCurrentTime();
		sprintf(filename, "%s\\TM_%d_%02d_%02d.txt", m_logDir, currentTime.GetYear(), currentTime.GetMonth(), currentTime.GetDay());
		sprintf(timeStamp, "%02d:%02d:%02d", currentTime.GetHour(), currentTime.GetMinute(), currentTime.GetSecond());
		WriteFile(filename, timeStamp, message);
	}
	catch (CException *ex)
	{
		ex->GetErrorMessage(exceptionMessage, MAX_MESSAGE_SIZE);
		sprintf(messageStr, "CCnt_trafficDlg::writeLog %s", exceptionMessage);
		WriteLog(messageStr);
		ex->Delete();
	}
}

void CCnt_trafficDlg::WriteFile(const char *filename, const char *timeStamp, const char *message)
{
	CFile			logFile;
	CFileException	ex;
	CFileException	fileException;
	char			exceptionMessage[MAX_MESSAGE_SIZE];
	char			messageStr[MAX_MESSAGE_SIZE];
	char			tmpStr[1000];
	int				strLength;

	try
	{
		if (!logFile.Open(filename, CFile::shareDenyWrite | CFile::modeWrite, &ex))
		{
			if (!logFile.Open(filename, CFile::modeCreate | CFile::modeWrite, &ex))
			{
				return;
			}
		}

		logFile.SeekToEnd(); // append new entry at the end of log file.
		if (strcmp(message, "") != 0)
		{
			sprintf(tmpStr, "%s %s", timeStamp, message);
		}
		else
		{
			strcpy(tmpStr, message);
		}
		strLength = strlen(tmpStr);
		tmpStr[strLength] = 13;
		tmpStr[strLength+1] = 10;
		logFile.Write(tmpStr, (strLength+2));
		logFile.Close();
	}
	catch (CException *ex)
	{
		ex->GetErrorMessage(exceptionMessage, MAX_MESSAGE_SIZE);
		sprintf(messageStr, "CCnt_trafficDlg::writeLog %s", exceptionMessage);
		WriteLog(messageStr);
		ex->Delete();
	}
}

BOOL CCnt_trafficDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	cleanUp();
	return CDialog::DestroyWindow();
}

void CCnt_trafficDlg::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	m_stopFlag = true;
	m_garbageCollectionFlag = false;
	
	m_buttonopen.EnableWindow(true);
	m_buttonLive.EnableWindow(true);
}

void CCnt_trafficDlg::setCapturePath()
{
	int		year;
	int		month;
	int		day;
	int		hour;
	int		min;
	int		sec;

	CTime currentTime = CTime::GetCurrentTime();

	year  = currentTime.GetYear();
	month = currentTime.GetMonth();
	day   = currentTime.GetDay();
	hour  = currentTime.GetHour();
	min   = currentTime.GetMinute();
	sec   = currentTime.GetSecond();

	sprintf(m_captureFullDir, "%s\\_%04d_%02d_%02d_%02d",
							   m_captureDir, year, month, day, hour);
	_mkdir(m_captureFullDir);
	sprintf(m_captureFullDir, "%s\\imgs",m_captureFullDir);
	_mkdir(m_captureFullDir);
	CreateFolder();
}

void CCnt_trafficDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if ((nIDEvent == 1)  && (m_liveFlag))
	{
		updateUpTime();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CCnt_trafficDlg::updateUpTime()
{
	CFileException	fileException;
	CTime			currTime;
	char			exceptionMessage[MAX_MESSAGE_SIZE];
	char			messageStr[MAX_MESSAGE_SIZE];
	try
	{
		currTime = CTime::GetCurrentTime();
		if (currTime.GetHour() != m_previousHour)
		{
			m_flagNextHour=true;
			m_previousHour = currTime.GetHour();
			if (currTime.GetDay()!=m_previousDay)
			{
				m_flagNextDay=true;
				m_previousDay=currTime.GetDay();
			}
		}
		if (ceil(currTime.GetMinute()/10)*10!=m_previousMinute)
		{
			m_flagNextMinute=true;
			m_previousMinute=ceil(currTime.GetMinute()/10)*10;
		}
	}
	catch (CException *ex)
	{
		ex->GetErrorMessage(exceptionMessage, MAX_MESSAGE_SIZE);
		sprintf(messageStr, "CCnt_trafficDlg::updateUpTime(1) %s", exceptionMessage);
		WriteLog(messageStr);
		ex->Delete();
	}
}

void CCnt_trafficDlg::WriteStats(const char *message)
{
	CFileException	ex;
	char			exceptionMessage[MAX_PATH];
	char			messageStr[MAX_PATH];
	char			tmpStr[1000];
//	char			timeStamp[MAX_PATH];
	int				strLength;
	int 			min;
	char			outFilename[1000];

	try
	{
		CTime currentTime = CTime::GetCurrentTime();
		min=ceil(currentTime.GetMinute()/10)*10;
		//TMS-OB-OVC-01_aeslog_20070424120610
		sprintf(tmpStr, "%s\\TMS-OB-OVC-01_aeslog_%d%02d%02d%02d%02d.txt", m_statsDir, currentTime.GetYear(), currentTime.GetMonth(), currentTime.GetDay(), currentTime.GetHour(),min);

		if (strcmp(tmpStr, m_statsFilename) == 0)
		{
			// write to the same file.
			if (m_statsFile.m_hFile == CFile::hFileNull)
			{
				return;
			}
		}
		else
		{
			// close old file and start a new one
			m_statsFile.Close();
			if (strcmp(m_outbox,"")!=0)
			{	
				sprintf(outFilename,"%s\\TMS-OB-OVC-01_aeslog_%d%02d%02d%02d%02d.txt",m_outbox,currentTime.GetYear(), currentTime.GetMonth(), currentTime.GetDay(), currentTime.GetHour(),min);
				DWORD error =CopyFile(m_statsFilename, outFilename,0);
			}
			strcpy(m_statsFilename, tmpStr);
			if (!m_statsFile.Open(m_statsFilename, CFile::shareDenyWrite | CFile::modeWrite, &ex))
			{
				if (!m_statsFile.Open(m_statsFilename, CFile::modeCreate | CFile::modeWrite, &ex))
				{
					// error
				}
			}
			m_statsFile.SeekToEnd(); // append new entry at the end of file.
		}
	//	sprintf(timeStamp, "%02d/%02d/%d %02d:%02d:%02d,",currentTime.GetMonth(),currentTime.GetDay(), currentTime.GetYear(),currentTime.GetHour(), currentTime.GetMinute(), currentTime.GetSecond());
		if (strcmp(message, "") != 0)
		{
		//	sprintf(tmpStr, "%s %s", timeStamp, message);
			sprintf(tmpStr, "%s", message);
			strLength = strlen(tmpStr);
		//	tmpStr[strLength] = 13;
		//	tmpStr[strLength+1] = 10;
			if (m_statsFile.m_hFile == CFile::hFileNull)
			{
				return;
			}
		//	m_statsFile.Write(tmpStr, (strLength+2));
			m_statsFile.Write(tmpStr, (strLength));
		}
	}
	catch (CException *ex)
	{
		ex->GetErrorMessage(exceptionMessage, MAX_MESSAGE_SIZE);
		sprintf(messageStr, "CCnt_trafficDlg::WriteStats %s", exceptionMessage);
		WriteLog(messageStr);
		ex->Delete();
	}
}

void CCnt_trafficDlg::maintainDiskSpace()
{
	BOOL			bResult;
	DWORD			freeDiskSpace;
	DWORDLONG		freeSpace;
	char			tmpStr[MAX_MESSAGE_SIZE];

	bResult = getFreeDiskSpace(m_captureDrive, freeSpace);
	if (bResult == FALSE) 
	{
		// drive don't exists
		return;
	}
	freeDiskSpace = (DWORD)(freeSpace / MB_SIZE); 
	if (freeDiskSpace < (m_diskSpaceLeft * 1024)) //m_diskSpaceLeft(GB)
	{
		sprintf(tmpStr, "free disk space = %d MB, garbage collection starts", freeDiskSpace);
		WriteLog(tmpStr);
		m_deleteCount = 0;
		deleteFirstFolder(m_captureDir);
	}
}

BOOL CCnt_trafficDlg::getFreeDiskSpace(const CString& sPath, DWORDLONG& rdwFreeSpace)
{
    BOOL bResult = FALSE;

    // Load the kernel to gain access to the functions we want
    // If this fails, something's REALLY wrong...
    HMODULE hKernel = ::GetModuleHandle( _T("Kernel32.dll") );
    ASSERT(NULL != hKernel);

    if (NULL != hKernel)
    {
#ifdef _UNICODE
        LPFNGETSPACEEX	pfnGetDiskFreeSpaceEx = 
             (LPFNGETSPACEEX)::GetProcAddress(hKernel, "GetDiskFreeSpaceExW" );
        LPFNGETSPACE	pfnGetDiskFreeSpace = 
             (LPFNGETSPACE)::GetProcAddress(hKernel, "GetDiskFreeSpaceW" );
#else
        LPFNGETSPACEEX	pfnGetDiskFreeSpaceEx = 
             (LPFNGETSPACEEX)::GetProcAddress(hKernel, "GetDiskFreeSpaceExA" );
        LPFNGETSPACE	pfnGetDiskFreeSpace = 
             (LPFNGETSPACE)::GetProcAddress(hKernel, "GetDiskFreeSpaceA" );
#endif

        ASSERT (NULL != pfnGetDiskFreeSpaceEx);
        if (NULL != pfnGetDiskFreeSpaceEx)
        {
            ULARGE_INTEGER nCallerFreeBytes; // Receives the number of bytes on
                                             // disk available to the caller
            ULARGE_INTEGER nDiskSize;        // Receives the number of bytes on disk
            ULARGE_INTEGER nTotalFreeBytes;  // Receives the total free bytes on 
                                             // the disk

            bResult = pfnGetDiskFreeSpaceEx(sPath,
                                            &nCallerFreeBytes,
                                            &nDiskSize,
                                            &nTotalFreeBytes);
            if (bResult)
            {
                rdwFreeSpace = nCallerFreeBytes.QuadPart;
            }
        }

        // Failing that try the old fashioned way...
	if (!bResult)
        {
            DWORD dwSectorsPerCluster;
            DWORD dwBytesPerSector;
            DWORD dwFreeClusters;
            DWORD dwTotalClusters;

            bResult = pfnGetDiskFreeSpace(sPath,
                                          &dwSectorsPerCluster,
                                          &dwBytesPerSector,
                                          &dwFreeClusters,
                                          &dwTotalClusters);
            if (bResult)
            {
                rdwFreeSpace = dwFreeClusters * dwSectorsPerCluster * dwBytesPerSector;
            }
        }
    }

    return bResult;
}

void CCnt_trafficDlg::deleteFirstFolder(char *currDir)
{
	BOOL			bWorking;
	CFileException	fileException;
	CFileFind		finder;
	CString			cStr;
	char			exceptionMessage[MAX_MESSAGE_SIZE];
	char			messageStr[MAX_MESSAGE_SIZE];
	char			tmpStr[MAX_MESSAGE_SIZE];
	char			nextDir[MAX_MESSAGE_SIZE];
	int				returnValue;

	try
	{
		sprintf(tmpStr, "%s\\*.*", currDir);
		bWorking = finder.FindFile(tmpStr);
		while(bWorking)
		{
			bWorking = finder.FindNextFile();
			cStr = finder.GetFileName();
			if (finder.IsDirectory())
			{
				if ((cStr == ".") || (cStr == ".."))
				{
					// skip
				}
				else
				{
					sprintf(nextDir, "%s\\%s", currDir, cStr.GetBufferSetLength(cStr.GetLength()));
					deleteFirstFolder(nextDir);
					if (m_deleteCount > DELETE_COUNT)
					{
						return;
					}
					returnValue = _rmdir(nextDir);
					if (returnValue == 0)
					{
						// directory is removed, otherwise it is either non-empty
						// or the directory is locked by some other process.
						sprintf(tmpStr, "Deleted directory %s", nextDir);
						WriteLog(tmpStr);
						m_deleteCount++;
					}
					else if (returnValue == -1)
					{
						sprintf(tmpStr, "%s: %s", nextDir, strerror(errno));
						WriteLog(tmpStr);
					}
				}
			}
			else
			{
				sprintf(tmpStr, "%s\\%s", currDir, cStr.GetBufferSetLength(cStr.GetLength()));
				cStr.ReleaseBuffer();
				CFile::Remove(tmpStr);
				m_deleteCount++;
				if (m_deleteCount > DELETE_COUNT)
				{
					sprintf(tmpStr, "Deleted %d files from %s", DELETE_COUNT, currDir);
					WriteLog(tmpStr);
					return;
				}
			}
		}
	//	finder.~CFileFind();
	}
	catch (CException *ex)
	{
		ex->GetErrorMessage(exceptionMessage, MAX_MESSAGE_SIZE);
		sprintf(messageStr, "CCnt_trafficDlg::DeleteFirstFolder() %s", exceptionMessage);
		WriteLog(messageStr);
		ex->Delete();
	}
	return;
}

void CCnt_trafficDlg::OnButtonChange() 
{
	// TODO: Add your control notification handler code here
	CString text;
	m_edit.GetWindowText(text);
	T_edge=atoi(text);
	sprintf(tmpStr, "Edge threshold changed to %d", T_edge);
	WriteLog(tmpStr);
	
}

/*inline float CCnt_trafficDlg::Interpolate_At_Ptr(int* upperLeft, int i, int columns)
{
	float dx = 1-offsets[i].x;
	float dy = 1-offsets[i].y;
	return
		*upperLeft*dx*dy +
		*(upperLeft+1)*offsets[i].x*dy +
		*(upperLeft+columns)*dx*offsets[i].y +
		*(upperLeft+columns+1)*offsets[i].x*offsets[i].y;
}*/

/*void CCnt_trafficDlg::Calculate_Points(void)
{
	float step = 2 * PI / bits, tmpX, tmpY;
	int i;
	for (i=bits;i--; )
		{
			tmpX = predicate * cos(i * step);
			tmpY = predicate * sin(i * step);
			points[i].x = (int)tmpX;
			points[i].y = (int)tmpY;
			offsets[i].x = tmpX - points[i].x;
			offsets[i].y = tmpY - points[i].y;
			if (offsets[i].x < 1.0e-10 && offsets[i].x > -1.0e-10) // rounding error
				offsets[i].x = 0;
			if (offsets[i].y < 1.0e-10 && offsets[i].y > -1.0e-10) // rounding error
				offsets[i].y = 0;
			
			if (tmpX < 0 && offsets[i].x != 0)
				{
					points[i].x -= 1;
					offsets[i].x += 1;
				}
			if (tmpY < 0 && offsets[i].y != 0)
				{
					points[i].y -= 1;
					offsets[i].y += 1;
				}
		}
}*/

/*void CCnt_trafficDlg::LBP(int* data, unsigned int rows, unsigned int columns, unsigned int* histogram)
{
	//Set up a circularly indexed neighborhood using 5 pointers.
	
	int leap = columns*predicate;

	int *p0 = data,
		*p1 = p0 + 2*predicate,
		*p2 = p1 + 2*leap,
		*p3 = p2 - 2*predicate,
		*centre = p3 - leap + predicate;
	unsigned int value;
	int pred2 = predicate <<1;
	int r,c;

	memset(histogram,0,sizeof(unsigned int)*64); // Clear result histogram

	for (r=rows-pred2;r--; )
	{
		for (c=columns-pred2;c--; )
		{
			value = 0;
			
			//Unrolled loop
			compab_mask_inc(p0,0);
			compab_mask_inc(p1,1);
			compab_mask_inc(p2,2);
			compab_mask_inc(p3,3);
			centre++;
	
			histogram[value]++; //Increase histogram bin value
		}
		p0 += pred2;
		p1 += pred2;
		p2 += pred2;
		p3 += pred2;
		centre += pred2;
	}
}*/

void CCnt_trafficDlg::Q_Sort(float* numbers, int left, int right)
{
  int l_hold, r_hold;

  float pivot;

  l_hold = left;
  r_hold = right;
  pivot = numbers[left];
  while (left < right)
  {
    while ((numbers[right] >= pivot) && (left < right))
      right--;
    if (left != right)
    {
      numbers[left] = numbers[right];
      left++;
    }
    while ((numbers[left] <= pivot) && (left < right))
      left++;
    if (left != right)
    {
      numbers[right] = numbers[left];
      right--;
    }
  }
  numbers[left] = pivot;
  pivot = left;
  left = l_hold;
  right = r_hold;
  if (left < pivot)
    Q_Sort(numbers, left, pivot-1);
  if (right > pivot)
    Q_Sort(numbers, pivot+1, right);
}
