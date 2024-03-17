// Preferences.cpp : implementation file
//

#include "stdafx.h"
#include "trak.h"
#include "Preferences.h"
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreferences dialog


CPreferences::CPreferences(CWnd* pParent /*=NULL*/)
	: CDialog(CPreferences::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPreferences)
	//}}AFX_DATA_INIT
}


void CPreferences::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferences)
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_SYNCPLAY, m_SyncPlay);
	DDX_Control(pDX, IDC_RECDEV, m_RecDevBox);
	DDX_Control(pDX, IDC_RECBUFFSIZE, m_RecBuffSize);
	DDX_Control(pDX, IDC_PLAYBUFFSIZE, m_PlayBuffSize);
	DDX_Control(pDX, IDC_NUMPLAYBUFF, m_NumPlayBuff);
	DDX_Control(pDX, IDC_PLAYDEV, m_PlayDevBox);
	//}}AFX_DATA_MAP

	RecDev = m_RecDevBox.GetItemData(m_RecDevBox.GetCurSel());
	PlayDev = m_PlayDevBox.GetItemData(m_PlayDevBox.GetCurSel());
	
	char Buff[10];
	m_PlayBuffSize.GetWindowText(Buff, 9);
	PlayBuff = atoi(Buff);
	m_NumPlayBuff.GetWindowText(Buff, 9);
	NumPlayBuff = atoi(Buff);
	m_RecBuffSize.GetWindowText(Buff, 9);
	RecBuff = atoi(Buff);
	
	SyncPlay = m_SyncPlay.GetCheck();
}


BEGIN_MESSAGE_MAP(CPreferences, CDialog)
	//{{AFX_MSG_MAP(CPreferences)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferences message handlers

BOOL CPreferences::OnInitDialog( ) {
   CDialog::OnInitDialog();

   HKEY hKey;
   if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\TRAK", 0, KEY_WRITE, &hKey) !=
		  			ERROR_SUCCESS) {
    RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE", 0, KEY_WRITE, &hKey);
    RegCreateKeyEx(hKey, "TRAK", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE,
     		NULL, &hKey, NULL);
   }
   RecDev = WAVE_MAPPER;
   PlayDev = WAVE_MAPPER;
   PlayBuff = 44;
   NumPlayBuff = 2;
   RecBuff = 44;
   SyncPlay = FALSE;

   // Get currently selected data if it's there
   if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\TRAK", 0, KEY_READ, &hKey) == 0) {
   	DWORD type;
      DWORD sData;
      sData = sizeof(type);
	  RegQueryValueEx(hKey, "RecDevNum", NULL, &type, (LPBYTE)&RecDev, &sData);
      sData = sizeof(type);
   	  RegQueryValueEx(hKey, "PlayDevNum", NULL, &type, (LPBYTE)&PlayDev, &sData);
      sData = sizeof(type);
	  RegQueryValueEx(hKey, "PlayBuffer", NULL, &type, (LPBYTE)&PlayBuff, &sData);
      sData = sizeof(type);
	  RegQueryValueEx(hKey, "PlayBuffers", NULL, &type, (LPBYTE)&NumPlayBuff, &sData);
      sData = sizeof(type);
	  RegQueryValueEx(hKey, "RecBuffer", NULL, &type, (LPBYTE)&RecBuff, &sData);
      sData = sizeof(type);
	  RegQueryValueEx(hKey, "SyncPlay", NULL, &type, (LPBYTE)&SyncPlay, &sData);
   }

   // Get number of waveform playback devices
   UINT NumPlayDevs = waveOutGetNumDevs();
   // Get number of waveform record devices 
   UINT NumRecDevs = waveInGetNumDevs();

   // Playback device info structure
   WAVEOUTCAPS woc;
   // Recording device info structure
   WAVEINCAPS wic;

   UINT EnumCounter;		// Device enumeration counter variable
   int	Index;				// Index of added string in a box

   // Add the wavemapper device to the boxes
   waveInGetDevCaps(WAVE_MAPPER, &wic, sizeof(WAVEINCAPS));
   Index = m_RecDevBox.AddString(wic.szPname);
   m_RecDevBox.SetItemData(Index, WAVE_MAPPER);
   if (RecDev == WAVE_MAPPER) m_RecDevBox.SetCurSel(Index);
   waveOutGetDevCaps(WAVE_MAPPER, &woc, sizeof(WAVEOUTCAPS));
   Index = m_PlayDevBox.AddString(woc.szPname);
   m_PlayDevBox.SetItemData(Index, WAVE_MAPPER);
   if (PlayDev == WAVE_MAPPER) m_PlayDevBox.SetCurSel(Index);

   // Enumerate recording devices
   for (EnumCounter = 0; EnumCounter < NumRecDevs; EnumCounter++) {
      waveInGetDevCaps(EnumCounter, &wic, sizeof(WAVEINCAPS));
	  Index = m_RecDevBox.AddString(wic.szPname);
	  m_RecDevBox.SetItemData(Index, EnumCounter);
	  if (RecDev == EnumCounter) m_RecDevBox.SetCurSel(Index);
   }

   // Enumerate playback devices
   for (EnumCounter = 0; EnumCounter < NumPlayDevs; EnumCounter++) {
      waveOutGetDevCaps(EnumCounter, &woc, sizeof(WAVEOUTCAPS));
	  int Index = m_PlayDevBox.AddString(woc.szPname);
	  m_PlayDevBox.SetItemData(Index, EnumCounter);
	  if (PlayDev == EnumCounter) m_PlayDevBox.SetCurSel(Index);
   }

   char Value[10];
   // Play buffer size
   itoa(PlayBuff, Value, 10);
   m_PlayBuffSize.SetWindowText(Value);
   // Number of play buffers
   itoa(NumPlayBuff, Value, 10);
   m_NumPlayBuff.SetWindowText(Value);
   // Record buffer size
   itoa(RecBuff, Value, 10);
   m_RecBuffSize.SetWindowText(Value);

   // Sync play box
   if (SyncPlay) 
	   m_SyncPlay.SetCheck(TRUE);
   else
	   m_SyncPlay.SetCheck(FALSE);

   if (!InstInfo.FilledPrefs) 
	   m_Cancel.EnableWindow(FALSE);
   else
	   m_Cancel.EnableWindow(TRUE);

   return TRUE;
}

