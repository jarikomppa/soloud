#include "soloud.h"
#include "soloud_thread.h"
#include <windows.h>
#include <mmsystem.h>

#if (__GNUC__ < 4)
static const WORD WAVE_FORMAT_IEEE_FLOAT = 0x0003;
#else
#include <mmreg.h>
#endif 

#ifdef _MSC_VER
#pragma comment(lib, "winmm.lib")
#endif

namespace SoLoud
{
    static const int BUFFER_COUNT = 2;

    struct SoLoudWinMMData
    {
        float *buffer[BUFFER_COUNT];
        int channels;
        HWAVEOUT waveOut;
        WAVEHDR header[BUFFER_COUNT];
        HANDLE audioEvent;
        Soloud *soloud;
        bool audioProcessingDone;
        bool threadRunning;
    };

    static void winMMThread(LPVOID param)
    {
        SoLoudWinMMData *data = static_cast<SoLoudWinMMData*>(param);
        data->threadRunning = true;
        while (!data->audioProcessingDone) {
            for (int i=0;i<BUFFER_COUNT;++i) {
                if ((data->header[i].dwFlags & WHDR_INQUEUE) != 0) 
                    continue;
                int samples = (data->header[i].dwBufferLength/4)/data->channels;
                data->soloud->mix(data->buffer[i], samples);
                if (MMSYSERR_NOERROR != waveOutWrite(data->waveOut, &data->header[i], sizeof(WAVEHDR)))
                    return;
            }
            WaitForSingleObject(data->audioEvent, INFINITE);
        }
        data->threadRunning = false;
    }

    static void winMMCleanup(Soloud *aSoloud)
    {
        if (0 == aSoloud->mBackendData)
            return;
        SoLoudWinMMData *data = static_cast<SoLoudWinMMData*>(aSoloud->mBackendData);
        waveOutReset(data->waveOut);
        data->audioProcessingDone = true;
        SetEvent(data->audioEvent);
        while (data->threadRunning)
            Sleep(10);
        for (int i=0;i<BUFFER_COUNT;++i) {
            waveOutUnprepareHeader(data->waveOut, &data->header[i], sizeof(WAVEHDR));
            if (0 != data->buffer[i])
                delete[] data->buffer[i];
        }
        CloseHandle(data->audioEvent);
        waveOutClose(data->waveOut);
        Thread::destroyMutex(data->soloud->mMutex);
        data->soloud->mMutex = 0;
        data->soloud->mLockMutexFunc = 0;
        data->soloud->mUnlockMutexFunc = 0;
        delete data;
        aSoloud->mBackendData = 0;
    }

    int winmm_init(Soloud *aSoloud, int aChannels, int aFlags, int aSamplerate, int aBuffer)
    {
        SoLoudWinMMData *data = new SoLoudWinMMData;
        ZeroMemory(data, sizeof(SoLoudWinMMData));
        aSoloud->mBackendData = data;
        aSoloud->mBackendCleanupFunc = winMMCleanup;
        data->audioEvent = CreateEvent(0, FALSE, FALSE, 0);
        if (0 == data->audioEvent)
            return 1;
        WAVEFORMATEX format;
        ZeroMemory(&format, sizeof(WAVEFORMATEX));
        format.nChannels = data->channels = min(aChannels, 2);
        format.nSamplesPerSec = aSamplerate;
        format.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
        format.nAvgBytesPerSec = aSamplerate*sizeof(float)*format.nChannels;
        format.nBlockAlign = sizeof(float)*format.nChannels;
        format.wBitsPerSample = sizeof(float)*8;
        if (MMSYSERR_NOERROR != waveOutOpen(&data->waveOut, WAVE_MAPPER, &format, 
                            reinterpret_cast<DWORD_PTR>(data->audioEvent), 0, CALLBACK_EVENT)) {
            return 2;
        }
        for (int i=0;i<BUFFER_COUNT;++i) {
            data->buffer[i] = new float[aBuffer*format.nChannels];
            ZeroMemory(&data->header[i], sizeof(WAVEHDR));
            data->header[i].dwBufferLength = aBuffer*sizeof(float)*2;
            data->header[i].lpData = reinterpret_cast<LPSTR>(data->buffer[i]);
            if (MMSYSERR_NOERROR != waveOutPrepareHeader(data->waveOut, &data->header[i], 
                                                         sizeof(WAVEHDR))) {
                return 3;
            }
        }
        aSoloud->mMutex = Thread::createMutex();
        aSoloud->mLockMutexFunc = Thread::lockMutex;
        aSoloud->mUnlockMutexFunc = Thread::unlockMutex;
        data->soloud = aSoloud;
        aSoloud->init(aChannels, aSamplerate, aBuffer * format.nChannels, aFlags);
        Thread::createThread(winMMThread, data);
        return 0;
    }
};
