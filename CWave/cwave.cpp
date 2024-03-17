#include "CWave.H"

#include <io.h>

#ifdef _DEBUGWAV
   #include <iostream.h>
#endif

// ------------------------------------------
// Implementation for CWave Class

// Setup class (specifically root file list node)
CWave::CWave() {
   Root = new LINKED_FILELIST;
   Root->FileHandle = NULL;
   Root->Next = NULL;
   CurrFile = Root;
   BufferSize = 1024 * 10;			// Default 10k buffer
   numFiles = 0;
}

// Clear the file list
void CWave::Clear() {
   // If there aren't any files the already cleared so exit
   if (Root->Next == NULL) return;

   // Current file is the next node after root
   CurrFile = Root->Next;
   // While there are still nodes delete them
   while (CurrFile->Next != NULL) {
      LINKED_FILELIST *DNode = CurrFile;
      CurrFile = CurrFile->Next;
      fclose(DNode->FileHandle);
      delete DNode;
   }
   fclose(CurrFile->FileHandle);
   // Delete the last node
   delete CurrFile;
   // Go to the root node
   CurrFile = Root;
   Root->Next = NULL;
   numFiles = 0;
}

BOOL CWave::AddFile(const char *Filename, double AmpPct, double PanLvl) {
   // Open the file given in read binary mode
   FILE *fHandle = fopen(Filename, "rb");
   // If it doesn't exist or there's a problem return false
   if (fHandle == NULL) {
      char Msg[255];
      strcpy(Msg, "Failed to open file ");
      strcat(Msg, Filename);
      strcpy(Msg, ".  Check to see that this file exists.");
      MessageBox(NULL, Msg, "Wave Operation Failed", MB_ICONSTOP);
   	return FALSE;
   }

   // Go to the root node
   CurrFile = Root;
   // Go to the last file in the list
   while (CurrFile->Next != NULL) CurrFile = CurrFile->Next;

   // Add the file to our list
   LINKED_FILELIST *NewNode = new LINKED_FILELIST;
   NewNode->FileHandle = fHandle;
   NewNode->Next = NULL;
   if (AmpPct < 0) {
        MessageBox(NULL, "Cannot amplify WAV to a negative percentage.", "Wave Operation Failed", MB_ICONSTOP);
        delete NewNode;
        return FALSE;
   }
   if (PanLvl < -100 || PanLvl > 100) {
        MessageBox(NULL, "Valid pan levels are in the range -100 to 100.", "Wave Operation Failed", MB_ICONSTOP);
        delete NewNode;
        return FALSE;
   }
   // Panning percentage variables
   double LeftPanPercent = 100;
   double RightPanPercent = 100;

   if (PanLvl > 0) { // Right
      LeftPanPercent = 100 - PanLvl;
   }
   if (PanLvl < 0) { // Left
      RightPanPercent = 100 + PanLvl;
   }
   NewNode->LeftLevel = (AmpPct * LeftPanPercent) / 10000.;
   NewNode->RightLevel = (AmpPct * RightPanPercent) / 10000.;
   CurrFile->Next = NewNode;
   CurrFile = NewNode;
   numFiles++;
   return TRUE;
}

