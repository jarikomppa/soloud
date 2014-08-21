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
#include <stdio.h>

#include "soloud.h"
#include "soloud_sfxr.h"
#include "soloud_speech.h"


SoLoud::Soloud gSoloud;
SoLoud::Sfxr gSfx_mouse, gSfx_orbit;
SoLoud::Speech gSfx_crazy;

SDL_Surface *screen;

void putpixel(int x, int y, int color)
{
	if (y < 0 || y > 255 || x < 0 || x > 400) 
		return;
	unsigned int *ptr = (unsigned int*)screen->pixels;
	int lineoffset = y * (screen->pitch / 4);
	ptr[lineoffset + x] = color;
}

int gMouseX = 0;
int gMouseY = 0;
int gSndHandle_mouse = 0;
int gSndHandle_orbit = 0;
int gSndHandle_crazy = 0;

void render()
{   
	// Lock surface if needed
	if (SDL_MUSTLOCK(screen))
		if (SDL_LockSurface(screen) < 0) 
			return;

	float tick = SDL_GetTicks() / 1000.0f;

	float crazyx = sin(tick) * sin(tick * 0.234) * sin(tick * 4.234) * 150;
	float crazyz = cos(tick) * cos(tick * 0.234) * cos(tick * 4.234) * 150 - 50;
	float tickd = tick - 0.1;
	float crazyxv = sin(tickd) * sin(tickd * 0.234) * sin(tickd * 4.234) * 150;
	float crazyzv = cos(tickd) * cos(tickd * 0.234) * cos(tickd * 4.234) * 150 - 50;
	crazyxv = crazyxv - crazyx;
	crazyzv = crazyzv - crazyz;

	gSoloud.set3dSourceParameters(gSndHandle_crazy, crazyx, 0, crazyz, crazyxv, 0, crazyzv);

	float orbitx = sin(tick) * 50;
	float orbitz = cos(tick) * 50;
	float orbitxv = sin(tickd) * 50;
	float orbitzv = cos(tickd) * 50;
	orbitxv = orbitxv - orbitx;
	orbitzv = orbitzv - orbitz;	

	gSoloud.set3dSourceParameters(gSndHandle_orbit, orbitx, 0, orbitz, orbitxv, 0, orbitzv);

	gSoloud.update3dAudio();

	int i, j;

	for (i = 0; i < 256; i++)
		for (j = 0; j < 400; j++)
			putpixel(j,i,0xff000000);

	for (i = 0; i < 9; i++)
		for (j = 0; j < 9; j++)
			putpixel(200-5 + i, 128-5 + j, 0xffffffff);

	putpixel(200+orbitx,128+orbitz,0xff00ffff);
	putpixel(200+orbitx+1,128+orbitz,0xff00ffff);
	putpixel(200+orbitx,128+orbitz+1,0xff00ffff);
	putpixel(200+orbitx+1,128+orbitz+1,0xff00ffff);
	putpixel(200+crazyx,128+crazyz,0xffffff00);
	putpixel(200+crazyx+1,128+crazyz,0xffffff00);
	putpixel(200+crazyx,128+crazyz+1,0xffffff00);
	putpixel(200+crazyx+1,128+crazyz+1,0xffffff00);

	putpixel(200+crazyxv+1,128+crazyzv+1,0xffffff00);

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

	// Init SoLoud
	gSoloud.init();
	gSoloud.setGlobalVolume(4);

	gSfx_mouse.loadPreset(SoLoud::Sfxr::LASER, 3);
	gSfx_mouse.setLooping(1);
	gSfx_mouse.set3dMinMaxDistance(1,200);
	gSfx_mouse.set3dAttenuation(SoLoud::AudioSource::EXPONENTIAL_DISTANCE, 0.5);
	gSndHandle_mouse = gSoloud.play3d(gSfx_mouse, 100, 0, 0);

	gSfx_orbit.loadPreset(SoLoud::Sfxr::COIN, 3);
	gSfx_orbit.setLooping(1);
	gSfx_orbit.set3dMinMaxDistance(1,200);
	gSfx_orbit.set3dAttenuation(SoLoud::AudioSource::EXPONENTIAL_DISTANCE, 0.5);
	gSndHandle_orbit = gSoloud.play3d(gSfx_orbit, 50, 0, 0);

	gSfx_crazy.setText("I'm going into space with my space ship space ship space ship spaceeeeeeeeeeeeeeeeeeeeeeeeeeeeee");
	gSfx_crazy.setLooping(1);
	gSfx_crazy.set3dMinMaxDistance(1,400);
	gSfx_crazy.set3dAttenuation(SoLoud::AudioSource::EXPONENTIAL_DISTANCE, 0.25);
	gSndHandle_crazy = gSoloud.play3d(gSfx_crazy, 50, 0, 0);

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
			case SDL_MOUSEMOTION:
				gMouseX = event.motion.x;
				gMouseY = event.motion.y;
				gSoloud.set3dSourcePosition(gSndHandle_mouse, gMouseX-200,0,gMouseY-128);
				break;

			case SDL_KEYDOWN:
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