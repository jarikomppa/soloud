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

#define USE_PORTMIDI

#include "soloud.h"
#include "soloud_basicwave.h"
#include "soloud_echofilter.h"
#include "soloud_speech.h"
#include "soloud_biquadresonantfilter.h"

#ifdef USE_PORTMIDI
#include "portmidi.h"
#include <Windows.h>
#endif

SoLoud::Speech gSpeech;
SoLoud::Soloud gSoloud;			// SoLoud engine core
SoLoud::Basicwave gWave;		// Simple wave audio source
SoLoud::EchoFilter gFilter;		// Simple echo filter
SoLoud::BiquadResonantFilter gBQRFilter;   // BQR filter
SoLoud::Bus gBus;

SDL_Surface *screen;

void putpixel(int x, int y, int color)
{
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

	// Calling calcFFT will cause SoLoud to actually calculate the FFT;
	// if we keep this pointer around, it'll just point to the old data.
	float *buf = gSoloud.calcFFT();
	float *mixbuf = gSoloud.getWave();
	int i, j;
	if (mixbuf)
	for (i = 0; i < 640; i++)
	{
		int v;
		float f;
		f = buf[i*256/640];
		v = (int)floor(480 - f * 4);
		for (j = 240; j < 480; j++)
		{
			int c = 0;
			if (j > v)
				c = 0xff0000;

			putpixel(i, j, c);
		}

		v = (int)floor(120*mixbuf[i * 256 / 640])+120;
		for (j = 0; j < 240; j++)
		{
			int c = 0;
			if (j > v)
				c = 0xff0000;

			putpixel(i, j, c);
		}
	}

	// Unlock if needed
	if (SDL_MUSTLOCK(screen)) 
		SDL_UnlockSurface(screen);

	// Tell SDL to update the whole screen
	SDL_UpdateRect(screen, 0, 0, 640, 480);    
}

void plonk(float rel, float vol = 0x50)
{
	vol = (vol + 10) / (float)(0x7f + 10);
	vol *= vol;
	float pan = (float)sin(SDL_GetTicks() * 0.0234) ;
//	int handle = gSoloud.play(gWave,1,pan);
	int handle = gBus.play(gWave,vol,pan);
//	gSoloud.fadePan(handle,pan,-pan,0.5);
//	gSoloud.oscillatePan(handle,-1,1,0.2);
	gSoloud.fadeVolume(handle, vol, 0, 0.5);
	gSoloud.scheduleStop(handle, 0.5);
	gSoloud.setRelativePlaySpeed(handle, 2*rel);
}

#ifdef USE_PORTMIDI
PmStream *midi = NULL;
#endif

// Entry point
int main(int argc, char *argv[])
{	
#ifdef USE_PORTMIDI
	PmEvent buffer[1];
#endif
	// Initialize SDL's subsystems - in this case, only video.
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) 
	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

#ifdef USE_PORTMIDI
	Pm_OpenInput(&midi, Pm_GetDefaultInputDeviceID(), NULL, 100, NULL, NULL);
	Pm_SetFilter(midi, PM_FILT_REALTIME);
    while (Pm_Poll(midi)) {
        Pm_Read(midi, buffer, 1);
    }
#endif

	SoLoud::sdl_init(&gSoloud, SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION, 44100, 2048);
	gSoloud.setGlobalVolume(0.75);
	gSoloud.setPostClipScaler(0.75);
//	gBus.setFilter(0, &gBQRFilter);
//	gBus.setFilter(1, &gFilter);
	gFilter.setParams(0.5f, 0.5f);
	int bushandle = gSoloud.play(gBus);
	gSpeech.setText(". . . . Use keyboard to play or adjust settings");
	gSoloud.play(gSpeech, 4);
	
	gSoloud.setGlobalFilter(0,0);
	gSoloud.setGlobalFilter(1,0);
	gSoloud.setGlobalFilter(2,0);
	gSoloud.setGlobalFilter(3,0);

	// Register SDL_Quit to be called at exit; makes sure things are
	// cleaned up when we quit.
	atexit(SDL_Quit);	

	// Attempt to create a 640x480 window with 32bit pixels.
	screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

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
#ifdef USE_PORTMIDI
		int i = Pm_Poll(midi);
		if (i)
		{
			i = Pm_Read(midi, buffer, 1);
			if (i)
			{
				char temp[200];
				sprintf(temp, "\n%x %x %x", Pm_MessageStatus(buffer[0].message), Pm_MessageData1(buffer[0].message), Pm_MessageData2(buffer[0].message));
				OutputDebugStringA(temp);
				if (Pm_MessageStatus(buffer[0].message) == 0x90)
					plonk(pow(0.943875f, 0x3c - Pm_MessageData1(buffer[0].message)),Pm_MessageData2(buffer[0].message));
			}
		}
