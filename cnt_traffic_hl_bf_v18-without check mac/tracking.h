// tracking.h: interface for the CTracking class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRACKING_H__940778AE_636B_4160_A6CD_25D1D480E1D4__INCLUDED_)
#define AFX_TRACKING_H__940778AE_636B_4160_A6CD_25D1D480E1D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ximage.h"
#include "cv.h"
#include "highgui.h"
#include "BlobResult.h"

#define MAX_OBJ 40

struct structObj
{ 
	 int bbox[4];
	 int bbox_old[4];
	 int trackHistory;
	 int lapse2remove;
	 bool gaveresult;
	 int type;
	 int prevType;
	 int typeCount[5];
	 bool changeresult;
	 int lane;
	 int dist;
	 int dist0;
	 int speed;
};

struct structBlob
{
	 CvPoint mu;
	 int bbox[4];
	 bool present;
	 int type;
};


class CTracking  
{
public:
	CTracking();
	virtual ~CTracking();
	void	run(structBlob* Blob, bool firstFrame);
	void	update_track(structBlob *Blob);
	int		get_unusedID();
	void	init_track(structBlob *Blob);
	void	drawBoundingBox(structBlob* Blob);
	void	giveVehicleCount();
	void	init();
	
	int					no_blob;
	unsigned int		v_count;
	CxImage				*m_figT;
	HDC					drawHnd;
	int					width;
	int					height;
	structObj			*Obj;
	int					*Obj_ID;
	RGBQUAD				fuchsia;
	RGBQUAD				lime;
	RGBQUAD				black;
	int					frameNo;
	int					m_top;
	
	unsigned int		mtb_count,car_count,light_count,heavy_count,bus_count;
	char stats[1000];
	float		m1,m2,c1,c2;
	int T;

private:
	char ID_str[50];
	char speed_str[50];
	char type_str[50];
	char tmpStr[1000];
	float lanewidth;
	int lane;
	int shift;
	int currenttime, oldtime;
	float time;
	int get_overlap_area(int *bbox_ii,int *bbox_k);
};

#endif // !defined(AFX_TRACKING_H__940778AE_636B_4160_A6CD_25D1D480E1D4__INCLUDED_)
