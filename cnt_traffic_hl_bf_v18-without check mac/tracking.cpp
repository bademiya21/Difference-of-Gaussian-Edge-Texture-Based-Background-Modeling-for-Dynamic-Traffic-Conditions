// tracking.cpp: implementation of the CTracking class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "tracking.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTracking::CTracking()
{	
	Obj=new structObj [MAX_OBJ];
}

CTracking::~CTracking()
{
	if (Obj) delete [] Obj;

}

void CTracking::init()
{

	no_blob=0;
	v_count=0;
	width=0;
	height=0;
	frameNo=0;
	mtb_count=0;
	car_count=0;
	light_count=0;
	heavy_count=0;
	bus_count=0;
	lanewidth=0;
	lane=0;
	shift=0;

}
void CTracking::run(structBlob* Blob,bool firstFrame)
{

	Obj_ID=new int [no_blob];
	fuchsia.rgbBlue=255;
	fuchsia.rgbGreen=0;
	fuchsia.rgbRed=255;

	oldtime=currenttime;
	currenttime= clock();
	time= (float)(currenttime-oldtime)/1000/60/60;


	sprintf(stats,"");
	if (!firstFrame) 
		update_track(Blob);
	else
	{		
		for (int i=0;i<MAX_OBJ;i++)
		{
			for (int j=0;j<5;j++)
			{
				if (j<4)
				{
					Obj[i].bbox[j]=0;
					Obj[i].bbox_old[j]=0;
				}
				Obj[i].typeCount[j]=0;
			}
			Obj[i].dist=0;
			Obj[i].dist0=0;
			Obj[i].speed=0;
			Obj[i].lane=0;
			Obj[i].type=0;
			Obj[i].prevType=0;
			Obj[i].trackHistory=0;
			Obj[i].lapse2remove=0;
			Obj[i].gaveresult=false;
			Obj[i].changeresult=false;
		}
		init_track(Blob);
	}

	giveVehicleCount();
	drawBoundingBox(Blob);

	sprintf(tmpStr,"v_count=%d", v_count);

	m_figT->DrawString(drawHnd,width-220, 70,tmpStr,fuchsia,"Courier",30,400,0,0,false);
	
	if (Obj_ID) delete [] Obj_ID;

}

