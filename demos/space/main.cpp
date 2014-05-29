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
#include "soloud_speech.h"
#include "soloud_modplug.h"


SoLoud::Soloud gSoloud;			// SoLoud engine core
SoLoud::Speech gSpeech;
SoLoud::Modplug gMod;

SDL_Surface *screen;
SDL_Surface *font;
SDL_Surface *bg;

void putpixel(int x, int y, int color)
{
	if (y < 0 || y > 480 || x < 0 || x > 640) 
		return;
	unsigned int *ptr = (unsigned int*)screen->pixels;
	int lineoffset = y * (screen->pitch / 4);
	ptr[lineoffset + x] = color;
}

int drawchar(int ch, int x, int y, int c)
{
	int i, j, maxx = 0;
	for (i = 0; i < 16; i++)
	{
		for (j = 0; j < 16; j++)
		{
			if (((char*)font->pixels)[((ch-32)*16+i)*16+j])
			{
				putpixel(x+j,y+i,c);
				if (j > maxx) maxx = j;
			}
		}
	}
	return maxx + 1;
}

void drawstring(char * s, int x, int y, int c, int max)
{
	int ox = x;
	while (*s && max)
	{
		max--;
		if (*s == '\n')
		{
			x = ox;
			y += 16;
		}
		else
		{
			x += drawchar(*s, x, y, c);
			if (*s == 32) x += 3;
		}
		s++;
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

	int i, j;
	for (i = 0; i < 480; i++)
		for (j = 0; j < 640; j++)
			putpixel(j, i, ((int*)bg->pixels)[i*bg->pitch/4+j] | 0xff000000);
	
	float *w = gSoloud.getWave();

	drawrect(317,231,226,133,0xff003f00);
	for (i = 0; i < 7; i++)
		drawrect(317+(i+1)*(226/8),231,1,133,0xff005f00);
	drawrect(317,231+65,226,1,0xff007f00);
	drawrect(317,231+32,226,1,0xff00af00);
	drawrect(317,231+97,226,1,0xff00af00);

	for (i = 0; i < 226; i+=2)
	{
		float v = fabs(w[i]);//(rand() % 1000)/1000.0f;
		if (v > 1) v = 1;
		float h = 132 * v / 2;
		drawrect(317+i, 231 + 66-h, 1, h*2, 0xff009f00);
	}

	float *f = gSoloud.calcFFT();

	drawrect(62,383,103,60,0xff003f00);
	for (i = 0; i < 103; i++)
	{
		float v = f[i & ~7];
		float h = 60 * v;
		if (h > 60) h = 60;
		drawrect(62+i,383+60-h,1,h,0xff007f00);
	}


	drawstring("The alien's speech might\n"
				"appear around here if this\n"
				"wasn't just a dummy mockup..", 317,100,0xff00ff00, tick / 100);

	// Unlock if needed
	if (SDL_MUSTLOCK(screen)) 
		SDL_UnlockSurface(screen);

	// Tell SDL to update the whole screen
	SDL_UpdateRect(screen, 0, 0, 640, 480);    
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

	gSoloud.init(1+2);
	gSoloud.setGlobalVolume(0.75);
	gSoloud.setPostClipScaler(0.75);

	gSpeech.setText("Flank pork loin tongue, swine rump porchetta jowl cow sirloin bresaola pork belly. Andouille ribeye t-bone turducken tail spare ribs, cow landjaeger. Venison pig meatball filet mignon, strip steak pancetta pork chop. Pork loin drumstick chuck, corned beef shoulder salami filet mignon short loin andouille tri-tip. Fatback salami doner, pancetta kevin ground round andouille shank.");
	gSpeech.setLooping(1);
	int sp = gSoloud.play(gSpeech, 4, -0.25);
	gSoloud.setRelativePlaySpeed(sp, 1.2);

	gMod.load("audio/BRUCE.S3M");
	gSoloud.play(gMod);

	
	// Register SDL_Quit to be called at exit; makes sure things are
	// cleaned up when we quit.
	atexit(SDL_Quit);	

	// Attempt to create a 640x480 window with 32bit pixels.
	screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
	font = SDL_LoadBMP("graphics/font.bmp");
	SDL_Surface *temp = SDL_LoadBMP("graphics/spaaaace.bmp");
	bg = SDL_DisplayFormat(temp);	
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