void CPreferences::OnOK() 
{
	BOOL IsError = FALSE;

	UpdateData();

	if (PlayBuff < 4 || PlayBuff > 256) {
  		MessageBox("Invalid play buffer length.  Buffer must be at least 4 kB and no longer than 256 kB",
      		"Trak Multitracking System", MB_ICONSTOP);
      return;
	}
	if (RecBuff < 4 || PlayBuff > 256) {
  		MessageBox("Invalid recording buffer length.  Buffer must be at least 4 kB and no longer than 256 kB",
      		"Trak Multitracking System", MB_ICONSTOP);
      return;
	}
	if (NumPlayBuff < 1 || NumPlayBuff > 10) {
  		MessageBox("Invalid number of play buffers.  Number of play buffers must between 1 and 10.",
      		"Trak Multitracking System", MB_ICONSTOP);
      return;
	}


	HKEY hKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\TRAK", 0, KEY_WRITE, &hKey) !=
  						ERROR_SUCCESS) {
		RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE", 0, KEY_WRITE, &hKey);
		RegCreateKeyEx(hKey, "TRAK", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE,
     					NULL, &hKey, NULL);
	}
	RegSetValueEx(hKey, "RecDevNum", 0, REG_DWORD, (LPBYTE)&RecDev, sizeof(RecDev));
	RegSetValueEx(hKey, "PlayDevNum", 0, REG_DWORD, (LPBYTE)&PlayDev, sizeof(PlayDev));
	RegSetValueEx(hKey, "PlayBuffer", 0, REG_DWORD, (LPBYTE)&PlayBuff, sizeof(PlayBuff));
	RegSetValueEx(hKey, "PlayBuffers", 0, REG_DWORD, (LPBYTE)&NumPlayBuff, sizeof(NumPlayBuff));
	RegSetValueEx(hKey, "RecBuffer", 0, REG_DWORD, (LPBYTE)&RecBuff, sizeof(RecBuff));
	RegSetValueEx(hKey, "SyncPlay", 0, REG_DWORD, (LPBYTE)&SyncPlay, sizeof(SyncPlay));

    InstInfo.RecDev = RecDev;
    InstInfo.PlayDev = PlayDev;
    InstInfo.PlayBuff = PlayBuff;
    InstInfo.NumPlayBuff = NumPlayBuff;
    InstInfo.RecBuff = RecBuff;
    InstInfo.SyncPlay = SyncPlay;
	InstInfo.FilledPrefs = TRUE;

    if (!IsError) CDialog::OnOK();
}
