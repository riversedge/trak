// TrakMain.cpp : implementation file
//

#include "stdafx.h"
#include "trak.h"
#include "TrakMain.h"
#include "Trak.h"
#include "VU.h"
#include "Record.h"
#include <process.h>

#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CTrakApp theApp;
extern CStatusBar  m_wndStatusBar;

/////////////////////////////////////////////////////////////////////////////
// CTrakMain

IMPLEMENT_DYNCREATE(CTrakMain, CFormView)

CTrakMain::CTrakMain()
	: CFormView(CTrakMain::IDD)
{
	//{{AFX_DATA_INIT(CTrakMain)
	m_Mute1 = FALSE;
	m_Mute2 = FALSE;
	m_Mute3 = FALSE;
	m_Mute4 = FALSE;
	m_Mute5 = FALSE;
	m_Mute6 = FALSE;
	m_Mute7 = FALSE;
	m_Mute8 = FALSE;
	m_Track1 = _T("");
	m_Track2 = _T("");
	m_Track3 = _T("");
	m_Track4 = _T("");
	m_Track5 = _T("");
	m_Track6 = _T("");
	m_Track7 = _T("");
	m_Track8 = _T("");
	//}}AFX_DATA_INIT
	hSyncPlay = CreateEvent(NULL, TRUE, FALSE, "PlaySyncEvent");
	hPlayReady = CreateEvent(NULL, TRUE, FALSE, "PlayReadyEvent");
	hRecReady = CreateEvent(NULL, TRUE, FALSE, "RecReadyEvent");
}

CTrakMain::~CTrakMain()
{
	Recorder.StopAndClose();
	bRecording = FALSE;
	theApp.bRecording = FALSE;
	Player.Stop();
	bPlaying = FALSE;
	theApp.bPlaying = FALSE;
	CloseHandle(hSyncPlay);
	CloseHandle(hPlayReady);
	CloseHandle(hRecReady);
}

