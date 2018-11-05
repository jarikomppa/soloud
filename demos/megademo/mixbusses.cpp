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
#include "soloud_speech.h"

namespace mixbusses
{

	SoLoud::Soloud gSoloud;			// SoLoud engine core
	SoLoud::Speech gSpeech[10];
	SoLoud::Wav gSfxloop, gMusicloop;
	SoLoud::Bus gSfxbus, gMusicbus, gSpeechbus;

	int gSfxbusHandle, gMusicbusHandle, gSpeechbusHandle;

	float gSfxvol = 1, gMusicvol = 1, gSpeechvol = 1;

	int DemoEntry(int argc, char *argv[])
	{
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
		return 0;
	}

	int speechtick = 0;
	int speechcount = 0;

	void DemoMainloop()
	{
		DemoUpdateStart();

		if (speechtick < DemoTick())
		{
			int h = gSpeechbus.play(gSpeech[speechcount % 10], (rand() % 200) / 50.0f + 2, (rand() % 20) / 10.0f - 1);
			speechcount++;
			gSoloud.setRelativePlaySpeed(h, (rand() % 100) / 200.0f + 0.75f);
			gSoloud.fadePan(h, (rand() % 20) / 10.0f - 1, 2);
			speechtick = DemoTick() + 4000;
		}

		float *buf = gSoloud.getWave();
		float *fft = gSoloud.calcFFT();

		ONCE(ImGui::SetNextWindowPos(ImVec2(500, 20)));
		ImGui::Begin("Output");
		ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
		ImGui::PlotHistogram("##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
		ImGui::Text("Speech bus volume : %d%%", (int)floor(gSoloud.getVolume(gSpeechbusHandle) * 100));
		ImGui::Text("Music bus volume  : %d%%", (int)floor(gSoloud.getVolume(gMusicbusHandle) * 100));
		ImGui::Text("Sfx bus volume    : %d%%", (int)floor(gSoloud.getVolume(gSfxbusHandle) * 100));
		ImGui::Text("Active voices     : %d", gSoloud.getActiveVoiceCount());
		ImGui::End();

		ONCE(ImGui::SetNextWindowPos(ImVec2(20, 20)));
		ImGui::Begin("Control");
		if (ImGui::SliderFloat("Speech bus volume", &gSpeechvol, 0, 2))
		{
			gSoloud.setVolume(gSpeechbusHandle, gSpeechvol);
		}
		if (ImGui::SliderFloat("Music bus volume", &gMusicvol, 0, 2))
		{
			gSoloud.setVolume(gMusicbusHandle, gMusicvol);
		}
		if (ImGui::SliderFloat("Sfx bus volume", &gSfxvol, 0, 2))
		{
			gSoloud.setVolume(gSfxbusHandle, gSfxvol);
		}
		ImGui::End();
		DemoUpdateEnd();
	}
}

int DemoEntry_mixbusses(int argc, char *argv[])
{
	return mixbusses::DemoEntry(argc, argv);
}

void DemoMainloop_mixbusses()
{
	return mixbusses::DemoMainloop();
}