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

namespace SoLoud
{
	class RadioSet
	{
		Soloud *mSoloud;
		Bus *mBus;
		AudioSource **mSource;
		AudioSource *mAck;
		unsigned int mSourceCount;
		unsigned int mSourceCountMax;
		unsigned int mAckLength;
	public:
		RadioSet()
		{
			mSource = 0;
			mSourceCount = 0;
			mSoloud = 0;
			mSourceCountMax = 0;
			mAck = 0;
			mBus = 0;
			mAckLength = 0;
		}

		result init(Soloud &aSoloud, Bus *aBus)
		{
			mSoloud = &aSoloud;
			mBus = aBus;
			return SO_NO_ERROR;
		}

		result setAck(AudioSource &aAudioSource, unsigned int aAckLength)
		{
			mAck = &aAudioSource;
			mAckLength = aAckLength;
			return SO_NO_ERROR;
		}

		result clearAck()
		{
			mAck = 0;
			return SO_NO_ERROR;
		}

		result attach(AudioSource &aAudioSource)
		{
			unsigned int i;
			for (i = 0; i < mSourceCount; i++)
				if (&aAudioSource == mSource[i])
					return INVALID_PARAMETER;

			if (mSourceCount == mSourceCountMax)
			{				
				mSourceCountMax += 16;
				AudioSource **t = new AudioSource*[mSourceCountMax];
				for (i = 0; i < mSourceCount; i++)
				{
					t[i] = mSource[i];
				}
				delete[] mSource;
				mSource = t;				
			}

			mSource[mSourceCount] = &aAudioSource;
			mSourceCount++;
			return SO_NO_ERROR;
		}

		handle play(AudioSource &aAudioSource)
		{
			// try to attach just in case we don't already have this
			attach(aAudioSource);

			bool found = false;
			unsigned int i;
			for (i = 0; i < mSourceCount; i++)
			{
				if (mSoloud->countAudioSource(*mSource[i]) > 0)
				{
					mSoloud->stopAudioSource(*mSource[i]);
					found = true;
				}
			}

			int delay = 0;

			if (mAck && found)
			{
				if (mBus)
					mBus->play(*mAck);
				else
					mSoloud->play(*mAck);
				delay = mAckLength;
			}

			int res;

			if (mBus)
				res = mBus->play(aAudioSource,-1,0,true);
			else
				res = mSoloud->play(aAudioSource,-1,0,true);
			// delay the sample by however long ack is
			mSoloud->setDelaySamples(res, delay);
			mSoloud->setPause(res, false);
			return res;
		}
	};
}

namespace thebutton
{
	SoLoud::Soloud gSoloud;
	SoLoud::RadioSet gRadioSet;
	SoLoud::Wav gPhrase[12];
	int gCycles;
	int gNextEvent;

	int DemoEntry(int argc, char *argv[])
	{
		gCycles = 0;
		gNextEvent = 0;
		gSoloud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);
		gRadioSet.init(gSoloud, NULL);
		gPhrase[0].load("audio/thebutton/button1.mp3");
		gPhrase[1].load("audio/thebutton/button2.mp3");
		gPhrase[2].load("audio/thebutton/button3.mp3");
		gPhrase[3].load("audio/thebutton/button4.mp3");
		gPhrase[4].load("audio/thebutton/button5.mp3");
		gPhrase[5].load("audio/thebutton/cough.mp3");
		gPhrase[6].load("audio/thebutton/button6.mp3");
		gPhrase[7].load("audio/thebutton/button7.mp3");
		gPhrase[8].load("audio/thebutton/button1.mp3");
		gPhrase[9].load("audio/thebutton/sigh.mp3");
		gPhrase[10].load("audio/thebutton/thankyou.mp3");
		gPhrase[11].load("audio/thebutton/ack.ogg");

		int i;
		for (i = 0; i < 11; i++)
			gRadioSet.attach(gPhrase[i]);
		gRadioSet.setAck(gPhrase[11], gPhrase[11].mSampleCount);

		return 0;
	}

	void DemoMainloop()
	{
		DemoUpdateStart();

		if (DemoTick() > gNextEvent)
		{
			gRadioSet.play(gPhrase[gCycles]);
			gNextEvent = DemoTick() + 5000;
			gCycles = (gCycles + 1) % 9;
		}

		float *buf = gSoloud.getWave();
		float *fft = gSoloud.calcFFT();

		ONCE(ImGui::SetNextWindowPos(ImVec2(500, 20)));
		ImGui::Begin("Output");
		ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
		ImGui::PlotHistogram("##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
		ImGui::Text("Active voices    : %d", gSoloud.getActiveVoiceCount());
		ImGui::Text("Thanks to Anthony Salter for\n"
					"voice acting!");
		ImGui::End();



		ONCE(ImGui::SetNextWindowPos(ImVec2(20, 20)));
		ImGui::Begin("Control");
		if (ImGui::Button("The button",ImVec2(300,200)))
		{
			gRadioSet.play(gPhrase[10]);
			gNextEvent = DemoTick() + 5000;
		}
		ImGui::End();
		DemoUpdateEnd();
	}
}

int DemoEntry_thebutton(int argc, char *argv[])
{
	return thebutton::DemoEntry(argc, argv);
}

void DemoMainloop_thebutton()
{
	return thebutton::DemoMainloop();
}