void CTrakMain::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrakMain)
	DDX_Control(pDX, IDC_VOLUME8, m_Volume8);
	DDX_Control(pDX, IDC_VOLUME7, m_Volume7);
	DDX_Control(pDX, IDC_VOLUME6, m_Volume6);
	DDX_Control(pDX, IDC_VOLUME5, m_Volume5);
	DDX_Control(pDX, IDC_VOLUME4, m_Volume4);
	DDX_Control(pDX, IDC_VOLUME3, m_Volume3);
	DDX_Control(pDX, IDC_VOLUME2, m_Volume2);
	DDX_Control(pDX, IDC_VOLUME1, m_Volume1);
	DDX_Control(pDX, IDC_PAN8, m_Pan8);
	DDX_Control(pDX, IDC_PAN7, m_Pan7);
	DDX_Control(pDX, IDC_PAN6, m_Pan6);
	DDX_Control(pDX, IDC_PAN5, m_Pan5);
	DDX_Control(pDX, IDC_PAN4, m_Pan4);
	DDX_Control(pDX, IDC_PAN3, m_Pan3);
	DDX_Control(pDX, IDC_PAN2, m_Pan2);
	DDX_Control(pDX, IDC_PAN1, m_Pan1);
	DDX_Control(pDX, IDC_MAINVOLUME, m_MainVolume);
	DDX_Check(pDX, IDC_MUTE1, m_Mute1);
	DDX_Check(pDX, IDC_MUTE2, m_Mute2);
	DDX_Check(pDX, IDC_MUTE3, m_Mute3);
	DDX_Check(pDX, IDC_MUTE4, m_Mute4);
	DDX_Check(pDX, IDC_MUTE5, m_Mute5);
	DDX_Check(pDX, IDC_MUTE6, m_Mute6);
	DDX_Check(pDX, IDC_MUTE7, m_Mute7);
	DDX_Check(pDX, IDC_MUTE8, m_Mute8);
	DDX_Text(pDX, IDC_TRACK1, m_Track1);
	DDX_Text(pDX, IDC_TRACK2, m_Track2);
	DDX_Text(pDX, IDC_TRACK3, m_Track3);
	DDX_Text(pDX, IDC_TRACK4, m_Track4);
	DDX_Text(pDX, IDC_TRACK5, m_Track5);
	DDX_Text(pDX, IDC_TRACK6, m_Track6);
	DDX_Text(pDX, IDC_TRACK7, m_Track7);
	DDX_Text(pDX, IDC_TRACK8, m_Track8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrakMain, CFormView)
	//{{AFX_MSG_MAP(CTrakMain)
	ON_COMMAND(ID_MIXER, OnMixer)
	ON_COMMAND(ID_VU_LVL, OnVuLvl)
	ON_BN_CLICKED(IDC_MUTE1, OnMute1)
	ON_BN_CLICKED(IDC_MUTE2, OnMute2)
	ON_BN_CLICKED(IDC_MUTE3, OnMute3)
	ON_BN_CLICKED(IDC_MUTE4, OnMute4)
	ON_BN_CLICKED(IDC_MUTE5, OnMute5)
	ON_BN_CLICKED(IDC_MUTE6, OnMute6)
	ON_BN_CLICKED(IDC_MUTE7, OnMute7)
	ON_BN_CLICKED(IDC_MUTE8, OnMute8)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_EN_CHANGE(IDC_TRACK1, OnChangeTrack1)
	ON_EN_CHANGE(IDC_TRACK2, OnChangeTrack2)
	ON_EN_CHANGE(IDC_TRACK3, OnChangeTrack3)
	ON_EN_CHANGE(IDC_TRACK4, OnChangeTrack4)
	ON_EN_CHANGE(IDC_TRACK5, OnChangeTrack5)
	ON_EN_CHANGE(IDC_TRACK6, OnChangeTrack6)
	ON_EN_CHANGE(IDC_TRACK7, OnChangeTrack7)
	ON_EN_CHANGE(IDC_TRACK8, OnChangeTrack8)
	ON_COMMAND(ID_RECORD, OnRecord)
	ON_COMMAND(ID_PLAY, OnPlay)
	ON_COMMAND(ID_SAVEWAVE, OnSaveAsWave)
	ON_COMMAND(ID_STOP, OnStop)
	ON_COMMAND(IDD_FINDNOCLIP, OnFindnoclip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrakMain diagnostics

#ifdef _DEBUG
void CTrakMain::AssertValid() const
{
	CFormView::AssertValid();
}

void CTrakMain::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTrakMain message handlers

void CTrakMain::OnMixer() 
{
  spawnlp(P_NOWAIT, "SNDVOL32.EXE", "SNDVOL32.EXE", NULL);
}

void CTrakMain::OnVuLvl() 
{
	if (bPlaying || bRecording) {
		MessageBox("Please stop all playback and recording before opening the monitor VU dialog.", "Trak Multitracking System", MB_ICONWARNING);
		return;
	}
	CVU VUlvl;
	
	VUlvl.DoModal();
}

void CTrakMain::OnMute1() 
{
	UpdateData();
	if (m_Mute1) 
		m_wndStatusBar.SetPaneText(0, "Track 1 muted");
	else
		m_wndStatusBar.SetPaneText(0, "Track 1 unmuted");	

	if (bPlaying) Player.SetFileLevels(1, LeftLevel(1), RightLevel(1));
}

void CTrakMain::OnMute2() 
{
	UpdateData();
	if (m_Mute2) 
		m_wndStatusBar.SetPaneText(0, "Track 2 muted");
	else
		m_wndStatusBar.SetPaneText(0, "Track 2 unmuted");	

	if (bPlaying) Player.SetFileLevels(2, LeftLevel(2), RightLevel(2));
}

void CTrakMain::OnMute3() 
{
	UpdateData();
	if (m_Mute3) 
		m_wndStatusBar.SetPaneText(0, "Track 3 muted");
	else
		m_wndStatusBar.SetPaneText(0, "Track 3 unmuted");	

	if (bPlaying) Player.SetFileLevels(3, LeftLevel(3), RightLevel(3));
}

void CTrakMain::OnMute4() 
{
	UpdateData();
	if (m_Mute4) 
		m_wndStatusBar.SetPaneText(0, "Track 4 muted");
	else
		m_wndStatusBar.SetPaneText(0, "Track 4 unmuted");	

	if (bPlaying) Player.SetFileLevels(4, LeftLevel(4), RightLevel(4));
}

void CTrakMain::OnMute5() 
{
	UpdateData();
	if (m_Mute5) 
		m_wndStatusBar.SetPaneText(0, "Track 5 muted");
	else
		m_wndStatusBar.SetPaneText(0, "Track 5 unmuted");	

	if (bPlaying) Player.SetFileLevels(5, LeftLevel(5), RightLevel(5));
}

void CTrakMain::OnMute6() 
{
	UpdateData();
	if (m_Mute6) 
		m_wndStatusBar.SetPaneText(0, "Track 6 muted");
	else
		m_wndStatusBar.SetPaneText(0, "Track 6 unmuted");	

	if (bPlaying) Player.SetFileLevels(6, LeftLevel(6), RightLevel(6));
}

void CTrakMain::OnMute7() 
{
	UpdateData();
	if (m_Mute7) 
		m_wndStatusBar.SetPaneText(0, "Track 7 muted");
	else
		m_wndStatusBar.SetPaneText(0, "Track 7 unmuted");	

	if (bPlaying) Player.SetFileLevels(7, LeftLevel(7), RightLevel(7));
}

void CTrakMain::OnMute8() 
{
	UpdateData();
	if (m_Mute8) 
		m_wndStatusBar.SetPaneText(0, "Track 8 muted");
	else
		m_wndStatusBar.SetPaneText(0, "Track 8 unmuted");	

	if (bPlaying) Player.SetFileLevels(8, LeftLevel(8), RightLevel(8));
}

void CTrakMain::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	theApp.Changed = TRUE;
	if (pScrollBar == (CScrollBar *)&m_Volume1) {	// Track 1 Volume
		char BarText[80];
		char Val[10];
		strcpy(BarText, "Track 1 Volume set to ");
		itoa(abs(m_Volume1.GetPos()), Val, 10);
		strcat(BarText, Val);
		strcat(BarText, "%");
		TProj.SetTrackVolume(1, m_Volume1.GetPos() * -1);
		m_wndStatusBar.SetPaneText(0, BarText);

		m_Mute1 = FALSE;
		UpdateData(FALSE);
		if (bPlaying) Player.SetFileLevels(1, LeftLevel(1), RightLevel(1));
	}
	if (pScrollBar == (CScrollBar *)&m_Volume2) {	// Track 2 Volume
		char BarText[80];
		char Val[10];
		strcpy(BarText, "Track 2 Volume set to ");
		itoa(abs(m_Volume2.GetPos()), Val, 10);
		strcat(BarText, Val);
		strcat(BarText, "%");
		TProj.SetTrackVolume(2, m_Volume2.GetPos() * -1);
		m_wndStatusBar.SetPaneText(0, BarText);

		m_Mute2 = FALSE;
		UpdateData(FALSE);
		if (bPlaying) Player.SetFileLevels(2, LeftLevel(2), RightLevel(2));
	}
	if (pScrollBar == (CScrollBar *)&m_Volume3) {	// Track 3 Volume
		char BarText[80];
		char Val[10];
		strcpy(BarText, "Track 3 Volume set to ");
		itoa(abs(m_Volume3.GetPos()), Val, 10);
		strcat(BarText, Val);
		strcat(BarText, "%");
		TProj.SetTrackVolume(3, m_Volume3.GetPos() * -1);
		m_wndStatusBar.SetPaneText(0, BarText);

		m_Mute3 = FALSE;
		UpdateData(FALSE);
		if (bPlaying) Player.SetFileLevels(3, LeftLevel(3), RightLevel(3));
	}
	if (pScrollBar == (CScrollBar *)&m_Volume4) {	// Track 4 Volume
		char BarText[80];
		char Val[10];
		strcpy(BarText, "Track 4 Volume set to ");
		itoa(abs(m_Volume4.GetPos()), Val, 10);
		strcat(BarText, Val);
		strcat(BarText, "%");
		TProj.SetTrackVolume(4, m_Volume4.GetPos() * -1);
		m_wndStatusBar.SetPaneText(0, BarText);

		m_Mute4 = FALSE;
		UpdateData(FALSE);
		if (bPlaying) Player.SetFileLevels(4, LeftLevel(4), RightLevel(4));
	}
	if (pScrollBar == (CScrollBar *)&m_Volume5) {	// Track 5 Volume
		char BarText[80];
		char Val[10];
		strcpy(BarText, "Track 5 Volume set to ");
		itoa(abs(m_Volume5.GetPos()), Val, 10);
		strcat(BarText, Val);
		strcat(BarText, "%");
		TProj.SetTrackVolume(5, m_Volume5.GetPos() * -1);
		m_wndStatusBar.SetPaneText(0, BarText);

		m_Mute5 = FALSE;
		UpdateData(FALSE);
		if (bPlaying) Player.SetFileLevels(5, LeftLevel(5), RightLevel(5));
	}
	if (pScrollBar == (CScrollBar *)&m_Volume6) {	// Track 6 Volume
		char BarText[80];
		char Val[10];
		strcpy(BarText, "Track 6 Volume set to ");
		itoa(abs(m_Volume6.GetPos()), Val, 10);
		strcat(BarText, Val);
		strcat(BarText, "%");
		TProj.SetTrackVolume(6, m_Volume6.GetPos() * -1);
		m_wndStatusBar.SetPaneText(0, BarText);

		m_Mute6 = FALSE;
		UpdateData(FALSE);
		if (bPlaying) Player.SetFileLevels(6, LeftLevel(6), RightLevel(6));
	}
	if (pScrollBar == (CScrollBar *)&m_Volume7) {	// Track 7 Volume
		char BarText[80];
		char Val[10];
		strcpy(BarText, "Track 7 Volume set to ");
		itoa(abs(m_Volume7.GetPos()), Val, 10);
		strcat(BarText, Val);
		strcat(BarText, "%");
		TProj.SetTrackVolume(7, m_Volume7.GetPos() * -1);
		m_wndStatusBar.SetPaneText(0, BarText);

		m_Mute7 = FALSE;
		UpdateData(FALSE);
		if (bPlaying) Player.SetFileLevels(7, LeftLevel(7), RightLevel(7));
	}
	if (pScrollBar == (CScrollBar *)&m_Volume8) {	// Track 8 Volume
		char BarText[80];
		char Val[10];
		strcpy(BarText, "Track 8 Volume set to ");
		itoa(abs(m_Volume8.GetPos()), Val, 10);
		strcat(BarText, Val);
		strcat(BarText, "%");
		TProj.SetTrackVolume(8, m_Volume8.GetPos() * -1);
		m_wndStatusBar.SetPaneText(0, BarText);

		m_Mute8 = FALSE;
		UpdateData(FALSE);
		if (bPlaying) Player.SetFileLevels(8, LeftLevel(8), RightLevel(8));
	}
	if (pScrollBar == (CScrollBar *)&m_Pan1) {	// Track 1 Pan
		char BarText[80];
		char Val[10];
		int  iVal;
		strcpy(BarText, "Track 1 Panning set to ");
		iVal = m_Pan1.GetPos();
		itoa(abs(iVal), Val, 10);
		if (iVal == 0) 
			strcat(BarText, "center") ;
		else
			strcat(BarText, Val);
		if (iVal < 0) strcat(BarText, " left");
		if (iVal > 0) strcat(BarText, " right");
		TProj.SetTrackPan(1, iVal);
		m_wndStatusBar.SetPaneText(0, BarText);

		if (bPlaying) Player.SetFileLevels(1, LeftLevel(1), RightLevel(1));
	}
	if (pScrollBar == (CScrollBar *)&m_Pan2) {	// Track 2 Pan
		char BarText[80];
		char Val[10];
		int  iVal;
		strcpy(BarText, "Track 2 Panning set to ");
		iVal = m_Pan2.GetPos();
		itoa(abs(iVal), Val, 10);
		if (iVal == 0) 
			strcat(BarText, "center");
		else
			strcat(BarText, Val);
		if (iVal < 0) strcat(BarText, " left");
		if (iVal > 0) strcat(BarText, " right");
		TProj.SetTrackPan(2, iVal);
		m_wndStatusBar.SetPaneText(0, BarText);

		if (bPlaying) Player.SetFileLevels(2, LeftLevel(2), RightLevel(2));
	}
	if (pScrollBar == (CScrollBar *)&m_Pan3) {	// Track 3 Pan
		char BarText[80];
		char Val[10];
		int  iVal;
		strcpy(BarText, "Track 3 Panning set to ");
		iVal = m_Pan3.GetPos();
		itoa(abs(iVal), Val, 10);
		if (iVal == 0) 
			strcat(BarText, "center") ;
		else
			strcat(BarText, Val);
		if (iVal < 0) strcat(BarText, " left");
		if (iVal > 0) strcat(BarText, " right");
		TProj.SetTrackPan(3, iVal);
		m_wndStatusBar.SetPaneText(0, BarText);

		if (bPlaying) Player.SetFileLevels(3, LeftLevel(3), RightLevel(3));
	}
	if (pScrollBar == (CScrollBar *)&m_Pan4) {	// Track 4 Pan
		char BarText[80];
		char Val[10];
		int  iVal;
		strcpy(BarText, "Track 4 Panning set to ");
		iVal = m_Pan4.GetPos();
		itoa(abs(iVal), Val, 10);
		if (iVal == 0) 
			strcat(BarText, "center") ;
		else
			strcat(BarText, Val);
		if (iVal < 0) strcat(BarText, " left");
		if (iVal > 0) strcat(BarText, " right");
		TProj.SetTrackPan(4, iVal);
		m_wndStatusBar.SetPaneText(0, BarText);

		if (bPlaying) Player.SetFileLevels(4, LeftLevel(4), RightLevel(4));
	}
	if (pScrollBar == (CScrollBar *)&m_Pan5) {	// Track 5 Pan
		char BarText[80];
		char Val[10];
		int  iVal;
		strcpy(BarText, "Track 5 Panning set to ");
		iVal = m_Pan5.GetPos();
		itoa(abs(iVal), Val, 10);
		if (iVal == 0) 
			strcat(BarText, "center");
		else
			strcat(BarText, Val);
		if (iVal < 0) strcat(BarText, " left");
		if (iVal > 0) strcat(BarText, " right");
		TProj.SetTrackPan(5, iVal);
		m_wndStatusBar.SetPaneText(0, BarText);

		if (bPlaying) Player.SetFileLevels(5, LeftLevel(5), RightLevel(5));
	}
	if (pScrollBar == (CScrollBar *)&m_Pan6) {	// Track 6 Pan
		char BarText[80];
		char Val[10];
		int  iVal;
		strcpy(BarText, "Track 6 Panning set to ");
		iVal = m_Pan6.GetPos();
		itoa(abs(iVal), Val, 10);
		if (iVal == 0) 
			strcat(BarText, "center");
		else
			strcat(BarText, Val);
		if (iVal < 0) strcat(BarText, " left");
		if (iVal > 0) strcat(BarText, " right");
		TProj.SetTrackPan(6, iVal);
		m_wndStatusBar.SetPaneText(0, BarText);

		if (bPlaying) Player.SetFileLevels(6, LeftLevel(6), RightLevel(6));
	}
	if (pScrollBar == (CScrollBar *)&m_Pan7) {	// Track 7 Pan
		char BarText[80];
		char Val[10];
		int  iVal;
		strcpy(BarText, "Track 7 Panning set to ");
		iVal = m_Pan7.GetPos();
		itoa(abs(iVal), Val, 10);
		if (iVal == 0) 
			strcat(BarText, "center");
		else
			strcat(BarText, Val);
		if (iVal < 0) strcat(BarText, " left");
		if (iVal > 0) strcat(BarText, " right");
		TProj.SetTrackPan(7, iVal);
		m_wndStatusBar.SetPaneText(0, BarText);

		if (bPlaying) Player.SetFileLevels(7, LeftLevel(7), RightLevel(7));
	}
	if (pScrollBar == (CScrollBar *)&m_Pan8) {	// Track 8 Pan
		char BarText[80];
		char Val[10];
		int  iVal;
		strcpy(BarText, "Track 8 Panning set to ");
		iVal = m_Pan8.GetPos();
		itoa(abs(iVal), Val, 10);
		if (iVal == 0) 
			strcat(BarText, "center");
		else
			strcat(BarText, Val);
		if (iVal < 0) strcat(BarText, " left");
		if (iVal > 0) strcat(BarText, " right");
		TProj.SetTrackPan(8, iVal);
		m_wndStatusBar.SetPaneText(0, BarText);

		if (bPlaying) Player.SetFileLevels(8, LeftLevel(8), RightLevel(8));
	}

	CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CTrakMain::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	theApp.Changed = TRUE;
	if (pScrollBar == (CScrollBar *)&m_MainVolume) {	// Main Volume
		char BarText[80];
		char Val[10];
		strcpy(BarText, "Main Volume set to ");
		itoa(m_MainVolume.GetPos(), Val, 10);
		strcat(BarText, Val);
		strcat(BarText, "%");
		TProj.SetMainVolume(m_MainVolume.GetPos());
		m_wndStatusBar.SetPaneText(0, BarText);
		if (bPlaying) {
			for (int Ctr = 0; Ctr < TProj.GetNumberTracks(); Ctr++) {
				int Track = Ctr + 1;
				Player.SetFileLevels(Track, LeftLevel(Track), RightLevel(Track));
			}
		}
	}
	
	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTrakMain::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	m_MainVolume.SetRange(0, 150, TRUE);
	m_MainVolume.SetPos(100);
	m_Volume1.SetRange(-150, 0, TRUE);
	m_Volume1.SetPos(-100);
	m_Volume2.SetRange(-150, 0, TRUE);
	m_Volume2.SetPos(-100);
	m_Volume3.SetRange(-150, 0, TRUE);
	m_Volume3.SetPos(-100);
	m_Volume4.SetRange(-150, 0, TRUE);
	m_Volume4.SetPos(-100);
	m_Volume5.SetRange(-150, 0, TRUE);
	m_Volume5.SetPos(-100);
	m_Volume6.SetRange(-150, 0, TRUE);
	m_Volume6.SetPos(-100);
	m_Volume7.SetRange(-150, 0, TRUE);
	m_Volume7.SetPos(-100);
	m_Volume8.SetRange(-150, 0, TRUE);
	m_Volume8.SetPos(-100);
	m_Pan1.SetRange(-100, 100, TRUE);
	m_Pan1.SetPos(0);
	m_Pan2.SetRange(-100, 100, TRUE);
	m_Pan2.SetPos(0);
	m_Pan3.SetRange(-100, 100, TRUE);
	m_Pan3.SetPos(0);
	m_Pan4.SetRange(-100, 100, TRUE);
	m_Pan4.SetPos(0);
	m_Pan5.SetRange(-100, 100, TRUE);
	m_Pan5.SetPos(0);
	m_Pan6.SetRange(-100, 100, TRUE);
	m_Pan6.SetPos(0);
	m_Pan7.SetRange(-100, 100, TRUE);
	m_Pan7.SetPos(0);
	m_Pan8.SetRange(-100, 100, TRUE);
	m_Pan8.SetPos(0);
	m_Mute1 = FALSE;
	m_Mute2 = FALSE;
	m_Mute3 = FALSE;
	m_Mute4 = FALSE;
	m_Mute5 = FALSE;
	m_Mute6 = FALSE;
	m_Mute7 = FALSE;
	m_Mute8 = FALSE;
	m_Track1 = "";
	m_Track2 = "";
	m_Track3 = "";
	m_Track4 = "";
	m_Track5 = "";
	m_Track6 = "";
	m_Track7 = "";
	m_Track8 = "";

	bPlaying = FALSE;
	theApp.bPlaying = FALSE;
	bRecording = FALSE;
	theApp.bRecording = FALSE;

	UpdateData(FALSE);
	if (!InstInfo.FilledStart) OnFileNew();
	UpdateFromProj();
}

