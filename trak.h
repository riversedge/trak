// trak.h : main header file for the TRAK application
//

#if !defined(AFX_TRAK_H__A46CD19D_A807_11D2_8728_00A02468635B__INCLUDED_)
#define AFX_TRAK_H__A46CD19D_A807_11D2_8728_00A02468635B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTrakApp:
// See trak.cpp for the implementation of this class
//

class CTrakApp : public CWinApp
{
public:
	CTrakApp();
	void OnFileNew();
	void OnFileSave();
	void OnFileOpen();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrakApp)
	public:
	virtual BOOL InitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTrakApp)
	afx_msg void OnAppAbout();
	afx_msg void OnPreferences();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL Loading;
	BOOL AlreadyLoading;
	BOOL Changed;
	CString SaveFileName;
	BOOL   bPlaying;
	BOOL   bRecording;

private:
	BOOL GetRegInfo();
	BOOL Saved;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAK_H__A46CD19D_A807_11D2_8728_00A02468635B__INCLUDED_)
