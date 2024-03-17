#if !defined(AFX_VU_H__F51D62D5_A859_11D2_8728_00A02468635B__INCLUDED_)
#define AFX_VU_H__F51D62D5_A859_11D2_8728_00A02468635B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// VU.h : header file
//

#include <mmsystem.h>

#include "InstInfo.h"
extern CInstanceInfo InstInfo;

/////////////////////////////////////////////////////////////////////////////
// CVU dialog

class CVU : public CDialog
{
// Construction
public:
	CVU(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVU)
	enum { IDD = IDD_VU };
	CProgressCtrl	m_vu;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVU)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog( );
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVU)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VU_H__F51D62D5_A859_11D2_8728_00A02468635B__INCLUDED_)
