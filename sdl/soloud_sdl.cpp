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

namespace SoLoud
{
	SDL_mutex *gSoloudMutex;

	void soloud_sdl_audiomixer(void *userdata, Uint8 *stream, int len)
	{
		int samples = len / 4;
		short *buf = (short*)stream;
		SoLoud::Soloud *soloud = (SoLoud::Soloud *)userdata;
		float *mixdata = (float*)(soloud->mMixerData);
		soloud->mix(mixdata, samples);

		int i;
		for (i = 0; i < samples*2; i++)
		{
			buf[i] = (short)(mixdata[i] * 0x7fff);
		}
	}

	void soloud_sdl_lockmutex()
	{
		SDL_mutexP(gSoloudMutex);
	}

	void soloud_sdl_unlockmutex()
	{
		SDL_mutexV(gSoloudMutex);
	}

	int sdl_init(SoLoud::Soloud * aSoloud)
	{
		gSoloudMutex = SDL_CreateMutex();
		SDL_AudioSpec as;
		as.freq = 44100;
		as.format = AUDIO_S16;
		as.channels = 2;
		as.samples = 2048;
		as.callback = soloud_sdl_audiomixer;
		as.userdata = (void*)aSoloud;

		SDL_AudioSpec as2;
		if (SDL_OpenAudio(&as, &as2) < 0)
		{
			return 1;
		}
		aSoloud->mMixerData = new float[as2.samples*4];

		aSoloud->lockMutex = soloud_sdl_lockmutex;
		aSoloud->unlockMutex = soloud_sdl_unlockmutex;

		SDL_PauseAudio(0);
		return 0;
	}
	
	void sdl_deinit(SoLoud::Soloud *aSoloud)
	{
		delete[] (float*)aSoloud->mMixerData;
		SDL_CloseAudio();
		SDL_DestroyMutex(gSoloudMutex);
	}
};
