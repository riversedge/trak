// Record.cpp : implementation file
//

#include "stdafx.h"
#include "trak.h"
#include "Record.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecord dialog


CRecord::CRecord(CWnd* pParent /*=NULL*/)
	: CDialog(CRecord::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecord)
	m_Properties = _T("");
	//}}AFX_DATA_INIT
}


void CRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecord)
	DDX_Text(pDX, IDC_PROPS, m_Properties);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecord, CDialog)
	//{{AFX_MSG_MAP(CRecord)
	ON_BN_CLICKED(IDC_MONO, OnMono)
	ON_BN_CLICKED(IDC_NEWTRACK, OnNewtrack)
	ON_BN_CLICKED(IDC_STEREO, OnStereo)
	ON_BN_CLICKED(IDC_TRACK1, OnTrack1)
	ON_BN_CLICKED(IDC_TRACK2, OnTrack2)
	ON_BN_CLICKED(IDC_TRACK3, OnTrack3)
	ON_BN_CLICKED(IDC_TRACK4, OnTrack4)
	ON_BN_CLICKED(IDC_TRACK5, OnTrack5)
	ON_BN_CLICKED(IDC_TRACK6, OnTrack6)
	ON_BN_CLICKED(IDC_TRACK7, OnTrack7)
	ON_BN_CLICKED(IDC_TRACK8, OnTrack8)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecord message handlers

BOOL CRecord::OnInitDialog( ) {
	CDialog::OnInitDialog();

	if (TProj.GetNumberTracks() == 8) {
		CWnd *m_newTrack = GetDlgItem(IDC_NEWTRACK);
		m_newTrack->EnableWindow(FALSE);
		CheckDlgButton(IDC_NEWTRACK, BST_UNCHECKED);
		CheckDlgButton(IDC_TRACK1, BST_CHECKED);
	} else {
		CWnd *m_newTrack = GetDlgItem(IDC_NEWTRACK);
		m_newTrack->EnableWindow(TRUE);
		CheckDlgButton(IDC_NEWTRACK, BST_CHECKED);
		CheckDlgButton(IDC_TRACK1, BST_UNCHECKED);
	}

	CheckDlgButton(IDC_TRACK2, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK3, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK4, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK5, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK6, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK7, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK8, BST_UNCHECKED);
	CheckDlgButton(IDC_MONO, BST_CHECKED);
	CheckDlgButton(IDC_STEREO, BST_UNCHECKED);

	CWnd *m_Track = GetDlgItem(IDC_TRACK1);
	if (TProj.GetNumberTracks()  < 1)
		m_Track->EnableWindow(FALSE);
	else
		m_Track->EnableWindow(TRUE);
	m_Track = GetDlgItem(IDC_TRACK2);
	if (TProj.GetNumberTracks()  < 2)
		m_Track->EnableWindow(FALSE);
	else
		m_Track->EnableWindow(TRUE);
	m_Track = GetDlgItem(IDC_TRACK3);
	if (TProj.GetNumberTracks()  < 3)
		m_Track->EnableWindow(FALSE);
	else
		m_Track->EnableWindow(TRUE);
	m_Track = GetDlgItem(IDC_TRACK4);
	if (TProj.GetNumberTracks()  < 4)
		m_Track->EnableWindow(FALSE);
	else
		m_Track->EnableWindow(TRUE);
	m_Track = GetDlgItem(IDC_TRACK5);
	if (TProj.GetNumberTracks()  < 5)
		m_Track->EnableWindow(FALSE);
	else
		m_Track->EnableWindow(TRUE);
	m_Track = GetDlgItem(IDC_TRACK6);
	if (TProj.GetNumberTracks()  < 6)
		m_Track->EnableWindow(FALSE);
	else
		m_Track->EnableWindow(TRUE);
	m_Track = GetDlgItem(IDC_TRACK7);
	if (TProj.GetNumberTracks()  < 7)
		m_Track->EnableWindow(FALSE);
	else
		m_Track->EnableWindow(TRUE);
	m_Track = GetDlgItem(IDC_TRACK8);
	if (TProj.GetNumberTracks()  < 8)
		m_Track->EnableWindow(FALSE);
	else
		m_Track->EnableWindow(TRUE);


	char Val[10];
	itoa(InstInfo.SampRate, Val, 10);
	m_Properties = Val;
	m_Properties += " kHz   ";
	itoa(InstInfo.SampBits, Val, 10);
	m_Properties += Val;
	m_Properties += " bits";

	UpdateData(FALSE);

	return TRUE;
}

