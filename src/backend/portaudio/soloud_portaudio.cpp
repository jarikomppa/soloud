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

typedef PaError (*Pa_InitializeProc)( void );
typedef PaError (*Pa_TerminateProc)( void );
typedef PaError (*Pa_CloseStreamProc)( PaStream *stream );
typedef PaError (*Pa_StartStreamProc)( PaStream *stream );
typedef PaError (*Pa_OpenDefaultStreamProc)( PaStream** stream,
                              int numInputChannels,
                              int numOutputChannels,
                              PaSampleFormat sampleFormat,
                              double sampleRate,
                              unsigned long framesPerBuffer,
                              PaStreamCallback *streamCallback,
                              void *userData );

static Pa_InitializeProc dPa_Initialize;
static Pa_TerminateProc dPa_Terminate;
static Pa_CloseStreamProc dPa_CloseStream;
static Pa_StartStreamProc dPa_StartStream;
static Pa_OpenDefaultStreamProc dPa_OpenDefaultStream;



#ifdef WINDOWS_VERSION
#include <Windows.h>

static int opendll(const char *dllfilename)
{
    HMODULE dllh = LoadLibrary(dllfilename);
    return (int)dllh;
}

static void *getdllproc(int dllhandle, const char *procname)
{
    HMODULE dllh = (HMODULE)dllhandle;
    return GetProcAddress(dllh, procname);
}

#else
#include <dlfcn.h> // dll functions

static void* opendll(const char *dllfilename)
{
    void* library = dlopen(dllfilename, RTLD_LAZY);
    return library;
}

static void *getdllproc(void* dllhandle, const char *procname)
{
    void* library = dllhandle;
    return dlsym(library,procname);
}

#endif


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
		dPa_CloseStream(gStream);
		dPa_Terminate();
		Thread::destroyMutex(aSoloud->mMutex);
		aSoloud->mMutex = 0;
		aSoloud->mLockMutexFunc = 0;
		aSoloud->mUnlockMutexFunc = 0;
	}

	int portaudio_init(SoLoud::Soloud *aSoloud, int aFlags, int aSamplerate, int aBuffer)
	{
#ifdef WINDOWS_VERSION
		int h = opendll("portaudio_x86.dll");
#else
		void *h = opendll("libportaudio_x86.so");
#endif

		dPa_Initialize = (Pa_InitializeProc)getdllproc(h,"Pa_Initialize");
		dPa_Terminate = (Pa_TerminateProc)getdllproc(h,"Pa_Terminate");
		dPa_CloseStream = (Pa_CloseStreamProc)getdllproc(h,"Pa_CloseStream");
		dPa_StartStream = (Pa_StartStreamProc)getdllproc(h,"Pa_StartStream");
		dPa_OpenDefaultStream = (Pa_OpenDefaultStreamProc)getdllproc(h,"Pa_OpenDefaultStream");

		if (dPa_Initialize == NULL ||
			dPa_Terminate == NULL ||
			dPa_CloseStream == NULL ||
			dPa_StartStream == NULL ||
			dPa_OpenDefaultStream == NULL)
			return -1;

		aSoloud->init(aSamplerate, aBuffer * 2, aFlags);
		aSoloud->mBackendCleanupFunc = soloud_portaudio_deinit;
		aSoloud->mMutex = Thread::createMutex();
		aSoloud->mLockMutexFunc = portaudio_mutex_lock;
		aSoloud->mUnlockMutexFunc = portaudio_mutex_unlock;
		dPa_Initialize();
		dPa_OpenDefaultStream(&gStream, 0, 2, paFloat32, aSamplerate, paFramesPerBufferUnspecified, portaudio_callback, (void*)aSoloud);
		dPa_StartStream(gStream);
		return 0;
	}
	
};