void CTracking::update_track(structBlob *Blob)
{
	int *Mapping_Blob_Obj, *b_minvalue, *Mapping_Obj_Blob,*a_maxvalue;
	int minvalue,maxvalue, pt_jj,delta,cnt,Ind_ii,Ind_Obj,Ind, max_count;
	float delta_area=0;
	
	int area_ii=0;
	long int overlap_area=0;
	
	Mapping_Blob_Obj= new int [no_blob];
	a_maxvalue = new int [no_blob];
	b_minvalue = new int [no_blob];
	Mapping_Obj_Blob = new int [MAX_OBJ];
	for (int i=0;i<no_blob;i++)
	{
		Mapping_Blob_Obj[i]=-1;
		a_maxvalue[i]=0;
		b_minvalue[i]=0;
		Obj_ID[i]=0;
	}

	for (i=0;i<MAX_OBJ;i++)
	{
		Mapping_Obj_Blob[i]=-1;
	}
	
	for (i=0;i<no_blob;i++)
	{
        minvalue=10000; 
		maxvalue=0;
		pt_jj=-1;
		area_ii=(Blob[i].bbox[1]-Blob[i].bbox[0])*(Blob[i].bbox[3]-Blob[i].bbox[2]);
        
		for (int j=0; j<MAX_OBJ;j++)
		{

            if (Obj[j].lapse2remove>0)
			{
				overlap_area=get_overlap_area(Blob[i].bbox,Obj[j].bbox);
                if (maxvalue<overlap_area)
				{
                    pt_jj=j;
                    maxvalue=overlap_area;
                    a_maxvalue[i]=maxvalue;
					b_minvalue[i]=10000;
				}
				if (maxvalue>0) continue;
				
				if (Obj[j].bbox_old[0]==0 && Obj[j].bbox_old[1]==0 && Obj[j].bbox_old[2]==0 && Obj[j].bbox_old[3]==0)
					delta=5*abs(Blob[i].bbox[0]-Obj[j].bbox[0]) + 5*abs(Blob[i].bbox[1]-Obj[j].bbox[1]) + abs(Blob[i].bbox[2]-Obj[j].bbox[2]) + abs(Blob[i].bbox[3]-Obj[j].bbox[3]);
				else 
					delta=5*abs(Blob[i].bbox[0]-(2*Obj[j].bbox[0]-Obj[j].bbox_old[0])) + 5*abs(Blob[i].bbox[1]-(2*Obj[j].bbox[1]-Obj[j].bbox_old[1])) + abs(Blob[i].bbox[2]-(2*Obj[j].bbox[2]-Obj[j].bbox_old[2])) + abs(Blob[i].bbox[3]-(2*Obj[j].bbox[3]-Obj[j].bbox_old[3]));
				delta/=3;
//              delta=abs(Blob[i].bbox[0]-Obj[j].bbox[0]) + abs(Blob[i].bbox[1]-Obj[j].bbox[1]);
//                delta_area=(float)((Blob[i].bbox[1]-Blob[i].bbox[0])*(Blob[i].bbox[3]-Blob[i].bbox[2]))/((Obj[j].bbox[1]-Obj[j].bbox[0])*(Obj[j].bbox[3]-Obj[j].bbox[2]));
//				if (delta_area>1)
//					delta_area=1/delta_area;
//				if (delta_area<0.4)
//					continue;
                if (minvalue>delta)
				{
                    pt_jj=j;
                    minvalue=delta;
                    b_minvalue[i]=minvalue;
				}
			}
		}
		Mapping_Blob_Obj[i]=pt_jj;
	}
	
   
    cnt=1;
    while (cnt<=no_blob)
	{
        minvalue=10000;  
		maxvalue=0;
		Ind_ii=-1;
        for (i=0;i<no_blob;i++)
		{
			if (a_maxvalue[i]>maxvalue)
			{
				Ind_ii=i;
				maxvalue=a_maxvalue[i];
			}
		}
		
        if (Ind_ii==-1)
		{
			for (i=0;i<no_blob;i++)
			{
				if (b_minvalue[i]<minvalue)
				{
					Ind_ii=i;
					minvalue=b_minvalue[i];
				}
			}
		}
		Ind_Obj=Mapping_Blob_Obj[Ind_ii];
        if (Mapping_Obj_Blob[Ind_Obj]==-1 && maxvalue>0)//&& Blob[Ind_ii].bbox[3]>=Obj[Ind_Obj].bbox[3])
		{
            Mapping_Obj_Blob[Ind_Obj]=Ind_ii;
            a_maxvalue[Ind_ii]=0;
		}
		else if (Mapping_Obj_Blob[Ind_Obj]==-1 && minvalue<200)
		{
            Mapping_Obj_Blob[Ind_Obj]=Ind_ii;
            b_minvalue[Ind_ii]=10000;
		} 
		else //Assign new ID
		{
			Ind_Obj=get_unusedID();
            
            Mapping_Obj_Blob[Ind_Obj]=Ind_ii;
            a_maxvalue[Ind_ii]=0;
            b_minvalue[Ind_ii]=10000;
		}

        cnt++;
	}
    // Update Obj
    for (i=0;i<MAX_OBJ;i++)
	{
		max_count=0;
        if (Mapping_Obj_Blob[i]>=0) 
		{
            Ind=Mapping_Obj_Blob[i];
			
			lanewidth = abs(((float)(Blob[Ind].mu.y-c1)/m1)- ((float)(Blob[Ind].mu.y-c2)/m2));
			if ((Blob[Ind].mu.x-(Blob[Ind].mu.y-c1)/m1)<lanewidth/3)
			{
				Obj[i].lane=1;
			}
			else if ((Blob[Ind].mu.x-(Blob[Ind].mu.y-c1)/m1)>2*lanewidth/3)
			{
				Obj[i].lane=3;
			}
			else 
			{
				Obj[i].lane = 2;
			}

			switch(Blob[Ind].type)
			{
				case 1:
					Obj[i].typeCount[0]++;
					break;
				case 2:
					Obj[i].typeCount[1]++;
					break;
				case 3:
					Obj[i].typeCount[2]++;
					break;
				case 4:
					Obj[i].typeCount[3]++;
					break;
				case 5:
					Obj[i].typeCount[4]++;
					break;
			}

			for (int j=0;j<5;j++)
			{
				if (j<4)
				{
					Obj[i].bbox_old[j]=Obj[i].bbox[j];
					Obj[i].bbox[j]=Blob[Ind].bbox[j];
				}
				if (Obj[i].typeCount[j]>max_count)
				{
					Obj[i].type=j+1;
					max_count=Obj[i].typeCount[j];
				}
			}
			
			Obj[i].dist0=Obj[i].dist;
			Obj[i].dist=((Obj[i].bbox[2]+Obj[i].bbox[3])/2)/lanewidth*3*3.5;
			Obj[i].speed=(((Obj[i].dist-Obj[i].dist0)/time/1000) + (Obj[i].speed*(Obj[i].trackHistory-1)))/Obj[i].trackHistory;

			if (Obj[i].type!=Obj[i].prevType)
			{ 
				Obj[i].changeresult=true;
				Obj[i].gaveresult=false;
			}		

			Obj[i].trackHistory=Obj[i].trackHistory+1;
            Obj[i].lapse2remove=3;
            Obj_ID[Ind]=i;  //the Obj_ID of Blob[Ind] is i
			
			switch(Obj[i].type)
			{
				case 1:
					sprintf(type_str,"MTB");
					break;
				case 2:
					sprintf(type_str,"CAR");
					break;
				case 3:
					sprintf(type_str,"BUS");
					break;
				case 4:
					sprintf(type_str,"HEAVY");
					break;
				case 5:
					sprintf(type_str,"LIGHT");
					break;
				default:
					sprintf(type_str,"UNCLASSIFIED");
			}
			
		}
        else
		{
			Obj[i].lapse2remove=Obj[i].lapse2remove-1;
            if (Obj[i].lapse2remove<=0)
			{
				for (int j=0;j<5;j++)
				{
					if (j<4)
					{
						Obj[i].bbox[j]=0;
						Obj[i].bbox_old[j]=0;
					}
					Obj[i].typeCount[j]=0;
				}
				Obj[i].dist=0;
				Obj[i].dist0=0;
				Obj[i].speed=0;
				Obj[i].lane=0;
				Obj[i].type=0;
                Obj[i].trackHistory=0;
                Obj[i].lapse2remove=0;
                Obj[i].gaveresult=false;
				Obj[i].changeresult=false;
				Obj[i].prevType=0;
			}
		}
	}
	if (Mapping_Obj_Blob) delete [] Mapping_Obj_Blob;
	if (b_minvalue) delete [] b_minvalue;
	if (Mapping_Blob_Obj) delete [] Mapping_Blob_Obj;
}