void CRecord::OnMono() 
{
	CheckDlgButton(IDC_MONO, BST_CHECKED);
	CheckDlgButton(IDC_STEREO, BST_UNCHECKED);
}

void CRecord::OnNewtrack() 
{
	CheckDlgButton(IDC_NEWTRACK, BST_CHECKED);
	CheckDlgButton(IDC_TRACK1, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK2, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK3, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK4, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK5, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK6, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK7, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK8, BST_UNCHECKED);	
}

void CRecord::OnStereo() 
{
	CheckDlgButton(IDC_MONO, BST_UNCHECKED);
	CheckDlgButton(IDC_STEREO, BST_CHECKED);
}

void CRecord::OnTrack1() 
{
	CheckDlgButton(IDC_NEWTRACK, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK1, BST_CHECKED);
	CheckDlgButton(IDC_TRACK2, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK3, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK4, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK5, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK6, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK7, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK8, BST_UNCHECKED);	
}

void CRecord::OnTrack2() 
{
	CheckDlgButton(IDC_NEWTRACK, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK1, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK2, BST_CHECKED);
	CheckDlgButton(IDC_TRACK3, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK4, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK5, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK6, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK7, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK8, BST_UNCHECKED);	
}

void CRecord::OnTrack3() 
{
	CheckDlgButton(IDC_NEWTRACK, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK1, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK2, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK3, BST_CHECKED);
	CheckDlgButton(IDC_TRACK4, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK5, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK6, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK7, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK8, BST_UNCHECKED);	
}

void CRecord::OnTrack4() 
{
	CheckDlgButton(IDC_NEWTRACK, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK1, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK2, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK3, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK4, BST_CHECKED);
	CheckDlgButton(IDC_TRACK5, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK6, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK7, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK8, BST_UNCHECKED);	
}

void CRecord::OnTrack5() 
{
	CheckDlgButton(IDC_NEWTRACK, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK1, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK2, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK3, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK4, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK5, BST_CHECKED);
	CheckDlgButton(IDC_TRACK6, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK7, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK8, BST_UNCHECKED);	
}

void CRecord::OnTrack6() 
{
	CheckDlgButton(IDC_NEWTRACK, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK1, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK2, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK3, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK4, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK5, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK6, BST_CHECKED);
	CheckDlgButton(IDC_TRACK7, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK8, BST_UNCHECKED);	
}

void CRecord::OnTrack7() 
{
	CheckDlgButton(IDC_NEWTRACK, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK1, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK2, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK3, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK4, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK5, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK6, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK7, BST_CHECKED);
	CheckDlgButton(IDC_TRACK8, BST_UNCHECKED);		
}

void CRecord::OnTrack8() 
{
	CheckDlgButton(IDC_NEWTRACK, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK1, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK2, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK3, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK4, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK5, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK6, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK7, BST_UNCHECKED);
	CheckDlgButton(IDC_TRACK8, BST_CHECKED);	
}

void CRecord::OnOK() 
{
	if (IsDlgButtonChecked(IDC_NEWTRACK)) {
		RecTrack = -1;
	}
	if (IsDlgButtonChecked(IDC_TRACK1)) {
		RecTrack = 1;
	}
	if (IsDlgButtonChecked(IDC_TRACK2)) {
		RecTrack = 2;
	}
	if (IsDlgButtonChecked(IDC_TRACK3)) {
		RecTrack = 3;
	}
	if (IsDlgButtonChecked(IDC_TRACK4)) {
		RecTrack = 4;
	}
	if (IsDlgButtonChecked(IDC_TRACK5)) {
		RecTrack = 5;
	}
	if (IsDlgButtonChecked(IDC_TRACK6)) {
		RecTrack = 6;
	}
	if (IsDlgButtonChecked(IDC_TRACK7)) {
		RecTrack = 7;
	}
	if (IsDlgButtonChecked(IDC_TRACK8)) {
		RecTrack = 8;
	}
	if (IsDlgButtonChecked(IDC_MONO)) {
		Stereo = FALSE;
	}
	if (IsDlgButtonChecked(IDC_STEREO)) {
		Stereo = TRUE;
	}
	
	CDialog::OnOK();
}
