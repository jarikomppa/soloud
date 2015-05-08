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
#include "soloud_speech.h"

// Entry point
int main(int argc, char *argv[])
{
	// Define a couple of variables
	SoLoud::Soloud soloud;  // SoLoud engine core
	SoLoud::Speech speech;  // A sound source (speech, in this case)

	// Configure sound source
	speech.setText("Hello!");

	// initialize SoLoud.
	soloud.init(SoLoud::Soloud::CLIP_ROUNDOFF, SoLoud::Soloud::NULLDRIVER);

	soloud.setGlobalVolume(10);

	// Play the sound source (we could do this several times if we wanted)
	soloud.play(speech);

	// Wait until sounds have finished
	while (soloud.getActiveVoiceCount() > 0)
	{
		float buf[256 * 2];
		soloud.mix(buf, 256);
		int i;
		for (i = 0; i < 256; i++)
		{
			int d = (int)(buf[i * 2 + 0] * 30 + 30);
			int j;
			putchar('|');
			for (j = 0; j < 60; j++)
				if (j == d)
					putchar('o');
				else
				if ((d < 30 && j < 30 && j > d) || (d >= 30 && j >= 30 && j < d))
					putchar('-');
				else
					putchar(' ');
			putchar('|');
			putchar('\n');
		}
	}

	// Clean up SoLoud
	soloud.deinit();

	// All done.
	return 0;
}