void CTrakMain::OnFileSave() 
{
	if (!InstInfo.FilledStart) return;

	TProj.SetMainVolume(m_MainVolume.GetPos());	
	TProj.SetTrackVolume(1, m_Volume1.GetPos() * -1);
	TProj.SetTrackVolume(2, m_Volume2.GetPos() * -1);
	TProj.SetTrackVolume(3, m_Volume3.GetPos() * -1);
	TProj.SetTrackVolume(4, m_Volume4.GetPos() * -1);
	TProj.SetTrackVolume(5, m_Volume5.GetPos() * -1);
	TProj.SetTrackVolume(6, m_Volume6.GetPos() * -1);
	TProj.SetTrackVolume(7, m_Volume7.GetPos() * -1);
	TProj.SetTrackVolume(8, m_Volume8.GetPos() * -1);
	TProj.SetTrackPan(1, m_Pan1.GetPos());
	TProj.SetTrackPan(2, m_Pan2.GetPos());
	TProj.SetTrackPan(3, m_Pan3.GetPos());
	TProj.SetTrackPan(4, m_Pan4.GetPos());
	TProj.SetTrackPan(5, m_Pan5.GetPos());
	TProj.SetTrackPan(6, m_Pan6.GetPos());
	TProj.SetTrackPan(7, m_Pan7.GetPos());
	TProj.SetTrackPan(8, m_Pan8.GetPos());
	UpdateData();
	TProj.SetTrackName(1, m_Track1);
	TProj.SetTrackName(2, m_Track2);
	TProj.SetTrackName(3, m_Track3);
	TProj.SetTrackName(4, m_Track4);
	TProj.SetTrackName(5, m_Track5);
	TProj.SetTrackName(6, m_Track6);
	TProj.SetTrackName(7, m_Track7);
	TProj.SetTrackName(8, m_Track8);

	theApp.OnFileSave();
}

