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

#ifndef SOLOUD_BACKEND_DATA_SDL3_H
#define SOLOUD_BACKEND_DATA_SDL3_H

#ifdef WITH_SDL3
#include "SDL3/SDL.h"

namespace SoLoud
{
	struct SoLoudBackendDataSdl3
	{
		SoLoudBackendDataSdl3()
			: audioStream{NULL}
			, audioDeviceId{0}
			, activeAudioSpec{}
		{}
		SDL_AudioStream * audioStream;
		SDL_AudioDeviceID audioDeviceId;
		SDL_AudioSpec activeAudioSpec;
	};

	struct SoLoudClientDataSdl3
	{
		SoLoudClientDataSdl3(unsigned int deviceId)
			: deviceId(deviceId)
		{}
		unsigned int deviceId;
	};
}
#endif

#endif
