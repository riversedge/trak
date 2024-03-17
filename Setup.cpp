// Setup.cpp : implementation file
//

#include "stdafx.h"
#include "trak.h"
#include "Setup.h"
#include <direct.h>
#include "Trak.h"

extern CTrakApp theApp;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetup dialog


CSetup::CSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetup)
	m_ProjName = _T("");
	//}}AFX_DATA_INIT
}


void CSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetup)
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Text(pDX, IDC_PROJNAME, m_ProjName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetup, CDialog)
	//{{AFX_MSG_MAP(CSetup)
	ON_BN_CLICKED(IDC_11KHZ, On11khz_Clicked)
	ON_BN_CLICKED(IDC_16BIT, On16bit_Clicked)
	ON_BN_CLICKED(IDC_22KHZ, On22khz_Clicked)
	ON_BN_CLICKED(IDC_44KHZ, On44khz_Clicked)
	ON_BN_CLICKED(IDC_8BIT, On8bit_Clicked)
	ON_BN_CLICKED(IDC_NEWPROJECT, OnNewproject_Clicked)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetup message handlers

void CSetup::On11khz_Clicked() 
{
	CheckDlgButton(IDC_11KHZ, BST_CHECKED);
	CheckDlgButton(IDC_22KHZ, BST_UNCHECKED);
	CheckDlgButton(IDC_44KHZ, BST_UNCHECKED);
}

void CSetup::On16bit_Clicked() 
{
	CheckDlgButton(IDC_16BIT, BST_CHECKED);
	CheckDlgButton(IDC_8BIT, BST_UNCHECKED);	
}

void CSetup::On22khz_Clicked() 
{
	CheckDlgButton(IDC_11KHZ, BST_UNCHECKED);
	CheckDlgButton(IDC_22KHZ, BST_CHECKED);
	CheckDlgButton(IDC_44KHZ, BST_UNCHECKED);
}

void CSetup::On44khz_Clicked() 
{
	CheckDlgButton(IDC_11KHZ, BST_UNCHECKED);
	CheckDlgButton(IDC_22KHZ, BST_UNCHECKED);
	CheckDlgButton(IDC_44KHZ, BST_CHECKED);
}

void CSetup::On8bit_Clicked() 
{
	CheckDlgButton(IDC_16BIT, BST_UNCHECKED);
	CheckDlgButton(IDC_8BIT, BST_CHECKED);	
}

void CSetup::OnNewproject_Clicked() 
{
	CheckDlgButton(IDC_NEWPROJECT, BST_CHECKED);	
	CWnd *m_editBox = GetDlgItem(IDC_PROJNAME);
	CWnd *m_btnBrowse = GetDlgItem(IDC_BROWSE);
	CWnd *m_16Bit = GetDlgItem(IDC_16BIT);
	CWnd *m_8Bit = GetDlgItem(IDC_8BIT);
	CWnd *m_44KHZ = GetDlgItem(IDC_44KHZ);
	CWnd *m_22KHZ = GetDlgItem(IDC_22KHZ);
	CWnd *m_11KHZ = GetDlgItem(IDC_11KHZ);
	m_editBox->EnableWindow(TRUE);
	m_btnBrowse->EnableWindow(TRUE);
	m_16Bit->EnableWindow(TRUE);
	m_8Bit->EnableWindow(TRUE);
	m_44KHZ->EnableWindow(TRUE);
	m_22KHZ->EnableWindow(TRUE);
	m_11KHZ->EnableWindow(TRUE);
}

BOOL CSetup::OnInitDialog( ) {
	CDialog::OnInitDialog();

	CheckDlgButton(IDC_NEWPROJECT, BST_CHECKED);
	CheckDlgButton(IDC_44KHZ, BST_CHECKED);
	CheckDlgButton(IDC_16BIT, BST_CHECKED);
	CWnd *m_editBox = GetDlgItem(IDC_PROJNAME);
	CWnd *m_btnBrowse = GetDlgItem(IDC_BROWSE);
	CWnd *m_16Bit = GetDlgItem(IDC_16BIT);
	CWnd *m_8Bit = GetDlgItem(IDC_8BIT);
	CWnd *m_44KHZ = GetDlgItem(IDC_44KHZ);
	CWnd *m_22KHZ = GetDlgItem(IDC_22KHZ);
	CWnd *m_11KHZ = GetDlgItem(IDC_11KHZ);

	m_editBox->EnableWindow(TRUE);
	m_btnBrowse->EnableWindow(TRUE);
	m_16Bit->EnableWindow(TRUE);
	m_8Bit->EnableWindow(TRUE);
	m_44KHZ->EnableWindow(TRUE);
	m_22KHZ->EnableWindow(TRUE);
	m_11KHZ->EnableWindow(TRUE);

	return TRUE;
}

void CSetup::OnBrowse() 
{
	// Format of dialog is:
	// CFileDialog( BOOL bOpenFileDialog, LPCTSTR lpszDefExt = NULL, 
	//				LPCTSTR lpszFileName = NULL, 
	//				DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
	//				LPCTSTR lpszFilter = NULL, CWnd* pParentWnd = NULL );

	CFileDialog fDialog(TRUE, "tpj", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						"Base File Name||", this);

	fDialog.DoModal();

	FilePath = fDialog.GetPathName();
	FileName = fDialog.GetFileName();
	BaseName = fDialog.GetFileTitle();

	m_ProjName = fDialog.GetPathName();
	UpdateData(FALSE);
}

void CSetup::OnOK() 
{
	int kHz;
	int Bits;

	UpdateData(TRUE);

	if (IsDlgButtonChecked(IDC_16BIT)) Bits = 16;
	if (IsDlgButtonChecked(IDC_8BIT))  Bits = 8;
	
	if (IsDlgButtonChecked(IDC_44KHZ)) kHz = 44;
	if (IsDlgButtonChecked(IDC_22KHZ)) kHz = 22;
	if (IsDlgButtonChecked(IDC_11KHZ)) kHz = 11;

	if (m_ProjName.IsEmpty()) {
		MessageBox("You enter a valid file name.", "Trak Multitracking System", MB_ICONEXCLAMATION);
		return;
	}

	// Update the project control
	InstInfo.Filename = FileName;
	InstInfo.Filepath = FilePath;
	InstInfo.BaseName = BaseName;
	InstInfo.SampRate = kHz;
	InstInfo.SampBits = Bits;
	InstInfo.FilledStart = TRUE;

	char BName[255];
	strncpy(BName, BaseName, 254);
	TProj.Reset();
	TProj.SetBase(BName);
	TProj.SetSampleRate(kHz);
	TProj.SetBitRate(Bits);
	BName[strlen(FilePath)-strlen(FileName)-1] = '\0';
	chdir(BName);

	theApp.SaveFileName = FilePath;

	CDialog::OnOK();
}
