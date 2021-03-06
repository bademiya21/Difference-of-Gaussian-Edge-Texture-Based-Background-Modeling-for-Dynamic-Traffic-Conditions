#if !defined(AFX_AXISMEDIACONTROL_H__566C3308_44B8_4473_9014_5CF055CC0D3B__INCLUDED_)
#define AFX_AXISMEDIACONTROL_H__566C3308_44B8_4473_9014_5CF055CC0D3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CAxisMediaControl wrapper class

class CAxisMediaControl : public CWnd
{
protected:
	DECLARE_DYNCREATE(CAxisMediaControl)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x745395c8, 0xd0e1, 0x4227, { 0x85, 0x86, 0x62, 0x4c, 0xa9, 0xa1, 0xa, 0x8d } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attributes
public:

// Operations
public:
	void Play();
	void Stop();
	void TogglePause();
	void StartRecord(LPCTSTR theFile);
	void StopRecord();
	void GetCurrentImage(long theFormat, VARIANT* theBuffer, long* theBufferSize);
	void SaveCurrentImage(long theFormat, LPCTSTR theFile);
	long GetMPEG4VideoDecodingMode();
	void SetMPEG4VideoDecodingMode(long nNewValue);
	BOOL GetDisplayMessages();
	void SetDisplayMessages(BOOL bNewValue);
	long GetMPEG4VideoRenderer();
	void SetMPEG4VideoRenderer(long nNewValue);
	BOOL GetEnableContextMenu();
	void SetEnableContextMenu(BOOL bNewValue);
	long GetEnableRecording();
	void SetEnableRecording(long nNewValue);
	BOOL GetEnableReconnect();
	void SetEnableReconnect(BOOL bNewValue);
	void AudioTransmitStart();
	void AudioReceiveStart();
	void AudioReceiveStop();
	void AudioTransmitStop();
	CString GetMediaURL();
	void SetMediaURL(LPCTSTR lpszNewValue);
	BOOL GetAutoStart();
	void SetAutoStart(BOOL bNewValue);
	CString GetUIMode();
	void SetUIMode(LPCTSTR lpszNewValue);
	BOOL GetMute();
	void SetMute(BOOL bNewValue);
	long GetVolume();
	void SetVolume(long nNewValue);
	CString GetMediaUsername();
	void SetMediaUsername(LPCTSTR lpszNewValue);
	CString GetMediaPassword();
	void SetMediaPassword(LPCTSTR lpszNewValue);
	long GetNetworkTimeout();
	void SetNetworkTimeout(long nNewValue);
	CString GetMediaType();
	void SetMediaType(LPCTSTR lpszNewValue);
	BOOL GetFullScreen();
	void SetFullScreen(BOOL bNewValue);
	CString GetMediaFile();
	void SetMediaFile(LPCTSTR lpszNewValue);
	long GetCurrentPosition();
	void SetCurrentPosition(long nNewValue);
	long GetDuration();
	long GetFramesDrawn();
	long GetStatus();
	BOOL GetStretchToFit();
	void SetStretchToFit(BOOL bNewValue);
	BOOL GetShowToolbar();
	void SetShowToolbar(BOOL bNewValue);
	BOOL GetShowStatusBar();
	void SetShowStatusBar(BOOL bNewValue);
	CString GetToolbarConfiguration();
	void SetToolbarConfiguration(LPCTSTR lpszNewValue);
	CString GetPTZControlURL();
	void SetPTZControlURL(LPCTSTR lpszNewValue);
	long GetMPEG2VideoDecodingMode();
	void SetMPEG2VideoDecodingMode(long nNewValue);
	CString GetMotionConfigURL();
	void SetMotionConfigURL(LPCTSTR lpszNewValue);
	CString GetMotionDataURL();
	void SetMotionDataURL(LPCTSTR lpszNewValue);
	CString GetAudioConfigURL();
	void SetAudioConfigURL(LPCTSTR lpszNewValue);
	CString GetAudioReceiveURL();
	void SetAudioReceiveURL(LPCTSTR lpszNewValue);
	CString GetAudioTransmitURL();
	void SetAudioTransmitURL(LPCTSTR lpszNewValue);
	CString GetAudioFile();
	void SetAudioFile(LPCTSTR lpszNewValue);
	BOOL GetAudioTransmitMute();
	void SetAudioTransmitMute(BOOL bNewValue);
	long GetAudioTransmitVolume();
	void SetAudioTransmitVolume(long nNewValue);
	BOOL GetClarifyEnabled();
	void SetClarifyEnabled(BOOL bNewValue);
	long GetClarifyLevel();
	void SetClarifyLevel(long nNewValue);
	CString GetClarifyLicenseFile();
	void SetClarifyLicenseFile(LPCTSTR lpszNewValue);
	CString GetClarifyValidatorsLocation();
	void SetClarifyValidatorsLocation(LPCTSTR lpszNewValue);
	void StartRecordMedia(LPCTSTR theFile, long theFlags, LPCTSTR theMediaTypes);
	void StopRecordMedia();
	void StartTransmitMedia(LPCTSTR theFile, long theFlags);
	void StopTransmitMedia();
	CString GetSvgDataUrl();
	void SetSvgDataUrl(LPCTSTR lpszNewValue);
	BOOL GetJoystickConnected();
	void SetImage(VARIANT* theImage, long* theSize);
	void SetReconnectionStrategy(long theDuration1, long theConnectionInterval1, long theDuration2, long theConnectionInterval2, long theDuration3, long theConnectionInterval3, BOOL infiniteMode);
	BOOL GetEnableJoystick();
	void SetEnableJoystick(BOOL bNewValue);
	BOOL GetEnableAreaZoom();
	void SetEnableAreaZoom(BOOL bNewValue);
	long GetClientOverlay();
	void SetClientOverlay(long nNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXISMEDIACONTROL_H__566C3308_44B8_4473_9014_5CF055CC0D3B__INCLUDED_)
