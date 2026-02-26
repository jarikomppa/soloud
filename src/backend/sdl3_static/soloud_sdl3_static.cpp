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
	result sdl3static_init(SoLoud::Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer, void const* clientData)
	{
		return NOT_IMPLEMENTED;
	}
}

#else

#include "soloud_backend_data_sdl3.h"
#include "SDL.h"
#include <math.h>

namespace SoLoud
{
    static SoLoudBackendDataSdl3 gBackendData{};

    void soloud_sdl2static_audiomixer(Uint8* stream, void* userdata, int len)
    {
        SoLoud::Soloud* soloud = (SoLoud::Soloud*)userdata;
        if (gBackendData.activeAudioSpec.format == SDL_AUDIO_F32)
        {
            int samples = len / (gBackendData.activeAudioSpec.channels * sizeof(float));
            soloud->mix((float*)stream, samples);
        }
        else // assume s16 if not float
        {
            int samples = len / (gBackendData.activeAudioSpec.channels * sizeof(short));
            soloud->mixSigned16((short*)stream, samples);
        }
    }

    void soloud_sdl3static_audiomixer(void* userdata, SDL_AudioStream* stream, int additionalAmount, int totalAmount)
    {
        SoLoud::Soloud* soloud = (SoLoud::Soloud*)userdata;

        if (additionalAmount > 0)
        {
            Uint8* data = SDL_stack_alloc(Uint8, additionalAmount);
            if (data)
            {
                soloud_sdl2static_audiomixer(
                    data,
                    userdata,
                    additionalAmount
                );
                SDL_PutAudioStreamData(stream, data, additionalAmount);
                SDL_stack_free(data);
            }
        }
    }

	static void soloud_sdl3static_deinit(SoLoud::Soloud *aSoloud)
	{
		SDL_CloseAudioDevice(gBackendData.audioDeviceId);
	}

	result sdl3static_init(SoLoud::Soloud *aSoloud, unsigned int aFlags, unsigned int aSamplerate, unsigned int aBuffer, unsigned int aChannels, void const* clientData)
	{
		if (!SDL_WasInit(SDL_INIT_AUDIO))
		{
			if (!SDL_InitSubSystem(SDL_INIT_AUDIO))
			{
				return UNKNOWN_ERROR;
			}
		}

        SoLoudClientDataSdl3 const* data = NULL;
        if (clientData != NULL)
        {
            data = static_cast<SoLoudClientDataSdl3 const*>(clientData);
            gBackendData.audioDeviceId = data->deviceId;
        }
        else
        {
            gBackendData.audioDeviceId = SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK;
        }

        SDL_AudioDeviceID const deviceId = gBackendData.audioDeviceId;
        SDL_AudioSpec as{};
        as.freq = aSamplerate;
        as.format = SDL_AUDIO_F32;
        as.channels = (Uint8)aChannels;

        gBackendData.audioStream = SDL_OpenAudioDeviceStream(
            gBackendData.audioDeviceId,
            &as,
            soloud_sdl3static_audiomixer,
            static_cast<void*>(aSoloud)
        );

        gBackendData.audioDeviceId = SDL_GetAudioStreamDevice(gBackendData.audioStream);

        if (gBackendData.audioDeviceId == 0)
        {
            as.format = SDL_AUDIO_S16;
            gBackendData.audioStream = SDL_OpenAudioDeviceStream(
                deviceId,
                NULL,
                soloud_sdl3static_audiomixer,
                static_cast<void*>(aSoloud)
            );

            gBackendData.audioDeviceId = SDL_GetAudioStreamDevice(gBackendData.audioStream);

            if (gBackendData.audioDeviceId == 0)
            {
                return UNKNOWN_ERROR;
            }
        }

        SDL_GetAudioDeviceFormat(
            gBackendData.audioDeviceId,
            &gBackendData.activeAudioSpec,
            NULL
        );

        aSoloud->postinit_internal(
            gBackendData.activeAudioSpec.freq,
            aBuffer,
            aFlags,
            gBackendData.activeAudioSpec.channels
        );

        aSoloud->mBackendCleanupFunc = soloud_sdl3static_deinit;

        //SDL_PauseAudioStreamDevice(gBackendData.audioStream);
        aSoloud->mBackendData = &gBackendData;
        aSoloud->mBackendString = "SDL3 (dynamic)";

        SDL_ResumeAudioDevice(SDL_GetAudioStreamDevice(gBackendData.audioStream));

        return 0;
	}	
};
#endif