void CTrakMain::OnFileOpen() 
{
	if (bPlaying || bRecording) {
		MessageBox("Please stop all playback and recording before opening a file.", "Trak Multitracking System", MB_ICONWARNING);
		return;
	}
	if (theApp.Changed) {
		int rc = MessageBox("File has changed.  Save changes?", "Trak Multitracking System", MB_ICONQUESTION | MB_YESNOCANCEL);
		if (rc == IDCANCEL) return;
		if (rc == IDYES) OnFileSave();
	}

	CString FileName = theApp.SaveFileName;

	theApp.OnFileOpen();

	// If name hasn't changed, quit
	if (theApp.SaveFileName == FileName) return;

	UpdateFromProj();	
}

void CTrakMain::UpdateFromProj()
{
	m_MainVolume.SetPos(TProj.GetMainVolume());	
	m_Volume1.SetPos(TProj.GetTrackVolume(1) * -1);
	m_Volume2.SetPos(TProj.GetTrackVolume(2) * -1);
	m_Volume3.SetPos(TProj.GetTrackVolume(3) * -1);
	m_Volume4.SetPos(TProj.GetTrackVolume(4) * -1);
	m_Volume5.SetPos(TProj.GetTrackVolume(5) * -1);
	m_Volume6.SetPos(TProj.GetTrackVolume(6) * -1);
	m_Volume7.SetPos(TProj.GetTrackVolume(7) * -1);
	m_Volume8.SetPos(TProj.GetTrackVolume(8) * -1);
	m_Pan1.SetPos(TProj.GetTrackPan(1));
	m_Pan2.SetPos(TProj.GetTrackPan(2));
	m_Pan3.SetPos(TProj.GetTrackPan(3));
	m_Pan4.SetPos(TProj.GetTrackPan(4));
	m_Pan5.SetPos(TProj.GetTrackPan(5));
	m_Pan6.SetPos(TProj.GetTrackPan(6));
	m_Pan7.SetPos(TProj.GetTrackPan(7));
	m_Pan8.SetPos(TProj.GetTrackPan(8));
	m_Track1 = TProj.GetTrackName(1);
	m_Track2 = TProj.GetTrackName(2);
	m_Track3 = TProj.GetTrackName(3);
	m_Track4 = TProj.GetTrackName(4);
	m_Track5 = TProj.GetTrackName(5);
	m_Track6 = TProj.GetTrackName(6);
	m_Track7 = TProj.GetTrackName(7);
	m_Track8 = TProj.GetTrackName(8);
	m_Mute1 = FALSE;
	m_Mute2 = FALSE;
	m_Mute3 = FALSE;
	m_Mute4 = FALSE;
	m_Mute5 = FALSE;
	m_Mute6 = FALSE;
	m_Mute7 = FALSE;
	m_Mute8 = FALSE;
	UpdateData(FALSE);		
}

