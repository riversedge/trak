#include "CRec.h"
#include <io.h>

CRec::CRec() {
   Recording = FALSE;
   wi = NULL;
   hWaitFor = NULL;
   hSendSignal = NULL;
   hGetData = CreateEvent(NULL, TRUE, FALSE, "CleaningUpRecord");
}

CRec::~CRec() {
   CloseHandle(hGetData);
   if (wi) delete wi;
}

void CRec::WriteHeader(Waveheader *wh, FILE *fp) {
   fwrite(wh->RIFF, sizeof(BYTE), 4, fp);
   fwrite(&wh->dwSize, sizeof(BYTE), 4, fp); 
   fwrite(wh->WAVE, sizeof(BYTE), 4, fp); 
   fwrite(wh->fmt_, sizeof(BYTE), 4, fp); 
   fwrite(&wh->dw18, sizeof(BYTE), 4, fp); 
   fwrite(&wh->wFFormat, sizeof(BYTE), 2, fp); 
   fwrite(&wh->wChnls, sizeof(BYTE), 2, fp); 
   fwrite(&wh->dwSRate, sizeof(BYTE), 4, fp); 
   fwrite(&wh->BytesPerSec, sizeof(BYTE), 4, fp); 
   fwrite(&wh->wBlkAlign, sizeof(BYTE), 2, fp); 
   fwrite(&wh->BitsPerSample, sizeof(BYTE), 2, fp); 
   fwrite(&wh->Zero, sizeof(BYTE), 2, fp); 
   fwrite(wh->FACT, sizeof(BYTE), 4, fp); 
   fwrite(&wh->Four, sizeof(BYTE), 4, fp); 
   fwrite(&wh->AvgBytesSec, sizeof(BYTE), 4, fp); 
   fwrite(wh->DATA, sizeof(BYTE), 4, fp); 
   fwrite(&wh->dwDSize, sizeof(BYTE), 4, fp); 
}

void CRec::ReadHeader(Waveheader *wh, FILE *fp) {
   memset(wh, 0, sizeof(Waveheader));
   fread(wh->RIFF, sizeof(BYTE), 4, fp);
   fread(&wh->dwSize, sizeof(BYTE), 4, fp); 
   fread(wh->WAVE, sizeof(BYTE), 4, fp); 
   fread(wh->fmt_, sizeof(BYTE), 4, fp); 
   fread(&wh->dw18, sizeof(BYTE), 4, fp); 
   fread(&wh->wFFormat, sizeof(BYTE), 2, fp); 
   fread(&wh->wChnls, sizeof(BYTE), 2, fp); 
   fread(&wh->dwSRate, sizeof(BYTE), 4, fp); 
   fread(&wh->BytesPerSec, sizeof(BYTE), 4, fp); 
   fread(&wh->wBlkAlign, sizeof(BYTE), 2, fp); 
   fread(&wh->BitsPerSample, sizeof(BYTE), 2, fp); 
   fread(&wh->Zero, sizeof(BYTE), 2, fp); 
   fread(wh->FACT, sizeof(BYTE), 4, fp); 
   fread(&wh->Four, sizeof(BYTE), 4, fp); 
   fread(&wh->AvgBytesSec, sizeof(BYTE), 4, fp); 
   fread(wh->DATA, sizeof(BYTE), 4, fp); 
   fread(&wh->dwDSize, sizeof(BYTE), 4, fp); 
}

BOOL CRec::FillHeader(Waveheader *wh, DWORD FileSize) {
   memset(wh, 0, sizeof(Waveheader));
   wh->RIFF[0] = 'R';
   wh->RIFF[1] = 'I';
   wh->RIFF[2] = 'F';
   wh->RIFF[3] = 'F';
   wh->dwSize = FileSize-8;		// Have to set this later
   wh->WAVE[0] = 'W';
   wh->WAVE[1] = 'A';
   wh->WAVE[2] = 'V';
   wh->WAVE[3] = 'E';
   wh->fmt_[0] = 'f';
   wh->fmt_[1] = 'm';
   wh->fmt_[2] = 't';
   wh->fmt_[3] = ' ';
   wh->dw18 = 18;
   wh->wFFormat = WAVE_FORMAT_PCM;
   wh->wChnls = wFormat.wf.nChannels;
   wh->dwSRate = wFormat.wf.nSamplesPerSec;
   wh->BytesPerSec = (wh->dwSRate * wFormat.wf.nBlockAlign);
   wh->wBlkAlign = wFormat.wf.nBlockAlign;
   wh->BitsPerSample = wFormat.wBitsPerSample;
   wh->Zero = 0;
   wh->FACT[0] = 'f';
   wh->FACT[1] = 'a';
   wh->FACT[2] = 'c';
   wh->FACT[3] = 't';
   wh->Four = 4;
   wh->dwDSize = wh->dwSize - 50;		// Have to set this later
   wh->AvgBytesSec = (wh->dwDSize / wh->BytesPerSec) * wh->dwSRate;
   wh->DATA[0] = 'd';
   wh->DATA[1] = 'a';
   wh->DATA[2] = 't';
   wh->DATA[3] = 'a';

   return TRUE;
}

