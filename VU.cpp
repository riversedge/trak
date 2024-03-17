// VU.cpp : implementation file
//

#include "stdafx.h"
#include "trak.h"
#include "VU.h"
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVU dialog


CVU::CVU(CWnd* pParent /*=NULL*/)
	: CDialog(CVU::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVU)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CVU::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVU)
	DDX_Control(pDX, IDC_LEVELBAR, m_vu);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVU, CDialog)
	//{{AFX_MSG_MAP(CVU)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVU message handlers

HWAVEIN m_hInDeva = NULL;

int Sampling;

DWORD WINAPI UpdateGauge(LPVOID ptr) {
   MMRESULT mmr;
   PCMWAVEFORMAT pFormat;

   Sleep(500);				// Sleep half a second before beginning

   CVU *parent = (CVU *)ptr;
  
   pFormat.wf.wFormatTag = WAVE_FORMAT_PCM;
   pFormat.wf.nChannels = 1;
   pFormat.wf.nSamplesPerSec = 44100;
   pFormat.wf.nAvgBytesPerSec = pFormat.wf.nSamplesPerSec;
   pFormat.wBitsPerSample = 16;
   pFormat.wf.nBlockAlign = (pFormat.wBitsPerSample * pFormat.wf.nChannels) / 8;

   WAVEFORMATEX wFormatEx;
   wFormatEx.wFormatTag = pFormat.wf.wFormatTag;
   wFormatEx.nChannels = pFormat.wf.nChannels;
   wFormatEx.nSamplesPerSec = pFormat.wf.nSamplesPerSec;
   wFormatEx.nAvgBytesPerSec = pFormat.wf.nAvgBytesPerSec;
   wFormatEx.nBlockAlign = pFormat.wf.nBlockAlign;
   wFormatEx.wBitsPerSample = pFormat.wBitsPerSample;
   wFormatEx.cbSize = 0;

   if (Sampling) 
	   mmr = waveInOpen(&m_hInDeva, InstInfo.RecDev, &wFormatEx, NULL, NULL, CALLBACK_NULL);
   else 
	   return FALSE;

   if (mmr == MMSYSERR_ALLOCATED) MessageBox(NULL, "Specified resource is already allocated.", "Trak Multitracking System", MB_ICONEXCLAMATION | MB_APPLMODAL);
   if (mmr == MMSYSERR_BADDEVICEID) MessageBox(NULL, "Specified device identifier is out of range.", "Trak Multitracking System", MB_ICONEXCLAMATION | MB_APPLMODAL);
   if (mmr == MMSYSERR_NODRIVER) MessageBox(NULL, "No device driver is present.", "Trak Multitracking System", MB_ICONEXCLAMATION | MB_APPLMODAL);
   if (mmr == MMSYSERR_NOMEM) MessageBox(NULL, "Unable to allocate or lock memory.", "Trak Multitracking System", MB_ICONEXCLAMATION | MB_APPLMODAL);
   if (mmr == WAVERR_BADFORMAT) MessageBox(NULL, "Attempted to open with an unsupported waveform-audio format.", "Trak Multitracking System", MB_ICONEXCLAMATION | MB_APPLMODAL);	
   if (mmr != MMSYSERR_NOERROR) return FALSE;

   if (Sampling) 
	   mmr = waveInStart(m_hInDeva);
   else 
	   return FALSE;

   if (mmr == MMSYSERR_INVALHANDLE) MessageBox(NULL, "Specified device handle is invalid.", "Trak Multitracking System", MB_ICONEXCLAMATION | MB_APPLMODAL);
   if (mmr == MMSYSERR_NODRIVER) MessageBox(NULL, "No device driver is present.", "Trak Multitracking System", MB_ICONEXCLAMATION | MB_APPLMODAL);
   if (mmr == MMSYSERR_NOMEM) MessageBox(NULL, "Unable to allocate or lock memory.", "Trak Multitracking System", MB_ICONEXCLAMATION | MB_APPLMODAL);
   if (mmr != MMSYSERR_NOERROR) return FALSE;

   while (Sampling) {
      // Allocate a header
      WAVEHDR* phdr = (WAVEHDR *)malloc(sizeof(WAVEHDR));
      // fill out the wave header
      memset(phdr, 0, sizeof(WAVEHDR));
      phdr->lpData = (char *)malloc(4096);
      phdr->dwBufferLength = 4096;
	  phdr->dwFlags = 0;
      phdr->dwUser = (DWORD)(void *)ptr;
      if (Sampling) 
		  mmr = waveInPrepareHeader(m_hInDeva, phdr, sizeof(WAVEHDR));
	  else
		  return FALSE;
	  if (mmr == MMSYSERR_INVALHANDLE) MessageBox(NULL, "Specified device handle is invalid.", "Trak Multitracking System", MB_ICONEXCLAMATION | MB_APPLMODAL);
	  if (mmr == MMSYSERR_NODRIVER) MessageBox(NULL, "No device driver is present.", "Trak Multitracking System", MB_ICONEXCLAMATION | MB_APPLMODAL);
	  if (mmr == MMSYSERR_NOMEM) MessageBox(NULL, "Unable to allocate or lock memory.", "Trak Multitracking System", MB_ICONEXCLAMATION | MB_APPLMODAL);
      if (mmr != MMSYSERR_NOERROR) return FALSE;

      if (Sampling) 
		  mmr = waveInAddBuffer(m_hInDeva, phdr, sizeof(WAVEHDR));
	  else
		  return FALSE;
	  if (mmr == WAVERR_UNPREPARED) MessageBox(NULL, "The buffer pointed to by the pwh parameter hasn't been prepared.", "Trak Multitracking System", MB_ICONEXCLAMATION | MB_APPLMODAL);
	  if (mmr == MMSYSERR_INVALHANDLE) MessageBox(NULL, "Specified device handle is invalid.", "Trak Multitracking System", MB_ICONEXCLAMATION | MB_APPLMODAL);
	  if (mmr == MMSYSERR_NODRIVER) MessageBox(NULL, "No device driver is present.", "Trak Multitracking System", MB_ICONEXCLAMATION | MB_APPLMODAL);
	  if (mmr == MMSYSERR_NOMEM) MessageBox(NULL, "Unable to allocate or lock memory.", "Trak Multitracking System", MB_ICONEXCLAMATION | MB_APPLMODAL);
      if (mmr != MMSYSERR_NOERROR) return FALSE;

	  while (phdr->dwFlags != (phdr->dwFlags | WHDR_DONE)) {
		  Sleep(100);
	  }
	  
	  // Evaluate old Data
	  int iSampleSize = 16 / 8;
	  int iSamples = phdr->dwBytesRecorded / iSampleSize;
	  short int * pData = (short int *)phdr->lpData;
	  int iMax = 0;
	  while (iSamples--) {
		 if (*pData > iMax) iMax = abs(*pData);
	     pData++;
	  }
	
	  if (Sampling) parent->m_vu.SetPos(iMax);

	  waveInUnprepareHeader(m_hInDeva, phdr, sizeof(WAVEHDR));
	  free(phdr->lpData);
	  free(phdr);
   }

   waveInReset(m_hInDeva);
   waveInClose(m_hInDeva);

   return TRUE;
}

BOOL CVU::OnInitDialog( ) {
	CDialog::OnInitDialog();

	Sampling = FALSE;
	m_vu.SetRange(0, 32768);
	m_vu.SetPos(0);

	DWORD tid;
	Sampling = TRUE;
	CreateThread(NULL, 0, &UpdateGauge, this, 0, &tid);

	return TRUE;
}

BOOL CVU::DestroyWindow() 
{
   Sampling = FALSE;

   return CDialog::DestroyWindow();
}
