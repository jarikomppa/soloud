/*
SoLoud audio engine
Copyright (c) 2013-2015 Jari Komppa

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

#include "soloud.h"

#if !defined(WITH_SDL2_NONDYN)

namespace SoLoud
{
	result sdl2nondyn_init(SoLoud::Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer)
	{
		return NOT_IMPLEMENTED;
	}
}

#else

#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif
#include <math.h>


namespace SoLoud
{
	void soloud_sdl2nondyn_audiomixer(void *userdata, Uint8 *stream, int len)
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

	void soloud_sdl2nondyn_lockmutex(void *aMutexPtr)
	{
		SDL_LockMutex((SDL_mutex*)aMutexPtr);	
	}

	void soloud_sdl2nondyn_unlockmutex(void *aMutexPtr)
	{
		SDL_UnlockMutex((SDL_mutex*)aMutexPtr);
	}

	static void soloud_sdl2nondyn_deinit(SoLoud::Soloud *aSoloud)
	{
		SDL_CloseAudio();
		delete[] (float*)aSoloud->mBackendData;
		SDL_DestroyMutex((SDL_mutex*)aSoloud->mMutex);
	}

	result sdl2nondyn_init(SoLoud::Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer)
	{
		aSoloud->mMutex = SDL_CreateMutex();
		SDL_AudioSpec as;
		as.freq = aSamplerate;
		as.format = AUDIO_S16;
		as.channels = 2;
		as.samples = aBuffer;
		as.callback = soloud_sdl2nondyn_audiomixer;
		as.userdata = (void*)aSoloud;

		SDL_AudioSpec as2;
		if (SDL_OpenAudio(&as, &as2) < 0)
		{
			return UNKNOWN_ERROR;
		}
		aSoloud->mBackendData = new float[as2.samples*4];

		aSoloud->postinit(as2.freq, as2.samples * 2, aFlags);

		aSoloud->mLockMutexFunc = soloud_sdl2nondyn_lockmutex;
		aSoloud->mUnlockMutexFunc = soloud_sdl2nondyn_unlockmutex;
		aSoloud->mBackendCleanupFunc = soloud_sdl2nondyn_deinit;

		SDL_PauseAudio(0);
        aSoloud->mBackendString = "SDL2 (static)";
		return 0;
	}
	
};
#endif