void CRec::SetWaitForHandle(HANDLE hHandle) {
   if (Recording) return;

   ResetEvent(hHandle);
   hWaitFor = hHandle;
}

void CRec::SetSignalHandle(HANDLE hHandle) {
   if (Recording) return;

   ResetEvent(hHandle);
   hSendSignal = hHandle;
}

BOOL CRec::Open(int Device, char *Filename, int BufferSize) {
   RecDev = Device;
   m_iBlockSize = BufferSize;

   strcpy(fpName, Filename);

   fp = fopen(Filename, "wb");

   if (fp == NULL) {
	   MessageBox(NULL, "Error opening file for recording.", "Trak Multitracking System", MB_ICONSTOP);
	   return FALSE;
   }

   Waveheader wHdr;   
   fwrite(&wHdr, sizeof(Waveheader), 1, fp);

   WAVEFORMATEX wFormatEx;
   wFormatEx.wFormatTag = wFormat.wf.wFormatTag;
   wFormatEx.nChannels = wFormat.wf.nChannels;
   wFormatEx.nSamplesPerSec = wFormat.wf.nSamplesPerSec;
   wFormatEx.nAvgBytesPerSec = wFormat.wf.nAvgBytesPerSec;
   wFormatEx.nBlockAlign = wFormat.wf.nBlockAlign;
   wFormatEx.wBitsPerSample = wFormat.wBitsPerSample;
   wFormatEx.cbSize = 0;

   wi = new CWaveIn(RecDev, &wFormatEx, BufferSize, fp);
   if (wi->WasError) {
	   delete wi;
	   wi = NULL;
	   return FALSE;
   }
   return TRUE;
}

BOOL CRec::Record() {
   if (Recording) return FALSE;

   Recording = TRUE;

   if (hSendSignal != NULL) SetEvent(hSendSignal);
   if (hWaitFor != NULL) WaitForSingleObject(hWaitFor, INFINITE);
   if (wi == NULL) return FALSE;
   if (!wi->Record()) return FALSE; 

   fflush(fp);

   TotalBytesWritten = wi->TotalBytesRecorded;
   TotalNumSamples = wi->TotalSamplesRecorded;

   if (wi) {
		delete wi;
		wi = NULL;
   }

   SetEvent(hGetData);

   delete wi;
   wi = NULL;

   return TRUE;
}

void CRec::StopAndClose() {
   if (wi) wi->Stop();
   if (Recording) {
	  Recording = FALSE;
	  // Wait for data to get retrieved, but not more than 10 seconds
	  WaitForSingleObject(hGetData, 10000);
      Waveheader wHdr;
      fclose(fp);
      fp = fopen(fpName, "r+b");
	  int fNum = fileno(fp);
	  FillHeader(&wHdr, filelength(fNum));
	  WriteHeader(&wHdr, fp);
	  fflush(fp);
      fclose(fp);
	  fp = fopen(fpName, "rb");
	  Waveheader wHdr2;
      ReadHeader(&wHdr2, fp);
      if (memcmp(&wHdr, &wHdr2, sizeof(Waveheader)) != 0) 
	      MessageBox(NULL, "An error has occurred writing the file header.  The file header does not match the specified header.", "Trak Multitracking System", MB_ICONEXCLAMATION);
	  fclose(fp);
   }
}

BOOL CRec::SetFormat(PCMWAVEFORMAT *pFormat) {
	wFormat.wf.wFormatTag = pFormat->wf.wFormatTag;
	wFormat.wf.nChannels = pFormat->wf.nChannels;
	wFormat.wf.nSamplesPerSec = pFormat->wf.nSamplesPerSec;
	wFormat.wf.nAvgBytesPerSec = pFormat->wf.nAvgBytesPerSec;
	wFormat.wf.nBlockAlign = pFormat->wf.nBlockAlign;
    wFormat.wBitsPerSample = pFormat->wBitsPerSample;

	return TRUE;
}

