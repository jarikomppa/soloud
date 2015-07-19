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
#include "soloud_speech.h"
#include "soloud_modplug.h"
#include "soloud_flangerfilter.h"
#include "soloud_lofifilter.h"
#include "soloud_biquadresonantfilter.h"

SoLoud::Soloud gSoloud;			// SoLoud engine core
SoLoud::Speech gSpeech;
SoLoud::Modplug gMod;

SoLoud::Bus gMusicBus;
SoLoud::Bus gSpeechBus;

SoLoud::FlangerFilter gFlanger;
SoLoud::LofiFilter gLofi;
SoLoud::BiquadResonantFilter gReso;

unsigned int gAlien = 0;

int gSpeechhandle = 0;

int gLastloop = 0;
int gTickofs = 0;

int DemoEntry(int argc, char *argv[])
{
	gSoloud.init();
	gSoloud.setVisualizationEnable(1);
	gSoloud.setGlobalVolume(3);
	gSoloud.setPostClipScaler(0.75);

	gSoloud.play(gSpeechBus);
	gSoloud.play(gMusicBus);

	gAlien = DemoLoadTexture("graphics/alien.png");

	gSpeech.setFilter(1, &gFlanger);
	gSpeech.setFilter(0, &gLofi);
	gSpeech.setFilter(2, &gReso);
	gLofi.setParams(8000, 4);
	gFlanger.setParams(0.002f, 100);
	//	gReso.setParams(SoLoud::BiquadResonantFilter::LOWPASS, 8000, 500, 5);
	gReso.setParams(SoLoud::BiquadResonantFilter::BANDPASS, 8000, 1000, 0.5);

	gSpeech.setText("What the alien has to say might\n"
		"appear around here if this\n"
		"wasn't just a dummy mockup..\n"
		"\n..........\n"
		"This is a demo of getting\n"
		"visualization data from different\n"
		"parts of the audio pipeline."
		"\n..........\n"
		"\n..........\n"
		"\n..........\n");
	gSpeech.setLooping(1);

	gSpeechhandle = gSpeechBus.play(gSpeech, 3, -0.25);
	gSoloud.setRelativePlaySpeed(gSpeechhandle, 1.2f);

	gSoloud.oscillateFilterParameter(gSpeechhandle, 0, SoLoud::LofiFilter::SAMPLERATE, 2000, 8000, 4);


	gMod.load("audio/BRUCE.S3M");
	gMusicBus.play(gMod);

	gSpeechBus.setVisualizationEnable(1);
	gMusicBus.setVisualizationEnable(1);

	return 0;
}

void DemoMainloop()
{
	int tick = DemoTick();

	DemoUpdateStart();

	float *fft = gMusicBus.calcFFT();

	ONCE(ImGui::SetNextWindowPos(ImVec2(500, 20)));
	ImGui::Begin("Output");
	ImGui::PlotHistogram("##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
	ImGui::Text("Active voices    : %d", gSoloud.getActiveVoiceCount());
	ImGui::Text(
		"Active voices include 2\n"
		"audio busses, music and\n"
		"speech.");
	ImGui::End();

	ONCE(ImGui::SetNextWindowPos(ImVec2(20, 20)));
	ONCE(ImGui::SetNextWindowSize(ImVec2(280, 360)));
	ImGui::Begin("Alien");
	float *buf = gSpeechBus.getWave();
	ImGui::Image((ImTextureID) gAlien,ImVec2(128,128));
	ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
	int loop = gSoloud.getLoopCount(gSpeechhandle);
	if (loop != gLastloop)
	{
		gLastloop = loop;
		gTickofs = tick;
	}

	char *s = "What the alien has to say might\n"
		"appear around here if this\n"
		"wasn't just a dummy mockup..\n"
		"\n       \n"
		"This is a demo of getting\n"
		"visualization data from different\n"
		"parts of the audio pipeline.";
	char temp[512];
	int i = 0;
	while (s[i] && i < (tick - gTickofs) / 70)
	{
		temp[i] = s[i];
		i++;
	}
	temp[i] = 0;
	ImGui::Text(temp);
	ImGui::End();

	DemoUpdateEnd();
}
