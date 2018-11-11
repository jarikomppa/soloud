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
#include "soloud_wav.h"

namespace speakers
{
	const char *filenames[] = {
		"ch6_1.flac",
		"ch6_2.flac",
		"ch6_3.flac",
		"ch6_4.flac",
		"ch6_5.flac",
		"ch6_6.flac",
		"ch8_1.flac",
		"ch8_2.flac",
		"ch8_3.flac",
		"ch8_4.flac",
		"ch8_5.flac",
		"ch8_6.flac",
		"ch8_7.flac",
		"ch8_8.flac"
	};
	int files = sizeof(filenames) / sizeof(char*);
	SoLoud::Soloud gSoloud;
	SoLoud::Wav *gWav;
	bool *gWavOk;
	bool *gWavStreamOk;


	int DemoEntry(int argc, char *argv[])
	{
		gSoloud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);

		gWav = new SoLoud::Wav[files];
		gWavOk = new bool[files];
		gWavStreamOk = new bool[files];

		int i;
		for (i = 0; i < files; i++)
		{
			char temp[256];
			sprintf(temp, "audio/wavformats/%s", filenames[i]);
			OutputDebugStringA(temp);
			OutputDebugStringA("\r\n");
			gWavOk[i] = gWav[i].load(temp) == SoLoud::SO_NO_ERROR;
		}

		return 0;
	}

	void DemoMainloop()
	{
		DemoUpdateStart();

		float *buf = gSoloud.getWave();
		float *fft = gSoloud.calcFFT();

		ONCE(ImGui::SetNextWindowPos(ImVec2(500, 20)));
		ImGui::Begin("Output");
		ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
		ImGui::PlotHistogram("##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
		ImGui::Text("Active voices    : %d", gSoloud.getActiveVoiceCount());
		ImGui::Text("Backend: %s Channels: %d", gSoloud.getBackendString(), gSoloud.getBackendChannels());
		ImGui::End();

		ONCE(ImGui::SetNextWindowPos(ImVec2(20, 0)));
		ImGui::Begin("Control");

		int i;
		for (i = 0; i < files; i++)
		{
			char temp[256];
			bool x = gWavOk[i];
			ImGui::Checkbox("", &x);
			ImGui::SameLine();
			sprintf(temp, "Play %s", filenames[i]);
			if (ImGui::Button(temp)) 
			{
				gSoloud.play(gWav[i]);
			}
		}

		ImGui::End();
		DemoUpdateEnd();
	}
}

int DemoEntry_speakers(int argc, char *argv[])
{
	return speakers::DemoEntry(argc, argv);
}

void DemoMainloop_speakers()
{
	return speakers::DemoMainloop();
}