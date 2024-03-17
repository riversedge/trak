#if !defined(AFX_PREFERENCES_H__F51D62D4_A859_11D2_8728_00A02468635B__INCLUDED_)
#define AFX_PREFERENCES_H__F51D62D4_A859_11D2_8728_00A02468635B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Preferences.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPreferences dialog

#include "InstInfo.h"
#include "trakproj.h"

extern SoundProject	TProj;	
extern CInstanceInfo InstInfo;

class CPreferences : public CDialog
{
// Construction
public:
	CPreferences(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPreferences)
	enum { IDD = IDD_PREFERENCES };
	CButton	m_Cancel;
	CButton	m_SyncPlay;
	CComboBox	m_RecDevBox;
	CEdit	m_RecBuffSize;
	CEdit	m_PlayBuffSize;
	CEdit	m_NumPlayBuff;
	CComboBox	m_PlayDevBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreferences)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog( );
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPreferences)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	DWORD RecDev;
	DWORD PlayDev;
	DWORD PlayBuff;
	DWORD NumPlayBuff;
	DWORD RecBuff;
	DWORD SyncPlay;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCES_H__F51D62D4_A859_11D2_8728_00A02468635B__INCLUDED_)