void CTrakMain::OnFileNew() 
{
	if (bPlaying || bRecording) {
		MessageBox("Please stop all playback and recording before starting a new file.", "Trak Multitracking System", MB_ICONWARNING);
		return;
	}
	theApp.OnFileNew();
	UpdateFromProj();
}

void CTrakMain::OnChangeTrack1() 
{
	UpdateData();
	TProj.SetTrackName(1, m_Track1);
	theApp.Changed = TRUE;
}

void CTrakMain::OnChangeTrack2() 
{
	UpdateData();
	TProj.SetTrackName(2, m_Track2);
	theApp.Changed = TRUE;
}

void CTrakMain::OnChangeTrack3() 
{
	UpdateData();
	TProj.SetTrackName(3, m_Track3);
	theApp.Changed = TRUE;
}

void CTrakMain::OnChangeTrack4() 
{
	UpdateData();
	TProj.SetTrackName(4, m_Track4);
	theApp.Changed = TRUE;
}

void CTrakMain::OnChangeTrack5() 
{
	UpdateData();
	TProj.SetTrackName(5, m_Track5);
	theApp.Changed = TRUE;
}

void CTrakMain::OnChangeTrack6() 
{
	UpdateData();
	TProj.SetTrackName(6, m_Track6);
	theApp.Changed = TRUE;
}

void CTrakMain::OnChangeTrack7() 
{
	UpdateData();
	TProj.SetTrackName(7, m_Track7);
	theApp.Changed = TRUE;
}

void CTrakMain::OnChangeTrack8() 
{
	UpdateData();
	TProj.SetTrackName(8, m_Track8);
	theApp.Changed = TRUE;
}

DWORD WINAPI DispCounter(LPVOID param) {
	CTrakMain *parent = (CTrakMain *)param;
	DWORD Time = 0;
	DWORD Min;
	DWORD Sec;
	char  sSec[10];
	char  sMin[10];
	char  sTime[30];

	char  sAction[25];
	if (parent->bPlaying)   strcpy(sAction, "Playing      ");
	if (parent->bRecording) strcpy(sAction, "Recording    ");

	HANDLE hSyncPlay = CreateEvent(NULL, TRUE, FALSE, "PlaySyncEvent");
	WaitForSingleObject(hSyncPlay, INFINITE);

	while (parent->bPlaying || parent->bRecording) {  // will terminate when stop is called
	  Sec = Time % 60;
      Min = Time / 60;
      itoa(Sec, sSec, 10);
      itoa(Min, sMin, 10);
      strcpy(sTime, sAction);
      if (Min < 10) strcat(sTime, "0");
      strcat(sTime, sMin);
      strcat(sTime, ":");
      if (Sec < 10) strcat(sTime, "0");
      strcat(sTime, sSec);
      m_wndStatusBar.SetWindowText(sTime);
	  m_wndStatusBar.UpdateWindow();
      Sleep(1000);
      Time++;
   }

	m_wndStatusBar.SetWindowText("Ready");
	return TRUE;
}

