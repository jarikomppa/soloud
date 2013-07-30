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

#include "soloud.h"
#include "soloud_filter.h"

namespace SoLoud
{
	FilterInstance::FilterInstance(AudioSource *aSource)
	{
		mOffset = 0;
		mSource = aSource->createInstance();
		mSource->init(0, aSource->mBaseSamplerate, aSource->mFlags);
	}

	void FilterInstance::getAudio(float *aBuffer, int aSamples)
	{
		mSource->getAudio(aBuffer, aSamples);

		int i;			
		for (i = 0; i < aSamples; i++, mOffset++)
			aBuffer[i] *= sin(mOffset*0.1f*mBaseSamplerate/mSamplerate);					
	}

	int FilterInstance::hasEnded()
	{
		return mSource->hasEnded();
	}

	FilterInstance::~FilterInstance()
	{
		delete mSource;
	}

	Filter::Filter(AudioSource *aSource)
	{
		mSource = aSource;
		mBaseSamplerate = aSource->mBaseSamplerate;
	}

	AudioInstance *Filter::createInstance()
	{
		return new FilterInstance(mSource);
	}
}
