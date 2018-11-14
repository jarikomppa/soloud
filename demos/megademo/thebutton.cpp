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
	public:
		RadioSet()
		{
			mSource = 0;
			mSourceCount = 0;
			mSoloud = 0;
			mSourceCountMax = 0;
			mAck = 0;
			mBus = 0;
		}

		result init(Soloud &aSoloud, Bus *aBus)
		{
			mSoloud = &aSoloud;
			mBus = aBus;
			return SO_NO_ERROR;
		}

		result setAck(AudioSource &aAudioSource)
		{
			mAck = &aAudioSource;
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

			if (mAck && found)
			{
				if (mBus)
					mBus->play(*mAck);
				else
					mSoloud->play(*mAck);
			}

			if (mBus)
				return mBus->play(aAudioSource);
			return mSoloud->play(aAudioSource);
		}
	};
}

namespace thebutton
{
	SoLoud::Soloud gSoloud;
	SoLoud::RadioSet gRadioSet;
	SoLoud::Speech gPhrase[10];
	int gCycles;
	int gNextEvent;

	int DemoEntry(int argc, char *argv[])
	{
		gCycles = 0;
		gNextEvent = 0;
		gSoloud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);
		gRadioSet.init(gSoloud, NULL);
		gPhrase[0].setText("Please press the button.");
		gPhrase[1].setText("Kindly press the button, please.");
		gPhrase[2].setText("Would you please press the button.");
		gPhrase[3].setText("Press the button. Please.");
		gPhrase[4].setText("Press the button.");
		gPhrase[5].setText("Press the button already.");
		gPhrase[6].setText("Could you possibly press the button?");
		gPhrase[7].setText("Just press the button.");
		gPhrase[8].setText("Thank you. Now, please press the button.");
		gPhrase[9].setText("k");

		int i;
		for (i = 0; i < 9; i++)
			gRadioSet.attach(gPhrase[i]);
		gRadioSet.setAck(gPhrase[9]);

		return 0;
	}

	void DemoMainloop()
	{
		DemoUpdateStart();

		if (DemoTick() > gNextEvent)
		{
			gRadioSet.play(gPhrase[gCycles]);
			gNextEvent = DemoTick() + 5000;
			gCycles = (gCycles + 1) % 8;
		}

		float *buf = gSoloud.getWave();
		float *fft = gSoloud.calcFFT();

		ONCE(ImGui::SetNextWindowPos(ImVec2(500, 20)));
		ImGui::Begin("Output");
		ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
		ImGui::PlotHistogram("##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
		ImGui::Text("Active voices    : %d", gSoloud.getActiveVoiceCount());
		ImGui::End();



		ONCE(ImGui::SetNextWindowPos(ImVec2(20, 20)));
		ImGui::Begin("Control");
		if (ImGui::Button("The button"))
		{
			gRadioSet.play(gPhrase[8]);
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