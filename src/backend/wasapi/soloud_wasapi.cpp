/*
SoLoud audio engine
Copyright (c) 2013-2014 Jari Komppa

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

#if !defined(WITH_WASAPI)

namespace SoLoud
{
	result wasapi_init(Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer)
	{
		return NOT_IMPLEMENTED;
	}
};

#else

#include <windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>

#define SAFE_RELEASE(x) \
    if (0 != (x)) \
    { \
        (x)->Release(); \
        (x) = 0; \
    }

namespace SoLoud
{
    struct WASAPIData
    {
        IMMDeviceEnumerator *deviceEnumerator;
        IMMDevice *device;
        IAudioClient *audioClient;
        IAudioRenderClient *renderClient;
        HANDLE bufferEndEvent;
        HANDLE audioProcessingDoneEvent;
        Thread::ThreadHandle thread;
        Soloud *soloud;
        UINT32 bufferFrames;
        int channels;
    };

    static void wasapiSubmitBuffer(WASAPIData *aData, UINT32 aFrames)
    {
        BYTE *buffer = 0;
        if (FAILED(aData->renderClient->GetBuffer(aFrames, &buffer)))
        {
            return;
        }
		aData->soloud->mixSigned16((short *)buffer, aFrames);
        aData->renderClient->ReleaseBuffer(aFrames, 0);
    }

    static void wasapiThread(LPVOID aParam)
    {
        WASAPIData *data = static_cast<WASAPIData*>(aParam);
        wasapiSubmitBuffer(data, data->bufferFrames);
        data->audioClient->Start();
        while (WAIT_OBJECT_0 != WaitForSingleObject(data->audioProcessingDoneEvent, 0)) 
        {
            WaitForSingleObject(data->bufferEndEvent, INFINITE);
            UINT32 padding = 0;
            if (FAILED(data->audioClient->GetCurrentPadding(&padding)))
            {
                continue;
            }
            UINT32 frames = data->bufferFrames - padding;
            wasapiSubmitBuffer(data, frames);
        }
    }

    static void wasapiCleanup(Soloud *aSoloud)
    {
        if (0 == aSoloud->mBackendData)
        {
            return;
        }
        WASAPIData *data = static_cast<WASAPIData*>(aSoloud->mBackendData);
        SetEvent(data->audioProcessingDoneEvent);
        SetEvent(data->bufferEndEvent);
		if (data->thread)
		{
			Thread::wait(data->thread);
			Thread::release(data->thread);
		}
        CloseHandle(data->bufferEndEvent);
        CloseHandle(data->audioProcessingDoneEvent);
        if (0 != data->audioClient)
        {
            data->audioClient->Stop();
        }
        SAFE_RELEASE(data->renderClient);
        SAFE_RELEASE(data->audioClient);
        SAFE_RELEASE(data->device);
        SAFE_RELEASE(data->deviceEnumerator);
        delete data;
        aSoloud->mBackendData = 0;
        CoUninitialize();
    }

	result wasapi_init(Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer, unsigned int aChannels)
    {
        if (FAILED(CoInitializeEx(0, COINIT_MULTITHREADED)))
        {
            return UNKNOWN_ERROR;
        }
        WASAPIData *data = new WASAPIData;
        ZeroMemory(data, sizeof(WASAPIData));
        aSoloud->mBackendData = data;
        aSoloud->mBackendCleanupFunc = wasapiCleanup;
		
        data->bufferEndEvent = CreateEvent(0, FALSE, FALSE, 0);
        if (0 == data->bufferEndEvent)
        {
            return UNKNOWN_ERROR;
        }
        data->audioProcessingDoneEvent = CreateEvent(0, FALSE, FALSE, 0);
        if (0 == data->audioProcessingDoneEvent)
        {
            return UNKNOWN_ERROR;
        }
        if (FAILED(CoCreateInstance(__uuidof(MMDeviceEnumerator), 0, CLSCTX_ALL, 
                   __uuidof(IMMDeviceEnumerator), 
                   reinterpret_cast<void**>(&data->deviceEnumerator)))) 
        {
            return UNKNOWN_ERROR;
        }
        if (FAILED(data->deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, 
                                                                   &data->device))) 
        {
            return UNKNOWN_ERROR;
        }
        if (FAILED(data->device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, 0, 
                                          reinterpret_cast<void**>(&data->audioClient)))) 
        {
            return UNKNOWN_ERROR;
        }
        WAVEFORMATEX format;
        ZeroMemory(&format, sizeof(WAVEFORMATEX));
        format.nChannels = 2;
        format.nSamplesPerSec = aSamplerate;
        format.wFormatTag = WAVE_FORMAT_PCM;
        format.wBitsPerSample = sizeof(short)*8;
        format.nBlockAlign = (format.nChannels*format.wBitsPerSample)/8;
        format.nAvgBytesPerSec = format.nSamplesPerSec*format.nBlockAlign;
        REFERENCE_TIME dur = static_cast<REFERENCE_TIME>(static_cast<double>(aBuffer)
                                           / (static_cast<double>(aSamplerate)*(1.0/10000000.0)));
		HRESULT res = data->audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED,
			AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
			dur, 0, &format, 0);
		if (FAILED(res)) 
        {
            return UNKNOWN_ERROR;
        }
        data->bufferFrames = 0;
        if (FAILED(data->audioClient->GetBufferSize(&data->bufferFrames)))
        {
            return UNKNOWN_ERROR;
        }
        if (FAILED(data->audioClient->GetService(__uuidof(IAudioRenderClient), 
                                                 reinterpret_cast<void**>(&data->renderClient)))) 
        {
            return UNKNOWN_ERROR;
        }
        if (FAILED(data->audioClient->SetEventHandle(data->bufferEndEvent)))
        {
            return UNKNOWN_ERROR;
        }
        data->channels = format.nChannels;
        data->soloud = aSoloud;
        aSoloud->postinit(aSamplerate, data->bufferFrames * format.nChannels, aFlags, 2);
        data->thread = Thread::createThread(wasapiThread, data);
        if (0 == data->thread)
        {
            return UNKNOWN_ERROR;
        }
        aSoloud->mBackendString = "WASAPI";
        return 0;
    }
};
#endif