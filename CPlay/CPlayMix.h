#ifndef CPLAYMIX_H
#define CPLAYMIX_H

#include "CPlayWav.h"
#include <stdio.h>

struct Whdr {                                                                          // Bytes Sec  Total
		BYTE        RIFF[4];          // "RIFF"                          | RIFF    4      4     4
		DWORD       dwSize;           // Filesize - 8                    |  chk    4      8     8
		BYTE        WAVE[4];          // "WAVE"                                    4      4     12
		BYTE        fmt_[4];          // "fmt "                          | fmt     4      4     16
		DWORD       dw18;             // fmt chk data len                |   chk   4      8     20
		WORD        wFFormat;         // 1 for a PCM File format         |         2      2     22
		WORD        wChnls;           // Number of Channels              |         2      4     24
		DWORD       dwSRate;          // Samples/Sec                     |         4      8     28
		DWORD       BytesPerSec;      // Bytes/Sec (SRate * Block Align) |         4      12    32
		WORD        wBlkAlign;        // Block Align                     |         2      14    34
		WORD        BitsPerSample;    // Sample size (sig bits/sample)   |         2      16    36
	   WORD 			Zero;					// 0    (bytes of extra data)      |         2      18    38
	   BYTE			FACT[4];				// "fact"                          | fact    4      4     42
	   DWORD			Four;					// 4                               |   chk   4      8     46
	   DWORD			AvgBytesSec;		// Average Bytes per Second        |         4      4     50
		BYTE        DATA[4];          // "data"                          | data    4      4     54
		DWORD       dwDSize;          // Datasize to follow              |  chk    4      8     58
};

class CPlayMix {
private:
	struct fPtr {
	  	FILE 	*fHandle;
		BOOL 	 Stereo;
      double LeftLevel;
      double RightLevel;
	};

	WAVEFORMATEX wfx;

	void ReadHeader(Whdr *wh, FILE *fp);

	FILE *LargestFile;
	fPtr *fQueue;
	int QueueFiles;

   int BufferLength;
   int numBuffers;
   
	int wavBits;

   HANDLE hWaitFor;
   HANDLE hSendSignal;

   BOOL GetHeaders();

   BOOL ShouldStop;
   BOOL Playing;

   CWaveOut *wo;

   signed short *ReadBlock16;
   signed long  *tempMixBlock16;
   signed short *MixBlock16;

   signed char  *ReadBlock8;
   signed short *tempMixBlock8;
   signed char  *MixBlock8;

   BOOL Play16(int Device);
   BOOL Play8(int Device);

public:
   CPlayMix();
   void InitQueue(int numFiles);
   void ClearQueue();
   void QueueFile(int FileNum, const char *FileName, double LeftLevel, double RightLevel);
   void SetFileLevels(int FileNum, double LeftLevel, double RightLevel);
   void SetWaitForHandle(HANDLE hHandle);
   void SetSignalHandle(HANDLE hHandle);
   BOOL Play(int Device);
   void Stop();
   void BufferSize(int BuffSize);
   void NumberBuffers(int numBuffs);
   ~CPlayMix();
};

#endif
