/*
SoLoud audio engine
Copyright (c) 2013-2014 Jari Komppa

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
#include "soloud_wav.h"
#include "soloud_wavstream.h"


SoLoud::Soloud gSoloud;
SoLoud::Wav gSfx;
SoLoud::WavStream gMusic1, gMusic2;
int gMusichandle1, gMusichandle2;

SDL_Surface *screen;

void putpixel(int x, int y, int color)
{
	if (y < 0 || y > 255 || x < 0 || x > 400) 
		return;
	unsigned int *ptr = (unsigned int*)screen->pixels;
	int lineoffset = y * (screen->pitch / 4);
	ptr[lineoffset + x] = color;
}


void render()
{   
	// Lock surface if needed
	if (SDL_MUSTLOCK(screen))
		if (SDL_LockSurface(screen) < 0) 
			return;

	// Ask SDL for the time in milliseconds
	int tick = SDL_GetTicks();

	float *buf = gSoloud.getWave();
	float *fft = gSoloud.calcFFT();


	if (buf && fft)
	{
		int i, j;
		for (i = 0; i < 256; i++)
			for (j = 0; j < 400; j++)
				putpixel(j, i, 0);

		int last = 0;
		for (i = 0; i < 256; i++)
		{			
			int v = 256-(int)floor(fft[i] * 127);
			for (j = v; j < 256; j++)
			{
				putpixel(i,j,0x0000ff);
			}

			v = (int)floor(buf[i] * 127 + 128);
			for (j = 0; j < 6; j++)
			{
				putpixel(i,j+v,0xff0000);
			}

			putpixel(200 + v,128+(v-last),0x00ff00);
			last = v;
		}
	}

	// Unlock if needed
	if (SDL_MUSTLOCK(screen)) 
		SDL_UnlockSurface(screen);

	// Tell SDL to update the whole screen
	SDL_UpdateRect(screen, 0, 0, 400, 256);    
}


// Entry point
int main(int argc, char *argv[])
{
	gSfx.load("audio/hit.ogg");
	gMusic1.load("audio/algebra_loop.ogg");
	gMusic2.load("audio/delphi_loop.ogg");

	gMusic1.setLooping(1);
	gMusic2.setLooping(1);

	// Initialize SDL's subsystems - in this case, only video.
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) 
	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	gSoloud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);

	// Register SDL_Quit to be called at exit; makes sure things are
	// cleaned up when we quit.
	atexit(SDL_Quit);	

	// Attempt to create a 640x480 window with 32bit pixels.
	screen = SDL_SetVideoMode(400, 256, 32, SDL_SWSURFACE);

	// If we fail, return error.
	if ( screen == NULL ) 
	{
		fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
		exit(1);
	}

	gMusichandle1 = gSoloud.play(gMusic1,0,0,1);
	gMusichandle2 = gSoloud.play(gMusic2);
	gSoloud.setProtectVoice(gMusichandle1, 1);
	gSoloud.setProtectVoice(gMusichandle2, 1);

	// Main loop: loop forever.
	while (1)
	{
		// Render stuff
		render();
		int h;
		// Poll for events, and handle the ones we care about.
		SDL_Event event;
		while (SDL_PollEvent(&event)) 
		{
			switch (event.type) 
			{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_1: 
					h = gSoloud.play(gSfx, 1, ((rand()%512)-256)/256.0f);
					gSoloud.setRelativePlaySpeed(h, (rand()%512)/256.0f + 0.1f);
					gSoloud.fadePan(h, 0, 0.5f);
					break;
				case SDLK_2:
					gSoloud.setPause(gMusichandle1, 0);
					gSoloud.fadeVolume(gMusichandle1, 1, 2);
					gSoloud.fadeVolume(gMusichandle2, 0, 2);
					gSoloud.schedulePause(gMusichandle2, 2);
					break;
				case SDLK_3:
					gSoloud.setPause(gMusichandle2, 0);
					gSoloud.fadeVolume(gMusichandle2, 1, 2);
					gSoloud.fadeVolume(gMusichandle1, 0, 2);
					gSoloud.schedulePause(gMusichandle1, 2);
					break;
				case SDLK_4:
					gSoloud.fadeRelativePlaySpeed(gMusichandle1, 0.2f, 5);
					gSoloud.fadeRelativePlaySpeed(gMusichandle2, 0.2f, 5);
					break;
				case SDLK_5:
					gSoloud.fadeRelativePlaySpeed(gMusichandle1, 1, 5);
					gSoloud.fadeRelativePlaySpeed(gMusichandle2, 1, 5);
					break;
				}
				break;
			case SDL_KEYUP:
				// If escape is pressed, return (and thus, quit)
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					gSoloud.deinit();
					return 0;
				}
				break;
			case SDL_QUIT:
				gSoloud.deinit();

				return(0);
			}
		}
	}
	return 0;
}