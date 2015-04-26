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
#include "soloud_tedsid.h"
#include "soloud_fftfilter.h"
#include "soloud_biquadresonantfilter.h"
#include "soloud_echofilter.h"
#include "soloud_dcremovalfilter.h"

SoLoud::Soloud gSoloud;
SoLoud::TedSid gMusic1, gMusic2;
SoLoud::BiquadResonantFilter gBiquad;
SoLoud::FFTFilter gFFT;
SoLoud::EchoFilter gEcho;
SoLoud::DCRemovalFilter gDCRemoval;
int gMusichandle1, gMusichandle2;

// Entry point
int main(int argc, char *argv[])
{
	DemoInit();

	gMusic1.load("audio/Modulation.sid.dump");
	gMusic2.load("audio/ted_storm.prg.dump");

	gEcho.setParams(0.2f, 0.5f, 0.05f);
	gBiquad.setParams(SoLoud::BiquadResonantFilter::LOWPASS, 44100, 4000, 2);

	gMusic1.setLooping(1);	
	gMusic2.setLooping(1);

	gSoloud.setGlobalFilter(0, &gBiquad);
	gSoloud.setGlobalFilter(1, &gFFT);
	gSoloud.setGlobalFilter(2, &gEcho);
	gSoloud.setGlobalFilter(3, &gDCRemoval);

	gSoloud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);

	gMusichandle1 = gSoloud.play(gMusic1);
	gMusichandle2 = gSoloud.play(gMusic2, 0);

	float filter_param0[4] = { 0, 0, 0, 0 };
	float filter_param1[4] = { 1000, 0, 0, 0 };
	float filter_param2[4] = { 2, 0,  0, 0 };
	
	float song1volume = 1;
	float song2volume = 0;

	// Main loop: loop forever.
	while (1)
	{
		gSoloud.setFilterParameter(0, 0, 0, filter_param0[0]);
		gSoloud.setFilterParameter(0, 1, 0, filter_param0[1]);
		gSoloud.setFilterParameter(0, 2, 0, filter_param0[2]);
		gSoloud.setFilterParameter(0, 3, 0, filter_param0[3]);

		gSoloud.setFilterParameter(0, 0, 2, filter_param1[0]);
		gSoloud.setFilterParameter(0, 0, 3, filter_param2[0]);

		DemoUpdateStart();

		float *buf = gSoloud.getWave();
		float *fft = gSoloud.calcFFT();

		ONCE(ImGui::SetNextWindowPos(ImVec2(500, 20)));
		ImGui::Begin("Output");
		ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264,80));
		ImGui::PlotHistogram("##FFT", fft, 256/2, 0, "FFT", 0, 10, ImVec2(264,80),8);
		ImGui::Text("Active voices    : %d", gSoloud.getActiveVoiceCount());
		ImGui::Text("SID: %02X %02X %02X %02X %02X %02X %02X %02X", (int)gSoloud.getInfo(gMusichandle1, 0), (int)gSoloud.getInfo(gMusichandle1, 1), (int)gSoloud.getInfo(gMusichandle1, 2), (int)gSoloud.getInfo(gMusichandle1, 3), (int)gSoloud.getInfo(gMusichandle1, 4), (int)gSoloud.getInfo(gMusichandle1, 5), (int)gSoloud.getInfo(gMusichandle1, 6), (int)gSoloud.getInfo(gMusichandle1, 7));
		ImGui::Text("     %02X %02X %02X %02X %02X %02X %02X %02X", (int)gSoloud.getInfo(gMusichandle1, 8), (int)gSoloud.getInfo(gMusichandle1, 9), (int)gSoloud.getInfo(gMusichandle1,10), (int)gSoloud.getInfo(gMusichandle1,11), (int)gSoloud.getInfo(gMusichandle1,12), (int)gSoloud.getInfo(gMusichandle1,13), (int)gSoloud.getInfo(gMusichandle1,14), (int)gSoloud.getInfo(gMusichandle1,15));
		ImGui::Text("     %02X %02X %02X %02X %02X %02X %02X %02X", (int)gSoloud.getInfo(gMusichandle1,16), (int)gSoloud.getInfo(gMusichandle1,17), (int)gSoloud.getInfo(gMusichandle1,18), (int)gSoloud.getInfo(gMusichandle1,19), (int)gSoloud.getInfo(gMusichandle1,20), (int)gSoloud.getInfo(gMusichandle1,21), (int)gSoloud.getInfo(gMusichandle1,22), (int)gSoloud.getInfo(gMusichandle1,23));
		ImGui::Text("     %02X %02X %02X %02X %02X %02X %02X %02X", (int)gSoloud.getInfo(gMusichandle1,24), (int)gSoloud.getInfo(gMusichandle1,25), (int)gSoloud.getInfo(gMusichandle1,26), (int)gSoloud.getInfo(gMusichandle1,27), (int)gSoloud.getInfo(gMusichandle1,28), (int)gSoloud.getInfo(gMusichandle1,29), (int)gSoloud.getInfo(gMusichandle1,30), (int)gSoloud.getInfo(gMusichandle1,31));
		ImGui::Text("TED: %02X %02X %02X %02X %02X", (int)gSoloud.getInfo(gMusichandle2, 64), (int)gSoloud.getInfo(gMusichandle2, 65), (int)gSoloud.getInfo(gMusichandle2, 66), (int)gSoloud.getInfo(gMusichandle2, 67), (int)gSoloud.getInfo(gMusichandle2, 68));
		ImGui::End();

		ONCE(ImGui::SetNextWindowPos(ImVec2(20, 20)));
		ImGui::Begin("Control");
		ImGui::Text("Song volumes");
		
		if (ImGui::SliderFloat("Song1 vol", &song1volume, 0, 1))
		{
			gSoloud.setVolume(gMusichandle1, song1volume);
		}
		if (ImGui::SliderFloat("Song2 vol", &song2volume, 0, 1))
		{
			gSoloud.setVolume(gMusichandle2, song2volume);
		}
		ImGui::Separator();
		ImGui::Text("Biquad filter (lowpass)");
		ImGui::SliderFloat("Wet##4", &filter_param0[0], 0, 1);
		ImGui::SliderFloat("Frequency##4", &filter_param1[0], 0, 8000);
		ImGui::SliderFloat("Resonance##4", &filter_param2[0], 1, 20);
		ImGui::Separator();
		ImGui::Text("FFT filter");
		ImGui::SliderFloat("Wet##2", &filter_param0[1], 0, 1);
		ImGui::Separator();
		ImGui::Text("Echo filter");
		ImGui::SliderFloat("Wet##3", &filter_param0[2], 0, 1);
		ImGui::Separator();
		ImGui::Text("DC removal filter");
		ImGui::SliderFloat("Wet##1", &filter_param0[3], 0, 1);
		ImGui::End();
		DemoUpdateEnd();
	}
	return 0;
}