#include <windows.h>
#include <mmsystem.h>

class CWaveBuffer {
    public:
        CWaveBuffer();
        ~CWaveBuffer();
        BOOL Init(HWAVEOUT hWave, int Size);
        void Done();
        BOOL Write(PBYTE pData, int nBytes, int& BytesWritten);
        void Flush();
	private:
		  WAVEHDR      m_Hdr;
        HWAVEOUT     m_hWave;
        int          m_nBytes;
};

class CWaveOut {
    public:
        CWaveOut(int Device, LPCWAVEFORMATEX Format, int nBuffers, int BufferSize);
        ~CWaveOut();
        void Write(PBYTE Data, int nBytes);
        void Flush();
        void Wait();
        void Reset();
        void Pause();
        void Restart();
		BOOL WasError;
	private:
        const HANDLE       m_hSem;
        const int          m_nBuffers;
        int          m_CurrentBuffer;
        BOOL         m_NoBuffer;
        CWaveBuffer *m_Hdrs;
        HWAVEOUT     m_hWave;
};/*    CWaveBuffer*/
