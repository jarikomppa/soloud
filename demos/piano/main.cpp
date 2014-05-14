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
#include "soloud_basicwave.h"
#include "soloud_echofilter.h"
#include "soloud_speech.h"
#include "soloud_fftfilter.h"
#include "soloud_biquadresonantfilter.h"

#ifdef USE_PORTMIDI
#include "portmidi.h"
#include <Windows.h>
#endif

struct plonked
{
	int mHandle;
	float mRel;
};

SoLoud::Speech gSpeech;
SoLoud::Soloud gSoloud;			// SoLoud engine core
SoLoud::Basicwave gWave;		// Simple wave audio source
SoLoud::Wav gLoadedWave;
SoLoud::EchoFilter gFilter;		// Simple echo filter
SoLoud::BiquadResonantFilter gBQRFilter;   // BQR filter
SoLoud::Bus gBus;
SoLoud::FFTFilter gFftFilter;

float gAttack = 0.02f;
float gRelease = 0.5f;

plonked gPlonked[128] = { 0 };
int gWaveSelect = 2;
int gEcho = 0;
char *gInfo = "";

SDL_Surface *screen;
SDL_Surface *font;

#ifdef USE_PORTMIDI
PmStream *midi = NULL;
#endif

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


	switch (gWaveSelect)
	{
	case 0:
		drawstring("Sine wave", 0, 0);
		break;
	case 1:
		drawstring("Triangle wave", 0, 0);
		break;
	case 2:
		drawstring("Square wave", 0, 0);
		break;
	case 3:
		drawstring("Saw wave", 0, 0);
		break;
	case 4:
		drawstring("Looped sample", 0, 0);
		break;
	}

	if (gEcho)
		drawstring("Echo on", 0, 16);

	drawstring(gInfo, 0, 256-16);

	// Unlock if needed
	if (SDL_MUSTLOCK(screen)) 
		SDL_UnlockSurface(screen);

	// Tell SDL to update the whole screen
	SDL_UpdateRect(screen, 0, 0, 400, 256);    
}

void plonk(float rel, float vol = 0x50)
{
	int i = 0;
	while (gPlonked[i].mHandle != 0 && i < 128) i++;
	if (i == 128) return;
	
	vol = (vol + 10) / (float)(0x7f + 10);
	vol *= vol;
	float pan = (float)sin(SDL_GetTicks() * 0.0234) ;
	int handle;
	if (gWaveSelect < 4)
	{
		handle = gBus.play(gWave,0);
	}
	else
	{
		handle = gBus.play(gLoadedWave,0);
	}
	gSoloud.fadeVolume(handle, vol, gAttack);
	gSoloud.setRelativePlaySpeed(handle, 2*rel);
	gPlonked[i].mHandle = handle;
	gPlonked[i].mRel = rel;
}

void unplonk(float rel)
{
	int i = 0;
	while (gPlonked[i].mRel != rel &&i < 128) i++;
	if (i == 128) return;
	gSoloud.fadeVolume(gPlonked[i].mHandle, 0, gRelease);
	gSoloud.scheduleStop(gPlonked[i].mHandle, gRelease);
	gPlonked[i].mHandle = 0;
}

