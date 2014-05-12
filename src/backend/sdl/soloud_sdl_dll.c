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
#define WINDOWS_VERSION
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif
#include <math.h>

typedef int (*SDLOpenAudio)(SDL_AudioSpec *desired, SDL_AudioSpec *obtained);
typedef void (*SDLCloseAudio)();
typedef void (*SDLPauseAudio)(int pause_on);
typedef SDL_mutex* (*SDLCreateMutex)();
typedef void (*SDLDestroyMutex)(SDL_mutex *mutex);
typedef int (*SDLmutexP)(SDL_mutex *mutex);
typedef int (*SDLmutexV)(SDL_mutex *mutex);

static SDLOpenAudio dSDL_OpenAudio = NULL;
static SDLCloseAudio dSDL_CloseAudio = NULL;
static SDLPauseAudio dSDL_PauseAudio = NULL;
static SDLCreateMutex dSDL_CreateMutex = NULL;
static SDLDestroyMutex dSDL_DestroyMutex = NULL;
static SDLmutexP dSDL_mutexP = NULL;
static SDLmutexV dSDL_mutexV = NULL;

#ifdef WINDOWS_VERSION
#include <windows.h>

static HMODULE openDll()
{
	HMODULE res = LoadLibraryA("SDL2.dll");
	if (!res) res = LoadLibraryA("SDL.dll");
    return res;
}

static void* getDllProc(HMODULE aDllHandle, const char *aProcName)
{
    return GetProcAddress(aDllHandle, aProcName);
}

#else
#include <dlfcn.h> // dll functions

static void * openDll()
{
	void * res;
	res = dlopen("/Library/Frameworks/SDL2.framework/SDL2", RTLD_LAZY);
	if (!res) res = dlopen("/Library/Frameworks/SDL.framework/SDL", RTLD_LAZY);
	if (!res) res = dlopen("SDL2.so", RTLD_LAZY);
	if (!res) res = dlopen("SDL.so", RTLD_LAZY);
    return res;
}

static void* getDllProc(void * aLibrary, const char *aProcName)
{
    return dlsym(aLibrary, aProcName);
}

#endif

static int load_dll()
{
#ifdef WINDOWS_VERSION
	HMODULE dll = NULL;
#else
	void * dll = NULL;
#endif

	if (dSDL_OpenAudio != NULL)
	{
		return 1;
	}

    dll = openDll();

    if (dll)
    {
	    dSDL_OpenAudio = (SDLOpenAudio)getDllProc(dll, "SDL_OpenAudio");
	    dSDL_CloseAudio = (SDLCloseAudio)getDllProc(dll, "SDL_CloseAudio");
	    dSDL_PauseAudio = (SDLPauseAudio)getDllProc(dll, "SDL_PauseAudio");
	    dSDL_CreateMutex = (SDLCreateMutex)getDllProc(dll, "SDL_CreateMutex");
	    dSDL_DestroyMutex = (SDLDestroyMutex)getDllProc(dll, "SDL_DestroyMutex");
	    dSDL_mutexP = (SDLmutexP)getDllProc(dll, "SDL_mutexP");
	    dSDL_mutexV = (SDLmutexV)getDllProc(dll, "SDL_mutexV");

        if (dSDL_OpenAudio && 
        	dSDL_CloseAudio &&
        	dSDL_PauseAudio &&
            dSDL_CreateMutex &&
            dSDL_DestroyMutex &&
            dSDL_mutexP &&
            dSDL_mutexV)
        {
        	return 1;
        }
	}
	dSDL_OpenAudio = NULL;
    return 0;
}

int dll_SDL_found()
{
	return load_dll();
}

int dll_SDL_OpenAudio(SDL_AudioSpec *desired, SDL_AudioSpec *obtained)
{
	if (load_dll())
		return dSDL_OpenAudio(desired, obtained);
	return 0;
}

void dll_SDL_CloseAudio()
{
	if (load_dll())
		dSDL_CloseAudio();
}

void dll_SDL_PauseAudio(int pause_on)
{
	if (load_dll())
		dSDL_PauseAudio(pause_on);
}

SDL_mutex * dll_SDL_CreateMutex()
{
	if (load_dll())
		return dSDL_CreateMutex();
	return NULL;
}

void dll_SDL_DestroyMutex(SDL_mutex * mutex)
{
	if (load_dll())
		dSDL_DestroyMutex(mutex);
}

int dll_SDL_mutexP(SDL_mutex * mutex)
{
	if (load_dll())
		return dSDL_mutexP(mutex);
	return 0;
}

int dll_SDL_mutexV(SDL_mutex * mutex)
{
	if (load_dll())
		return dSDL_mutexV(mutex);
	return 0;
}