DWORD WINAPI RecThread(LPVOID param) {
	CTrakMain *parent = (CTrakMain *)param;
	CRec *Recorder = &(parent->Recorder);
	CPlayMix *Player = &(parent->Player);

	if (!Recorder->Record()) {
		SetEvent(parent->hSyncPlay);
		TProj.SetNumberTracks(TProj.GetNumberTracks() - 1);
		Player->Stop();
		parent->bPlaying = FALSE;
		theApp.bPlaying = FALSE;
		Recorder->StopAndClose();
		parent->bRecording = FALSE;
		theApp.bRecording = FALSE;

		return FALSE;
	}

	parent->bRecording = FALSE;
	theApp.bRecording = FALSE;

	return TRUE;	
}


DWORD WINAPI PlayThread(LPVOID param) {
	CTrakMain *parent = (CTrakMain *)param;
	
	parent->Player.Play(InstInfo.PlayDev);

	parent->bPlaying = FALSE;
	theApp.bPlaying = FALSE;

	return TRUE;
}

void CTrakMain::OnRecord() 
{
	if (!InstInfo.FilledStart) {
		OnFileNew();
		return;
	}

	if (bPlaying && !InstInfo.SyncPlay) {
		MessageBox("Synchronous playback is not enabled.  Cannot record and play at same time.", "Trak Multitracking System", MB_ICONWARNING);
		return;
	}
	if (bPlaying && InstInfo.SyncPlay) {
		MessageBox("Synchronous playback/record must be started when everything is stopped.  Stop playback and then select record.", "Trak Multitracking System", MB_ICONWARNING);
		return;
	}
	
	ResetEvent(hSyncPlay);
	ResetEvent(hSyncPlay);
	ResetEvent(hPlayReady);
	ResetEvent(hRecReady);

	CRecord		RecordPrefs;

	if (RecordPrefs.DoModal() != IDOK) return;

	DWORD tid;	

	// Setup stuff to pass to recording object
	PCMWAVEFORMAT PWFmt;

	PWFmt.wf.wFormatTag = WAVE_FORMAT_PCM;
	switch (InstInfo.SampRate) {
	case 44:
		PWFmt.wf.nSamplesPerSec = 44100;
		break;
	case 22:
		PWFmt.wf.nSamplesPerSec = 22050;
		break;
	case 11:
		PWFmt.wf.nSamplesPerSec = 11025;
		break;
	}
	PWFmt.wf.nAvgBytesPerSec = PWFmt.wf.nSamplesPerSec * PWFmt.wf.nChannels * (PWFmt.wBitsPerSample / 8);
	PWFmt.wBitsPerSample = InstInfo.SampBits;
	
	if (RecordPrefs.Stereo) 
		PWFmt.wf.nChannels = 2;
	else
		PWFmt.wf.nChannels = 1;

	PWFmt.wf.nBlockAlign = (PWFmt.wBitsPerSample * PWFmt.wf.nChannels) / 8;

	Recorder.SetFormat(&PWFmt);

	Recorder.SetSignalHandle(hRecReady);
	Recorder.SetWaitForHandle(hSyncPlay);

	RecFilename = TProj.GetBase();
	RecFilename += "-T";
	char Val[10];

	if (RecordPrefs.RecTrack <= TProj.GetNumberTracks() && RecordPrefs.RecTrack != -1) {
		CString Msg;
		Msg = "Are you sure you want to overwrite the data in track ";
		switch (RecordPrefs.RecTrack) {
		case 1:
			Msg += "1 (";
			Msg += m_Track1;
			Msg += ")?";
			break;
		case 2:
			Msg += "2 (";
			Msg += m_Track2;
			Msg += ")?";
			break;
		case 3:
			Msg += "3 (";
			Msg += m_Track3;
			Msg += ")?";
			break;
		case 4:
			Msg += "4 (";
			Msg += m_Track4;
			Msg += ")?";
			break;
		case 5:
			Msg += "5 (";
			Msg += m_Track5;
			Msg += ")?";
			break;
		case 6:
			Msg += "6 (";
			Msg += m_Track6;
			Msg += ")?";
			break;
		case 7:
			Msg += "7 (";
			Msg += m_Track7;
			Msg += ")?";
			break;
		case 8:
			Msg += "8 (";
			Msg += m_Track8;
			Msg += ")?";
			break;
		}
		int MsgRC = MessageBox(Msg, "Trak Multitracking System", MB_ICONQUESTION | MB_YESNO);
		if (MsgRC == IDNO) return;
	}

	switch (RecordPrefs.RecTrack) {
	case -1:	// New track
		itoa(TProj.GetNumberTracks() + 1, Val, 10);
		RecFilename += Val;
		RecFilename += ".WAV";
		break;
	case 1:
		RecFilename += "1.WAV";
		break;
	case 2:
		RecFilename += "2.WAV";
		break;
	case 3:
		RecFilename += "3.WAV";
		break;
	case 4:
		RecFilename += "4.WAV";
		break;
	case 5:
		RecFilename += "5.WAV";
		break;
	case 6:
		RecFilename += "6.WAV";
		break;
	case 7:
		RecFilename += "7.WAV";
		break;
	case 8:
		RecFilename += "8.WAV";
		break;
	}

	// Open recording device
	int BufferSize = InstInfo.RecBuff * 1024;	// KB to Bytes
	char RecordFileName[255];
	strncpy(RecordFileName, RecFilename, 254);
	
	if (!Recorder.Open(InstInfo.RecDev, RecordFileName, BufferSize)) {
		Recorder.StopAndClose();
		return;
	}
	
	// Setup playback stuff

	// Wait and setup for sync if necessary
		// If statement: sync play enabled and more than 1 track
		//			     not only 1 track and recording track 1  (special case)
	if ((InstInfo.SyncPlay && TProj.GetNumberTracks() > 0) &&		
		!(TProj.GetNumberTracks() == 1 && RecordPrefs.RecTrack == 1)) {
		Player.SetSignalHandle(hPlayReady);
		Player.SetWaitForHandle(hSyncPlay);

		Player.BufferSize(InstInfo.PlayBuff * 1024);	// KB to bytes of play buffer
		Player.NumberBuffers(InstInfo.NumPlayBuff);
	
		// Queue Play Files
		Player.InitQueue(TProj.GetNumberTracks());
		for (int Ctr = 0; Ctr < TProj.GetNumberTracks(); Ctr++) {
			int Track = Ctr + 1;
			if (Track != RecordPrefs.RecTrack) 
				Player.QueueFile(Track, GenFilename(Track), LeftLevel(Track), RightLevel(Track));
			else
				Player.QueueFile(Track, NULL, 0.0, 0.0);
		}

		bPlaying = TRUE;
		theApp.bPlaying = TRUE;
		// Create playback thread
		hPlayer = CreateThread(NULL, 0, PlayThread, this, NULL, &tid);
		bRecording = TRUE;
		theApp.bRecording = TRUE;
		// Create the record thread 
		hRecorder = CreateThread(NULL, 0, RecThread, this, NULL, &tid);	
		// Create play counter thread
		hPlayRecCounter = CreateThread(NULL, 0, DispCounter, this, NULL, &tid);
		// Wait until recorder object is ready
		WaitForSingleObject(hRecReady, INFINITE);
		// Wait until playback object is ready
		WaitForSingleObject(hPlayReady, INFINITE);
		SetEvent(hSyncPlay);
	} else {
		bRecording = TRUE;
		theApp.bRecording = TRUE;

		// Create the record thread 
		hRecorder = CreateThread(NULL, 0, RecThread, this, NULL, &tid);	
		hPlayRecCounter = CreateThread(NULL, 0, DispCounter, this, NULL, &tid);
		SetEvent(hSyncPlay);
	}	
	theApp.Changed = TRUE;
	if (RecordPrefs.RecTrack == -1) TProj.SetNumberTracks(TProj.GetNumberTracks() + 1);
}