BOOL CWave::WriteOutHeader(FILE *outFile) {
     Whdr   o_wh;							// Out Header
     Whdr   wh;							// Current Filled Header
     DWORD  o_dwSize = 0;           // Largest File Size

     DWORD wavSRate;       // Sample Rate		(for checking)

     // If there aren't any files then quite
     if (numFiles < 1) return FALSE;

     // Move all files to the start position
     CurrFile = Root->Next;
     while (CurrFile != NULL) {
        fseek(CurrFile->FileHandle, 0L, SEEK_SET);
        CurrFile = CurrFile->Next;
     }
     
     // Set the current file as the one right after the root
     CurrFile = Root->Next;

     // Set data for the first file
     memset(&wh, 0, sizeof(Whdr));
	 ReadHeader(&wh, CurrFile->FileHandle);
     if (wh.dw18 != 18) fseek(CurrFile->FileHandle, -2, SEEK_CUR);
     wavSRate = wh.dwSRate;
     wavBits = wh.BitsPerSample;
     if (wh.wFFormat != 1) {   // PCM File
        MessageBox(NULL, "Only PCM WAV files can be mixed using this object.", "Wave Operation Failed", MB_ICONSTOP);
        return FALSE;
     }

     for (int i = 0; i < numFiles; i++) {
        if (i > 0) {   // If not first file then read header
			 ReadHeader(&wh, CurrFile->FileHandle);
           if (wh.dw18 != 18) fseek(CurrFile->FileHandle, -2, SEEK_CUR);
        }
	     if (wh.wChnls == 1) {
	        CurrFile->Stereo = FALSE;
	     } else {
	        CurrFile->Stereo = TRUE;
        }
        // Ensure they're of proper format
        if (wh.wFFormat != 1) {   // PCM File
           MessageBox(NULL, "Only PCM WAV files can be mixed using this object.", "Wave Operation Failed", MB_ICONSTOP);
           return FALSE;
        }
        if (wh.dwSRate != wavSRate) {		// Consistent sampling rate
           MessageBox(NULL, "Only WAV files of the sample sampling rate can be mixed using this object.", "Wave Operation Failed", MB_ICONSTOP);
           return FALSE;
        }
        if (wh.BitsPerSample != wavBits) {// Consistent bits/sample
           MessageBox(NULL, "Only WAV files of the sample bits per sample can be mixed using this object.", "Wave Operation Failed", MB_ICONSTOP);
           return FALSE;
        }
        // If it's a mono file convert data length to stereo
	     if (wh.wChnls == 1) {
          // Get the size of the file currently
          long dwFileSize = filelength(fileno(CurrFile->FileHandle));
          // Increase the outlength by 2
          // Length of file doubled - the doubled header - 8 for already read suff
	       wh.dwSize = (dwFileSize * 2) - 58;
	     } else {
          wh.dwSize = filelength(fileno(CurrFile->FileHandle)) - 8;
        }
		  if (wh.dwSize > o_dwSize) {
		     o_dwSize = wh.dwSize;  // Set our largest data variable
           // Set the outputted wave structure
		     memset(&o_wh, 0, sizeof(Whdr));
           // Directly Dependent Data
           o_wh.dwSize = wh.dwSize;
           o_wh.wFFormat = wh.wFFormat;
           o_wh.dwSRate = wh.dwSRate;
           o_wh.BitsPerSample = wh.BitsPerSample;
           o_wh.dwDSize = wh.dwSize - 58;
           // Constants of o_wh based on data
           strncpy((char *)o_wh.RIFF, "RIFF", 4);
           strncpy((char *)o_wh.WAVE, "WAVE", 4);
           strncpy((char *)o_wh.fmt_, "fmt ", 4);
           o_wh.dw18 = 18;
           o_wh.wChnls = 2;
           o_wh.wBlkAlign = (2 * o_wh.BitsPerSample) / 8;
           o_wh.Zero = 0;
           strncpy((char *)o_wh.FACT, "fact", 4);
           o_wh.Four = 4;
           o_wh.BytesPerSec = o_wh.dwSRate * o_wh.wBlkAlign;
           o_wh.AvgBytesSec = (o_wh.dwDSize / o_wh.BytesPerSec) * o_wh.dwSRate;
           o_wh.BytesPerSec;
           strncpy((char *)o_wh.DATA, "data", 4);
           // Set Largest File Name
           LargestFile = CurrFile->FileHandle;
           // Set Largest File Size
           LargestFileSize = o_dwSize + 8;
	        }
        CurrFile = CurrFile->Next;
     }

     if (outFile != NULL) WriteHeader(&o_wh, outFile);

     return TRUE;
}

