/*
SoLoud audio engine
Copyright (c) 2013 Jari Komppa

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
*/

#include "soloud.h"
#include "soloud_thread.h"
#include <windows.h>
#include <mmsystem.h>

#ifdef _MSC_VER
#pragma comment(lib, "winmm.lib")
#endif

namespace SoLoud
{
    static const int BUFFER_COUNT = 2;

    struct SoLoudWinMMData
    {
        float *buffer;
        short *sampleBuffer[BUFFER_COUNT];
        WAVEHDR header[BUFFER_COUNT];
        HWAVEOUT waveOut;
        HANDLE audioEvent;
        Soloud *soloud;
        int samples;
        bool audioProcessingDone;
        Thread::ThreadHandle threadHandle;
    };

    static void winMMThread(LPVOID param)
    {
        SoLoudWinMMData *data = static_cast<SoLoudWinMMData*>(param);
        while (!data->audioProcessingDone) {
            for (int i=0;i<BUFFER_COUNT;++i) {
                if (0 != (data->header[i].dwFlags & WHDR_INQUEUE)) 
                    continue;
                data->soloud->mix(data->buffer, data->samples);
                short *tgtBuf = data->sampleBuffer[i];
                for (DWORD j=0;j<(data->header[i].dwBufferLength/sizeof(short));++j) {
                    tgtBuf[j] = static_cast<short>(floor(data->buffer[j] 
                                                         * static_cast<float>(0x7fff)));
                }
                if (MMSYSERR_NOERROR != waveOutWrite(data->waveOut, &data->header[i], 
                                                     sizeof(WAVEHDR))) {
                    return;
                }
            }
            WaitForSingleObject(data->audioEvent, INFINITE);
        }
    }

    static void winMMCleanup(Soloud *aSoloud)
    {
        if (0 == aSoloud->mBackendData)
            return;
        SoLoudWinMMData *data = static_cast<SoLoudWinMMData*>(aSoloud->mBackendData);
        data->audioProcessingDone = true;
        SetEvent(data->audioEvent);
        Thread::wait(data->threadHandle, INFINITE);
        Thread::release(data->threadHandle);
        CloseHandle(data->audioEvent);
        waveOutReset(data->waveOut);
        for (int i=0;i<BUFFER_COUNT;++i) {
            waveOutUnprepareHeader(data->waveOut, &data->header[i], sizeof(WAVEHDR));
            if (0 != data->sampleBuffer[i])
                delete[] data->sampleBuffer[i];
        }
        if (0 != data->buffer)
            delete[] data->buffer;
        waveOutClose(data->waveOut);
        Thread::destroyMutex(data->soloud->mMutex);
        data->soloud->mMutex = 0;
        data->soloud->mLockMutexFunc = 0;
        data->soloud->mUnlockMutexFunc = 0;
        delete data;
        aSoloud->mBackendData = 0;
    }

    int winmm_init(Soloud *aSoloud, int aVoices, int aFlags, int aSamplerate, int aBuffer)
    {
        SoLoudWinMMData *data = new SoLoudWinMMData;
        ZeroMemory(data, sizeof(SoLoudWinMMData));
        aSoloud->mBackendData = data;
        aSoloud->mBackendCleanupFunc = winMMCleanup;
        data->samples = aBuffer;
        data->soloud = aSoloud;
        data->audioEvent = CreateEvent(0, FALSE, FALSE, 0);
        if (0 == data->audioEvent)
            return 1;
        WAVEFORMATEX format;
        ZeroMemory(&format, sizeof(WAVEFORMATEX));
        format.nChannels = 2;
        format.nSamplesPerSec = aSamplerate;
        format.wFormatTag = WAVE_FORMAT_PCM;
        format.wBitsPerSample = sizeof(short)*8;
        format.nBlockAlign = (format.nChannels*format.wBitsPerSample)/8;
        format.nAvgBytesPerSec = format.nSamplesPerSec*format.nBlockAlign;
        if (MMSYSERR_NOERROR != waveOutOpen(&data->waveOut, WAVE_MAPPER, &format, 
                            reinterpret_cast<DWORD_PTR>(data->audioEvent), 0, CALLBACK_EVENT)) {
            return 2;
        }
        data->buffer = new float[data->samples*format.nChannels];
        for (int i=0;i<BUFFER_COUNT;++i) {
            data->sampleBuffer[i] = new short[data->samples*format.nChannels];
            ZeroMemory(&data->header[i], sizeof(WAVEHDR));
            data->header[i].dwBufferLength = data->samples*sizeof(short)*format.nChannels;
            data->header[i].lpData = reinterpret_cast<LPSTR>(data->sampleBuffer[i]);
            if (MMSYSERR_NOERROR != waveOutPrepareHeader(data->waveOut, &data->header[i], 
                                                         sizeof(WAVEHDR))) {
                return 3;
            }
        }
        aSoloud->mMutex = Thread::createMutex();
        aSoloud->mLockMutexFunc = Thread::lockMutex;
        aSoloud->mUnlockMutexFunc = Thread::unlockMutex;
        aSoloud->init(aVoices, aSamplerate, data->samples * format.nChannels, aFlags);
        data->threadHandle = Thread::createThread(winMMThread, data);
        if (0 == data->threadHandle)
            return 4;
        return 0;
    }
};
