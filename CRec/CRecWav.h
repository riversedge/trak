#ifndef CPLAYWAV_H
#define CPLAYWAV_H

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

class CWaveInBuffer {
    public:
        CWaveInBuffer();
        ~CWaveInBuffer();
        BOOL Init(HWAVEIN hWave, int Size, FILE *OutFile);
        BOOL isDone();
        int Write();
        void Flush();
	private:
		  WAVEHDR      m_Hdr;
        HWAVEIN      m_hWave;
        int          m_nBytes;
        FILE        *DatFile;
};

class CWaveIn {
    public:
        CWaveIn(int Device, LPCWAVEFORMATEX Format, int BufferSize, FILE *OutFile);
        ~CWaveIn();
        BOOL Record();
        void Stop();
        void Flush();
        void Wait();
        void Reset();
        int  TotalBytesRecorded;
        int  TotalSamplesRecorded;
		BOOL WasError;
	private:
   	  BOOL 				  Recording;
        const HANDLE       m_hSem;
        const int          m_nBuffers;
        int          m_CurrentBuffer;
        BOOL         m_NoBuffer;
        CWaveInBuffer *m_Hdrs;
        HWAVEIN     m_hWave;
};/*    CWaveBuffer*/

#endif
