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
#include <stdlib.h>
#include <math.h>

#include "portaudio.h"

#include "soloud.h"
#include "soloud_thread.h"

namespace SoLoud
{
	static PaStream *gStream;

	static int portaudio_callback( 
				const void *input,
				void *output,
				unsigned long frameCount,
				const PaStreamCallbackTimeInfo* timeInfo,
				PaStreamCallbackFlags statusFlags,
				void *userData ) 
	{
		SoLoud::Soloud *soloud = (SoLoud::Soloud *)userData;
		float *mixdata = (float*)(soloud->mBackendData);
		soloud->mix((float*)output, frameCount);

		return 0;
	}

	static void portaudio_mutex_lock(void * mutex)
	{
		Thread::lockMutex(mutex);
	}

	static void portaudio_mutex_unlock(void * mutex)
	{
		Thread::unlockMutex(mutex);
	}

	void soloud_portaudio_deinit(SoLoud::Soloud *aSoloud)
	{
		Pa_CloseStream(gStream);
		Pa_Terminate();
		Thread::destroyMutex(aSoloud->mMutex);
		aSoloud->mMutex = 0;
		aSoloud->mLockMutexFunc = 0;
		aSoloud->mUnlockMutexFunc = 0;
	}

	int portaudio_init(SoLoud::Soloud *aSoloud, int aFlags, int aSamplerate, int aBuffer)
	{
		aSoloud->init(aSamplerate, aBuffer * 2, aFlags);
		aSoloud->mBackendCleanupFunc = soloud_portaudio_deinit;
		aSoloud->mMutex = Thread::createMutex();
		aSoloud->mLockMutexFunc = portaudio_mutex_lock;
		aSoloud->mUnlockMutexFunc = portaudio_mutex_unlock;
		Pa_Initialize();
		Pa_OpenDefaultStream(&gStream, 0, 2, paFloat32, aSamplerate, paFramesPerBufferUnspecified, portaudio_callback, (void*)aSoloud);
		Pa_StartStream(gStream);
		return 0;
	}
	
};
