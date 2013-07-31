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

	SDL_Init(0);

	SoLoud::Soloud soloud;
	SoLoud::Speech speech;

	speech.setText("Hello world. You will be assimilated.");

	SoLoud::sdl_init(&soloud);

	soloud.play(speech);

	while (soloud.getActiveVoiceCount())
	{
		SDL_Delay(100);
	}

	SoLoud::sdl_deinit(&soloud);
	SDL_Quit();
	return 0;
}