int CTracking::get_unusedID()
{
	int unusedID = MAX_OBJ-1;
	for (int i=0; i<MAX_OBJ;i++)
	{
        if (Obj[i].lapse2remove==0)
		{
			Obj[i].lapse2remove=3;
            unusedID=i;
            break;
		}
	}
	return unusedID;
}

void CTracking::init_track(structBlob *Blob)
{
	for (int i=0;i<no_blob;i++)
	{
		Obj_ID[i]=0;
		for (int j=0;j<4;j++)
		{
			Obj[i].bbox[j]=Blob[i].bbox[j];
		}
		switch(Blob[i].type)
		{
			case 1:
				Obj[i].typeCount[0]++;
				break;
				
			case 2:
				Obj[i].typeCount[1]++;
				break;
			case 3:
				Obj[i].typeCount[2]++;
				break;
			case 4:
				Obj[i].typeCount[3]++;
				break;
			case 5:
				Obj[i].typeCount[4]++;
				break;
		}
		Obj[i].type=Blob[i].type;
		Obj[i].prevType=Obj[i].type;
		Obj[i].trackHistory=1;
		Obj[i].lapse2remove=3;
		Obj_ID[i]=i;
		Obj[i].gaveresult=false;
	}
}

void CTracking::drawBoundingBox(structBlob* Blob)
{
	
	black.rgbBlue=255;
	black.rgbGreen=255;
	black.rgbRed=255;

	lime.rgbBlue=255;
	lime.rgbGreen=0;
	lime.rgbRed=0;

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

		switch(Obj[Obj_ID[i]].type)
		{
			case 1:
				sprintf(type_str,"MTB");
				break;
			case 2:
				sprintf(type_str,"CAR");
				break;
			case 3:
				sprintf(type_str,"LIGHT");
				break;
			case 4:
				sprintf(type_str,"HEAVY/BUS");
				break;
		//	case 5:
		//		sprintf(type_str,"LIGHT");
		//		break;
			default:
				sprintf(type_str,"");
		}

		sprintf(ID_str,"%.2d",Obj_ID[i]);
		if (Obj[Obj_ID[i]].trackHistory>1 && Obj[Obj_ID[i]].type!=0)
			sprintf(speed_str,"%dkm/h",Obj[Obj_ID[i]].speed);
		else
			sprintf(speed_str,"");

		m_figT->DrawLine(left, right, bottom,bottom,RGB(255,0,255));
		m_figT->DrawLine(left, right, top, top,RGB(255,0,255));
		m_figT->DrawLine(left, left, bottom, top,RGB(255,0,255));
		m_figT->DrawLine(right, right, bottom, top,RGB(255,0,255));
                
		
		if (Obj[Obj_ID[i]].gaveresult)
		{
			m_figT->DrawString(drawHnd,left, height-top,type_str,lime,"Courier",14,400,0,0,false);
			m_figT->DrawString(drawHnd,right+10, height-top,ID_str,lime,"Courier",14,400,0,0,false);
			m_figT->DrawString(drawHnd,right+10, height-top+15,speed_str,lime,"Courier",14,400,0,0,false);
		}
		
		else
		{
			m_figT->DrawString(drawHnd,left, height-top,type_str,black,"Courier",14,400,0,0,false);
			m_figT->DrawString(drawHnd,right+10, height-top,ID_str,black,"Courier",14,400,0,0,false);
			m_figT->DrawString(drawHnd,right+10, height-top+15,speed_str,black,"Courier",14,400,0,0,false);
		}
	}	
}  

