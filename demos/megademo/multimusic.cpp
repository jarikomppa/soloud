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
#include "soloud_wavstream.h"
#include "soloud_sfxr.h"

namespace multimusic
{
	SoLoud::Soloud gSoloud;
	SoLoud::Sfxr gSfx;
	SoLoud::WavStream gMusic1, gMusic2;
	int gMusichandle1, gMusichandle2;

	int DemoEntry(int argc, char *argv[])
	{
		gMusic1.load("audio/plonk_wet.ogg");
		gMusic2.load("audio/plonk_dry.ogg");

		gMusic1.setLooping(1);
		gMusic2.setLooping(1);

		gSoloud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);

		gMusichandle1 = gSoloud.play(gMusic1, 0, 0, 1);
		gMusichandle2 = gSoloud.play(gMusic2, 1, 0, 1);

		SoLoud::handle grouphandle = gSoloud.createVoiceGroup();
		gSoloud.addVoiceToGroup(grouphandle, gMusichandle1);
		gSoloud.addVoiceToGroup(grouphandle, gMusichandle2);

		gSoloud.setProtectVoice(grouphandle, 1); // protect all voices in group 
		gSoloud.setPause(grouphandle, 0);        // unpause all voices in group 

		gSoloud.destroyVoiceGroup(grouphandle); // remove group, leaves voices alone
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
		ImGui::Text("Music1 volume    : %d%%", (int)floor(gSoloud.getVolume(gMusichandle1) * 100));
		ImGui::Text("Music2 volume    : %d%%", (int)floor(gSoloud.getVolume(gMusichandle2) * 100));
		ImGui::Text("Music rel. speed : %d%%", (int)floor(gSoloud.getRelativePlaySpeed(gMusichandle2) * 100));
		ImGui::Text("Music position   : %d%%", (int)floor(gSoloud.getStreamPosition(gMusichandle2) * 100 / gMusic1.getLength()));
		ImGui::Text("Active voices    : %d", gSoloud.getActiveVoiceCount());
		ImGui::End();

		ONCE(ImGui::SetNextWindowPos(ImVec2(20, 20)));
		ImGui::Begin("Control");

		if (ImGui::Button("Fade to music 1"))
		{
			gSoloud.fadeVolume(gMusichandle1, 1, 2);
			gSoloud.fadeVolume(gMusichandle2, 0, 2);
		}
		if (ImGui::Button("Fade to music 2"))
		{
			gSoloud.fadeVolume(gMusichandle2, 1, 2);
			gSoloud.fadeVolume(gMusichandle1, 0, 2);
		}
		if (ImGui::Button("Fade music out"))
		{
			gSoloud.fadeVolume(gMusichandle2, 0, 2);
			gSoloud.fadeVolume(gMusichandle1, 0, 2);
		}
		if (ImGui::Button("Fade music speed down"))
		{
			gSoloud.fadeRelativePlaySpeed(gMusichandle1, 0.2f, 5);
			gSoloud.fadeRelativePlaySpeed(gMusichandle2, 0.2f, 5);
		}
		if (ImGui::Button("Fade music speed to normal"))
		{
			gSoloud.fadeRelativePlaySpeed(gMusichandle1, 1, 5);
			gSoloud.fadeRelativePlaySpeed(gMusichandle2, 1, 5);
		}
		if (ImGui::Button("Fade music speed up"))
		{
			gSoloud.fadeRelativePlaySpeed(gMusichandle1, 1.5f, 5);
			gSoloud.fadeRelativePlaySpeed(gMusichandle2, 1.5f, 5);
		}

		ImGui::Separator();

		if (ImGui::Button("Play random SFXR preset EXPLOSION"))
		{
			gSfx.loadPreset(SoLoud::Sfxr::EXPLOSION, rand());
			gSoloud.play(gSfx, 2, ((rand() % 512) - 256) / 256.0f);
		}
		if (ImGui::Button("Play random SFXR preset BLIP"))
		{
			gSfx.loadPreset(SoLoud::Sfxr::BLIP, rand());
			gSoloud.play(gSfx, 2, ((rand() % 512) - 256) / 256.0f);
		}
		if (ImGui::Button("Play random SFXR preset COIN"))
		{
			gSfx.loadPreset(SoLoud::Sfxr::COIN, rand());
			gSoloud.play(gSfx, 2, ((rand() % 512) - 256) / 256.0f);
		}
		if (ImGui::Button("Play random SFXR preset HURT"))
		{
			gSfx.loadPreset(SoLoud::Sfxr::HURT, rand());
			gSoloud.play(gSfx, 2, ((rand() % 512) - 256) / 256.0f);
		}
		if (ImGui::Button("Play random SFXR preset JUMP"))
		{
			gSfx.loadPreset(SoLoud::Sfxr::JUMP, rand());
			gSoloud.play(gSfx, 2, ((rand() % 512) - 256) / 256.0f);
		}
		if (ImGui::Button("Play random SFXR preset LASER"))
		{
			gSfx.loadPreset(SoLoud::Sfxr::LASER, rand());
			gSoloud.play(gSfx, 2, ((rand() % 512) - 256) / 256.0f);
		}
		ImGui::End();
		DemoUpdateEnd();
	}
}

int DemoEntry_multimusic(int argc, char *argv[])
{
	return multimusic::DemoEntry(argc, argv);
}

void DemoMainloop_multimusic()
{
	return multimusic::DemoMainloop();
}