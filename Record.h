#if !defined(AFX_RECORD_H__A46CD1C8_A807_11D2_8728_00A02468635B__INCLUDED_)
#define AFX_RECORD_H__A46CD1C8_A807_11D2_8728_00A02468635B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Record.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRecord dialog
#include "InstInfo.h"
#include "trakproj.h"

extern SoundProject	TProj;	
extern CInstanceInfo InstInfo;

class CRecord : public CDialog
{
// Construction
public:
	BOOL Stereo;
	int RecTrack;
	CRecord(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRecord)
	enum { IDD = IDD_RECORD };
	CString	m_Properties;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecord)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog ( );
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecord)
	afx_msg void OnMono();
	afx_msg void OnNewtrack();
	afx_msg void OnStereo();
	afx_msg void OnTrack1();
	afx_msg void OnTrack2();
	afx_msg void OnTrack3();
	afx_msg void OnTrack4();
	afx_msg void OnTrack5();
	afx_msg void OnTrack6();
	afx_msg void OnTrack7();
	afx_msg void OnTrack8();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORD_H__A46CD1C8_A807_11D2_8728_00A02468635B__INCLUDED_)
