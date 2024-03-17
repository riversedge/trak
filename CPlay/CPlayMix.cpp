#include "CPlayMix.h"
#include <windows.h>

CPlayMix::CPlayMix() {
	LargestFile = NULL;
   QueueFiles = 0;
   fQueue = NULL;
   hWaitFor = NULL;
	hSendSignal = NULL;
   wo = NULL;
   BufferLength = 44 * 1024;
   numBuffers = 2;
   ShouldStop = FALSE;
   Playing = FALSE;
   // Set buffers to null
   ReadBlock16 = NULL;
   tempMixBlock16 = NULL;
   MixBlock16 = NULL;
   ReadBlock8 = NULL;
   tempMixBlock8 = NULL;
   MixBlock8 = NULL;
}

void CPlayMix::ReadHeader(Whdr *wh, FILE *fp) {
   memset(wh, 0, sizeof(Whdr));
   if (fp == NULL) return;
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

BOOL CPlayMix::GetHeaders() {
	  if (!Playing) return FALSE;
	  int i;

     Whdr   wh;
     DWORD  o_dwSize = 0;           // Largest File Size

     DWORD wavSRate;       // Sample Rate		(for checking)

	 LargestFile = NULL;

     // If there aren't any files then quit
     if (QueueFiles < 1) return FALSE;
	 if (QueueFiles < 2 && fQueue[0].fHandle == NULL) return FALSE;

     // Move all files to the start position
     for (i = 0; i < QueueFiles; i++) {
        if (fQueue[i].fHandle != NULL) fseek(fQueue[i].fHandle, 0L, SEEK_SET);
     }

	 int FirstFile = 0;
	 if (fQueue[FirstFile].fHandle == NULL) FirstFile++;
     // Set data for the first file
     memset(&wh, sizeof(Whdr), 0);
	 ReadHeader(&wh, fQueue[FirstFile].fHandle);
     if (wh.dw18 != 18) fseek(fQueue[FirstFile].fHandle, -2, SEEK_CUR);
     wavSRate = wh.dwSRate;
     wavBits = wh.BitsPerSample;
     if (wh.wFFormat != 1) {   // PCM File
        MessageBox(NULL, "Only PCM WAV files can be mixed using this object.", "Wave Operation Failed", MB_ICONSTOP);
        return FALSE;
     }

     for (i = 0; i < QueueFiles; i++) {
		 if (fQueue[i].fHandle != NULL) {
			if (i > FirstFile) {   // If not first file then read header
				ReadHeader(&wh, fQueue[i].fHandle);
				if (wh.dw18 != 18) fseek(fQueue[i].fHandle, -2, SEEK_CUR);
			}
			if (wh.wChnls == 1) {
		        fQueue[i].Stereo = FALSE;
			} else {
		        fQueue[i].Stereo = TRUE;
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
				LargestFile = fQueue[i].fHandle;
			}
		}
	 }

     wfx.wFormatTag = wh.wFFormat;
     wfx.nSamplesPerSec = wh.dwSRate;
     wfx.wBitsPerSample = wh.BitsPerSample;
     if (wh.wChnls == 2) {
	     wfx.nChannels = wh.wChnls;
	     wfx.nAvgBytesPerSec = wh.BytesPerSec;
	     wfx.nBlockAlign = wh.wBlkAlign;
     } else {
	     wfx.nChannels = wh.wChnls * 2;
	     wfx.nAvgBytesPerSec = wh.BytesPerSec * 2;
	     wfx.nBlockAlign = wh.wBlkAlign * 2;
     }

     wfx.cbSize = 0;

     return TRUE;
}

void CPlayMix::BufferSize(int BuffSize) {
	if (Playing) return;

	BufferLength = BuffSize;
}

void CPlayMix::NumberBuffers(int NumBuffs) {
	if (Playing) return;

	numBuffers = NumBuffs;
}

void CPlayMix::InitQueue(int numFiles) {
	if (Playing) return;

   if (fQueue) delete fQueue;
   QueueFiles = numFiles;
   fQueue = new fPtr[QueueFiles];
   ShouldStop = FALSE;
}

void CPlayMix::QueueFile(int FileNum, const char *FileName, double LeftLevel,
									 double RightLevel) {
	if (Playing) return;

   if (FileNum > QueueFiles) {
   	MessageBox(NULL, "Invalid file number.  File number exceeds prepared queue.",
      						"Play Mixer Error", MB_ICONEXCLAMATION);
      return;
   }

   FileNum--;
   if (FileName == NULL) {
	   fQueue[FileNum].fHandle = NULL;
	   return;
   }
   fQueue[FileNum].fHandle = fopen(FileName, "rb");
   if (fQueue[FileNum].fHandle == NULL) {
    	char Msg[255];
      strcpy(Msg, "Error opening file \"");
      strcat(Msg, FileName);
      strcat(Msg, "\".");
   	MessageBox(NULL, Msg, "Play Mixer Error", MB_ICONEXCLAMATION);
      return;
   }
   fQueue[FileNum].Stereo = FALSE;
   fQueue[FileNum].LeftLevel = LeftLevel;
   fQueue[FileNum].RightLevel = RightLevel;
}

void CPlayMix::ClearQueue() {
	if (Playing) return;

	for (int Ctr = 0; Ctr < QueueFiles; Ctr++) {
   	fclose(fQueue[Ctr].fHandle);
   }
   delete fQueue;
   fQueue = NULL;
   ShouldStop = FALSE;
}

void CPlayMix::SetFileLevels(int FileNum, double LeftLevel, double RightLevel) {
	FileNum--;
	fQueue[FileNum].LeftLevel = LeftLevel;
   fQueue[FileNum].RightLevel = RightLevel;
}

void CPlayMix::SetWaitForHandle(HANDLE hHandle) {
	if (Playing) return;

    ResetEvent(hHandle);
	hWaitFor = hHandle;
}

void CPlayMix::SetSignalHandle(HANDLE hHandle) {
	if (Playing) return;

    ResetEvent(hHandle);
	hSendSignal = hHandle;
}

BOOL CPlayMix::Play16(int Device) {
   if (!Playing) {
      if (hSendSignal != NULL) SetEvent(hSendSignal);
	  return FALSE;
   }

   wo = new CWaveOut(Device, &wfx, numBuffers, BufferLength);
   if (wo->WasError) {
	   delete wo;
	   wo = NULL;
	   if (hSendSignal != NULL) SetEvent(hSendSignal);
	   return FALSE;
   }

   if (wo) wo->Pause();

   long BufferSize = BufferLength / sizeof(signed short) * sizeof(BYTE);
   long BlocksRead;
   unsigned long TotBytesRead;

   // Allocate Memory for 16 bit files
   ReadBlock16 = new signed short[BufferSize];
   // These two are stereo
   tempMixBlock16 = new signed long[BufferSize];
   long  tempBuffSize = BufferSize * sizeof(signed long);
   MixBlock16 = new signed short[BufferSize];

   // T(n) = (QueueFiles + 2) * n
   //    for the mixing algorithm, n being the buffer length (n = BufferSize)
   //		QueueFiles * n for the mixing of the files
   //    an additional 2 n for the testing of the clipping
   //    Additional work is done in converting to stereo
   for (int BuffCt = 0; BuffCt < numBuffers; BuffCt++) {
	  ZeroMemory(tempMixBlock16, tempBuffSize);
      TotBytesRead = 0;
      // Mix together the files in the queue
   	 for (int QueueCt = 0; QueueCt < QueueFiles; QueueCt++) {
		if (fQueue[QueueCt].fHandle != NULL) {
	   	    // Mix data to fill initial buffers
            if (fQueue[QueueCt].Stereo) {
				BlocksRead = fread(ReadBlock16, sizeof(signed short), BufferSize,
   	      								fQueue[QueueCt].fHandle);
				unsigned long BytesRead = BlocksRead * sizeof(signed short);
					TotBytesRead = max(TotBytesRead, BytesRead);
				for (int MixCt = 0; MixCt < BlocksRead; MixCt += 2) {
						tempMixBlock16[MixCt] += ReadBlock16[MixCt] *
               										fQueue[QueueCt].LeftLevel;
						tempMixBlock16[MixCt+1] += ReadBlock16[MixCt+1] *
               										fQueue[QueueCt].RightLevel;
				}
			} else {
				BlocksRead = fread(ReadBlock16, sizeof(signed short), BufferSize / 2,
   	      								fQueue[QueueCt].fHandle);
				int BytesRead = BlocksRead * sizeof(signed short) * 2;
				//TotBlocksRead = max(TotBlocksRead, BlocksRead);
					TotBytesRead = max(TotBytesRead, BytesRead);
				for (int MixCt = 0; MixCt < BlocksRead; MixCt++) {
						tempMixBlock16[MixCt*2] += ReadBlock16[MixCt] *
               												fQueue[QueueCt].LeftLevel;
						tempMixBlock16[(MixCt*2)+1] += ReadBlock16[MixCt] *
               												fQueue[QueueCt].RightLevel;
				}
			}
		}
	 }
	 // Check for high and low clipping
	 for (int MixCt = 0; MixCt < BufferSize; MixCt++) {
	  	 (tempMixBlock16[MixCt] > 32767) ? MixBlock16[MixCt] = 32767 :
      	 (tempMixBlock16[MixCt] < -32768) ? MixBlock16[MixCt] = -32768 :
	       	MixBlock16[MixCt] = tempMixBlock16[MixCt];
	 }
	 if (wo) wo->Write((PBYTE)MixBlock16, TotBytesRead);
   }

   if (hSendSignal != NULL) SetEvent(hSendSignal);
   if (hWaitFor != NULL) WaitForSingleObject(hWaitFor, INFINITE);
   if (wo) wo->Restart();

   while (!feof(LargestFile) && !ShouldStop) {
   	 // Keep writing data to output device
	 ZeroMemory(tempMixBlock16, tempBuffSize);
     TotBytesRead = 0;
     // Mix together the files in the queue
   	 for (int QueueCt = 0; QueueCt < QueueFiles; QueueCt++) {
		if (fQueue[QueueCt].fHandle != NULL) {
	   		// Mix data to fill initial buffers
			if (fQueue[QueueCt].Stereo) {
				BlocksRead = fread(ReadBlock16, sizeof(signed short), BufferSize,
   	      								fQueue[QueueCt].fHandle);
				unsigned long BytesRead = BlocksRead * sizeof(signed short);
					TotBytesRead = max(TotBytesRead, BytesRead);
				for (int MixCt = 0; MixCt < BlocksRead; MixCt += 2) {
						tempMixBlock16[MixCt] += ReadBlock16[MixCt] *
               										fQueue[QueueCt].LeftLevel;
						tempMixBlock16[MixCt+1] += ReadBlock16[MixCt+1] *
               										fQueue[QueueCt].RightLevel;
				}
			} else {
				BlocksRead = fread(ReadBlock16, sizeof(signed short), BufferSize / 2,
   	      								fQueue[QueueCt].fHandle);
				int BytesRead = BlocksRead * sizeof(signed short) * 2;
					TotBytesRead = max(TotBytesRead, BytesRead);
				for (int MixCt = 0; MixCt < BlocksRead; MixCt++) {
						tempMixBlock16[MixCt*2] += ReadBlock16[MixCt] *
               										fQueue[QueueCt].LeftLevel;
						tempMixBlock16[(MixCt*2)+1] += ReadBlock16[MixCt] *
               										fQueue[QueueCt].RightLevel;
			    }
			}
		}
	 }
	 // Check for high and low clipping
	 for (int MixCt = 0; MixCt < BufferSize; MixCt++) {
	 		(tempMixBlock16[MixCt] > 32767) ? MixBlock16[MixCt] = 32767 :
    		(tempMixBlock16[MixCt] < -32768) ? MixBlock16[MixCt] = -32768 :
	       		MixBlock16[MixCt] = tempMixBlock16[MixCt];
	 }
	 if (wo) wo->Write((PBYTE)MixBlock16, TotBytesRead);
   }

   if (wo) {
	   delete wo;
	   wo = NULL;
   }

   delete ReadBlock16;
   delete tempMixBlock16;
   delete MixBlock16;
   ReadBlock16 = NULL;
   tempMixBlock16 = NULL;
   MixBlock16 = NULL;

   Playing = FALSE;

   if (hSendSignal != NULL) SetEvent(hSendSignal);
   return TRUE;
}

BOOL CPlayMix::Play8(int Device) {
   if (!Playing) {
      if (hSendSignal != NULL) SetEvent(hSendSignal);
	  return FALSE;
   }

   wo = new CWaveOut(Device, &wfx, numBuffers, BufferLength);
   if (wo->WasError) {
	   delete wo;
	   wo = NULL;
       if (hSendSignal != NULL) SetEvent(hSendSignal);
	   return FALSE;
   }

   if (wo) wo->Pause();

   long BufferSize = BufferLength / sizeof(signed char) * sizeof(BYTE);
   long BlocksRead;
   long TotBytesRead;

   // Allocate Memory for 8 bit files
   ReadBlock8 = new signed char[BufferSize];
   // These two are stereo
   tempMixBlock8 = new signed short[BufferSize];
   long  tempBuffSize = BufferSize * sizeof(signed short);
   MixBlock8 = new signed char[BufferSize];

   // T(n) = (QueueFiles + 2) * n
   //    for the mixing algorithm, n being the buffer length (n = BufferSize)
   //		QueueFiles * n for the mixing of the files
   //    an additional 2 n for the testing of the clipping
   //    Additional work is done in converting to stereo
   for (int BuffCt = 0; BuffCt < numBuffers; BuffCt++) {
	 ZeroMemory(tempMixBlock8, tempBuffSize);
     TotBytesRead = 0;
     // Mix together the files in the queue
   	 for (int QueueCt = 0; QueueCt < QueueFiles; QueueCt++) {
		if (fQueue[QueueCt].fHandle != NULL) {
		   	// Mix data to fill initial buffers
			if (fQueue[QueueCt].Stereo) {
				BlocksRead = fread(ReadBlock8, sizeof(signed char), BufferSize,
   	      								fQueue[QueueCt].fHandle);
				int BytesRead = BlocksRead * sizeof(signed char);
					TotBytesRead = max(TotBytesRead, BytesRead);
				for (int MixCt = 0; MixCt < BlocksRead; MixCt += 2) {
						tempMixBlock8[MixCt] += ReadBlock8[MixCt] *
               										fQueue[QueueCt].LeftLevel;
						tempMixBlock8[MixCt+1] += ReadBlock8[MixCt+1] *
               										fQueue[QueueCt].RightLevel;
				}
			} else {
				BlocksRead = fread(ReadBlock8, sizeof(signed char), BufferSize / 2,
   	      								fQueue[QueueCt].fHandle);
				int BytesRead = BlocksRead * sizeof(signed char) * 2;
					TotBytesRead = max(TotBytesRead, BytesRead);
				for (int MixCt = 0; MixCt < BlocksRead; MixCt++) {
						tempMixBlock8[(MixCt * 2)] += ReadBlock8[MixCt] *
               												fQueue[QueueCt].LeftLevel;
						tempMixBlock8[(MixCt+1) * 2] += ReadBlock8[MixCt] *
               												fQueue[QueueCt].RightLevel;
				}	
			}
		}
	 }


	 // Check for high and low clipping
	 for (int MixCt = 0; MixCt < BufferSize; MixCt++) {
			(tempMixBlock8[MixCt] > 127) ? MixBlock8[MixCt] = 127 :
    		(tempMixBlock8[MixCt] < -128) ? MixBlock8[MixCt] = -128 :
	       		MixBlock8[MixCt] = tempMixBlock8[MixCt];
	 }
	 if (wo) wo->Write((PBYTE)MixBlock8, BufferLength);
   }

   if (hSendSignal != NULL) SetEvent(hSendSignal);
   if (hWaitFor != NULL) WaitForSingleObject(hWaitFor, INFINITE);
   if (wo) wo->Restart();

   while (!feof(LargestFile) && !ShouldStop) {
   	 // Keep writing data to output device
	 ZeroMemory(tempMixBlock8, tempBuffSize);
     TotBytesRead = 0;
     // Mix together the files in the queue
   	 for (int QueueCt = 0; QueueCt < QueueFiles; QueueCt++) {
		if (fQueue[QueueCt].fHandle != NULL) {
		   	// Mix data to fill initial buffers
		     if (fQueue[QueueCt].Stereo) {
			      BlocksRead = fread(ReadBlock8, sizeof(signed char), BufferSize,
   	      							fQueue[QueueCt].fHandle);
				  int BytesRead = BlocksRead * sizeof(signed char);
				  TotBytesRead = max(TotBytesRead, BytesRead);
				  for (int MixCt = 0; MixCt < BlocksRead; MixCt += 2) {
						tempMixBlock8[MixCt] += ReadBlock8[MixCt] *
               									fQueue[QueueCt].LeftLevel;
						tempMixBlock8[MixCt+1] += ReadBlock8[MixCt+1] *
               									fQueue[QueueCt].RightLevel;
				  }
			 } else {
				 BlocksRead = fread(ReadBlock8, sizeof(signed char), BufferSize / 2,
   	      							fQueue[QueueCt].fHandle);
				 int BytesRead = BlocksRead * sizeof(signed char) * 2;
				 TotBytesRead = max(TotBytesRead, BytesRead);
				 for (int MixCt = 0; MixCt < BlocksRead; MixCt++) {
						tempMixBlock8[(MixCt * 2)] += ReadBlock8[MixCt] *
               											fQueue[QueueCt].LeftLevel;
						tempMixBlock8[(MixCt+1) * 2] += ReadBlock8[MixCt] *
               											fQueue[QueueCt].RightLevel;
				 }
			}
		}
     }
     // Check for high and low clipping
	 for (int MixCt = 0; MixCt < BufferSize; MixCt++) {
		 	(tempMixBlock8[MixCt] > 127) ? MixBlock8[MixCt] = 127 :
      		(tempMixBlock8[MixCt] < -128) ? MixBlock8[MixCt] = -128 :
           	MixBlock8[MixCt] = tempMixBlock8[MixCt];
	 }
     if (wo) wo->Write((PBYTE)MixBlock8, BufferLength);
   }

   if (wo) {
	   delete wo;
	   wo = NULL;
   }

   delete ReadBlock8;
   delete tempMixBlock8;
   delete MixBlock8;
   ReadBlock8 = NULL;
   tempMixBlock8 = NULL;
   MixBlock8 = NULL;

   Playing = FALSE;

   if (hSendSignal != NULL) SetEvent(hSendSignal);
   return TRUE;
}

BOOL CPlayMix::Play(int Device) {
   if (Playing) return FALSE;

   if (wo != NULL) {
   	delete wo;
      wo = NULL;
   }

	Playing = TRUE;
   ShouldStop = FALSE;

   if (!GetHeaders()) return FALSE;

   switch (wavBits) {
   case 16:
   	  return Play16(Device);
      break;
   case 8:
   	  return Play8(Device);
      break;
   }

   // Shouldn't ever get here
   if (hSendSignal != NULL) SetEvent(hSendSignal);
   return FALSE;
}

void CPlayMix::Stop() {
   if (!Playing) return;

   ShouldStop = TRUE;
   
   for (int i = 0; i < QueueFiles; i++) {
	   if (fQueue[i].fHandle != NULL) {
		   fclose(fQueue[i].fHandle);
		   fQueue[i].fHandle = NULL;
	   }
   }
   QueueFiles = 0;
}

CPlayMix::~CPlayMix() {
	Playing = FALSE;
	if (wo != NULL) delete wo;
   // Make sure memory is cleaned up
   if (ReadBlock16) delete ReadBlock16;
   if (tempMixBlock16) delete tempMixBlock16;
   if (MixBlock16) delete MixBlock16;
   if (ReadBlock8) delete ReadBlock8;
   if (tempMixBlock8) delete tempMixBlock8;
   if (MixBlock8) delete MixBlock8;
}

