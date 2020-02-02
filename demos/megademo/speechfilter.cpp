/*
SoLoud audio engine
Copyright (c) 2013-2018 Jari Komppa

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
#include "soloud_vizsn.h"
#include "soloud_lofifilter.h"
#include "soloud_biquadresonantfilter.h"
#include "soloud_echofilter.h"
#include "soloud_dcremovalfilter.h"
#include "soloud_waveshaperfilter.h"
#include "soloud_robotizefilter.h"

namespace speechfilter
{

	SoLoud::Soloud gSoloud;
	SoLoud::Speech gSpeech;
	SoLoud::Vizsn gVizsn;
	SoLoud::BiquadResonantFilter gBiquad;
	SoLoud::LofiFilter gLofi;
	SoLoud::EchoFilter gEcho;
	SoLoud::WaveShaperFilter gWaveShaper;
	SoLoud::DCRemovalFilter gDCRemoval;
	SoLoud::RobotizeFilter gRobotize;
	SoLoud::Bus gBus;
	int gSpeechhandle;
	int gVizsnhandle;
	int gBushandle;

	float filter_param0[6] = { 0, 0, 0, 0, 0, 0 };
	float filter_param1[6] = { 1000, 8000, 0, 0, 0 ,0 };
	float filter_param2[6] = { 2, 3,  0, 0, 0, 0 };

	int hwchannels = 4;
	int waveform = 0;

	float basefreq = 1330;
	float basespeed = 10;
	float basedeclination = 0.5;
	int basewaveform = KW_SAW;

	void initspeech()
	{
		gVizsn.setText(
			"The beige hue on the waters of the loch impressed all, including the French queen, before she heard that symphony again, just as young Arthur wanted. "
			"Are those shy Eurasian footwear, cowboy chaps, or jolly earthmoving headgear? "
			"Shaw, those twelve beige hooks are joined if I patch a young, gooey mouth. "
			"With tenure, Suzie'd have all the more leisure for yachting, but her publications are no good.");
		gSpeech.setText(
			"The beige hue on the waters of the loch impressed all, including the French queen, before she heard that symphony again, just as young Arthur wanted. "
			"Are those shy Eurasian footwear, cowboy chaps, or jolly earthmoving headgear? "
			"Shaw, those twelve beige hooks are joined if I patch a young, gooey mouth. "
			"With tenure, Suzie'd have all the more leisure for yachting, but her publications are no good.");
		gSpeech.setParams((unsigned int)floor(basefreq), basespeed, basedeclination);
		gBushandle = gSoloud.play(gBus);
		gSpeechhandle = gBus.play(gSpeech);
		gVizsnhandle = gBus.play(gVizsn, -1, 0, true);
	}

	void DemoMainloop()
	{
		gSoloud.setFilterParameter(gBushandle, 1, 0, filter_param0[0]);
		gSoloud.setFilterParameter(gBushandle, 2, 0, filter_param0[1]);
		gSoloud.setFilterParameter(gBushandle, 3, 0, filter_param0[2]);
		gSoloud.setFilterParameter(gBushandle, 4, 0, filter_param0[3]);
		gSoloud.setFilterParameter(gBushandle, 0, 0, filter_param0[4]);
		gSoloud.setFilterParameter(gBushandle, 5, 0, filter_param0[5]);

		gSoloud.setFilterParameter(gBushandle, 1, 2, filter_param1[0]);
		gSoloud.setFilterParameter(gBushandle, 1, 3, filter_param2[0]);
		gSoloud.setFilterParameter(gBushandle, 2, 1, filter_param1[1]);
		gSoloud.setFilterParameter(gBushandle, 2, 2, filter_param2[1]);

		gSoloud.setFilterParameter(gBushandle, 0, 1, filter_param1[4]);

		DemoUpdateStart();

		float *buf = gSoloud.getWave();
		float *fft = gSoloud.calcFFT();

		ONCE(ImGui::SetNextWindowPos(ImVec2(500, 20)));
		ImGui::Begin("Output");
		ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
		ImGui::PlotHistogram("##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
		ImGui::Text("Music volume     : %d%%", (int)floor(gSoloud.getVolume(gSpeechhandle) * 100));
		ImGui::Text("Active voices    : %d", gSoloud.getActiveVoiceCount());
		ImGui::End();

		ONCE(ImGui::SetNextWindowPos(ImVec2(20, 20)));
		ONCE(ImGui::SetNextWindowContentSize(ImVec2(200, 300)));
		ImGui::Begin("Filters");
		if (ImGui::CollapsingHeader("WaveShaper", (const char*)0, true, false))
		{
			ImGui::SliderFloat("Wet##5", &filter_param0[4], 0, 1);
			ImGui::SliderFloat("Amount##5", &filter_param1[4], -1, 1);
		}
		if (ImGui::CollapsingHeader("Biquad (lowpass)", (const char*)0, true, false))
		{
			ImGui::SliderFloat("Wet##4", &filter_param0[0], 0, 1);
			ImGui::SliderFloat("Frequency##4", &filter_param1[0], 0, 8000);
			ImGui::SliderFloat("Resonance##4", &filter_param2[0], 1, 20);
		}
		if (ImGui::CollapsingHeader("LoFi", (const char*)0, true, false))
		{
			ImGui::SliderFloat("Wet##2", &filter_param0[1], 0, 1);
			ImGui::SliderFloat("Rate##2", &filter_param1[1], 1000, 8000);
			ImGui::SliderFloat("Bit depth##2", &filter_param2[1], 0, 8);
		}
		if (ImGui::CollapsingHeader("Echo", (const char*)0, true, false))
		{
			ImGui::SliderFloat("Wet##3", &filter_param0[2], 0, 1);
		}
		if (ImGui::CollapsingHeader("DC removal", (const char*)0, true, false))
		{
			ImGui::SliderFloat("Wet##1", &filter_param0[3], 0, 1);
		}
		if (ImGui::CollapsingHeader("Robotize", (const char*)0, true, false))
		{
			ImGui::SliderFloat("Wet##5", &filter_param0[5], 0, 1);
		}
		ImGui::End();

		ONCE(ImGui::SetNextWindowPos(ImVec2(240, 20)));
		ONCE(ImGui::SetNextWindowContentSize(ImVec2(200, 300)));
		ImGui::Begin("Speech params");
		int changed = 0;
		if (ImGui::CollapsingHeader("Engine", (const char*)0, true, false))
		{
			if (ImGui::RadioButton("Speech", gSoloud.getPause(gSpeechhandle) == false))
			{
				gSoloud.setPause(gVizsnhandle, true);
				gSoloud.setPause(gSpeechhandle, false);
			}
			if (ImGui::RadioButton("Vizsn", gSoloud.getPause(gVizsnhandle) == false))
			{
				gSoloud.setPause(gVizsnhandle, false);
				gSoloud.setPause(gSpeechhandle, true);
			}
		}
		if (ImGui::CollapsingHeader("Waveform", (const char*)0, true, false))
		{
			if (ImGui::RadioButton("Sin", basewaveform == KW_SIN))
			{
				basewaveform = KW_SIN;
				changed = 1;
			}
			if (ImGui::RadioButton("Triangle", basewaveform == KW_TRIANGLE))
			{
				basewaveform = KW_TRIANGLE;
				changed = 1;
			}
			if (ImGui::RadioButton("Saw", basewaveform == KW_SAW))
			{
				basewaveform = KW_SAW;
				changed = 1;
			}
			if (ImGui::RadioButton("Square", basewaveform == KW_SQUARE))
			{
				basewaveform = KW_SQUARE;
				changed = 1;
			}
			if (ImGui::RadioButton("Pulse", basewaveform == KW_PULSE))
			{
				basewaveform = KW_PULSE;
				changed = 1;
			}
			if (ImGui::RadioButton("Warble", basewaveform == KW_WARBLE))
			{
				basewaveform = KW_WARBLE;
				changed = 1;
			}
			if (ImGui::RadioButton("Noise", basewaveform == KW_NOISE))
			{
				basewaveform = KW_NOISE;
				changed = 1;
			}
		}
		if (ImGui::CollapsingHeader("Base params", (const char*)0, true, false))
		{
			if (ImGui::SliderFloat("Base freq", &basefreq, 0, 3000)) changed = 1;
			if (ImGui::SliderFloat("Base speed", &basespeed, 0.1f, 30)) changed = 1;
			if (ImGui::SliderFloat("Base declination", &basedeclination, -3, 3)) changed = 1;
		}
		ImGui::End();
		if (changed)
		{
			gSpeech.setParams((unsigned int)floor(basefreq), basespeed, basedeclination, basewaveform);
		}

		DemoUpdateEnd();
	}



	int DemoEntry(int argc, char *argv[])
	{
		gEcho.setParams(0.2f, 0.5f, 0.05f);
		gBiquad.setParams(SoLoud::BiquadResonantFilter::LOWPASS, 4000, 2);

		gSpeech.setLooping(1);
		gVizsn.setLooping(1);
		gBus.setFilter(0, &gWaveShaper);
		gBus.setFilter(1, &gBiquad);
		gBus.setFilter(2, &gLofi);
		gBus.setFilter(3, &gEcho);
		gBus.setFilter(4, &gDCRemoval);
		gBus.setFilter(5, &gRobotize);

		gSoloud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);

		initspeech();

		return 0;
	}
}

int DemoEntry_speechfilter(int argc, char *argv[])
{
	return speechfilter::DemoEntry(argc, argv);
}

void DemoMainloop_speechfilter()
{
	return speechfilter::DemoMainloop();
}