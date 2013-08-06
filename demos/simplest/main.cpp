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
#include "soloud_speech.h"

// Entry point
int main(int argc, char *argv[])
{
	// Init SDL (minimally, in this case))
	SDL_Init(0); 

	// Define a couple of variables
	SoLoud::Soloud soloud;  // SoLoud engine core
	SoLoud::Speech speech;  // A sound source (speech, in this case)

	// Configure sound source
	speech.setText("1 2 3   1 2 3   Hello world. Welcome to So-Loud.");

	// initialize SoLoud for SDL
	SoLoud::sdl_init(&soloud);

	// Play the sound source (we could do this several times if we wanted)
	soloud.play(speech);

	// Wait until sounds have finished
	while (soloud.getActiveVoiceCount())
	{
		// Still going, sleep for a bit
		SDL_Delay(100);
	}

	// Clean up SoLoud for SDL
	SoLoud::sdl_deinit(&soloud);

	// Clean up SDL
	SDL_Quit();

	// All done.
	return 0;
}