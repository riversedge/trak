#if !defined(AFX_SPLASH_H__A46CD1B1_A807_11D2_8728_00A02468635B__INCLUDED_)
#define AFX_SPLASH_H__A46CD1B1_A807_11D2_8728_00A02468635B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Splash.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSplash dialog

class CSplash : public CDialog
{
// Construction
public:
	CSplash(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSplash)
	enum { IDD = IDD_SPLASH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplash)
	public:
	virtual int DoModal();
	virtual void OnCancel();
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSplash)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLASH_H__A46CD1B1_A807_11D2_8728_00A02468635B__INCLUDED_)
