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

#define VOICEGRID 24
#define VOICES VOICEGRID*VOICEGRID
SoLoud::Soloud gSoloud;
SoLoud::Sfxr gSfx[VOICES];

int gSndHandle[VOICES];

// Entry point
int main(int argc, char *argv[])
{
	DemoInit();

	gSoloud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);
	gSoloud.setGlobalVolume(4);
	gSoloud.setMaxActiveVoiceCount(16);
	int i;
	for (i = 0; i < VOICES; i++)
	{
		gSfx[i].loadPreset(SoLoud::Sfxr::COIN, i);
		gSfx[i].setLooping(1);
		gSfx[i].set3dMinMaxDistance(1, 100);
		gSfx[i].set3dAttenuation(SoLoud::AudioSource::LINEAR_DISTANCE, 1);
	}
	int j;
	for (i = 0; i < VOICEGRID; i++)
	{
		for (j = 0; j < VOICEGRID; j++)
		{
			gSndHandle[i * VOICEGRID + j] = gSoloud.play3d(gSfx[i * VOICEGRID + j], i * 15 + 20, 0, j * 15 + 20);
		}
	}

	// Main loop: loop forever.
	while (1)
	{
		DemoUpdateStart();

		float tick = DemoTick() / 1000.0f;

		gSoloud.set3dListenerParameters(
			gMouseX, 0, gMouseY,
			0, 0, 1, 
			0, 1, 0);

		gSoloud.update3dAudio();

		for (i = 0; i < VOICEGRID; i++)
		{
			for (j = 0; j < VOICEGRID; j++)
			{
				float v = gSoloud.getVolume(gSndHandle[i * VOICEGRID + j]);
				DemoTriangle(
					i * 15 + 20, j * 15 + 20,
					i * 15 + 20 - 5, j * 15 + 20 + 5,
					i * 15 + 20 + 5, j * 15 + 20 + 5,
					0xff000000 | (int)(v * 0xff) * 0x010101);
			}
		}


		/*
		DemoTriangle(
			5 + 400 + mousex, 5 + 200 + mousez - 10,
			5 + 400 + mousex - 10, 5 + 200 + mousez + 10,
			5 + 400 + mousex + 10, 5 + 200 + mousez + 10,
			0x77000000);
		DemoTriangle(
			400 + mousex, 200 + mousez - 10,
			400 + mousex - 10, 200 + mousez + 10,
			400 + mousex + 10, 200 + mousez + 10,
			0xff00ffff);
			*/
		float *buf = gSoloud.getWave();
		float *fft = gSoloud.calcFFT();

		ONCE(ImGui::SetNextWindowPos(ImVec2(500, 20)));
		ImGui::Begin("Output");
		ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
		ImGui::PlotHistogram("##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
		ImGui::Text("Active voices    : %d", gSoloud.getActiveVoiceCount());
		ImGui::End();

		DemoUpdateEnd();
	}
	return 0;
}