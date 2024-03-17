#if !defined(AFX_SETUP_H__A46CD1B2_A807_11D2_8728_00A02468635B__INCLUDED_)
#define AFX_SETUP_H__A46CD1B2_A807_11D2_8728_00A02468635B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Setup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetup dialog
#include "InstInfo.h"
#include "trakproj.h"

extern SoundProject	TProj;	
extern CInstanceInfo InstInfo;

class CSetup : public CDialog
{
// Construction
public:
	CSetup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetup)
	enum { IDD = IDD_SETUP };
	CButton	m_Cancel;
	CButton	m_btnBrowse;
	CString	m_ProjName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog( );
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetup)
	afx_msg void On11khz_Clicked();
	afx_msg void On16bit_Clicked();
	afx_msg void On22khz_Clicked();
	afx_msg void On44khz_Clicked();
	afx_msg void On8bit_Clicked();
	afx_msg void OnNewproject_Clicked();
	afx_msg void OnBrowse();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString FilePath;
	CString FileName;
	CString BaseName;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUP_H__A46CD1B2_A807_11D2_8728_00A02468635B__INCLUDED_)