#endif
		// Poll for events, and handle the ones we care about.
		SDL_Event event;
		while (SDL_PollEvent(&event)) 
		{
			switch (event.type) 
			{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_z: 
					gBus.setFilter(0, 0); 
					gBus.setFilter(1, 0); 
					gBus.setFilter(2, 0); 
					gBus.setFilter(3, 0); 
					gSpeech.setText("Filter clear");
					gSoloud.play(gSpeech, 4);
					break;
				case SDLK_x: 
					gBus.setFilter(0, &gFilter); 
					gSpeech.setText("Echo filter");
					gSoloud.play(gSpeech, 4);
					break;
				case SDLK_c: 
					gBQRFilter.setParams(SoLoud::BiquadResonantFilter::LOWPASS, 44100, 1000, 2);  
					gBus.setFilter(1, &gBQRFilter); 
					gSpeech.setText("Low pass filter 1000 hz");
					gSoloud.play(gSpeech, 4);
					break;
				case SDLK_v: 
					gBQRFilter.setParams(SoLoud::BiquadResonantFilter::LOWPASS, 44100, 500, 8);   
					gBus.setFilter(1, &gBQRFilter); 
					gSpeech.setText("Low pass filter 500 hz");
					gSoloud.play(gSpeech, 4);
					break;
				case SDLK_b: 
					gBQRFilter.setParams(SoLoud::BiquadResonantFilter::HIGHPASS, 44100, 1000, 8); 
					gBus.setFilter(1, &gBQRFilter); 
					gSpeech.setText("High pass filter 1000 hz");
					gSoloud.play(gSpeech, 4);
					break;
				case SDLK_n: 
					gBQRFilter.setParams(SoLoud::BiquadResonantFilter::BANDPASS, 44100, 1000, 1); 
					gBus.setFilter(1, &gBQRFilter); 
					gSpeech.setText("Band pass filter 1000 hz");
					gSoloud.play(gSpeech, 4);
					break;
				case SDLK_m: 
					gBQRFilter.setParams(SoLoud::BiquadResonantFilter::LOWPASS, 44100, 1000, 2);  
					gBus.setFilter(1, &gBQRFilter); 
					gSoloud.oscillateFilterParameter(bushandle, 1, 1, 500, 6000, 4);  
					gSpeech.setText("Oscillating low pass filter");
					gSoloud.play(gSpeech, 4);
					break; 				
				case SDLK_a: 
					gWave.setWaveform(SoLoud::Basicwave::SINE); 
					gSpeech.setText("Sine wave");
					gSoloud.play(gSpeech, 4);
					break;
				case SDLK_s: 
					gWave.setWaveform(SoLoud::Basicwave::TRIANGLE); 
					gSpeech.setText("Triangle wave");
					gSoloud.play(gSpeech, 4);
					break;
				case SDLK_d: 
					gWave.setWaveform(SoLoud::Basicwave::SQUARE); 
					gSpeech.setText("Square wave");
					gSoloud.play(gSpeech, 4);
					break;
				case SDLK_f: 
					gWave.setWaveform(SoLoud::Basicwave::SAW); 
					gSpeech.setText("Saw wave");
					gSoloud.play(gSpeech, 4);
					break;
				case SDLK_g: 
					gWave.setWaveform(SoLoud::Basicwave::INVERSESAW); 
					gSpeech.setText("Inverse saw wave");
					gSoloud.play(gSpeech, 4);
					break;

				case SDLK_p: plonk(1); break;                  // C
				case SDLK_o: plonk(pow(0.943875f, 1)); break;  // B
				case SDLK_9: plonk(pow(0.943875f, 2)); break;  //  A#
				case SDLK_i: plonk(pow(0.943875f, 3)); break;  // A
				case SDLK_8: plonk(pow(0.943875f, 4)); break;  //  G#
				case SDLK_u: plonk(pow(0.943875f, 5)); break;  // G
				case SDLK_7: plonk(pow(0.943875f, 6)); break;  //  F#
				case SDLK_y: plonk(pow(0.943875f, 7)); break;  // F
				case SDLK_t: plonk(pow(0.943875f, 8)); break;  // E
				case SDLK_5: plonk(pow(0.943875f, 9)); break;  //  D#
				case SDLK_r: plonk(pow(0.943875f, 10)); break; // D
				case SDLK_4: plonk(pow(0.943875f, 11)); break; //  C#
				case SDLK_e: plonk(pow(0.943875f, 12)); break; // C
				case SDLK_w: plonk(pow(0.943875f, 13)); break; // B
				case SDLK_2: plonk(pow(0.943875f, 14)); break; //  A#
				case SDLK_q: plonk(pow(0.943875f, 15)); break; // A
				case SDLK_1: plonk(pow(0.943875f, 16)); break; //  G#
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