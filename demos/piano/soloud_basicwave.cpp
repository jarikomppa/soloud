/*
SoLoud audio engine
Copyright (c) 2013-2021 Jari Komppa

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
#include "soloud_misc.h"


namespace SoLoud
{

	BasicwaveInstance::BasicwaveInstance(Basicwave *aParent)
	{
		mParent = aParent;
		mOffset = 0;
		mFreq = aParent->mFreq;
		mT = 0;
	}

	unsigned int BasicwaveInstance::getAudio(float *aBuffer, unsigned int aSamplesToRead, unsigned int aBufferSize)
	{
		unsigned int i;
		int waveform = mParent->mWaveform;
		float d = 1.0f / mSamplerate;
		for (i = 0; i < aSamplesToRead; i++)
		{
			aBuffer[i] = SoLoud::Misc::generateWaveform(waveform, fmod(mFreq * (float)mOffset, 1.0f)) * mParent->mADSR.val(mT, 10000000000000.0f);
			mOffset++;
			mT += d;
		}
		return aSamplesToRead;
	}

	bool BasicwaveInstance::hasEnded()
	{
		// This audio source never ends.
		return 0;
	}
	
	Basicwave::Basicwave()
	{
		setSamplerate(44100);
		mWaveform = SoLoud::Soloud::WAVE_SQUARE;
	}

	Basicwave::~Basicwave()
	{
		stop();
	}

	void Basicwave::setSamplerate(float aSamplerate)
	{
		mBaseSamplerate = aSamplerate;
		mFreq = (float)(440 / mBaseSamplerate);
	}

	void Basicwave::setFreq(float aFreq)
	{
		mFreq = aFreq / mBaseSamplerate;
	}

	void Basicwave::setWaveform(int aWaveform)
	{
		mWaveform = aWaveform;
	}

	AudioSourceInstance * Basicwave::createInstance() 
	{
		return new BasicwaveInstance(this);
	}

};