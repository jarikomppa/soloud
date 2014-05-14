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
#include "soloud_speech.h"


SoLoud::Soloud gSoloud;			// SoLoud engine core
SoLoud::Speech gSpeech[10];
SoLoud::Wav gSfxloop, gMusicloop;
SoLoud::Bus gSfxbus, gMusicbus, gSpeechbus;

int gSfxbusHandle, gMusicbusHandle, gSpeechbusHandle;

float gSfxvol = 1, gMusicvol = 1, gSpeechvol = 1;

SDL_Surface *screen;
SDL_Surface *font;

void putpixel(int x, int y, int color)
{
	if (y < 0 || y > 255 || x < 0 || x > 400) 
		return;
	unsigned int *ptr = (unsigned int*)screen->pixels;
	int lineoffset = y * (screen->pitch / 4);
	ptr[lineoffset + x] = color;
}

int drawchar(int ch, int x, int y)
{
	int i, j, maxx = 0;
	for (i = 0; i < 16; i++)
	{
		for (j = 0; j < 16; j++)
		{
			if (((char*)font->pixels)[((ch-32)*16+i)*16+j])
			{
				putpixel(x+j,y+i,0xffffffff);
				if (j > maxx) maxx = j;
			}
		}
	}
	return maxx + 1;
}

void drawstring(char * s, int x, int y)
{
	while (*s)
	{
		x += drawchar(*s, x, y);
		if (*s == 32) x += 3;
		s++;
	}
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

		static int last = 0;
		for (i = 0; i < 256; i++)
		{			
			int v = 256-(int)floor(fft[i] * 127 * 0.1);
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

	char temp[256];

	sprintf(temp, "Speech volume [q w]: %3.3f%%", gSpeechvol * 100);
	drawstring(temp,0,0);
	sprintf(temp, "   SFX volume [a s]: %3.3f%%", gSfxvol * 100);
	drawstring(temp,12,20);
	sprintf(temp, " Music volume [z x]: %3.3f%%", gMusicvol * 100);
	drawstring(temp,9,40);

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

	gSoloud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);
	gSoloud.setGlobalVolume(0.75);
	gSoloud.setPostClipScaler(0.75);

	gSpeechbusHandle = gSoloud.play(gSpeechbus);
	gSfxbusHandle = gSoloud.play(gSfxbus);
	gMusicbusHandle = gSoloud.play(gMusicbus);

	gSpeech[0].setText("There is flaky pastry in my volkswagon.");
	gSpeech[1].setText("The fragmentation of empiricism is hardly influential in its interdependence.");
	gSpeech[2].setText("Sorry, my albatros is not inflatable.");
	gSpeech[3].setText("The clairvoyance of omnipotence is in fact quite closed-minded in its ecology.");
	gSpeech[4].setText("Cheese is quite nice.");
	gSpeech[5].setText("Pineapple Scones with Squash and Pastrami Sandwich");
	gSpeech[6].setText("The smart trader nowadays will be sure not to prorate OTC special-purpose entities.");
	gSpeech[7].setText("The penguins are in the toilets.");
	gSpeech[8].setText("Don't look, but there is a mountain lion stalking your children");
	gSpeech[9].setText("The train has already gone, would you like to hire a bicycle?");

	gSfxloop.load("audio/war_loop.ogg");
	gSfxloop.setLooping(1);
	gMusicloop.load("audio/algebra_loop.ogg");
	gMusicloop.setLooping(1);

	gSfxbus.play(gSfxloop);
	gMusicbus.play(gMusicloop);
	
	// Register SDL_Quit to be called at exit; makes sure things are
	// cleaned up when we quit.
	atexit(SDL_Quit);	

	// Attempt to create a 640x480 window with 32bit pixels.
	screen = SDL_SetVideoMode(400, 256, 32, SDL_SWSURFACE);
	font = SDL_LoadBMP("graphics/font.bmp");
	// If we fail, return error.
	if ( screen == NULL ) 
	{
		fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	unsigned int speechtick = 0;
	int speechcount = 0;

	// Main loop: loop forever.
	while (1)
	{
		// Render stuff
		render();

		if (speechtick < SDL_GetTicks())
		{
			int h = gSpeechbus.play(gSpeech[speechcount%10],(rand()%200)/50.0f+2,(rand()%20)/10.0f-1);
			speechcount++;
			gSoloud.setRelativePlaySpeed(h, (rand()%100)/200.0f + 0.75f);
			speechtick = SDL_GetTicks() + 4000;
		}

		// Poll for events, and handle the ones we care about.
		SDL_Event event;
		while (SDL_PollEvent(&event)) 
		{
			switch (event.type) 
			{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_q: gSpeechvol -= 0.1f; if (gSpeechvol < 0) gSpeechvol = 0; break;
				case SDLK_w: gSpeechvol += 0.1f; if (gSpeechvol > 10) gSpeechvol = 10; break;
				case SDLK_a: gSfxvol -= 0.1f; if (gSfxvol < 0) gSfxvol = 0; break;
				case SDLK_s: gSfxvol += 0.1f; if (gSfxvol > 10) gSfxvol = 10; break;
				case SDLK_z: gMusicvol -= 0.1f; if (gMusicvol < 0) gMusicvol = 0; break;
				case SDLK_x: gMusicvol += 0.1f; if (gMusicvol > 10) gMusicvol = 10; break;
				}
				gSoloud.fadeVolume(gSpeechbusHandle, gSpeechvol, 0.2f);
				gSoloud.fadeVolume(gSfxbusHandle, gSfxvol, 0.2f);
				gSoloud.fadeVolume(gMusicbusHandle, gMusicvol, 0.2f);
				break;
			case SDL_KEYUP:
				// If escape is pressed, return (and thus, quit)
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					{
						gSoloud.deinit();
						return 0;
					}
					break;
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