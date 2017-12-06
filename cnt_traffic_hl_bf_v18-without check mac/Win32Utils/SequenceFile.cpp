#include "stdafx.h"
#include "sequencefile.h"


//==============================================
//	CSequenceFileInput
//==============================================

CSequenceFileInput::CSequenceFileInput ()
{
	AVIFileInit ();
	pavifile = NULL;
}

CSequenceFileInput::~CSequenceFileInput ()
{
	if (pavifile)
	{
		AVIStreamRelease (pavistream);
		AVIFileRelease (pavifile);
	}
	AVIFileExit ();
}

bool CSequenceFileInput::Init (LPTSTR filename)
{
	if (pavifile)
	{
		AVIStreamGetFrameClose (pgf);
		AVIFileRelease (pavifile);
	}
	if (AVIFileOpen (&pavifile, filename, OF_READ, NULL)) 
	{
		pavifile = NULL;
		return false;
	}
	int ret;
//	AVIFILEINFO fi;
//	AVIFileInfo (pavifile, &fi, sizeof(fi));
	ret = AVIFileGetStream (pavifile, &pavistream, streamtypeVIDEO, 0);
	if (ret!=0)
	{
		AVIFileRelease (pavifile);
		pavifile = NULL;
		return false;
	}
	AVISTREAMINFO sinfo;
	ret = AVIStreamInfo (pavistream, &sinfo, sizeof(sinfo));
	if (ret!=0)
	{
		AVIStreamRelease (pavistream);
		AVIFileRelease (pavifile);
		pavifile = NULL;
		return false;
	}
	frame_start = sinfo.dwStart;
	frame_end = sinfo.dwLength + sinfo.dwStart - 1;
	frame_width = sinfo.rcFrame.right - sinfo.rcFrame.left;
	frame_height = sinfo.rcFrame.bottom - sinfo.rcFrame.top;
	frame_rate_num = sinfo.dwRate;
	frame_rate_denom = sinfo.dwScale;
	BITMAPINFOHEADER bmpi;
	memset (&bmpi, 0, sizeof(bmpi));
	bmpi.biSize = sizeof(bmpi);
	bmpi.biWidth = frame_width;
	bmpi.biHeight = frame_height;
	bmpi.biPlanes = 1;
	bmpi.biBitCount = 24;
	bmpi.biCompression = 0;
	bmpi.biSizeImage = frame_width * frame_height * 3;
	pgf = AVIStreamGetFrameOpen (pavistream, &bmpi);
	if (pgf==NULL)
	{
		AVIStreamRelease (pavistream);
		AVIFileRelease (pavifile);
		pavifile = NULL;
		return false;
	}
	
	return true;
}

unsigned char * CSequenceFileInput::GetFrame (LONG pos)
{
	if (!pavifile) return 0;

	LPBITMAPINFOHEADER lpbi;
	lpbi = (LPBITMAPINFOHEADER)	AVIStreamGetFrame (pgf, pos);
	if (lpbi==NULL) return NULL;
	return (unsigned char *) lpbi + sizeof(BITMAPINFOHEADER);
}




//==============================================
//	CSequenceFileOutput
//==============================================

CSequenceFileOutput::CSequenceFileOutput ()
{
	AVIFileInit ();
	pavifile = NULL;
	pavitext = NULL;
}

CSequenceFileOutput::~CSequenceFileOutput ()
{
	if (pavifile)
	{
		if (pavitext)
		{
			AVIStreamRelease (pavitext);
		}
		AVIStreamRelease (pavistream);
		AVIFileRelease (pavifile);
	}
	AVIFileExit ();
}

bool CSequenceFileOutput::Init (LPTSTR filename, int w, int h, int fps_num, int fps_denom)
{
	if (pavifile)
	{
		AVIStreamGetFrameClose (pgf);
		AVIStreamRelease (pavistream);
		if (pavitext)
		{
			AVIStreamRelease (pavitext);
			pavitext = 0;
		}
		AVIFileRelease (pavifile);
	}
	FILE * fp = fopen (filename, "wb");
	if (fp)
		fclose (fp);
	if (AVIFileOpen (&pavifile, filename, OF_CREATE, NULL)) 
	{
		pavifile = NULL;
		return false;
	}

	frame_count = 0;
	frame_width = w;
	frame_height = h;

	RECT rect;
	AVISTREAMINFO si;
	rect.top = 0; rect.bottom = 0; rect.right = frame_width; rect.bottom = frame_height;
	si.fccType = streamtypeVIDEO;
	si.fccHandler = mmioFOURCC('M','S','V','C');
	si.dwFlags = 0;
	si.dwCaps = AVIFILECAPS_ALLKEYFRAMES|AVIFILECAPS_NOCOMPRESSION;
	si.wPriority = 0;
	si.wLanguage = 0;
	si.dwScale = fps_denom; 
	si.dwRate = fps_num; // FrameRate = Rate / Scale
	si.dwStart = 0;
	si.dwLength = 0;	// Can be zero, then append frames with contiguous frame numbers.
	si.dwInitialFrames = 0;
	si.dwSuggestedBufferSize = 3*frame_width*frame_height;
	si.dwQuality = 10000;
	si.dwSampleSize = 3*frame_width*frame_height;
	si.rcFrame = rect;
	si.dwEditCount = 0;     
	si.dwFormatChangeCount = 0;
	sprintf (si.szName, "By TK Chiew"); 
	if (AVIFileCreateStream (pavifile, &pavistream, &si)!=0)
	{
		AVIFileRelease (pavifile);
		pavifile = NULL;
		return false;
	}

	BITMAPINFO bmpinfo;
	memset (&bmpinfo.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biBitCount = 24;
	bmpinfo.bmiHeader.biWidth = frame_width;
	bmpinfo.bmiHeader.biHeight = frame_height;
	bmpinfo.bmiHeader.biSizeImage = 3*frame_width*frame_height;
	AVIStreamSetFormat (pavistream, 0, &bmpinfo, sizeof (BITMAPINFO));

	return true;
}

bool CSequenceFileOutput::SetFrame (unsigned char * p)
{
	return AVIStreamWrite (pavistream, frame_count++, 1, p, 3*frame_width*frame_height, AVIIF_KEYFRAME, NULL, NULL)==0;
}
