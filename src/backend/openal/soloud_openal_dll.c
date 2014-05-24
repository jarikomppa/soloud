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
#include <stdlib.h>
#include <math.h>

#ifdef __APPLE__
#include "OpenAL/al.h"
#include "OpenAL/alc.h"
#else
#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"
#endif

#if defined(_MSC_VER)
#define WINDOWS_VERSION
#endif

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

static int load_dll()
{
#ifdef WINDOWS_VERSION
	HMODULE dll = NULL;
#else
	void * dll = NULL;
#endif

	if (dAlcOpenDevice != NULL)
	{
		return 1;
	}

    dll = openDll();

    if (dll)
    {
        dAlcOpenDevice = (alc_OpenDevice)getDllProc(dll, "alcOpenDevice");
        dAlcCreateContext = (alc_CreateContext)getDllProc(dll, "alcCreateContext");
        dAlcMakeContextCurrent = (alc_MakeContextCurrent)getDllProc(dll, "alcMakeContextCurrent");
        dAlGetSourcei = (al_GetSourcei)getDllProc(dll, "alGetSourcei");
        dAlSourceQueueBuffers = (al_SourceQueueBuffers)getDllProc(dll, "alSourceQueueBuffers");
        dAlSourceUnqueueBuffers = (al_SourceUnqueueBuffers)getDllProc(dll, "alSourceUnqueueBuffers");
        dAlBufferData = (al_BufferData)getDllProc(dll, "alBufferData");
        dAlSourcePlay = (al_SourcePlay)getDllProc(dll, "alSourcePlay");
        dAlGenBuffers = (al_GenBuffers)getDllProc(dll, "alGenBuffers");
        dAlGenSources = (al_GenSources)getDllProc(dll, "alGenSources");

        if (dAlcOpenDevice &&
			dAlcCreateContext &&
			dAlcMakeContextCurrent &&
            dAlGetSourcei &&
			dAlSourceQueueBuffers &&
            dAlSourceUnqueueBuffers &&
			dAlBufferData && 
			dAlSourcePlay &&
            dAlGenBuffers &&
			dAlGenSources)
        {
            return 1;
        }
	}
	dAlcOpenDevice = 0;
	return 0;
}

int dll_al_found()
{
	return load_dll();
}

ALCdevice* dll_alc_OpenDevice(const ALCchar *devicename)
{
	if (load_dll())
		return dAlcOpenDevice(devicename);
	return NULL;
}

ALCcontext* dll_alc_CreateContext(ALCdevice *device, const ALCint* attrlist)
{
	if (load_dll())
		return dAlcCreateContext(device, attrlist);
	return NULL;
}

ALCboolean dll_alc_MakeContextCurrent(ALCcontext *context)
{
	if (load_dll())
		return dAlcMakeContextCurrent(context);
	return 0;
}

void dll_al_GetSourcei(ALuint source, ALenum param, ALint *value)
{
	if (load_dll())
		dAlGetSourcei(source, param, value);
}

void dll_al_SourceQueueBuffers(ALuint source, ALsizei nb, const ALuint *buffers)
{
	if (load_dll())
		dAlSourceQueueBuffers(source, nb, buffers);
}

void dll_al_SourceUnqueueBuffers(ALuint source, ALsizei nb, ALuint *buffers)
{
	if (load_dll())
		dAlSourceUnqueueBuffers(source, nb, buffers);
}

void dll_al_BufferData(ALuint buffer, ALenum format, const ALvoid *data, ALsizei size, ALsizei freq)
{
	if (load_dll())
		dAlBufferData(buffer, format, data, size, freq);
}

void dll_al_SourcePlay(ALuint source)
{
	if (load_dll())
		dAlSourcePlay(source);
}

void dll_al_GenBuffers(ALsizei n, ALuint *buffers)
{
	if (load_dll())
		dAlGenBuffers(n, buffers);
}

void dll_al_GenSources(ALsizei n, ALuint *sources)
{
	if (load_dll())
		dAlGenSources(n, sources);
}
