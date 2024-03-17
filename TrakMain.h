#if !defined(AFX_TRAKMAIN_H__A46CD1AF_A807_11D2_8728_00A02468635B__INCLUDED_)
#define AFX_TRAKMAIN_H__A46CD1AF_A807_11D2_8728_00A02468635B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TrakMain.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrakMain form view
#include "InstInfo.h"
#include "trakproj.h"
#include "CRec.h"
#include "CWave.h"
#include "CPlayMix.h"

extern SoundProject	TProj;	
extern CInstanceInfo InstInfo;

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CTrakMain : public CFormView
{
protected:
	CTrakMain();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTrakMain)

// Form Data
public:
	//{{AFX_DATA(CTrakMain)
	enum { IDD = IDD_TRAKMAIN };
	CSliderCtrl	m_Volume8;
	CSliderCtrl	m_Volume7;
	CSliderCtrl	m_Volume6;
	CSliderCtrl	m_Volume5;
	CSliderCtrl	m_Volume4;
	CSliderCtrl	m_Volume3;
	CSliderCtrl	m_Volume2;
	CSliderCtrl	m_Volume1;
	CSliderCtrl	m_Pan8;
	CSliderCtrl	m_Pan7;
	CSliderCtrl	m_Pan6;
	CSliderCtrl	m_Pan5;
	CSliderCtrl	m_Pan4;
	CSliderCtrl	m_Pan3;
	CSliderCtrl	m_Pan2;
	CSliderCtrl	m_Pan1;
	CSliderCtrl	m_MainVolume;
	BOOL	m_Mute1;
	BOOL	m_Mute2;
	BOOL	m_Mute3;
	BOOL	m_Mute4;
	BOOL	m_Mute5;
	BOOL	m_Mute6;
	BOOL	m_Mute7;
	BOOL	m_Mute8;
	CString	m_Track1;
	CString	m_Track2;
	CString	m_Track3;
	CString	m_Track4;
	CString	m_Track5;
	CString	m_Track6;
	CString	m_Track7;
	CString	m_Track8;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	double RightLevel(int Track);
	double LeftLevel(int Track);
	CString GenFilename(int Track);
	BOOL Stereo;
	int RecTrack;
	void RecThreadHandler();
	CString RecFilename;
	void UpdateFromProj();

	HANDLE hPlayRecCounter;
	HANDLE hPlayer;
	HANDLE hRecorder;

	HANDLE hSyncPlay;
	HANDLE hPlayReady;
	HANDLE hRecReady;

	CWave		WaveMixer;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrakMain)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTrakMain();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTrakMain)
	afx_msg void OnMixer();
	afx_msg void OnVuLvl();
	afx_msg void OnMute1();
	afx_msg void OnMute2();
	afx_msg void OnMute3();
	afx_msg void OnMute4();
	afx_msg void OnMute5();
	afx_msg void OnMute6();
	afx_msg void OnMute7();
	afx_msg void OnMute8();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
	afx_msg void OnFileNew();
	afx_msg void OnChangeTrack1();
	afx_msg void OnChangeTrack2();
	afx_msg void OnChangeTrack3();
	afx_msg void OnChangeTrack4();
	afx_msg void OnChangeTrack5();
	afx_msg void OnChangeTrack6();
	afx_msg void OnChangeTrack7();
	afx_msg void OnChangeTrack8();
	afx_msg void OnRecord();
	afx_msg void OnPlay();
	afx_msg void OnSaveAsWave();
	afx_msg void OnFindnoclip();
	//}}AFX_MSG
public:
	afx_msg void OnStop();

	CRec		Recorder;
	CPlayMix	Player;

	BOOL   bPlaying;
	BOOL   bRecording;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAKMAIN_H__A46CD1AF_A807_11D2_8728_00A02468635B__INCLUDED_)
