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
#include "soloud_wav.h"

namespace radiogaga
{

	SoLoud::Soloud gSoloud;			// SoLoud engine core
	SoLoud::Speech gSpeechPhrase[19];
	SoLoud::Wav gMusicPhrase[10];
	SoLoud::Speech gIntro;

	SoLoud::Bus gMusicBus;
	SoLoud::Bus gSpeechBus;
	SoLoud::Queue gSpeechQueue;
	SoLoud::Queue gMusicQueue;

	SoLoud::handle gSpeechqueuehandle;
	SoLoud::handle gIntrohandle;

	const char *phrase[19] =
	{
		"............................",
		"in empty eternity",
		"rain pours down from above",
		"Because it is",
		"but that was then",
		"Let everyone know",
		"in the spaces between",
		"than your bitter heart",
		"a cry of seabirds high over",
		"in my head",
		"sit down and please don't move",
		"I don't feel good",
		"Even if time",
		"For all it's worth.",
		"the only sound",
		"a vast thrumming of crickets",
		"voices of angels",
		"don't bother me",
		"but from an early age"
	};

	int DemoEntry(int argc, char *argv[])
	{
		gSoloud.init();
		gSoloud.setVisualizationEnable(1);

		gSoloud.play(gSpeechBus, 0.5f);

		gSoloud.play(gMusicBus);

		int i;
		for (i = 0; i < 19; i++)
			gSpeechPhrase[i].setText(phrase[i]);

		gMusicPhrase[0].load("audio/9 (102 BPM)_Seq01.wav");
		gMusicPhrase[1].load("audio/9 (102 BPM)_Seq02.wav");
		gMusicPhrase[2].load("audio/9 (102 BPM)_Seq03.wav");
		gMusicPhrase[3].load("audio/9 (102 BPM)_Seq04.wav");
		gMusicPhrase[4].load("audio/9 (102 BPM)_Seq05.wav");
		gMusicPhrase[5].load("audio/9 (102 BPM)_Seq06.wav");
		gMusicPhrase[6].load("audio/9 (102 BPM)_Seq07.wav");
		gMusicPhrase[7].load("audio/9 (102 BPM)_Seq08.wav");
		gMusicPhrase[8].load("audio/9 (102 BPM)_Seq09.wav");
		gMusicPhrase[9].load("audio/9 (102 BPM)_Seq10.wav");

		gSpeechQueue.setParamsFromAudioSource(gSpeechPhrase[0]);
		gSpeechqueuehandle = gSpeechBus.play(gSpeechQueue, 1, 0, true);
		
		gSoloud.oscillateRelativePlaySpeed(gSpeechqueuehandle, 0.6f, 1.4f, 4);

		gSpeechQueue.play(gSpeechPhrase[rand() % 19]);
		

		gMusicQueue.setParamsFromAudioSource(gMusicPhrase[0]);
		gMusicBus.play(gMusicQueue);
		gMusicQueue.play(gMusicPhrase[0]);
		gMusicQueue.play(gMusicPhrase[(rand() % 8) + 1]);

		gSpeechBus.setVisualizationEnable(1);
		gMusicBus.setVisualizationEnable(1);

		gIntro.setText("Eat, Sleep, Rave, Repeat");
		gIntro.setLooping(true);
		gIntro.setLoopPoint(1.45f);
		gIntrohandle = gSoloud.play(gIntro);
		gSoloud.fadeVolume(gIntrohandle, 0, 10);
		gSoloud.scheduleStop(gIntrohandle, 10);
		return 0;
	}

	void DemoMainloop()
	{
		int tick = DemoTick();

		DemoUpdateStart();


		ONCE(ImGui::SetNextWindowPos(ImVec2(500, 20)));
		ImGui::Begin("Music");
		{
			float *fft = gMusicBus.calcFFT();
			float *buf = gMusicBus.getWave();
			ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
			ImGui::PlotHistogram("##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
			ImGui::Text("Music queue      : %d", gMusicQueue.getQueueCount());
			ImGui::Separator();
			ImGui::Text("Active voices    : %d", gSoloud.getActiveVoiceCount());
			ImGui::Text(
				"Active voices include 2\n"
				"audio busses, music and\n"
				"speech.");
		}
		ImGui::End();

		if (gSpeechQueue.getQueueCount() < 2)
		{
			int i;
			for (i = 0; i < 8; i++)
			{
				gSpeechQueue.play(gSpeechPhrase[(rand() % 18) + 1]);
				gSpeechQueue.play(gSpeechPhrase[0]);
			}
		}

		if (gSoloud.getPause(gSpeechqueuehandle) && !gSoloud.isValidVoiceHandle(gIntrohandle))
			gSoloud.setPause(gSpeechqueuehandle, false);

		if (gMusicQueue.getQueueCount() < 2)
		{
			int i;
			for (i = 0; i < 4; i++)
				gMusicQueue.play(gMusicPhrase[(rand() % 8) + 1]);
		}

		ONCE(ImGui::SetNextWindowPos(ImVec2(20, 20)));
		ONCE(ImGui::SetNextWindowSize(ImVec2(280, 360)));
		ImGui::Begin("Poet");
		{
			float *buf = gSpeechBus.getWave();
			float *fft = gSpeechBus.calcFFT();
			ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
			ImGui::PlotHistogram("##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
			ImGui::Text("Speech queue     : %d", gSpeechQueue.getQueueCount());
			int i;
			for (i = 0; i < 19; i++)
				if (gSpeechQueue.isCurrentlyPlaying(gSpeechPhrase[i]))
				{
					ImGui::Text("Speech phrase    : %d", i);
					ImGui::Separator();
					ImGui::Text("%s", phrase[i]);
					ImGui::Separator();
				}

		}
		ImGui::End();

		DemoUpdateEnd();
	}
}

int DemoEntry_radiogaga(int argc, char *argv[])
{
	return radiogaga::DemoEntry(argc, argv);
}

void DemoMainloop_radiogaga()
{
	return radiogaga::DemoMainloop();
}