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
#include "soloud_sfxr.h"
#include "soloud_speech.h"

namespace virtualvoices
{

#define VOICEGRID 24
#define VOICES (VOICEGRID * VOICEGRID)
	SoLoud::Soloud gSoloud;
	SoLoud::Sfxr gSfx[VOICES];

	int gSndHandle[VOICES];

	int DemoEntry(int argc, char *argv[])
	{
		gSoloud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);
		gSoloud.setGlobalVolume(4);
		gSoloud.setMaxActiveVoiceCount(16);
		int i;
		for (i = 0; i < VOICES; i++)
		{
			gSfx[i].loadPreset(SoLoud::Sfxr::COIN, i);
			gSfx[i].setLooping(1);
			gSfx[i].setInaudibleBehavior(false, false); // make sure we don't kill inaudible sounds
			gSfx[i].set3dMinMaxDistance(1, 100);
			gSfx[i].set3dAttenuation(SoLoud::AudioSource::LINEAR_DISTANCE, 1);
		}
		int j;
		for (i = 0; i < VOICEGRID; i++)
		{
			for (j = 0; j < VOICEGRID; j++)
			{
				gSndHandle[i * VOICEGRID + j] = gSoloud.play3d(gSfx[i * VOICEGRID + j], i * 15 + 20.0f, 0, j * 15 + 20.0f);
			}
		}
		return 0;
	}

	void DemoMainloop()
	{
		int i, j;
		DemoUpdateStart();

		float tick = DemoTick() / 1000.0f;

		gSoloud.set3dListenerParameters(
			(float)gMouseX, 0, (float)gMouseY,
			0, 0, 0,
			0, 1, 0);

		gSoloud.update3dAudio();

		for (i = 0; i < VOICEGRID; i++)
		{
			for (j = 0; j < VOICEGRID; j++)
			{
				float v = gSoloud.getOverallVolume(gSndHandle[i * VOICEGRID + j]);
				DemoTriangle(
					i * 15 + 20.0f, j * 15 + 20.0f,
					i * 15 + 20.0f - 5, j * 15 + 20.0f + 5,
					i * 15 + 20.0f + 5, j * 15 + 20.0f + 5,
					0xff000000 | (int)(v * 0xff) * 0x010101);
				DemoTriangle(
					i * 15 + 20.0f, j * 15 + 20.0f + 10,
					i * 15 + 20.0f - 5, j * 15 + 20.0f + 5,
					i * 15 + 20.0f + 5, j * 15 + 20.0f + 5,
					0xff000000 | (int)(v * 0xff) * 0x010101);
			}
		}

		float *buf = gSoloud.getWave();
		float *fft = gSoloud.calcFFT();

		ONCE(ImGui::SetNextWindowPos(ImVec2(500, 20)));
		ImGui::Begin("Output");
		ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
		ImGui::PlotHistogram("##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
		ImGui::Text("Active voices    : %d", gSoloud.getActiveVoiceCount());
		ImGui::Text("Total voices     : %d", gSoloud.getVoiceCount());
		ImGui::Text("Maximum voices   : %d", VOICE_COUNT);
		ImGui::End();

		DemoUpdateEnd();
	}
}


int DemoEntry_virtualvoices(int argc, char *argv[])
{
	return virtualvoices::DemoEntry(argc, argv);
}

void DemoMainloop_virtualvoices()
{
	return virtualvoices::DemoMainloop();
}