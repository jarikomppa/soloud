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
#include <stdio.h>

#include "soloud.h"
#include "soloud_thread.h"

const char * getBackendEnumString(int aBackend)
{
	SOLOUD_ASSERT(SoLoud::Soloud::BACKEND_MAX == 17); // if this fails, this function needs adjustment
	switch (aBackend)
	{
	case SoLoud::Soloud::AUTO: return "AUTO";
	case SoLoud::Soloud::SDL1: return "SDL1";
	case SoLoud::Soloud::SDL2: return "SDL2";
	case SoLoud::Soloud::PORTAUDIO: return "PORTAUDIO";
	case SoLoud::Soloud::WINMM: return "WINMM";
	case SoLoud::Soloud::XAUDIO2: return "XAUDIO2";
	case SoLoud::Soloud::WASAPI: return "WASAPI";
	case SoLoud::Soloud::ALSA: return "ALSA";
	case SoLoud::Soloud::JACK: return "JACK";
	case SoLoud::Soloud::OSS: return "OSS";
	case SoLoud::Soloud::OPENAL: return "OPENAL";
	case SoLoud::Soloud::COREAUDIO: return "COREAUDIO";
	case SoLoud::Soloud::OPENSLES: return "OPENSLES";
	case SoLoud::Soloud::VITA_HOMEBREW: return "VITA_HOMEBREW";
	case SoLoud::Soloud::NULLDRIVER: return "NULLDRIVER";
	case SoLoud::Soloud::NOSOUND: return "NOSOUND";
	case SoLoud::Soloud::MINIAUDIO: return "MINIAUDIO";
	}
	return "?!";
}

const char * getChannelString(int aChannels)
{
	switch (aChannels)
	{
	case 8:
		return " (7.1 surround)";
	case 6:
		return " (5.1 surround)";
	case 4:
		return " (quad)";
	case 2:
		return " (stereo)";
	case 1:
		return " (mono)";
	}
	return " (?!)";
}

int main(int argc, char *argv[])
{
	SoLoud::Soloud soloud;

	int i;
	for (i = 0; i < SoLoud::Soloud::BACKEND_MAX; i++)
	{
		printf("-----\n"
			"Backend %d:%s\n",
			i, getBackendEnumString(i));
		int res = soloud.init(0, i);
		if (res == SoLoud::SO_NO_ERROR)
		{
			printf(
				"ID:       %d\n"
				"String:   \"%s\"\n"
				"Rate:     %d\n"
				"Buffer:   %d\n"
				"Channels: %d%s (default)\n",
				soloud.getBackendId(),
				soloud.getBackendString() == 0 ? "(null)" : soloud.getBackendString(),
				soloud.getBackendSamplerate(),
				soloud.getBackendBufferSize(),
				soloud.getBackendChannels(),
				getChannelString(soloud.getBackendChannels()));
			soloud.deinit();
			int j;
			for (j = 1; j < 12; j++)
			{
				int res = soloud.init(0, i, 0, 0, j);
				if (res == SoLoud::SO_NO_ERROR && soloud.getBackendChannels() == j)
				{
					printf("Channels: %d%s\n", soloud.getBackendChannels(), getChannelString(soloud.getBackendChannels()));
					soloud.deinit();					
					SoLoud::Thread::sleep(200);
				}
			}
		}
		else
		{
			printf("Failed: %s\n", soloud.getErrorString(res));
		}
	}
	return 0;
}