void CTrakMain::OnStop() 
{
	if (bRecording) {
		Recorder.StopAndClose();
		bRecording = FALSE;
		theApp.bRecording = FALSE;
	}
	if (bPlaying) {
		Player.Stop();
		bPlaying = FALSE;
		theApp.bPlaying = FALSE;
	}
}

void CTrakMain::OnPlay() 
{
	if (TProj.GetNumberTracks() < 1) return;

	if (!InstInfo.FilledStart) {
		OnFileNew();
		return;
	}

	if (bRecording) {
		MessageBox("Synchronous playback is activated if you record with that option on.  Cannot begin playback after recording has started.", "Trak Multitracking System", MB_ICONWARNING);
		return;
	}

	ResetEvent(hSyncPlay);
	ResetEvent(hPlayReady);
	ResetEvent(hRecReady);
	
	Player.SetSignalHandle(hPlayReady);
	Player.SetWaitForHandle(NULL);

	Player.BufferSize(InstInfo.PlayBuff * 1024);	// KB to bytes of play buffer
	Player.NumberBuffers(InstInfo.NumPlayBuff);
	
	bPlaying = TRUE;
	theApp.bPlaying = TRUE;

	// Queue Play Files
	Player.InitQueue(TProj.GetNumberTracks());
	for (int Ctr = 0; Ctr < TProj.GetNumberTracks(); Ctr++) {
		int Track = Ctr + 1;
		Player.QueueFile(Track, GenFilename(Track), LeftLevel(Track), RightLevel(Track));
	}

	// Create playback thread
	DWORD tid;
	hPlayer = CreateThread(NULL, 0, PlayThread, this, NULL, &tid);

	// Create the play counter thread
	hPlayRecCounter = CreateThread(NULL, 0, DispCounter, this, NULL, &tid);

	// Begin playing
	SetEvent(hSyncPlay);
}

CString CTrakMain::GenFilename(int Track)
{
   CString ReturnVal;

   ReturnVal = TProj.GetBase();
   ReturnVal += "-T";

   char sTrack[10];
   itoa(Track, sTrack, 10);
   ReturnVal += sTrack;
   ReturnVal += ".WAV";

   return ReturnVal;
}

double CTrakMain::LeftLevel(int Track)
{
	double Level;

	if (Track == 1 && m_Mute1) return 0.0;
	if (Track == 2 && m_Mute2) return 0.0;
	if (Track == 3 && m_Mute3) return 0.0;
	if (Track == 4 && m_Mute4) return 0.0;
	if (Track == 5 && m_Mute5) return 0.0;
	if (Track == 6 && m_Mute6) return 0.0;
	if (Track == 7 && m_Mute7) return 0.0;
	if (Track == 8 && m_Mute8) return 0.0;

	double MainVolume = TProj.GetMainVolume();
	double Volume = TProj.GetTrackVolume(Track);
	double Pan = TProj.GetTrackPan(Track);
	double LeftPan = 100.0;

	// Turn main volume into decimal percent representation
	MainVolume /= 100.0;
	// Turn track volume into decimal percent representation
	Volume /= 100.0;
	// Turn track pan into decimal percent representation (R/L)
	if (Pan > 0) {	// Right panning high
		// Right panning level is 100%
		// Left panning level is 100% - % panned right
		LeftPan -= Pan;
	}
	LeftPan /= 100.0;

	Level = MainVolume * Volume * LeftPan;

	return Level;
}

double CTrakMain::RightLevel(int Track)
{
	double Level;

	if (Track == 1 && m_Mute1) return 0.0;
	if (Track == 2 && m_Mute2) return 0.0;
	if (Track == 3 && m_Mute3) return 0.0;
	if (Track == 4 && m_Mute4) return 0.0;
	if (Track == 5 && m_Mute5) return 0.0;
	if (Track == 6 && m_Mute6) return 0.0;
	if (Track == 7 && m_Mute7) return 0.0;
	if (Track == 8 && m_Mute8) return 0.0;

	double MainVolume = TProj.GetMainVolume();
	double Volume = TProj.GetTrackVolume(Track);
	double Pan = TProj.GetTrackPan(Track);
	double RightPan = 100.0;

	// Turn main volume into decimal percent representation
	MainVolume /= 100.0;
	// Turn track volume into decimal percent representation
	Volume /= 100.0;
	// Turn track pan into decimal percent representation (R/L)
	if (Pan < 0) {	// Left panning high
		// Left panning level is 100%
		// Right panning level is 100% - % panned left
		RightPan += Pan;		
	}
	RightPan /= 100.0;

	Level = MainVolume * Volume * RightPan;

	return Level;
}

