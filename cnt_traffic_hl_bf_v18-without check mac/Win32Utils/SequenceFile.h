#ifndef __SEQUENCE_FILE__H__
#define __SEQUENCE_FILE__H__

#include <windows.h>
#include <vfw.h>
#pragma comment(lib, "vfw32.lib")


//===================================================================
//	Class Name:
//		CSequenceFileInput
//	Description:
//		A class to read a raw AVI file with RGB24 format.
//	Usage:
//		1.  Call Init with an avi filename, all frame parameters
//		    will be filled.
//		2.  Call GetFrame to get individual frames.
//===================================================================

class CSequenceFileInput
{
public:
	CSequenceFileInput ();
	~CSequenceFileInput ();
	bool Init (LPTSTR filename);
	unsigned char * GetFrame (LONG pos);
	DWORD frame_start;
	DWORD frame_end;
	int frame_width;
	int frame_height;
	int frame_rate_num;
	int frame_rate_denom;

protected:
	PAVIFILE pavifile;
	PAVISTREAM pavistream;
	PGETFRAME pgf;
};


//===================================================================
//	Class Name:
//		CSequenceFileOutput
//	Description:
//		A class to write a raw AVI file with RGB24 format.
//	Derivation Steps:
//		1.  Call Init with an avi filename and all frame parameters.
//		2.  Call SetFrame to set individual frames sequentially.
//===================================================================

class CSequenceFileOutput
{
public:
	CSequenceFileOutput ();
	~CSequenceFileOutput ();
	bool Init (LPTSTR filename, int w, int h, int fps_num, int fps_denom);
	bool SetFrame (unsigned char * p);
	int frame_width;
	int frame_height;
	LONG frame_count;

protected:
	PAVIFILE pavifile;
	PAVISTREAM pavistream;
	PAVISTREAM pavitext;
	PGETFRAME pgf;
};

#endif //__SEQUENCE_FILE__H__