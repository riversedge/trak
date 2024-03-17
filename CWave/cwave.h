#ifndef CWave_H
#define CWave_H

#include <memory.h>
#include <stdio.h>
#include <math.h>
#ifndef _DEBUGWAV
   #include "..\stdafx.h"
#endif
#include <windows.h>

class CWave {
private:
   // Linked List File Structure
   struct LINKED_FILELIST {
      FILE 				  *FileHandle;
      BOOL				   Stereo;
      double			   LeftLevel;
      double 				RightLevel;
      LINKED_FILELIST   *Next;
   };

	struct Whdr
	{                                                                          // Bytes Sec  Total
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

   // Root Node of Linked File List
   LINKED_FILELIST *Root;
   // Current Node of Linked File List
   LINKED_FILELIST *CurrFile;

   // Number of files in the linked list
   long numFiles;
   FILE *LargestFile;				// Largest File Handle
   long LargestFileSize;			// Largest File Size
   BOOL GetHeaders();
   BOOL WriteOutHeader(FILE *outFile);   
   WORD wavBits; 
   void ReadHeader(Whdr *wh, FILE *fp);
   void WriteHeader(Whdr *wh, FILE *fp);

public:
   CWave();
   ~CWave();

   long BufferSize;

   void Clear();
   BOOL AddFile(const char *Filename, double AmpPct, double PanLvl);
   #ifdef _DEBUG
   void MixTo(const char *Filename);
   void AmplifyWave(const char *InputFile, const char *OutputFile, double Percentage);
   int  FindNormal(int MasterLevel, int DesiredMaxLevel);
   #endif
   #ifndef _DEBUGWAV
   void MixTo(const char *Filename, CStatusBar *sb);
   void AmplifyWave(const char *InputFile, const char *OutputFile, double Percentage, CStatusBar *sb);
   int  FindNormal(int MasterLevel, int DesiredMaxLevel, CStatusBar *sb);
   #endif
};

#endif