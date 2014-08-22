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
#include "soloud_wav.h"
#include "soloud_biquadresonantfilter.h"


SoLoud::Soloud gSoloud;			// SoLoud engine core
SoLoud::BiquadResonantFilter gLPFilter;
SoLoud::Wav gRain, gWind, gMusic;

int gRainHandle, gWindHandle, gMusicHandle;

SDL_Surface *screen;
SDL_Surface *font;
SDL_Surface *bg;
SDL_Surface *walker;

void putpixel(int x, int y, int color)
{
	if (y < 0 || y > 255 || x < 0 || x > 400) 
		return;
	unsigned int *ptr = (unsigned int*)screen->pixels;
	int lineoffset = y * (screen->pitch / 4);
	ptr[lineoffset + x] = color;
}

void mulpixel(int x, int y, int color)
{
	if (y < 0 || y > 255 || x < 0 || x > 400) 
		return;
	unsigned int *ptr = (unsigned int*)screen->pixels;
	int lineoffset = y * (screen->pitch / 4);
	ptr[lineoffset + x] &= color;
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
				putpixel(x+j,y+i,0xff000000);
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

void drawwalker(int frame, int x, int y)
{
	int ofs = frame * walker->w * 2 * walker->pitch / 4;
	int i, j;
	for (i = 0; i < walker->w*2; i++)
	{
		for (j = 0; j < walker->w; j++)
		{
			mulpixel(x+j,y+i,((int*)walker->pixels)[ofs + i * walker->pitch / 4 + j]);
		}
	}
}

void drawrect(int x, int y, int w, int h, int c)
{
	int i, j;
	for (i = 0; i < w; i++)
		for (j = 0; j < h; j++)
			putpixel(i+x, j+y, c);			
}

void render()
{   
	// Lock surface if needed
	if (SDL_MUSTLOCK(screen))
		if (SDL_LockSurface(screen) < 0) 
			return;

	// Ask SDL for the time in milliseconds
	int tick = SDL_GetTicks();

	float p = (tick % 60000) / 60000.0f;

	//p = 0.4f;

	int ypos;
	int xpos;
	int dudey;
#define SMOOTHSTEP(x) ((x) * (x) * (3 - 2 * (x)))
	if (p < 0.1f)
	{
		xpos = 0;
		ypos = 600-256;
		dudey= -8;
	}
	else
	if (p < 0.5f)
	{
		float v = (p - 0.1f) * 2.5f;
		v = SMOOTHSTEP(v);
		v = SMOOTHSTEP(v);
		v = SMOOTHSTEP(v);

		xpos = (int)floor(v * (800 - 400));
		ypos = 600 - 256;
		dudey = (int)floor((1 - v) * -8);
	}
	else
	if (p < 0.9f)
	{
		float v = (p - 0.5f) * 2.5f;
		v = SMOOTHSTEP(v);
		v = SMOOTHSTEP(v);
		v = SMOOTHSTEP(v);
		xpos = 800 - 400;
		ypos = (int)floor((1 - v) * (600 - 256));
		dudey = (int)floor(v * 90);
	}
	else
	{
		xpos = 800-400;
		ypos = 0;
		dudey = 90;
	}

	static int mode_a = 0;

	if (p < 0.35f)
	{
		if (mode_a != 0)
			gSoloud.fadeVolume(gRainHandle,1,0.2f);
		mode_a = 0;
	}
	else
	{
		if (mode_a != 1)
			gSoloud.fadeVolume(gRainHandle,0,0.2f);
		mode_a = 1;
	}

	static int mode_b = 0;

	if (p < 0.7f)
	{
		if (mode_b != 0)
			gSoloud.fadeVolume(gWindHandle,0,0.2f);
		mode_b = 0;
	}
	else
	if (p < 0.8f)
	{
		gSoloud.setVolume(gWindHandle,(p-0.7f)*10);
		mode_b = 1;
	}
	else
	{
		if (mode_b != 2)
			gSoloud.fadeVolume(gWindHandle,1,0.2f);
		mode_b = 2;
	}

	static int mode_c = 0;

	if (p < 0.2f)
	{
		if (mode_c != 0)
			gSoloud.fadeVolume(gMusicHandle, 0, 0.2f);
		mode_c = 0;
	}
	else
	if (p < 0.4f)
	{
		gSoloud.setVolume(gMusicHandle, (p-0.2f)*5);
		mode_c = 1;
	}
	else
	if (p < 0.5f)
	{
		if (mode_c != 2)
			gSoloud.fadeVolume(gMusicHandle, 1, 0.2f);
		mode_c = 2;
	}
	else
	if (p < 0.7f)
	{	
		gSoloud.setVolume(gMusicHandle, 1 - (p - 0.5f) * 4.5f);
		mode_c = 3;
	}
	else
	{
		if (mode_c != 4)
			gSoloud.fadeVolume(gMusicHandle, 0.1f, 0.2f);
		mode_c = 4;
	}

	static int mode_d = 0;

	if (p < 0.25f)
	{
		if (mode_d != 0)
		{
			gSoloud.fadeFilterParameter(gMusicHandle, 0, SoLoud::BiquadResonantFilter::FREQUENCY, 200, 0.2f);
			gSoloud.fadeFilterParameter(gMusicHandle, 0, SoLoud::BiquadResonantFilter::WET, 1, 0.2f);
		}
		mode_d = 0;
	}
	else
	if (p < 0.35f)
	{
		if (mode_d != 1)
		{
			gSoloud.fadeFilterParameter(gMusicHandle, 0, SoLoud::BiquadResonantFilter::WET, 0.5f, 2.0f);
		}
		mode_d = 1;
	}
	else
	if (p < 0.55f)
	{
		if (mode_d != 2)
		{
			gSoloud.fadeFilterParameter(gMusicHandle, 0, SoLoud::BiquadResonantFilter::FREQUENCY, 2000, 1.0f);
			gSoloud.fadeFilterParameter(gMusicHandle, 0, SoLoud::BiquadResonantFilter::WET, 0, 1.0f);
		}
		mode_d = 2;
	}
	else
	{
		if (mode_d != 3)
		{
			gSoloud.fadeFilterParameter(gMusicHandle, 0, SoLoud::BiquadResonantFilter::FREQUENCY, 200, 0.3f);
			gSoloud.fadeFilterParameter(gMusicHandle, 0, SoLoud::BiquadResonantFilter::WET, 1, 0.3f);
		}
		mode_d = 3;
	}

	static int mode_e = 0;

	if (p < 0.2f)
	{
		if (mode_e != 0)
			gSoloud.fadePan(gMusicHandle, 1, 0.2f);
		mode_e = 0;
	}
	else
	if (p < 0.4f)
	{
		gSoloud.setPan(gMusicHandle, 1-((p-0.2f)*5));
		mode_e = 1;
	}
	else
	{
		if (mode_e != 2)
			gSoloud.fadePan(gMusicHandle, 0, 0.2f);
		mode_e = 2;
	}

	int i, j;
	for (i = 0; i < 256; i++)
		for (j = 0; j < 400; j++)
			putpixel(j, i, *(((int*)bg->pixels)+j+xpos+(i+ypos)*bg->pitch/4));

	drawwalker((tick >> 7) % ((tick >> 8) % 5 + 1), (400-32)/2 + 12, 256-32*2-32-dudey);

	if (p > 0.5f)
	{
		int w = (int)floor((p - 0.5f) * 600);
		if (w > 32) w = 32;
		drawrect((400-32)/2+12, 256-32*2-32-ypos+600-256, w/2, 64, 0xffffffff);
		drawrect((400-32)/2+12+32-(w/2), 256-32*2-32-ypos+600-256, w/2, 64, 0xffffffff);

		drawrect((400-32)/2+12+(w/2), 256-32*2-32-ypos+600-256, 1, 64, 0xffaaaaaa);
		drawrect((400-32)/2+12+32-(w/2), 256-32*2-32-ypos+600-256, 1, 64, 0xffaaaaaa);
	}

	char temp[256];
	//sprintf(temp, "%3.8f", p);
	//drawstring(temp,0,0);
	drawrect(0, 0, 100, 12, 0xff808080);
	drawrect(0, 2, (int)floor(100 * p), 8, 0xffe0e0e0);
	sprintf(temp, "Rain volume: %3.3f", gSoloud.getVolume(gRainHandle));
	drawstring(temp,0,20);
	sprintf(temp, "Music volume: %3.3f", gSoloud.getVolume(gMusicHandle));
	drawstring(temp,0,40);
	sprintf(temp, "Wind volume: %3.3f", gSoloud.getVolume(gWindHandle));
	drawstring(temp,0,60);
	sprintf(temp, "Music pan: %3.3f", gSoloud.getPan(gMusicHandle));
	drawstring(temp,0,80);
	sprintf(temp, "Music filter wet: %3.3f", gSoloud.getFilterParameter(gMusicHandle,0,SoLoud::BiquadResonantFilter::WET));
	drawstring(temp,0,100);
	sprintf(temp, "Music filter freq: %3.3f", gSoloud.getFilterParameter(gMusicHandle,0,SoLoud::BiquadResonantFilter::FREQUENCY));
	drawstring(temp,0,120);

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

	gSoloud.init();
	gSoloud.setGlobalVolume(0.75);
	gSoloud.setPostClipScaler(0.75);

	gRain.load("audio/rainy_ambience.ogg");
	gRain.setLooping(1);
	gWind.load("audio/windy_ambience.ogg");
	gWind.setLooping(1);
	gMusic.load("audio/tetsno.ogg");
	gMusic.setLooping(1);
	gLPFilter.setParams(SoLoud::BiquadResonantFilter::LOWPASS,44100,100,10);
	gMusic.setFilter(0, &gLPFilter);

	gRainHandle = gSoloud.play(gRain,1);
	gWindHandle = gSoloud.play(gWind,0);
	gMusicHandle = gSoloud.play(gMusic,0);
	
	// Register SDL_Quit to be called at exit; makes sure things are
	// cleaned up when we quit.
	atexit(SDL_Quit);	

	// Attempt to create a 640x480 window with 32bit pixels.
	screen = SDL_SetVideoMode(400, 256, 32, SDL_SWSURFACE);
	font = SDL_LoadBMP("graphics/font.bmp");
	SDL_Surface *temp = SDL_LoadBMP("graphics/env_bg.bmp");
	bg = SDL_DisplayFormat(temp);	
	SDL_FreeSurface(temp);
	temp = SDL_LoadBMP("graphics/env_walker.bmp");
	walker = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	// If we fail, return error.
	if ( screen == NULL ) 
	{
		fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

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