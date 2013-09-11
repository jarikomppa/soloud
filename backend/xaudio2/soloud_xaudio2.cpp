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
#include <xaudio2.h>

#ifdef _MSC_VER
#pragma comment(lib, "xaudio2.lib")
#endif

namespace SoLoud
{
    static const int BUFFER_COUNT = 2;

    struct BufferData
    {
        int index;
        float *buffer;
        XAUDIO2_BUFFER bufferInfo;
        struct Xaudio2Data *parent;
    };

    struct Xaudio2Data
    {
        BufferData *bufferData[BUFFER_COUNT];
        bool buffersToFill[BUFFER_COUNT];
        IXAudio2 *xaudio2;
        IXAudio2MasteringVoice *masteringVoice;
        IXAudio2SourceVoice *sourceVoice;
        class VoiceCallback *voiceCb;
        int samples;
        HANDLE audioEvent;
        Soloud *soloud;
        bool audioProcessingDone;
        Thread::ThreadHandle thread;
    };

    class VoiceCallback : public IXAudio2VoiceCallback
    {
    public:
        VoiceCallback() 
            : IXAudio2VoiceCallback() {}
        virtual ~VoiceCallback() {}

    private:
        // Called just before this voice's processing pass begins.
        void __stdcall OnVoiceProcessingPassStart(UINT32 bytesRequired) {}

        // Called just after this voice's processing pass ends.
        void __stdcall OnVoiceProcessingPassEnd() {}

        // Called when this voice has just finished playing a buffer stream
        // (as marked with the XAUDIO2_END_OF_STREAM flag on the last buffer).
        void __stdcall OnStreamEnd() {}

        // Called when this voice is about to start processing a new buffer.
        void __stdcall OnBufferStart(void *bufferContext) {}

        // Called when this voice has just finished processing a buffer.
        // The buffer can now be reused or destroyed.
        void __stdcall OnBufferEnd(void *bufferContext) 
        {
            BufferData *bd = reinterpret_cast<BufferData*>(bufferContext);
            bd->parent->buffersToFill[bd->index] = true;
            SetEvent(bd->parent->audioEvent);
        }

        // Called when this voice has just reached the end position of a loop.
        void __stdcall OnLoopEnd(void *bufferContext) {}

        // Called in the event of a critical error during voice processing,
        // such as a failing xAPO or an error from the hardware XMA decoder.
        // The voice may have to be destroyed and re-created to recover from
        // the error.  The callback arguments report which buffer was being
        // processed when the error occurred, and its HRESULT code.
        void __stdcall OnVoiceError(void *bufferContext, HRESULT error) {}
    };

    static void xaudio2Thread(LPVOID param)
    {
        Xaudio2Data *data = static_cast<Xaudio2Data*>(param);
        while (!data->audioProcessingDone) {
            for (int i=0;i<BUFFER_COUNT;++i) {
                if (!data->buffersToFill[i])
                    continue;
                data->soloud->mix(data->bufferData[i]->buffer, data->samples);
                data->buffersToFill[i] = false;
                data->sourceVoice->SubmitSourceBuffer(&data->bufferData[i]->bufferInfo);
            }
            WaitForSingleObject(data->audioEvent, INFINITE);
        }
    }

    static void xaudio2Cleanup(Soloud *aSoloud)
    {
        if (0 == aSoloud->mBackendData)
            return;
        Xaudio2Data *data = static_cast<Xaudio2Data*>(aSoloud->mBackendData);
        data->audioProcessingDone = true;
        SetEvent(data->audioEvent);
        Thread::wait(data->thread, INFINITE);
        Thread::release(data->thread);
        CloseHandle(data->audioEvent);
        if (0 != data->sourceVoice) {
            data->sourceVoice->Stop();
            data->sourceVoice->FlushSourceBuffers();
        }
        if (0 != data->xaudio2)
            data->xaudio2->StopEngine();
        if (0 != data->sourceVoice)
            data->sourceVoice->DestroyVoice();
        if (0 != data->voiceCb)
            delete data->voiceCb;
        if (0 != data->masteringVoice)
            data->masteringVoice->DestroyVoice();
        if (0 != data->xaudio2)
            data->xaudio2->Release();
        for (int i=0;i<BUFFER_COUNT;++i) {
            if (0 != data->bufferData[i]->buffer)
                delete[] data->bufferData[i]->buffer;
            delete data->bufferData[i];
        }
        Thread::destroyMutex(data->soloud->mMutex);
        data->soloud->mMutex = 0;
        data->soloud->mLockMutexFunc = 0;
        data->soloud->mUnlockMutexFunc = 0;
        delete data;
        aSoloud->mBackendData = 0;
    }

    int xaudio2_init(Soloud *aSoloud, int aVoices, int aFlags, int aSamplerate, int aBuffer)
    {
        Xaudio2Data *data = new Xaudio2Data;
        ZeroMemory(data, sizeof(Xaudio2Data));
        aSoloud->mBackendData = data;
        aSoloud->mBackendCleanupFunc = xaudio2Cleanup;
        data->audioEvent = CreateEvent(0, FALSE, FALSE, 0);
        if (0 == data->audioEvent)
            return 1;
        WAVEFORMATEX format;
        ZeroMemory(&format, sizeof(WAVEFORMATEX));
        format.nChannels = 2;
        format.nSamplesPerSec = aSamplerate;
        format.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
        format.nAvgBytesPerSec = aSamplerate*sizeof(float)*format.nChannels;
        format.nBlockAlign = sizeof(float)*format.nChannels;
        format.wBitsPerSample = sizeof(float)*8;
        if (FAILED(XAudio2Create(&data->xaudio2)))
            return 2;
        if (FAILED(data->xaudio2->CreateMasteringVoice(&data->masteringVoice, 
                                                       format.nChannels, aSamplerate))) {
            return 3;
        }
        data->voiceCb = new VoiceCallback;
        if (FAILED(data->xaudio2->CreateSourceVoice(&data->sourceVoice, 
            &format, 0, 2.f, data->voiceCb))) {
            return 4;
        }
        for (int i=0;i<BUFFER_COUNT;++i) {
            data->bufferData[i] = new BufferData;
            data->bufferData[i]->index = i;
            data->bufferData[i]->buffer = new float[aBuffer * format.nChannels];
            ZeroMemory(&data->bufferData[i]->bufferInfo, sizeof(XAUDIO2_BUFFER));
            data->bufferData[i]->bufferInfo.AudioBytes = sizeof(float)*aBuffer*format.nChannels;
            data->bufferData[i]->bufferInfo.pAudioData = reinterpret_cast<const BYTE*>(data->bufferData[i]->buffer);
            data->bufferData[i]->bufferInfo.pContext = data->bufferData[i];
            data->bufferData[i]->parent = data;
            data->buffersToFill[i] = true;
        }
        data->samples = aBuffer;
        aSoloud->mMutex = Thread::createMutex();
        aSoloud->mLockMutexFunc = Thread::lockMutex;
        aSoloud->mUnlockMutexFunc = Thread::unlockMutex;
        data->soloud = aSoloud;
        aSoloud->init(aVoices, aSamplerate, aBuffer * format.nChannels, aFlags);
        data->thread = Thread::createThread(xaudio2Thread, data);
        data->sourceVoice->Start();
        return 0;
    }
};
