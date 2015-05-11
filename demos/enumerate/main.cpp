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

int main(int argc, char *argv[])
{
	SoLoud::Soloud soloud;

	int i;
	for (i = 0; i < SoLoud::Soloud::BACKEND_MAX; i++)
	{
		printf("-----\n"
			"Backend %d:%s\n",
			i, i==0?"(auto)":"");
		int res = soloud.init(0, i);
		if (res == SoLoud::SO_NO_ERROR)
		{
			printf(
				"ID:       %d\n"
				"String:   \"%s\"\n"
				"Rate:     %d\n"
				"Channels: %d%s\n"
				"Buffer:   %d\n",
				soloud.getBackendId(),
				soloud.getBackendString() == 0 ? "(null)" : soloud.getBackendString(),
				soloud.getBackendSamplerate(),
				soloud.getBackendChannels(),
				(soloud.getBackendChannels() == 2 ? " (stereo)" : soloud.getBackendChannels() == 1 ? " (mono)" : ""),
				soloud.getBackendBufferSize());
			soloud.deinit();
		}
		else
		{
			printf("Failed: %s\n", soloud.getErrorString(res));
		}
	}
	return 0;
}