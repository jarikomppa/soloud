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

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

#include "soloud.h"
#include "soloud_thread.h"

#define NUM_BUFFERS 2
#define BUFFER_SIZE 4096

static ALCdevice* device = NULL;
static ALCcontext* context = NULL;
static ALenum format = 0;
static ALuint source = 0;
static int frequency = 0;
static volatile int threadrun = 0;

typedef ALCdevice* (*alc_OpenDevice)(const ALCchar *devicename);
typedef ALCcontext* (*alc_CreateContext)(ALCdevice *device, const ALCint* attrlist);
typedef ALCboolean (*alc_MakeContextCurrent)(ALCcontext *context);
typedef void (*al_GetSourcei)(ALuint source, ALenum param, ALint *value);
typedef void (*al_SourceQueueBuffers)(ALuint source, ALsizei nb, const ALuint *buffers);
typedef void (*al_SourceUnqueueBuffers)(ALuint source, ALsizei nb, ALuint *buffers);
typedef void (*al_BufferData)(ALuint buffer, ALenum format, const ALvoid *data, ALsizei size, ALsizei freq);
typedef void (*al_SourcePlay)(ALuint source);
typedef void (*al_GenBuffers)(ALsizei n, ALuint *buffers);
typedef void (*al_GenSources)(ALsizei n, ALuint *sources);

static alc_OpenDevice dAlcOpenDevice;
static alc_CreateContext dAlcCreateContext;
static alc_MakeContextCurrent dAlcMakeContextCurrent;
static al_GetSourcei dAlGetSourcei;
static al_SourceQueueBuffers dAlSourceQueueBuffers;
static al_SourceUnqueueBuffers dAlSourceUnqueueBuffers;
static al_BufferData dAlBufferData;
static al_SourcePlay dAlSourcePlay;
static al_GenBuffers dAlGenBuffers;
static al_GenSources dAlGenSources;

#ifdef WINDOWS_VERSION
#include <windows.h>

static HMODULE openDll()
{
    return LoadLibraryA("soft_oal.dll");
}

static void* getDllProc(HMODULE aDllHandle, const char *aProcName)
{
    return GetProcAddress(aDllHandle, aProcName);
}

#else
#include <dlfcn.h> // dll functions

typedef void* HMODULE;

static HMODULE openDll()
{
    return dlopen("libopenal.so", RTLD_LAZY);
}

static void* getDllProc(HMODULE aLibrary, const char *aProcName)
{
    return dlsym(aLibrary, aProcName);
}

#endif

namespace SoLoud
{
	void soloud_openal_deinit(SoLoud::Soloud *aSoloud)
	{
		threadrun++;
		while (threadrun == 1)
		{
			Thread::sleep(10);
		}
		Thread::destroyMutex(aSoloud->mMutex);
		aSoloud->mMutex = 0;
		aSoloud->mLockMutexFunc = 0;
		aSoloud->mUnlockMutexFunc = 0;
	}
	
	static void openal_mutex_lock(void * mutex)
	{
		Thread::lockMutex(mutex);
	}

	static void openal_mutex_unlock(void * mutex)
	{
		Thread::unlockMutex(mutex);
	}

	static void openal_iterate(SoLoud::Soloud *aSoloud)
	{
		ALuint buffer = 0;
		ALint buffersProcessed = 0;
		ALint state;
		dAlGetSourcei(source, AL_BUFFERS_PROCESSED, &buffersProcessed);

		float mixbuf[BUFFER_SIZE*2];
		short downbuf[BUFFER_SIZE*2];

		while (buffersProcessed--) 
		{
			aSoloud->mix(mixbuf,BUFFER_SIZE);
			int i;
			for (i = 0; i < BUFFER_SIZE*2; i++)
				downbuf[i] = (short)floor(mixbuf[i] * 0x7fff);

			dAlSourceUnqueueBuffers(source, 1, &buffer);

			dAlBufferData(buffer, format, downbuf, BUFFER_SIZE*4, frequency);

			dAlSourceQueueBuffers(source, 1, &buffer);
		}

		dAlGetSourcei(source, AL_SOURCE_STATE, &state);
		if (state != AL_PLAYING)
			dAlSourcePlay(source);
	}

	static void openal_thread(void *aParam)
	{
		Soloud *soloud = (Soloud *)aParam;
		while (threadrun == 0)
		{
			openal_iterate(soloud);
			Thread::sleep(1);
		}
		threadrun++;
	}

	int openal_init(SoLoud::Soloud *aSoloud, int aFlags, int aSamplerate, int aBuffer)
	{
        HMODULE dll = openDll();
        if (0 == dll)
        {
            return -1;
        }

        dAlcOpenDevice = static_cast<alc_OpenDevice>(getDllProc(dll, "alcOpenDevice"));
        dAlcCreateContext = static_cast<alc_CreateContext>(getDllProc(dll, "alcCreateContext"));
        dAlcMakeContextCurrent = static_cast<alc_MakeContextCurrent>(getDllProc(dll, "alcMakeContextCurrent"));
        dAlGetSourcei = static_cast<al_GetSourcei>(getDllProc(dll, "alGetSourcei"));
        dAlSourceQueueBuffers = static_cast<al_SourceQueueBuffers>(getDllProc(dll, "alSourceQueueBuffers"));
        dAlSourceUnqueueBuffers = static_cast<al_SourceUnqueueBuffers>(getDllProc(dll, "alSourceUnqueueBuffers"));
        dAlBufferData = static_cast<al_BufferData>(getDllProc(dll, "alBufferData"));
        dAlSourcePlay = static_cast<al_SourcePlay>(getDllProc(dll, "alSourcePlay"));
        dAlGenBuffers = static_cast<al_GenBuffers>(getDllProc(dll, "alGenBuffers"));
        dAlGenSources = static_cast<al_GenSources>(getDllProc(dll, "alGenSources"));

        if ((0 == dAlcOpenDevice) || (0 == dAlcCreateContext) || (0 == dAlcMakeContextCurrent)
            || (0 == dAlGetSourcei) || (0 == dAlSourceQueueBuffers) 
            || (0 == dAlSourceUnqueueBuffers) || (0 == dAlBufferData) || (0 == dAlSourcePlay)
            || (0 == dAlGenBuffers) || (0 == dAlGenSources))
        {
            return -2;
        }

		aSoloud->init(aSamplerate,aBuffer,aFlags);
		aSoloud->mBackendCleanupFunc = soloud_openal_deinit;
		aSoloud->mMutex = Thread::createMutex();
		aSoloud->mLockMutexFunc = openal_mutex_lock;
		aSoloud->mUnlockMutexFunc = openal_mutex_unlock;

		device = dAlcOpenDevice(NULL);
		context = dAlcCreateContext(device, NULL);
		dAlcMakeContextCurrent(context);
		format = AL_FORMAT_STEREO16;
		ALuint buffers[NUM_BUFFERS];
		dAlGenBuffers(NUM_BUFFERS, buffers);
		dAlGenSources(1, &source);

		frequency = aSamplerate;

		int i;
		short data[BUFFER_SIZE * 2];
		for (i = 0; i < BUFFER_SIZE*2; i++)
			data[i] = 0;
		for (i = 0; i < NUM_BUFFERS; i++)
		{
			dAlBufferData(buffers[i], format, data, BUFFER_SIZE, frequency);
			dAlSourceQueueBuffers(source, 1, &buffers[i]);
		}

		dAlSourcePlay(source);

		Thread::createThread(openal_thread, (void*)aSoloud);

		return 0;
	}	
};
