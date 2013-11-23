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
#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif
#include <math.h>

#include "soloud.h"

typedef int (*SDLOpenAudio)(SDL_AudioSpec *desired, SDL_AudioSpec *obtained);
typedef void (*SDLCloseAudio)();
typedef void (*SDLPauseAudio)(int pause_on);
typedef SDL_mutex* (*SDLCreateMutex)();
typedef void (*SDLDestroyMutex)(SDL_mutex *mutex);
typedef int (*SDLmutexP)(SDL_mutex *mutex);
typedef int (*SDLmutexV)(SDL_mutex *mutex);

static SDLOpenAudio dSDL_OpenAudio;
static SDLCloseAudio dSDL_CloseAudio;
static SDLPauseAudio dSDL_PauseAudio;
static SDLCreateMutex dSDL_CreateMutex;
static SDLDestroyMutex dSDL_DestroyMutex;
static SDLmutexP dSDL_mutexP;
static SDLmutexV dSDL_mutexV;

#ifdef WINDOWS_VERSION
#include <windows.h>

static HMODULE openDll()
{
    return LoadLibraryA("SDL.dll");
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
    return dlopen("SDL.so", RTLD_LAZY);
}

static void* getDllProc(HMODULE aLibrary, const char *aProcName)
{
    return dlsym(aLibrary, aProcName);
}

#endif

namespace SoLoud
{
	void soloud_sdl_audiomixer(void *userdata, Uint8 *stream, int len)
	{
		int samples = len / 4;
		short *buf = (short*)stream;
		SoLoud::Soloud *soloud = (SoLoud::Soloud *)userdata;
		float *mixdata = (float*)(soloud->mBackendData);
		soloud->mix(mixdata, samples);

		int i;
		for (i = 0; i < samples*2; i++)
		{
			buf[i] = (short)(mixdata[i] * 0x7fff);
		}
	}

	void soloud_sdl_lockmutex(void *aMutexPtr)
	{
		dSDL_mutexP((SDL_mutex*)aMutexPtr);
	}

	void soloud_sdl_unlockmutex(void *aMutexPtr)
	{
		dSDL_mutexV((SDL_mutex*)aMutexPtr);
	}

	static void soloud_sdl_deinit(SoLoud::Soloud *aSoloud)
	{
		dSDL_CloseAudio();
		delete[] (float*)aSoloud->mBackendData;
		dSDL_DestroyMutex((SDL_mutex*)aSoloud->mMutex);
	}

	int sdl_init(SoLoud::Soloud *aSoloud, int aFlags, int aSamplerate, int aBuffer)
	{
        HMODULE dll = openDll();
        if (0 == dll)
        {
            return -1;
        }

        dSDL_OpenAudio = static_cast<SDLOpenAudio>(getDllProc(dll, "SDL_OpenAudio"));
        dSDL_CloseAudio = static_cast<SDLCloseAudio>(getDllProc(dll, "SDL_CloseAudio"));
        dSDL_PauseAudio = static_cast<SDLPauseAudio>(getDllProc(dll, "SDL_PauseAudio"));
        dSDL_CreateMutex = static_cast<SDLCreateMutex>(getDllProc(dll, "SDL_CreateMutex"));
        dSDL_DestroyMutex = static_cast<SDLDestroyMutex>(getDllProc(dll, "SDL_DestroyMutex"));
        dSDL_mutexP = static_cast<SDLmutexP>(getDllProc(dll, "SDL_mutexP"));
        dSDL_mutexV = static_cast<SDLmutexV>(getDllProc(dll, "SDL_mutexV"));

        if ((0 == dSDL_OpenAudio) || (0 == dSDL_CloseAudio) || (0 == dSDL_PauseAudio)
            || (0 == dSDL_CreateMutex) || (0 == dSDL_DestroyMutex) 
            || (0 == dSDL_mutexP) || (0 == dSDL_mutexV))
        {
            return -2;
        }

		aSoloud->mMutex = dSDL_CreateMutex();
		SDL_AudioSpec as;
		as.freq = aSamplerate;
		as.format = AUDIO_S16;
		as.channels = 2;
		as.samples = aBuffer;
		as.callback = soloud_sdl_audiomixer;
		as.userdata = (void*)aSoloud;

		SDL_AudioSpec as2;
		if (dSDL_OpenAudio(&as, &as2) < 0)
		{
			return 1;
		}
		aSoloud->mBackendData = new float[as2.samples*4];

		aSoloud->init(as2.freq, as2.samples * 2, aFlags);

		aSoloud->mLockMutexFunc = soloud_sdl_lockmutex;
		aSoloud->mUnlockMutexFunc = soloud_sdl_unlockmutex;
		aSoloud->mBackendCleanupFunc = soloud_sdl_deinit;

		dSDL_PauseAudio(0);
		return 0;
	}
	
};
