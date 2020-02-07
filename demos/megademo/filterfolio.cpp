/*
SoLoud audio engine
Copyright (c) 2013-2020 Jari Komppa

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
#include "soloud_speech.h"
#include "soloud_noise.h"

#include "soloud_bassboostfilter.h"
#include "soloud_biquadresonantfilter.h"
#include "soloud_dcremovalfilter.h"
#include "soloud_echofilter.h"
#include "soloud_fftfilter.h"
#include "soloud_flangerfilter.h"
#include "soloud_freeverbfilter.h"
#include "soloud_lofifilter.h"
#include "soloud_robotizefilter.h"
#include "soloud_waveshaperfilter.h"

namespace filterfolio
{
	SoLoud::Soloud gSoloud;
	SoLoud::Sfxr gSfx;
	SoLoud::Speech gSpeech;
	SoLoud::WavStream gMusic1, gMusic2, gMusic3;
	int gMusichandle1, gMusichandle2, gMusichandle3;
	SoLoud::Filter *gFilter[10];
	SoLoud::Noise gNoise;
	int gNoisehandle;

	int gFilterSelect[4] = { 0, 0, 0, 0 };

	int DemoEntry(int argc, char* argv[])
	{
		gMusic1.load("audio/plonk_wet.ogg");
		gMusic2.load("audio/delphi_loop.ogg");
		gMusic3.load("audio/tetsno.ogg");

		gMusic1.setLooping(1);
		gMusic2.setLooping(1);
		gMusic3.setLooping(1);

		gSoloud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);

		gMusichandle1 = gSoloud.play(gMusic1);
		gMusichandle2 = gSoloud.play(gMusic2, 0);
		gMusichandle3 = gSoloud.play(gMusic3, 0);
		gSoloud.setProtectVoice(gMusichandle1, 1);
		gSoloud.setProtectVoice(gMusichandle2, 1);
		gSoloud.setProtectVoice(gMusichandle3, 1);

		gNoisehandle = gSoloud.play(gNoise, 0);

		gFilter[0] = new SoLoud::BassboostFilter;
		gFilter[1] = new SoLoud::BiquadResonantFilter;
		gFilter[2] = new SoLoud::DCRemovalFilter;
		gFilter[3] = new SoLoud::EchoFilter;
		gFilter[4] = new SoLoud::FFTFilter;
		gFilter[5] = new SoLoud::FlangerFilter;
		gFilter[6] = new SoLoud::FreeverbFilter;
		gFilter[7] = new SoLoud::LofiFilter;
		gFilter[8] = new SoLoud::RobotizeFilter;
		gFilter[9] = new SoLoud::WaveShaperFilter;

		gSpeech.setText("My banana is yellow");

		return 0;
	}

	void DemoMainloop()
	{
		DemoUpdateStart();

		float* buf = gSoloud.getWave();
		float* fft = gSoloud.calcFFT();

		ONCE(ImGui::SetNextWindowPos(ImVec2(500, 20)));
		ImGui::Begin("Sounds");
		ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
		ImGui::PlotHistogram("##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
		ImGui::Text("Active voices    : %d", gSoloud.getActiveVoiceCount());
		ImGui::End();

		ONCE(ImGui::SetNextWindowPos(ImVec2(20, 20)));
		ImGui::Begin("Control");
		bool b = gSoloud.getVolume(gMusichandle1) > 0.5;
		if (ImGui::Checkbox("Toggle Music 1", &b))
		{
			if (gSoloud.getVolume(gMusichandle1) > 0.5)
				gSoloud.fadeVolume(gMusichandle1, 0, 0.5);
			else
				gSoloud.fadeVolume(gMusichandle1, 1, 0.5);
		}
		ImGui::SameLine();
		if (ImGui::Button("SFXR EXPLOSION"))
		{
			gSfx.loadPreset(SoLoud::Sfxr::EXPLOSION, rand());
			gSoloud.play(gSfx, 2, ((rand() % 512) - 256) / 256.0f);
		}
		ImGui::SameLine();
		if (ImGui::Button("SFXR BLIP"))
		{
			gSfx.loadPreset(SoLoud::Sfxr::BLIP, rand());
			gSoloud.play(gSfx, 2, ((rand() % 512) - 256) / 256.0f);
		}
		ImGui::SameLine();
		if (ImGui::Button("SFXR COIN"))
		{
			gSfx.loadPreset(SoLoud::Sfxr::COIN, rand());
			gSoloud.play(gSfx, 2, ((rand() % 512) - 256) / 256.0f);
		}

		b = gSoloud.getVolume(gMusichandle2) > 0.5;
		if (ImGui::Checkbox("Toggle Music 2", &b))
		{
			if (gSoloud.getVolume(gMusichandle2) > 0.5)
				gSoloud.fadeVolume(gMusichandle2, 0, 0.5);
			else
				gSoloud.fadeVolume(gMusichandle2, 1, 0.5);
		}
		ImGui::SameLine();
		if (ImGui::Button("SFXR HURT"))
		{
			gSfx.loadPreset(SoLoud::Sfxr::HURT, rand());
			gSoloud.play(gSfx, 2, ((rand() % 512) - 256) / 256.0f);
		}
		ImGui::SameLine();
		if (ImGui::Button("SFXR JUMP"))
		{
			gSfx.loadPreset(SoLoud::Sfxr::JUMP, rand());
			gSoloud.play(gSfx, 2, ((rand() % 512) - 256) / 256.0f);
		}
		ImGui::SameLine();
		if (ImGui::Button("SFXR LASER"))
		{
			gSfx.loadPreset(SoLoud::Sfxr::LASER, rand());
			gSoloud.play(gSfx, 2, ((rand() % 512) - 256) / 256.0f);
		}

		b = gSoloud.getVolume(gMusichandle3) > 0.5;
		if (ImGui::Checkbox("Toggle Music 3", &b))
		{
			if (gSoloud.getVolume(gMusichandle3) > 0.5)
				gSoloud.fadeVolume(gMusichandle3, 0, 0.5);
			else
				gSoloud.fadeVolume(gMusichandle3, 1, 0.5);
		}
		ImGui::SameLine();
		if (ImGui::Button("Speech"))
		{
			gSoloud.play(gSpeech, 1);
		}
		ImGui::SameLine();
		b = gSoloud.getVolume(gNoisehandle) > 0.5;
		if (ImGui::Checkbox("Toggle Noise", &b))
		{
			if (gSoloud.getVolume(gNoisehandle) > 0.5)
				gSoloud.fadeVolume(gNoisehandle, 0, 0.5);
			else
				gSoloud.fadeVolume(gNoisehandle, 1, 0.5);
		}


		ImGui::End();

		
		ONCE(ImGui::SetNextWindowPos(ImVec2(20, 140)));
		ONCE(ImGui::SetNextWindowSize(ImVec2(350, 250)));
		ImGui::Begin("Filters");
		for (int filterindex = 0; filterindex < 4; filterindex++)
		{
			if (filterindex != 0)
				ImGui::Separator();

			char* label[4] = { "Filter 1", "Filter 2", "Filter 3", "Filter 4" };

			if (ImGui::Combo(label[filterindex], &gFilterSelect[filterindex],
				"None\x00"
				"BassboostFilter\x00"
				"BiquadResonantFilter\x00"
				"DCRemovalFilter\x00"
				"EchoFilter\x00"
				"FFTFilter\x00"
				"FlangerFilter\x00"
				"FreeverbFilter\x00"
				"LofiFilter\x00"
				"RobotizeFilter\x00"
				"WaveShaperFilter\x00\x00"))
			{
				if (gFilterSelect[filterindex])
					gSoloud.setGlobalFilter(filterindex, gFilter[gFilterSelect[filterindex] - 1]);
				else
					gSoloud.setGlobalFilter(filterindex, 0);
			}

			if (gFilterSelect[filterindex] != 0)
			{
				SoLoud::Filter* f = gFilter[gFilterSelect[filterindex] - 1];
				int count = f->getParamCount();
				for (int i = 0; i < count; i++)
				{
					int filtertype = f->getParamType(i);
					float filtermin = f->getParamMin(i);
					float filtermax = f->getParamMax(i);

					if (filtertype == SoLoud::Filter::INT_PARAM)
					{
						int v = (int)gSoloud.getFilterParameter(0, filterindex, i);
						char temp[128];
						sprintf(temp, "%s##%d-%d", f->getParamName(i), filterindex, i);
						if (ImGui::SliderInt(temp, &v, (int)filtermin, (int)filtermax))
						{
							gSoloud.setFilterParameter(0, filterindex, i, (float)v);
						}
					}

					if (filtertype == SoLoud::Filter::FLOAT_PARAM)
					{
						float v = gSoloud.getFilterParameter(0, filterindex, i);
						char temp[128];
						sprintf(temp, "%s##%d-%d", f->getParamName(i), filterindex, i);
						if (ImGui::SliderFloat(temp, &v, filtermin, filtermax))
						{
							gSoloud.setFilterParameter(0, filterindex, i, v);
						}
					}

					if (filtertype == SoLoud::Filter::BOOL_PARAM)
					{
						float v = gSoloud.getFilterParameter(0, filterindex, i);
						bool bv = v > 0.5f;
						char temp[128];
						sprintf(temp, "%s##%d-%d", f->getParamName(i), filterindex, i);
						if (ImGui::Checkbox(temp, &bv))
							//SliderFloat(temp, &v, filtermin, filtermax))
						{
							gSoloud.setFilterParameter(0, filterindex, i, bv?1.0f:0.0f);
						}
					}
				}
			}
		}
		ImGui::End();
		
		DemoUpdateEnd();
	}
}

int DemoEntry_filterfolio(int argc, char *argv[])
{
	return filterfolio::DemoEntry(argc, argv);
}

void DemoMainloop_filterfolio()
{
	return filterfolio::DemoMainloop();
}