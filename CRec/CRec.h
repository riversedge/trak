#ifndef CREC_H
#define CREC_H

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include "CRecWav.h"

struct Waveheader
{                                                                          // Bytes Sec  Total
	BYTE        RIFF[4];          // "RIFF"                          | RIFF    4      4     4
	DWORD       dwSize;           // Filesize - 8                    |  chk    4      8     8
	BYTE        WAVE[4];          // "WAVE"                            WAVE    4      4     12
	BYTE        fmt_[4];          // "fmt "                          | fmt     4      4     16
	DWORD       dw18;             // fmt chk data len                |   chk   4      8     20
	WORD        wFFormat;         // 1 for a PCM File format         |         2      2     22
	WORD        wChnls;           // Number of Channels              |         2      4     24
	DWORD       dwSRate;          // Samples/Sec                     |         4      8     28
	DWORD       BytesPerSec;      // Bytes/Sec (SRate * Block Align) |         4      12    32
	WORD        wBlkAlign;        // Block Align                     |         2      14    34
	WORD        BitsPerSample;    // Sample size (sig bits/sample)   |         2      16    36
    WORD 		Zero;			  // 0    (bytes of extra data)      |         2      18    38
    BYTE		FACT[4];		  // "fact"                          | fact    4      4     42
    DWORD		Four;			  // 4                               |   chk   4      8     46
    DWORD		AvgBytesSec;	  // Average Bytes per Second        |         4      4     50
	BYTE        DATA[4];          // "data"                          | data    4      4     54
	DWORD       dwDSize;          // Datasize to follow              |  chk    4      8     58
};

class CRec {
private:
	BOOL FillHeader(Waveheader *wh, DWORD FileSize);
    PCMWAVEFORMAT wFormat;

    void WriteHeader(Waveheader *wh, FILE *fp);
	void ReadHeader(Waveheader *wh, FILE *fp);

    int m_iBlockSize;
	char  fpName[500];
    FILE *fp;
    int RecDev;

	DWORD TotalBytesWritten;
    DWORD TotalNumSamples;

 	CWaveIn	*wi;

    HANDLE hWaitFor;
    HANDLE hSendSignal;
	HANDLE hGetData;
    
    BOOL Recording;

public:
   CRec();
   ~CRec();
   void SetWaitForHandle(HANDLE hHandle);
   void SetSignalHandle(HANDLE hHandle);
   BOOL Open(int Device, char *Filename, int BufferSize);
   BOOL Record();
   void StopAndClose();
   BOOL SetFormat(PCMWAVEFORMAT *pFormat);
};

#endif