BOOL CWave::GetHeaders() {
     Whdr   wh;							// Current Filled Header
     DWORD  o_dwSize = 0;           // Largest File Size

     DWORD wavSRate;       // Sample Rate		(for checking)

     // If there aren't any files then quite
     if (numFiles < 1) return FALSE;

     // Move all files to the start position
     CurrFile = Root->Next;
     while (CurrFile != NULL) {
        fseek(CurrFile->FileHandle, 0L, SEEK_SET);
        CurrFile = CurrFile->Next;
     }

     // Set the current file as the one right after the root
     CurrFile = Root->Next;

     // Set data for the first file
     memset(&wh, sizeof(Whdr), 0);
	 ReadHeader(&wh, CurrFile->FileHandle);
     if (wh.dw18 != 18) fseek(CurrFile->FileHandle, -2, SEEK_CUR);
     wavSRate = wh.dwSRate;
     wavBits = wh.BitsPerSample;
     if (wh.wFFormat != 1) {   // PCM File
        MessageBox(NULL, "Only PCM WAV files can be mixed using this object.", "Wave Operation Failed", MB_ICONSTOP);
        return FALSE;
     }

     for (int i = 0; i < numFiles; i++) {
        if (i > 0) {   // If not first file then read header
			 ReadHeader(&wh, CurrFile->FileHandle);
           if (wh.dw18 != 18) fseek(CurrFile->FileHandle, -2, SEEK_CUR);
        }
	     if (wh.wChnls == 1) {
	        CurrFile->Stereo = FALSE;
	     } else {
	        CurrFile->Stereo = TRUE;
        }
        // Ensure they're of proper format
        if (wh.wFFormat != 1) {   // PCM File
           MessageBox(NULL, "Only PCM WAV files can be read using this object.", "Wave Operation Failed", MB_ICONSTOP);
           return FALSE;
        }
        if (wh.dwSRate != wavSRate) {		// Consistent sampling rate
           MessageBox(NULL, "Only WAV files of the sample sampling rate can be read using this object.", "Wave Operation Failed", MB_ICONSTOP);
           return FALSE;
        }
        if (wh.BitsPerSample != wavBits) {// Consistent bits/sample
           MessageBox(NULL, "Only WAV files of the sample bits per sample can be read using this object.", "Wave Operation Failed", MB_ICONSTOP);
           return FALSE;
        }
		  if (wh.dwSize > o_dwSize) {
		     o_dwSize = wh.dwSize;  // Set our largest data variable
           // Set Largest File Name
           LargestFile = CurrFile->FileHandle;
           // Set Largest File Size
           LargestFileSize = o_dwSize + 8;
        }
        CurrFile = CurrFile->Next;
     }

     return TRUE;
}