void say(char *text)
{
	gInfo = text;
	gSpeech.setText(text);
	gSoloud.play(gSpeech, 4);
}

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

	gSpeech.setFilter(1, &gFftFilter);

	gSpeech.setText(". . . . . . . . . . . . . . . Use keyboard to play or adjust settings");
	gInfo = "Use keyboard to play or adjust settings";
	gSoloud.play(gSpeech, 4);


	
	gSoloud.setGlobalFilter(0,0);
	gSoloud.setGlobalFilter(1,0);
	gSoloud.setGlobalFilter(2,0);
	gSoloud.setGlobalFilter(3,0);

	gLoadedWave.load("audio/AKWF_c604_0024.wav");
	gLoadedWave.setLooping(1);

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
					plonk(pow(0.943875f, 0x3c - Pm_MessageData1(buffer[0].message)),(float)Pm_MessageData2(buffer[0].message));
				if (Pm_MessageStatus(buffer[0].message) == 0x80)
					unplonk(pow(0.943875f, 0x3c - Pm_MessageData1(buffer[0].message)));
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
					say("Filter clear");
					gEcho = 0;
					break;
				case SDLK_x: 
					gBus.setFilter(0, &gFilter); 
					say("Echo filter");
					gEcho = 1;
					break;
				case SDLK_c: 
					gBQRFilter.setParams(SoLoud::BiquadResonantFilter::LOWPASS, 44100, 1000, 2);  
					gBus.setFilter(1, &gBQRFilter); 
					say("Low pass filter 1000 hz");
					break;
				case SDLK_v: 
					gBQRFilter.setParams(SoLoud::BiquadResonantFilter::LOWPASS, 44100, 500, 8);   
					gBus.setFilter(1, &gBQRFilter); 
					say("Low pass filter 500 hz");
					break;
				case SDLK_b: 
					gBQRFilter.setParams(SoLoud::BiquadResonantFilter::HIGHPASS, 44100, 1000, 8); 
					gBus.setFilter(1, &gBQRFilter); 
					say("High pass filter 1000 hz");
					break;
				case SDLK_n: 
					gBQRFilter.setParams(SoLoud::BiquadResonantFilter::BANDPASS, 44100, 1000, 1); 
					gBus.setFilter(1, &gBQRFilter); 
					say("Band pass filter 1000 hz");
					break;
				case SDLK_m: 
					gBQRFilter.setParams(SoLoud::BiquadResonantFilter::LOWPASS, 44100, 1000, 2);  
					gBus.setFilter(1, &gBQRFilter); 
					gSoloud.oscillateFilterParameter(bushandle, 1, 1, 500, 6000, 4);  
					say("Oscillating low pass filter");
					break; 				
				case SDLK_a: 
					gWaveSelect = 0;
					gWave.setWaveform(SoLoud::Basicwave::SINE); 
					say("Sine wave");
					break;
				case SDLK_s: 
					gWaveSelect = 1;
					gWave.setWaveform(SoLoud::Basicwave::TRIANGLE); 
					say("Triangle wave");
					break;
				case SDLK_d: 
					gWaveSelect = 2;
					gWave.setWaveform(SoLoud::Basicwave::SQUARE); 
					say("Square wave");
					break;
				case SDLK_f: 
					gWaveSelect = 3;
					gWave.setWaveform(SoLoud::Basicwave::SAW); 
					say("Saw wave");
					break;
				case SDLK_g: 
					gWaveSelect = 4;
					say("Looping sample");
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
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					{
						gSoloud.deinit();
						return 0;
					}
					break;
				case SDLK_p: unplonk(1); break;                  // C
				case SDLK_o: unplonk(pow(0.943875f, 1)); break;  // B
				case SDLK_9: unplonk(pow(0.943875f, 2)); break;  //  A#
				case SDLK_i: unplonk(pow(0.943875f, 3)); break;  // A
				case SDLK_8: unplonk(pow(0.943875f, 4)); break;  //  G#
				case SDLK_u: unplonk(pow(0.943875f, 5)); break;  // G
				case SDLK_7: unplonk(pow(0.943875f, 6)); break;  //  F#
				case SDLK_y: unplonk(pow(0.943875f, 7)); break;  // F
				case SDLK_t: unplonk(pow(0.943875f, 8)); break;  // E
				case SDLK_5: unplonk(pow(0.943875f, 9)); break;  //  D#
				case SDLK_r: unplonk(pow(0.943875f, 10)); break; // D
				case SDLK_4: unplonk(pow(0.943875f, 11)); break; //  C#
				case SDLK_e: unplonk(pow(0.943875f, 12)); break; // C
				case SDLK_w: unplonk(pow(0.943875f, 13)); break; // B
				case SDLK_2: unplonk(pow(0.943875f, 14)); break; //  A#
				case SDLK_q: unplonk(pow(0.943875f, 15)); break; // A
				case SDLK_1: unplonk(pow(0.943875f, 16)); break; //  G#
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