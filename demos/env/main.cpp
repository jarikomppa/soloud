/*
SoLoud audio engine
Copyright (c) 2013-2015 Jari Komppa

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
#include <math.h>
#include <stdio.h>
#include "imgui.h"
#include "soloud_demo_framework.h"

#include "soloud.h"
#include "soloud_wav.h"
#include "soloud_biquadresonantfilter.h"


SoLoud::Soloud gSoloud;			// SoLoud engine core
SoLoud::BiquadResonantFilter gLPFilter;
SoLoud::Wav gRain, gWind, gMusic;

int gWalker, gBackground;
int gRainHandle, gWindHandle, gMusicHandle;

void drawbg(int x, int y)
{
	DemoTexQuad(gBackground, (float)x, (float)y + 100, (float)x + 800, (float)y + 600 + 100);
}

void drawwalker(int frame, int x, int y)
{
	DemoTexQuad(gWalker, (float)x - 12, (float)y + frame + 100, (float)x + 60 - 12, (float)y + 60 + frame + 100);
}

void drawrect(int x, int y, int w, int h, int c)
{
	DemoQuad((float)x, (float)y + 100, (float)x + w, (float)y + h + 100, c);
}

float render()
{   
	// Ask SDL for the time in milliseconds
	int tick = DemoTick();

	float p = (tick % 60000) / 60000.0f;

	//p = 0.4f;

	int ypos;
	int xpos;
	int dudey;
#define SMOOTHSTEP(x) ((x) * (x) * (3 - 2 * (x)))
	if (p < 0.1f)
	{
		xpos = 0;
		ypos = -340;
		dudey= -8;
	}
	else
	if (p < 0.5f)
	{
		float v = (p - 0.1f) * 2.5f;
		v = SMOOTHSTEP(v);
		v = SMOOTHSTEP(v);
		v = SMOOTHSTEP(v);

		xpos = -(int)floor(v * (800 - 400));
		ypos = -340;
		dudey = (int)floor((1 - v) * -8);
	}
	else
	if (p < 0.9f)
	{
		float v = (p - 0.5f) * 2.5f;
		v = SMOOTHSTEP(v);
		v = SMOOTHSTEP(v);
		v = SMOOTHSTEP(v);
		xpos = -(800 - 400);
		ypos = (int)floor((1 - v) * (- 340));
		dudey = (int)floor(v * 90);
	}
	else
	{
		xpos = -(800-400);
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
	
	drawbg(xpos, ypos);

	drawwalker((tick >> 7) % ((tick >> 8) % 5 + 1), (400-32)/2 + 12, 256-32*2-32-dudey);
	
	if (p > 0.5f)
	{
		int w = (int)floor((p - 0.5f) * 600);
		if (w > 32) w = 32;
		drawrect((400-32)/2+12, 256-32*2-32+ypos+340, w/2, 64, 0xffffffff);
		drawrect((400-32)/2+12+32-(w/2), 256-32*2-32+ypos+340, w/2, 64, 0xffffffff);

		drawrect((400-32)/2+12+(w/2), 256-32*2-32+ypos+340, 1, 64, 0xffaaaaaa);
		drawrect((400-32)/2+12+32-(w/2), 256-32*2-32+ypos+340, 1, 64, 0xffaaaaaa);
	}
	return p;
}


int DemoEntry(int argc, char *argv[])
{
	gSoloud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);
	gSoloud.setGlobalVolume(0.75);
	gSoloud.setPostClipScaler(0.75);

	gRain.load("audio/rainy_ambience.ogg");
	gRain.setLooping(1);
	gWind.load("audio/windy_ambience.ogg");
	gWind.setLooping(1);
	gMusic.load("audio/tetsno.ogg");
	gMusic.setLooping(1);
	gLPFilter.setParams(SoLoud::BiquadResonantFilter::LOWPASS, 44100, 100, 10);
	gMusic.setFilter(0, &gLPFilter);

	gRainHandle = gSoloud.play(gRain, 1);
	gWindHandle = gSoloud.play(gWind, 0);
	gMusicHandle = gSoloud.play(gMusic, 0);

	gWalker = DemoLoadTexture("graphics/env_walker.png");
	gBackground = DemoLoadTexture("graphics/env_bg.png");
	return 0;
}

void DemoMainloop()
{
	DemoUpdateStart();

	float p = render();

	float *buf = gSoloud.getWave();
	float *fft = gSoloud.calcFFT();

	ONCE(ImGui::SetNextWindowPos(ImVec2(500, 20)));
	ImGui::Begin("Output");
	ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
	ImGui::PlotHistogram("##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
	ImGui::Text("Active voices    : %d", gSoloud.getActiveVoiceCount());

	ImGui::Text("Progress         : %3.3f%%", 100 * p);
	ImGui::Text("Rain volume      : %3.3f", gSoloud.getVolume(gRainHandle));
	ImGui::Text("Music volume     : %3.3f", gSoloud.getVolume(gMusicHandle));
	ImGui::Text("Wind volume      : %3.3f", gSoloud.getVolume(gWindHandle));
	ImGui::Text("Music pan        : %3.3f", gSoloud.getPan(gMusicHandle));
	ImGui::Text("Music filter wet : %3.3f", gSoloud.getFilterParameter(gMusicHandle, 0, SoLoud::BiquadResonantFilter::WET));
	ImGui::Text("Music filter freq: %3.3f", gSoloud.getFilterParameter(gMusicHandle, 0, SoLoud::BiquadResonantFilter::FREQUENCY));
	ImGui::End();

	DemoUpdateEnd();
}
