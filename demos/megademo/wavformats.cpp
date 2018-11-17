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
#include "soloud_wavstream.h"
#include "soloud_sfxr.h"

namespace wavformats
{
	const char *filenames[] = {
		"ch1.flac",
		"ch1.mp3",
		"ch1.ogg",
		"ch1_16bit.wav",
		"ch1_24bit.wav",
		"ch1_32bit.wav",
		"ch1_8bit.wav",
		"ch1_alaw.wav",
		"ch1_double.wav",
		"ch1_float.wav",
		"ch1_imaadpcm.wav",
		"ch1_msadpcm.wav",
		"ch1_ulaw.wav",
		"ch2.flac",
		"ch2.mp3",
		"ch2.ogg",
		"ch2_16bit.wav",
		"ch2_24bit.wav",
		"ch2_32bit.wav",
		"ch2_8bit.wav",
		"ch2_alaw.wav",
		"ch2_double.wav",
		"ch2_float.wav",
		"ch2_imaadpcm.wav",
		"ch2_msadpcm.wav",
		"ch2_ulaw.wav",
		"ch4.flac",
		"ch4.ogg",
		"ch4_16bit.wav",
		"ch4_24bit.wav",
		"ch4_32bit.wav",
		"ch4_8bit.wav",
		"ch4_alaw.wav",
		"ch4_double.wav",
		"ch4_float.wav",
		"ch4_imaadpcm.wav",
		"ch4_msadpcm.wav",
		"ch4_ulaw.wav",
		"ch6.flac",
		"ch6.ogg",
		"ch6_16bit.wav",
		"ch6_24bit.wav",
		"ch6_32bit.wav",
		"ch6_8bit.wav",
		"ch6_alaw.wav",
		"ch6_double.wav",
		"ch6_float.wav",
		"ch6_imaadpcm.wav",
		"ch6_msadpcm.wav",
		"ch6_ulaw.wav",
		"ch8.flac",
		"ch8.ogg",
		"ch8_16bit.wav",
		"ch8_24bit.wav",
		"ch8_32bit.wav",
		"ch8_8bit.wav",
		"ch8_alaw.wav",
		"ch8_double.wav",
		"ch8_float.wav",
		"ch8_imaadpcm.wav",
		"ch8_msadpcm.wav",
		"ch8_ulaw.wav"
	};
	int files = sizeof(filenames) / sizeof(char*);
	SoLoud::Soloud gSoloud;
	SoLoud::Wav *gWav;
	bool *gWavOk;
	SoLoud::WavStream *gWavstream;
	bool *gWavStreamOk;


	int DemoEntry(int argc, char *argv[])
	{
		gSoloud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);

		gWav = new SoLoud::Wav[files];
		gWavstream = new SoLoud::WavStream[files];
		gWavOk = new bool[files];
		gWavStreamOk = new bool[files];

		int i;
		for (i = 0; i < files; i++)
		{
			char temp[256];
			sprintf(temp, "audio/wavformats/%s", filenames[i]);
			gWavOk[i] = gWav[i].load(temp) == SoLoud::SO_NO_ERROR;
			gWavStreamOk[i] = gWavstream[i].load(temp) == SoLoud::SO_NO_ERROR;
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
			ImGui::SameLine(200);			
			x = gWavStreamOk[i];
			ImGui::Checkbox("", &x);
			ImGui::SameLine();
			sprintf(temp, "Stream %s", filenames[i]);
			if (ImGui::Button(temp)) 
			{
				gSoloud.play(gWavstream[i]);
			}
		}

		ImGui::End();
		DemoUpdateEnd();
	}
}

int DemoEntry_wavformats(int argc, char *argv[])
{
	return wavformats::DemoEntry(argc, argv);
}

void DemoMainloop_wavformats()
{
	return wavformats::DemoMainloop();
}