#ifndef _DEBUGWAV
int CWave::FindNormal(int MasterLevel, int DesiredMaxLevel, CStatusBar *sb) {
#endif
#ifdef _DEBUGWAV
int CWave::FindNormal(int MasterLevel, int DesiredMaxLevel) {
#endif
   // If there aren't any files then quite
   if (numFiles < 1) {
      MessageBox(NULL, "There are no files to analyze.", "Wave Operation Failed", MB_ICONSTOP);
   	return -1;
   }

   if (GetHeaders() == FALSE) {
      return -1;
   }

   BOOL *ProbablyClips;
   ProbablyClips = new BOOL[numFiles];
   for (int i = 0; i < numFiles; i++) ProbablyClips[i] = FALSE;

   // Allocate Memory for 16 bit files
   signed short *ReadBlock16;
   signed long *LargestLocal16;
   if (wavBits == 16) {
      ReadBlock16 = new signed short[BufferSize];
      LargestLocal16 = new signed long[BufferSize];
   }
   signed long Largest16 = 0;

   // Allocate Memory for 8 bit files
   signed char *ReadBlock8;
   signed short *LargestLocal8;
   if (wavBits == 8) {
      ReadBlock8 = new signed char[BufferSize];
      LargestLocal8 = new signed short[BufferSize];
   }
   signed short Largest8 = 0;

   int CV = 0;								// Percent Complete

   if (wavBits == 16) {
      while (!feof(LargestFile)) {
         CurrFile = Root->Next;
         memset(LargestLocal16, 0, sizeof(signed long) * BufferSize);
         for (int i = 0; i < numFiles; i++) {
            memset(ReadBlock16, 0, sizeof(signed short) * BufferSize);
            if (CurrFile->Stereo) {
               fread(ReadBlock16, sizeof(signed short), BufferSize, CurrFile->FileHandle);
    				for (int k = 0; k < BufferSize; k += 2) {
                  if (!ProbablyClips[i] && (ReadBlock16[k] == 32767 || ReadBlock16[k] == -32768)) ProbablyClips[i] = TRUE;
                  LargestLocal16[k] += CurrFile->LeftLevel * ReadBlock16[k];
                  LargestLocal16[k+1] += CurrFile->RightLevel * ReadBlock16[k+1];
               }
            } else {
               fread(ReadBlock16, sizeof(signed short), BufferSize / 2, CurrFile->FileHandle);
    				for (int k = 0; k < (BufferSize / 2); k++) {
                  if (!ProbablyClips[i] && (ReadBlock16[k] == 32767 || ReadBlock16[k] == -32768)) ProbablyClips[i] = TRUE;
                  LargestLocal16[2*k] += CurrFile->LeftLevel * ReadBlock16[k];
                  LargestLocal16[(2*k)+1] += CurrFile->RightLevel * ReadBlock16[k];
               }
            }
            CurrFile = CurrFile->Next;
         }
         for (int k = 0; k < BufferSize; k++) {
            if (LargestLocal16[k] < 0) LargestLocal16[k] *= -1;
            if (LargestLocal16[k] > Largest16) {
            	Largest16 = LargestLocal16[k];
            }
         }
         // Update status bar if one was given
         if ((ftell(LargestFile) / 10) / (LargestFileSize / 1000) > CV) {
            CV = (ftell(LargestFile) / 10) / (LargestFileSize / 1000);
            #ifndef _DEBUGWAV
            if (sb != NULL && CV % 2 == 0) {
               char BarText[40];
   	         char CVText[5];
 	            strcpy(BarText, "Analyzing tracks . . .   ");
 	            itoa(CV, CVText, 10);
 	            strcat(BarText, CVText);
	            strcat(BarText, "% completed");
               sb->SetWindowText(BarText);
               sb->UpdateWindow();
            }
            #endif
         }
      }
   }

   if (wavBits == 8) {
      while (!feof(LargestFile)) {
         CurrFile = Root->Next;
         memset(LargestLocal8, 0, sizeof(signed short) * BufferSize);
         for (int i = 0; i < numFiles; i++) {
            memset(ReadBlock8, 0, sizeof(signed char) * BufferSize);
            if (CurrFile->Stereo) {
               fread(ReadBlock8, sizeof(signed char), BufferSize, CurrFile->FileHandle);
    				for (int k = 0; k < BufferSize; k += 2) {
                  if (!ProbablyClips[i] && (ReadBlock8[k] == 127 || ReadBlock8[k] == -128)) ProbablyClips[i] = TRUE;
                  LargestLocal8[k] += CurrFile->LeftLevel * ReadBlock8[k];
                  LargestLocal8[k+1] += CurrFile->RightLevel * ReadBlock8[k+1];
               }
            } else {
               fread(ReadBlock8, sizeof(signed char), BufferSize / 2, CurrFile->FileHandle);
    				for (int k = 0; k < (BufferSize / 2); k++) {
                  if (!ProbablyClips[i] && (ReadBlock8[k] == 127 || ReadBlock16[k] == -128)) ProbablyClips[i] = TRUE;
                  LargestLocal8[2*k] += CurrFile->LeftLevel * ReadBlock8[k];
                  LargestLocal8[(2*k)+1] += CurrFile->RightLevel * ReadBlock8[k];
               }
            }
            CurrFile = CurrFile->Next;
         }
         for (int k = 0; k < BufferSize; k++) {
            if (LargestLocal8[k] < 0) LargestLocal8[k] *= -1;
            if (LargestLocal8[k] > Largest8) {
            	Largest8 = LargestLocal8[k];
            }
         }
         // Update status bar if one was given
         if ((ftell(LargestFile) / 10) / (LargestFileSize / 1000) > CV) {
            CV = (ftell(LargestFile) / 10) / (LargestFileSize / 1000);
            #ifndef _DEBUGWAV
            if (sb != NULL && CV % 2 == 0) {
               char BarText[40];
   	         char CVText[5];
 	            strcpy(BarText, "Analyzing tracks . . .   ");
 	            itoa(CV, CVText, 10);
 	            strcat(BarText, CVText);
	            strcat(BarText, "% completed");
               sb->SetWindowText(BarText);
               sb->UpdateWindow();
            }
            #endif
         }
      }
   }

   #ifdef _DEBUGWAV
   for (i = 0; i < numFiles; i++) {
      if (ProbablyClips[i]) cout << "Track " << (i + 1) << " probably clips" << endl;
   }
   #endif

   delete ProbablyClips;
   int RetVal = -1;
   // Cleanup memory
   if (wavBits == 16) {            				// 16 bit
      delete ReadBlock16;
      delete LargestLocal16;
      int LevelOfCurrent = (floor((32767. * DesiredMaxLevel) / Largest16));
      RetVal = floor((MasterLevel * LevelOfCurrent) / 100.);
   }
   if (wavBits == 8) {								//  8 bit
      delete ReadBlock8;
      delete LargestLocal8;
      int LevelOfCurrent = (floor((127. * DesiredMaxLevel) / Largest8));
      RetVal = floor((MasterLevel * LevelOfCurrent) / 100.);
   }

   return RetVal;
}

#ifndef _DEBUGWAV
void CWave::MixTo(const char *Filename, CStatusBar *sb) {
#endif
#ifdef _DEBUGWAV
void CWave::MixTo(const char *Filename) {
#endif
   // If there aren't any files then quite
   if (numFiles < 1) {
      MessageBox(NULL, "There are no files to mix.", "Wave Operation Failed", MB_ICONSTOP);
   	return;
   }

   FILE *out = fopen(Filename, "wb");
   if (out == NULL) {
      MessageBox(NULL, "Error opening output file for writing.", "Wave Operation Failed", MB_ICONSTOP);
   	return;
   }
   if (WriteOutHeader(out) == FALSE) {
      fclose(out);
      return;
   }

   // Make BufferSize an even number if it isn't
   if (BufferSize % 2 != 0) BufferSize++;
   #ifndef _DEBUGWAV
   if (sb != NULL) {
     sb->SetWindowText("Writing WAVE . . .   0% completed");
     sb->UpdateWindow();
	}
   #endif
   BOOL *ProbablyClips;
   ProbablyClips = new BOOL[numFiles];

   // Allocate Memory for 16 bit files
   signed short *ReadBlock16;
   signed long *LargestLocal16;
   if (wavBits == 16) {
      ReadBlock16 = new signed short[BufferSize];
      LargestLocal16 = new signed long[BufferSize];
   }

   // Allocate Memory for 8 bit files
   signed char *ReadBlock8;
   signed short *LargestLocal8;
   if (wavBits == 8) {
      ReadBlock8 = new signed char[BufferSize];
      LargestLocal8 = new signed short[BufferSize];
   }

   // Number of bytes written
   long Written = 58;						// (Already written Header)
   int CV = 0;								// Percent Complete

   #ifdef _DEBUGWAV
   cout << "File Size Should Be " << LargestFileSize << " bytes" << endl;
   #endif

   if (wavBits == 16) {
      while (!feof(LargestFile)) {
         CurrFile = Root->Next;
         memset(LargestLocal16, 0, sizeof(signed long) * BufferSize);
         for (int i = 0; i < numFiles; i++) {
            memset(ReadBlock16, 0, sizeof(signed short) * BufferSize);
            if (CurrFile->Stereo) {
               fread(ReadBlock16, sizeof(signed short), BufferSize, CurrFile->FileHandle);
    				for (int k = 0; k < BufferSize; k += 2) {
                  if (!ProbablyClips[i] && (ReadBlock16[k] == 32767 || ReadBlock16[k] == -32768)) ProbablyClips[i] = TRUE;
                  LargestLocal16[k] += CurrFile->LeftLevel * ReadBlock16[k];
                  LargestLocal16[k+1] += CurrFile->RightLevel * ReadBlock16[k+1];
               }
            } else {
               fread(ReadBlock16, sizeof(signed short), BufferSize / 2, CurrFile->FileHandle) * 2;
    				for (int k = 0; k < (BufferSize / 2); k++) {
                  if (!ProbablyClips[i] && (ReadBlock16[k] == 32767 || ReadBlock16[k] == -32768)) ProbablyClips[i] = TRUE;
                  LargestLocal16[2*k] += CurrFile->LeftLevel * ReadBlock16[k];
                  LargestLocal16[(2*k)+1] += CurrFile->RightLevel * ReadBlock16[k];
               }
            }
            CurrFile = CurrFile->Next;
         }
         for (int k = 0; k < BufferSize; k++) {
           (LargestLocal16[k] > 32767) ? ReadBlock16[k] = 32767 :
           		(LargestLocal16[k] < -32768) ? ReadBlock16[k] = -32768 : ReadBlock16[k] = LargestLocal16[k];
         }
         long outB;
         ((Written + BufferSize * sizeof(signed short)) > LargestFileSize) ? outB = (LargestFileSize - Written) / sizeof(signed short) : outB = BufferSize;
		   Written += fwrite(ReadBlock16, sizeof(signed short), outB, out) * sizeof(signed short);
         // Update status bar if one was given
         if ((Written / 10) / (LargestFileSize / 1000) > CV) {
            CV = (Written /10) / (LargestFileSize / 1000);
            #ifndef _DEBUGWAV
            if (sb != NULL && CV % 2 == 0) {
               char BarText[40];
   	         char CVText[5];
 	            strcpy(BarText, "Writing WAVE . . .   ");
 	            itoa(CV, CVText, 10);
 	            strcat(BarText, CVText);
	            strcat(BarText, "% completed");
               sb->SetWindowText(BarText);
               sb->UpdateWindow();
            }
            #endif
         }
      }
   }

   if (wavBits == 8) {
      while (!feof(LargestFile)) {
         CurrFile = Root->Next;
         memset(LargestLocal8, 0, sizeof(signed short) * BufferSize);
         for (int i = 0; i < numFiles; i++) {
            memset(ReadBlock8, 0, sizeof(signed char) * BufferSize);
            if (CurrFile->Stereo) {
               fread(ReadBlock8, sizeof(signed char), BufferSize, CurrFile->FileHandle);
    				for (int k = 0; k < BufferSize; k += 2) {
                  if (!ProbablyClips[i] && (ReadBlock8[k] == 127 || ReadBlock8[k] == -128)) ProbablyClips[i] = TRUE;
                  LargestLocal8[k] += CurrFile->LeftLevel * ReadBlock8[k];
                  LargestLocal8[k+1] += CurrFile->RightLevel * ReadBlock8[k+1];
               }
            } else {
               fread(ReadBlock8, sizeof(signed char), BufferSize / 2, CurrFile->FileHandle) * 2;
    				for (int k = 0; k < (BufferSize / 2); k++) {
                  if (!ProbablyClips[i] && (ReadBlock8[k] == 127 || ReadBlock16[k] == -128)) ProbablyClips[i] = TRUE;
                  LargestLocal8[2*k] += CurrFile->LeftLevel * ReadBlock8[k];
                  LargestLocal8[(2*k)+1] += CurrFile->RightLevel * ReadBlock8[k];
               }
            }
            CurrFile = CurrFile->Next;
         }
         for (int k = 0; k < BufferSize; k++) {
           (LargestLocal8[k] > 127) ? ReadBlock8[k] = 127 :
           		(LargestLocal8[k] < -128) ? ReadBlock8[k] = -128 : ReadBlock8[k] = LargestLocal8[k];
         }
         long outB;
         ((Written + BufferSize * sizeof(signed char)) > LargestFileSize) ? outB = (LargestFileSize - Written) / sizeof(signed char) : outB = BufferSize;
		   Written += fwrite(ReadBlock8, sizeof(signed char), outB, out) * sizeof(signed char);
         // Update status bar if one was given
         if ((Written / 10) / (LargestFileSize / 1000) > CV) {
            CV = (Written /10) / (LargestFileSize / 1000);
            #ifndef _DEBUGWAV
            if (sb != NULL && CV % 2 == 0) {
               char BarText[40];
   	         char CVText[5];
 	            strcpy(BarText, "Writing WAVE . . .   ");
 	            itoa(CV, CVText, 10);
 	            strcat(BarText, CVText);
	            strcat(BarText, "% completed");
               sb->SetWindowText(BarText);
               sb->UpdateWindow();
            }
            #endif
         }
      }
   }

   fclose(out);

   // Cleanup memory
   if (wavBits == 16) {            				// 16 bit
      delete ReadBlock16;
      delete LargestLocal16;
   }
   if (wavBits == 8) {								//  8 bit
      delete ReadBlock8;
      delete LargestLocal8;
   }

   #ifdef _DEBUGWAV
   if (feof(LargestFile)) cout << "Bytes Written: " << Written << endl;
   cout << "Bytes Remaining: " << (LargestFileSize - Written) << endl;
   #endif
}

// Uses an instance it's own class to amplify a track
#ifndef _DEBUGWAV
void CWave::AmplifyWave(const char *InputFile, const char *OutputFile, double Percentage, CStatusBar *sb) {
#endif
#ifdef _DEBUGWAV
void CWave::AmplifyWave(const char *InputFile, const char *OutputFile, double Percentage) {
#endif
   CWave  AmpWave;

   // Add a the input file, desired level, and no change in panning
   AmpWave.AddFile(InputFile, Percentage, 0);

   AmpWave.BufferSize = BufferSize;
   // Now mix it to it's own type
   #ifdef _DEBUGWAV
   AmpWave.MixTo(OutputFile);
   #endif
   #ifndef _DEBUGWAV
   AmpWave.MixTo(OutputFile, sb);
   #endif
}

void CWave::WriteHeader(Whdr *wh, FILE *fp) {
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

void CWave::ReadHeader(Whdr *wh, FILE *fp) {
   memset(wh, 0, sizeof(Whdr));
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

CWave::~CWave() {
   Clear();
   delete Root;
}

