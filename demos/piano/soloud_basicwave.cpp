/*
SoLoud audio engine
Copyright (c) 2013 Jari Komppa

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

#include "soloud_basicwave.h"

namespace SoLoud
{

	BasicwaveInstance::BasicwaveInstance(Basicwave *aParent)
	{
		mParent = aParent;
		mOffset = 0;
	}

	void BasicwaveInstance::getAudio(float *aBuffer, int aSamples)
	{
		int i;
		switch (mParent->mWaveform)
		{
			case Basicwave::SINE:
				for (i = 0; i < aSamples; i++)
				{
					aBuffer[i] = sin(mParent->mFreq * mOffset * M_PI * 2);
					mOffset++;
				}
				break;
			case Basicwave::TRIANGLE:
				for (i = 0; i < aSamples; i++)
				{
					aBuffer[i] = (1 - fmod(mParent->mFreq * mOffset, 1)) * 2 - 1;
					mOffset++;
				}
				break;
			case Basicwave::SQUARE:
				for (i = 0; i < aSamples; i++)
				{
					aBuffer[i] = (fmod(mParent->mFreq * mOffset, 1) > 0.5) ? -1 : 1;
					mOffset++;
				}
				break;
		}
	}

	int BasicwaveInstance::hasEnded()
	{
		// This audio source never ends.
		return 0;
	}
	
	Basicwave::Basicwave()
	{
		setSamplerate(4000);
		mWaveform = SQUARE;
	}

	void Basicwave::setSamplerate(float aSamplerate)
	{
		mBaseSamplerate = aSamplerate;
		mFreq = (float)(440 / mBaseSamplerate);
	}

	AudioSourceInstance * Basicwave::createInstance() 
	{
		return new BasicwaveInstance(this);
	}

};