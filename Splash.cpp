// Splash.cpp : implementation file
//

#include "stdafx.h"
#include "trak.h"
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplash dialog


DWORD WINAPI SplashDestroy(LPVOID Param) {
	CSplash *hDlg = (CSplash *)Param;

	Sleep(2000);

	if (IsWindow(hDlg->m_hWnd))
	{
		hDlg->EndModalLoop(IDOK);
	}

	return 0;
}

CSplash::CSplash(CWnd* pParent /*=NULL*/)
	: CDialog(CSplash::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSplash)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSplash::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSplash)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSplash, CDialog)
	//{{AFX_MSG_MAP(CSplash)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplash message handlers

int CSplash::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	DWORD tid;
	CreateThread(NULL, 0, SplashDestroy, this, 0, &tid);

	return CDialog::DoModal();
}

void CSplash::OnCancel() {
	// Don't let users cancel or ESC
}

void CSplash::OnOK() {
	// Don't let users hit enter and get out
}