void CTrakMain::OnSaveAsWave() 
{
	if (bPlaying || bRecording) {
		MessageBox("Please stop all playback and recording before mixing this file to a standard PCM wave file.", "Trak Multitracking System", MB_ICONWARNING);
		return;
	}
  if (TProj.GetNumberTracks() < 1) {
   	  MessageBox("Cannot mix to standard PCM WAVE file.  There is no data to mix.", "Trak Multitracking System",
      				MB_ICONEXCLAMATION);
      return;
  }

  m_wndStatusBar.SetWindowText("");

  char WorkDirName[5000];
  int   WorkDisk;
  getcwd(WorkDirName, 4999);
  WorkDisk = _getdrive();

  CFileDialog data(FALSE, "wav", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
							"PCM WAVE Files (*.wav)|*.wav|All Files (*.*)|*.*|", this);


  DWORD start, end;

  if (data.DoModal() == IDOK) {
	  _chdrive(WorkDisk);
	 chdir(WorkDirName);
     SetCursor(HCURSOR(IDC_WAIT));

	 m_wndStatusBar.SetWindowText("Analyzing wave files . . .");
	 m_wndStatusBar.UpdateWindow();

	 CWave  mixer;

     mixer.BufferSize = 1024 * 10 * TProj.GetNumberTracks(); // 10k * Tracks Buffers

     for (int i = 0; i < TProj.GetNumberTracks(); i++) {
		UpdateWindow();
        char WorkFile[80];
        double   Amplification;
        double   PanLevel;
	    char Num[5];
		strcpy(WorkFile, TProj.GetBase());
		strcat(WorkFile, "-T");
	    itoa((i+1), Num, 10);
	    strcat(WorkFile, Num);
	    strcat(WorkFile, ".Wav");
        // This tracks amplification level
        Amplification = TProj.GetMainVolume() * (TProj.GetTrackVolume(i+1)) / 100.0;
        // This tracks pan level (+ right, - left)
        PanLevel = TProj.GetTrackPan(i+1); // Panning
        mixer.AddFile(WorkFile, Amplification, PanLevel);
     }
	 m_wndStatusBar.SetWindowText("Writing wave file to disk . . .");
	 m_wndStatusBar.UpdateWindow();

    _chdrive(WorkDisk);
	chdir(WorkDirName);

     start = timeGetTime();
     mixer.MixTo(data.GetPathName(), &m_wndStatusBar);
     end = timeGetTime();
  }

  SetCursor(HCURSOR(IDC_ARROW));
  char Msg[120];
  sprintf(Msg, "Total elapsed time: %.3lfs", ((end - start) / 1000.));
  m_wndStatusBar.SetWindowText(Msg);
  m_wndStatusBar.UpdateWindow();
}


void CTrakMain::OnFindnoclip() 
{
	if (bPlaying || bRecording) {
		MessageBox("Please stop all playback and recording before attempting to find the no clipping zone.", "Trak Multitracking System", MB_ICONWARNING);
		return;
	}
   int RC;

   RC = MessageBox("This process may take a few minutes.  Continue?", "Trak Multitracking System", MB_APPLMODAL | MB_ICONQUESTION | MB_YESNO);
   if (RC == IDNO) return;   

   SetCursor(HCURSOR(IDC_WAIT));
   m_wndStatusBar.SetWindowText("Calculating maximum non-clip level . . .");
   m_wndStatusBar.UpdateWindow();

   CWave  NoClipTester;

   NoClipTester.BufferSize = 1024 * 10 * TProj.GetNumberTracks(); // 10k * Tracks Buffers

   for (int i = 0; i < TProj.GetNumberTracks(); i++) {
		UpdateWindow();
        char WorkFile[80];
        double   Amplification;
        double   PanLevel;
	    char Num[5];
		strcpy(WorkFile, TProj.GetBase());
		strcat(WorkFile, "-T");
	    itoa((i+1), Num, 10);
	    strcat(WorkFile, Num);
	    strcat(WorkFile, ".Wav");
        // This tracks amplification level
        Amplification = TProj.GetMainVolume() * (TProj.GetTrackVolume(i+1)) / 100.0;
        // This tracks pan level (+ right, - left)
        PanLevel = TProj.GetTrackPan(i+1); // Panning
        NoClipTester.AddFile(WorkFile, Amplification, PanLevel);
   }

   // Find the no clip zone that it 99% of the maximum total clip zone
   int NewMainLevel = NoClipTester.FindNormal(TProj.GetMainVolume(), 99, &m_wndStatusBar);

   SetCursor(HCURSOR(IDC_ARROW));
   m_wndStatusBar.SetWindowText("");

   if (NewMainLevel > 150) NewMainLevel = 150;

   if (NewMainLevel < 0) {
      MessageBox("Could not find a non-clip level.", "Trak Multitracking System", MB_ICONSTOP);
      return;
   }

   char Message[500];
   char sMainLevel[17];

   strcpy(Message, "TRAK has determined that setting your master volume to ");
   itoa(NewMainLevel, sMainLevel, 10);
   strcat(Message, sMainLevel);
   strcat(Message, " will prevent any mixing induced clipping.  If any of the");
   strcat(Message, " individual files clip then clipping may still occur.");
   strcat(Message, "  To resolve such a problem you must re-record that track.");
   strcat(Message, "  Would you like to TRAK to automatically adjust the main");
   strcat(Message, " volume level?");
   RC = MessageBox(Message, "Trak Multitracking System", MB_APPLMODAL | MB_ICONQUESTION | MB_YESNO);
   if (RC == IDYES) {
	  m_MainVolume.SetPos(NewMainLevel);	
   	  TProj.SetMainVolume(NewMainLevel);
      strcpy(Message, "Main Volume set to ");
      strcat(Message, sMainLevel);
	  m_wndStatusBar.SetWindowText(Message);
   }
}
