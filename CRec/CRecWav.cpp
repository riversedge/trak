#include <windows.h>
#include <mmsystem.h>
#include "CRecWav.h"

/********************************************************************
   Trivial wave player stuff
 ********************************************************************/

CWaveInBuffer::CWaveInBuffer() {
}

BOOL CWaveInBuffer::Init(HWAVEIN hWave, int Size, FILE *OutFile){
	 DatFile = OutFile;
    m_hWave  = hWave;
    m_nBytes = 0;
    /*  Allocate a buffer and initialize the header */
	 m_Hdr.lpData = (LPSTR)LocalAlloc(LMEM_FIXED, Size);
    if (m_Hdr.lpData == NULL) {   return FALSE;    }
    m_Hdr.dwBufferLength  = Size;
    m_Hdr.dwBytesRecorded = 0;
    m_Hdr.dwUser = 0;
    m_Hdr.dwFlags = 0;
    m_Hdr.dwLoops = 0;
    m_Hdr.lpNext = 0;
    m_Hdr.reserved = 0;    /*  Prepare it */
    waveInPrepareHeader(hWave, &m_Hdr, sizeof(WAVEHDR));
    waveInAddBuffer(hWave, &m_Hdr, sizeof(WAVEHDR));
    return TRUE;
}

CWaveInBuffer::~CWaveInBuffer() {
	 if (m_Hdr.lpData) {
        waveInUnprepareHeader(m_hWave, &m_Hdr, sizeof(WAVEHDR));
        LocalFree(m_Hdr.lpData);
    }
}

BOOL CWaveInBuffer::isDone() {
   if (m_Hdr.dwFlags == (m_Hdr.dwFlags | WHDR_DONE)) return TRUE;

   return FALSE;
}

void CWaveInBuffer::Flush() {
    //ASSERT(m_nBytes != 0);
    flushall();
}

int CWaveInBuffer::Write() {
    //ASSERT((DWORD)m_nBytes != m_Hdr.dwBufferLength);
    int BytesWritten = 0;
	 if (m_Hdr.lpData) {
        waveInUnprepareHeader(m_hWave, &m_Hdr, sizeof(WAVEHDR));
        BytesWritten = fwrite(m_Hdr.lpData, sizeof(BYTE), m_Hdr.dwBytesRecorded, DatFile);
        LocalFree(m_Hdr.lpData);
    }
    Init(m_hWave, m_Hdr.dwBufferLength, DatFile);

    return BytesWritten;
}
void CALLBACK WaveInCallback(HWAVEIN hWave, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2) {
	if (uMsg == WIM_DATA) {
   	ReleaseSemaphore((HANDLE)dwUser, 1, NULL);
   }
}

/*    CWaveIn*/
CWaveIn::CWaveIn(int Device, LPCWAVEFORMATEX Format, int BufferSize, FILE *OutFile) :
    m_nBuffers(2),
    m_CurrentBuffer(0),
    m_NoBuffer(TRUE),
    m_hSem(CreateSemaphore(NULL, 2, 2, NULL)),
    m_Hdrs(new CWaveInBuffer[2]),
    m_hWave(NULL)
{
    /*  Create wave device */
    MMRESULT mmr = waveInOpen(&m_hWave,
         	     					  Device,
					                Format,
					                (DWORD)WaveInCallback,
					                (DWORD)m_hSem,
					                CALLBACK_FUNCTION);

	WasError = FALSE;
    if (mmr != MMSYSERR_NOERROR) {
		if (mmr == MMSYSERR_ALLOCATED) MessageBox(NULL, "Specified resource is already allocated.", "Trak Multitracking System", MB_ICONWARNING);
		if (mmr == MMSYSERR_BADDEVICEID) MessageBox(NULL, "Specified device identifier is out of range.", "Trak Multitracking System", MB_ICONWARNING);
		if (mmr == MMSYSERR_NODRIVER) MessageBox(NULL, "No device driver is present.", "Trak Multitracking System", MB_ICONWARNING);
		if (mmr == MMSYSERR_NOMEM) MessageBox(NULL, "Unable to allocate or lock memory.", "Trak Multitracking System", MB_ICONWARNING);
		if (mmr == WAVERR_BADFORMAT) MessageBox(NULL, "Attempted to open with an unsupported waveform-audio format.", "Trak Multitracking System", MB_ICONWARNING);
		WasError = TRUE;
        return;
    }
    /*  Initialize the wave buffers */
    m_Hdrs[0].Init(m_hWave, BufferSize, OutFile);
    m_Hdrs[1].Init(m_hWave, BufferSize, OutFile);
    Recording = FALSE;
}

CWaveIn::~CWaveIn() {
    Recording = FALSE;
    /*  First get the buffers back */
    waveInReset(m_hWave);
    /*  Free the buffers */
    delete [] m_Hdrs;
    /*  Close the wave device */
    waveInClose(m_hWave);
    /*  Free the semaphore */
    CloseHandle(m_hSem);
}

void CWaveIn::Flush() {
   m_Hdrs[0].Flush();
   m_Hdrs[1].Flush();
	if (!m_NoBuffer) {
        m_NoBuffer = TRUE;
        m_CurrentBuffer = (m_CurrentBuffer + 1) % m_nBuffers;
   }
}

void CWaveIn::Reset() {
   Recording = FALSE;
	waveInReset(m_hWave);
}

void CWaveIn::Stop() {
   Recording = FALSE;
   waveInStop(m_hWave);
   ReleaseSemaphore(m_hSem, 2, NULL);
   Flush();
}

BOOL CWaveIn::Record() {
   Recording = TRUE;
   TotalBytesRecorded = 0;
   if (waveInStart(m_hWave) != MMSYSERR_NOERROR) {
	   MessageBox(NULL, "A recording device error has occurred.  Recording terminated.", "Trak Multitracking System", MB_ICONSTOP);
	   waveInStop(m_hWave);
	   return FALSE;
   }
	while (Recording) {
        /*  Get a buffer if necessary */
        if (m_NoBuffer) {
            WaitForSingleObject(m_hSem, INFINITE);
            m_NoBuffer = FALSE;
        }        /*  Write into a buffer */
        if (!Recording) {
           waveInStop(m_hWave);
           return TRUE;
        }
        if (m_Hdrs[0].isDone()) {
           TotalBytesRecorded += m_Hdrs[0].Write();
           m_NoBuffer = TRUE;
           m_CurrentBuffer = 1;
			  TotalSamplesRecorded++;
        }
        if (m_Hdrs[1].isDone()) {
           TotalBytesRecorded += m_Hdrs[1].Write();
           m_NoBuffer = TRUE;
           m_CurrentBuffer = 0;
			  TotalSamplesRecorded++;
        }
   }

   waveInStop(m_hWave);
   return TRUE;
}

void CWaveIn::Wait() {
	/*  Send any remaining buffers */
   Flush();
   /*  Wait for the buffers back */
   for (int i = 0; i < m_nBuffers; i++) {
        WaitForSingleObject(m_hSem, INFINITE);
   }
   LONG lPrevCount;
   ReleaseSemaphore(m_hSem, m_nBuffers, &lPrevCount);
}


