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

#if !defined(WITH_SDL3_STATIC)

namespace SoLoud
{
	result sdl3static_init(SoLoud::Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer)
	{
		return NOT_IMPLEMENTED;
	}
}

#else

#include <SDL3/SDL.h>
#include <math.h>

namespace SoLoud
{
	static unsigned int gSamples;
	static SDL_AudioSpec gActiveAudioSpec;
	static SDL_AudioStream *gAudioStream;

	void soloud_sdl3static_audiomixer(void *userdata, SDL_AudioStream* stream, int additional_amount, int total_amount)
	{
		if (additional_amount > 0) {
			Uint8 *data = SDL_stack_alloc(Uint8, additional_amount);
			if (data) {
				SoLoud::Soloud *soloud = (SoLoud::Soloud *)userdata;

				int samples = additional_amount / (gActiveAudioSpec.channels * sizeof(float));
				// TODO: in practice it seems like it's safe to ignore the case where samples > gSamples
				// because the scratch buffers are always so big, but is this really safe?
				soloud->mix((float*)data, samples);

				SDL_PutAudioStreamData(stream, data, additional_amount);
				SDL_stack_free(data);
			}
		}
	}

	static void soloud_sdl3static_deinit(SoLoud::Soloud *aSoloud)
	{
		SDL_DestroyAudioStream(gAudioStream);
	}

	result sdl3static_init(SoLoud::Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer, unsigned int aChannels)
	{
		if (!SDL_WasInit(SDL_INIT_AUDIO))
		{
			if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
			{
				return UNKNOWN_ERROR;
			}
		}

		gSamples = aBuffer;

		gActiveAudioSpec = { SDL_AUDIO_F32LE, (int)aChannels, (int)aSamplerate };

		gAudioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_OUTPUT, &gActiveAudioSpec, soloud_sdl3static_audiomixer, (void*)aSoloud);
		if (!gAudioStream)
		{
			return UNKNOWN_ERROR;
		}

		aSoloud->postinit_internal(gActiveAudioSpec.freq, gSamples, aFlags, gActiveAudioSpec.channels);

		aSoloud->mBackendCleanupFunc = soloud_sdl3static_deinit;

		SDL_ResumeAudioDevice(SDL_GetAudioStreamDevice(gAudioStream));
		aSoloud->mBackendString = "SDL3 (static)";
		return 0;
	}
};
#endif
