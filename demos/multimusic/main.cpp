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
#include "soloud_sfxr.h"


SoLoud::Soloud gSoloud;
SoLoud::Sfxr gSfx;
SoLoud::WavStream gMusic1, gMusic2;
int gMusichandle1, gMusichandle2;

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

		int last = 0;
		for (i = 0; i < 256; i++)
		{			
			int v = 256-(int)floor(fft[i] * 127);
			if (v < 0) v = 0;
			if (v > 256) v = 256;
			for (j = v; j < 256; j++)
			{
				putpixel(i,j,0x0000ff);
			}

			v = (int)floor(buf[i] * 127 + 128);
			if (v < 0) v = 0;
			if (v > 256) v = 256;
			for (j = 0; j < 6; j++)
			{
				putpixel(i,j+v,0xff0000);
			}

			putpixel(200 + v,128+(v-last),0x00ff00);
			last = v;
		}
	}

	drawstring("1-6: sfxr sound effects",0,0);
	drawstring("7-8: switch music",0,16);
	drawstring("9-0: slow down/speed up",0,32);

	char temp[256];
	sprintf(temp, "Music1 volume: %d%%", (int)floor(gSoloud.getVolume(gMusichandle1) * 100));
	drawstring(temp,0,48);
	sprintf(temp, "Music2 volume: %d%%", (int)floor(gSoloud.getVolume(gMusichandle2) * 100));
	drawstring(temp,0,64);
	sprintf(temp, "Music rel. speed: %d%%", (int)floor(gSoloud.getRelativePlaySpeed(gMusichandle2) * 100));
	drawstring(temp,0,80);
	sprintf(temp, "Active voices: %d", gSoloud.getActiveVoiceCount());
	drawstring(temp,0,96);

	// Unlock if needed
	if (SDL_MUSTLOCK(screen)) 
		SDL_UnlockSurface(screen);

	// Tell SDL to update the whole screen
	SDL_UpdateRect(screen, 0, 0, 400, 256);    
}


// Entry point
int main(int argc, char *argv[])
{
	gMusic1.load("audio/plonk_wet.ogg");
	gMusic2.load("audio/plonk_dry.ogg");

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
	font = SDL_LoadBMP("graphics/font.bmp");

	// If we fail, return error.
	if ( screen == NULL ) 
	{
		fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
		exit(1);
	}

	gMusichandle1 = gSoloud.play(gMusic1,0,0,1);
	gMusichandle2 = gSoloud.play(gMusic2,1,0,1);

	SoLoud::handle grouphandle = gSoloud.createVoiceGroup();
	gSoloud.addVoiceToGroup(grouphandle, gMusichandle1);
	gSoloud.addVoiceToGroup(grouphandle, gMusichandle2);
	
	gSoloud.setProtectVoice(grouphandle, 1); // protect all voices in group 
	gSoloud.setPause(grouphandle, 0);        // unpause all voices in group 
	
	gSoloud.destroyVoiceGroup(grouphandle); // remove group, leaves voices alone

	int cycle = 0;

	// Main loop: loop forever.
	while (1)
	{
		cycle++;
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
					gSfx.loadPreset(SoLoud::Sfxr::EXPLOSION, cycle);
					gSoloud.play(gSfx, 1, ((rand()%512)-256)/256.0f);
					break;
				case SDLK_2: 
					gSfx.loadPreset(SoLoud::Sfxr::BLIP, cycle);
					gSoloud.play(gSfx, 1, ((rand()%512)-256)/256.0f);
					break;
				case SDLK_3: 
					gSfx.loadPreset(SoLoud::Sfxr::COIN, cycle);
					gSoloud.play(gSfx, 1, ((rand()%512)-256)/256.0f);
					break;
				case SDLK_4: 
					gSfx.loadPreset(SoLoud::Sfxr::HURT, cycle);
					gSoloud.play(gSfx, 1, ((rand()%512)-256)/256.0f);
					break;
				case SDLK_5: 
					gSfx.loadPreset(SoLoud::Sfxr::JUMP, cycle);
					gSoloud.play(gSfx, 1, ((rand()%512)-256)/256.0f);
					break;
				case SDLK_6: 
					gSfx.loadPreset(SoLoud::Sfxr::LASER, cycle);
					gSoloud.play(gSfx, 1, ((rand()%512)-256)/256.0f);
					break;
				case SDLK_7:
					gSoloud.fadeVolume(gMusichandle1, 1, 2);
					gSoloud.fadeVolume(gMusichandle2, 0, 2);
					break;
				case SDLK_8:
					gSoloud.fadeVolume(gMusichandle2, 1, 2);
					gSoloud.fadeVolume(gMusichandle1, 0, 2);
					break;
				case SDLK_9:
					gSoloud.fadeRelativePlaySpeed(gMusichandle1, 0.2f, 5);
					gSoloud.fadeRelativePlaySpeed(gMusichandle2, 0.2f, 5);
					break;
				case SDLK_0:
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