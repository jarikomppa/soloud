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

#include "SDL3/SDL.h"
#include <math.h>

namespace SoLoud
{
	static SDL_AudioSpec gActiveAudioSpec;
	static SDL_AudioDeviceID gAudioDeviceID;
	static SDL_AudioStream *gAudioStream;

	void soloud_sdl3static_audiocallback(void *userdata, SDL_AudioStream *stream, int additional_amount, int total_amount)
	{
		SoLoud::Soloud *soloud = (SoLoud::Soloud *)userdata;
		
		// Calculate how many samples we need to provide
		int samples_needed = additional_amount / (gActiveAudioSpec.channels * sizeof(float));
		
		if (samples_needed > 0)
		{
			// Allocate temporary buffer for the samples
			float *buffer = new float[samples_needed * gActiveAudioSpec.channels];
			
			// Get the mixed audio from SoLoud
			if (gActiveAudioSpec.format == SDL_AUDIO_F32)
			{
				soloud->mix(buffer, samples_needed);
			}
			else // assume s16 if not float, but convert to float for SDL3
			{
				short *temp_buffer = new short[samples_needed * gActiveAudioSpec.channels];
				soloud->mixSigned16(temp_buffer, samples_needed);
				
				// Convert from signed 16-bit to float
				for (int i = 0; i < samples_needed * gActiveAudioSpec.channels; i++)
				{
					buffer[i] = temp_buffer[i] / 32768.0f;
				}
				delete[] temp_buffer;
			}
			
			// Put the data into the audio stream
			SDL_PutAudioStreamData(stream, buffer, samples_needed * gActiveAudioSpec.channels * sizeof(float));
			
			delete[] buffer;
		}
	}

	static void soloud_sdl3static_deinit(SoLoud::Soloud *aSoloud)
	{
		if (gAudioStream)
		{
			SDL_DestroyAudioStream(gAudioStream);
			gAudioStream = nullptr;
		}
		if (gAudioDeviceID)
		{
			SDL_CloseAudioDevice(gAudioDeviceID);
			gAudioDeviceID = 0;
		}
	}

	result sdl3static_init(SoLoud::Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer, unsigned int aChannels)
	{
		if (!SDL_WasInit(SDL_INIT_AUDIO))
		{
			if (!SDL_InitSubSystem(SDL_INIT_AUDIO))
			{
				return UNKNOWN_ERROR;
			}
		}

		// Set up the desired audio specification
		SDL_AudioSpec desired_spec;
		desired_spec.freq = aSamplerate;
		desired_spec.format = SDL_AUDIO_F32; // SDL3 prefers float format
		desired_spec.channels = aChannels;

		// Open the audio device using the simplified SDL3 API
		gAudioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &desired_spec, soloud_sdl3static_audiocallback, (void*)aSoloud);
		
		if (!gAudioStream)
		{
			// Try with 16-bit signed format if float fails
			desired_spec.format = SDL_AUDIO_S16;
			gAudioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &desired_spec, soloud_sdl3static_audiocallback, (void*)aSoloud);
			
			if (!gAudioStream)
			{
				return UNKNOWN_ERROR;
			}
		}

		// Get the actual audio spec from the stream
		SDL_AudioSpec src_spec, dst_spec;
		if (!SDL_GetAudioStreamFormat(gAudioStream, &src_spec, &dst_spec))
		{
			soloud_sdl3static_deinit(aSoloud);
			return UNKNOWN_ERROR;
		}

		// Store the active audio spec for use in the callback
		gActiveAudioSpec = dst_spec;
		
		// Get the device ID from the stream
		gAudioDeviceID = SDL_GetAudioStreamDevice(gAudioStream);

		// Initialize SoLoud with the actual audio parameters
		aSoloud->postinit_internal(gActiveAudioSpec.freq, aBuffer, aFlags, gActiveAudioSpec.channels);

		aSoloud->mBackendCleanupFunc = soloud_sdl3static_deinit;

		// Resume the audio stream (SDL3 opens devices in paused state)
		SDL_ResumeAudioStreamDevice(gAudioStream);
		
		aSoloud->mBackendString = "SDL3 (static)";
		return 0;
	}	
};
#endif 