/*
SoLoud audio engine
Copyright (c) 2013-2019 Jari Komppa

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

#if !defined(WITH_NULL)

namespace SoLoud
{
    result null_init(Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer)
	{
		return NOT_IMPLEMENTED;
	}
};

#else

namespace SoLoud
{
    struct SoLoudNullData
    {
        short* sampleBuffer;
        HANDLE audioProcessingDoneEvent;
        Soloud *soloud;
        unsigned int sampleCount;
		unsigned int sampleRate;
        Thread::ThreadHandle threadHandle;
    };

    static void nullThread(LPVOID aParam)
    {
        SoLoudNullData *data = static_cast<SoLoudNullData*>(aParam);

		while (true)
        {
			float milliseconds = 1000.0f * data->sampleCount / data->sampleRate;
			DWORD result = WaitForSingleObject(data->audioProcessingDoneEvent, milliseconds);

			if (WAIT_TIMEOUT == result)
			{
				data->soloud->mixSigned16(data->sampleBuffer, data->sampleCount);
			}
			else
			{
				break;
			}
        }
    }

    static void nullCleanup(Soloud *aSoloud)
    {
        if (0 == aSoloud->mBackendData)
        {
            return;
        }
        SoLoudNullData *data = static_cast<SoLoudNullData*>(aSoloud->mBackendData);
        SetEvent(data->audioProcessingDoneEvent);
        if (data->threadHandle)
		{
			Thread::wait(data->threadHandle);
			Thread::release(data->threadHandle);
		}
        if (0 != data->sampleBuffer)
        {
            delete[] data->sampleBuffer;
        }
        CloseHandle(data->audioProcessingDoneEvent);
        delete data;
        aSoloud->mBackendData = 0;
    }

    result null_init(Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer, unsigned int aChannels)
    {
		if (aChannels == 0 || aChannels == 3 || aChannels == 5 || aChannels == 7 || aChannels > MAX_CHANNELS || aBuffer < SAMPLE_GRANULARITY)
			return INVALID_PARAMETER;

        SoLoudNullData *data = new SoLoudNullData;
        ZeroMemory(data, sizeof(SoLoudNullData));
        aSoloud->mBackendData = data;
        aSoloud->mBackendCleanupFunc = nullCleanup;
        data->sampleCount = aBuffer;
		data->sampleRate = aSamplerate;
        data->soloud = aSoloud;
        data->audioProcessingDoneEvent = CreateEvent(0, FALSE, FALSE, 0);
        if (0 == data->audioProcessingDoneEvent)
        {
            return UNKNOWN_ERROR;
        }
        data->sampleBuffer = new short[data->sampleRate * aChannels];
        aSoloud->postinit(aSamplerate, aBuffer, aFlags, aChannels);
        data->threadHandle = Thread::createThread(nullThread, data);
        if (0 == data->threadHandle)
        {
            return UNKNOWN_ERROR;
        }
        aSoloud->mBackendString = "null driver";
        return SO_NO_ERROR;
    }
};
#endif