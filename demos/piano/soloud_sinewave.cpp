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

#include "soloud_sinewave.h"

namespace SoLoud
{

	SinewaveInstance::SinewaveInstance(Sinewave *aParent)
	{
		mParent = aParent;
		mOffset = 0;
	}

	void SinewaveInstance::getAudio(float *aBuffer, int aSamples)
	{
		int i;
		for (i = 0; i < aSamples; i++)
		{
			aBuffer[i] = sin(mParent->mFreq * mOffset);
			mOffset++;
		}
	}

	int SinewaveInstance::hasEnded()
	{
		return 0;
	}
	
	Sinewave::Sinewave()
	{
		mBaseSamplerate = 4000;
		mFreq = (float)(440 * M_PI * 2 / mBaseSamplerate);
	}

	AudioInstance * Sinewave::createInstance() 
	{
		return new SinewaveInstance(this);
	}

};