void CTracking::giveVehicleCount()
{
    for (int i=0;i<MAX_OBJ;i++)
	{
        if (Obj[i].trackHistory>=T && !Obj[i].gaveresult && Obj[i].type!=0)
		{
			switch(Obj[i].type)
			{
				case 1:
					mtb_count++;
					break;
				case 2:
					car_count++;
					break;
				case 3:
					light_count++;
					break;
				case 4:
					heavy_count++;
					break;
			//	case 5:
			//		bus_count++;
			//		break;
			}

			if (Obj[i].changeresult)
			{
				switch(Obj[i].prevType)
				{
					case 1:
						mtb_count--;
						break;
					case 2:
						car_count--;
						break;
					case 3:
						light_count--;
						break;
					case 4:
						heavy_count--;
						break;
				//	case 5:
				//		bus_count--;
				//		break;
				}
			}

			v_count=mtb_count+car_count+light_count+heavy_count+bus_count;
            Obj[i].gaveresult=true;
            Obj[i].changeresult=false;
			Obj[i].prevType=Obj[i].type;
			
			CTime currentTime=CTime::GetCurrentTime();
			sprintf(tmpStr,"%02d/%02d/%d %02d:%02d:%02d, %d, %d, %d\r\n",currentTime.GetMonth(),currentTime.GetDay(), currentTime.GetYear(),currentTime.GetHour(), currentTime.GetMinute(), currentTime.GetSecond(), Obj[i].lane,Obj[i].type,v_count);
			strcat(stats,tmpStr);	
		}
	}
}

int CTracking::get_overlap_area(int *bbox_ii,int *bbox_k)
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
	return area;
	
}
