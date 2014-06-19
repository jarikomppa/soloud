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
#include "soloud_sfxr.h"


SoLoud::Soloud gSoloud;
SoLoud::Sfxr gSfx;

SDL_Surface *screen;

void putpixel(int x, int y, int color)
{
	if (y < 0 || y > 255 || x < 0 || x > 400) 
		return;
	unsigned int *ptr = (unsigned int*)screen->pixels;
	int lineoffset = y * (screen->pitch / 4);
	ptr[lineoffset + x] = color;
}

int fire1 = 0;
int fire2 = 0;
int fire3 = 0;

int lasttick = 0;

int bullet[1024] = { 0 };
int bulletidx = 0;

void render()
{   
	// Lock surface if needed
	if (SDL_MUSTLOCK(screen))
		if (SDL_LockSurface(screen) < 0) 
			return;

	// Ask SDL for the time in milliseconds
	int tick = SDL_GetTicks();

	if (lasttick == 0) lasttick = tick;

	while (lasttick < tick)
	{
		gSfx.loadPreset(SoLoud::Sfxr::LASER, 3);
		if (fire1)
		{
			int h = gSoloud.playClocked(lasttick / 1000.0f, gSfx, 1,(rand()%100)/50.0f-1);
			gSoloud.setRelativePlaySpeed(h, 1.0f - ((rand() % 200) / 1000.0f));
		}

		if (fire2)
		{
			int h = gSoloud.play(gSfx, 1, (rand()%100)/50.0f-1);
			gSoloud.setRelativePlaySpeed(h, 1.0f - ((rand() % 200) / 1000.0f));
		}

		if (fire3)
		{
			int h = gSoloud.playClocked(lasttick / 1000.0f, gSfx, 1, (rand()%100)/50.0f-1);
			gSoloud.setRelativePlaySpeed(h, 1.0f - ((rand() % 200) / 1000.0f));
		}

		if (fire1 || fire2 || fire3)
		{
			bullet[bulletidx & 1023] = 100;
			bulletidx++;			
		}

		if (fire3) fire3 = 0;

		int i;
		for (i = 0; i < 1024; i++)
		{
			if (bullet[i])
			{
				bullet[i] += 5;
				if (bullet[i] >= 400)
					bullet[i] = 0;
			}
		}

		lasttick += 10;
	}

	int i, j;

	for (i = 0; i < 256; i++)
		for (j = 0; j < 400; j++)
			putpixel(j,i,0xff000000);

	for (i = 0; i < 9; i++)
		for (j = 0; j < 9; j++)
			putpixel(90 + i, 124 + j, 0xffffffff);


	for (i = 0; i < 1024; i++)
		if (bullet[i])
			putpixel(bullet[i], 128, 0xffffffff);

	for (i = 0; i < (signed)gSoloud.getActiveVoiceCount(); i++)
		putpixel(i * 4, 8, 0xffffffff);

	// Unlock if needed
	if (SDL_MUSTLOCK(screen)) 
		SDL_UnlockSurface(screen);

	// Tell SDL to update the whole screen
	SDL_UpdateRect(screen, 0, 0, 400, 256);    
}


// Entry point
int main(int argc, char *argv[])
{
	// Initialize SDL's subsystems - in this case, only video.
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) 
	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	// Use a slightly larger audio buffer to exaggarate the effect
	gSoloud.init(SoLoud::Soloud::CLIP_ROUNDOFF, 0, 0, 8192);

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

	// Main loop: loop forever.
	while (1)
	{
		// Render stuff
		render();		
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
					fire1 = 1;
					break;
				case SDLK_2: 
					fire2 = 1;
					break;
				case SDLK_3: 
					fire3 = 1;
					break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
				case SDLK_1:
					fire1 = 0;
					break;
				case SDLK_2: 
					fire2 = 0;
					break;
				}
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