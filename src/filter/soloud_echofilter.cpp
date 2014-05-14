/*
SoLoud audio engine
Copyright (c) 2013-2014 Jari Komppa

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
#include "soloud_echofilter.h"

namespace SoLoud
{
	EchoFilterInstance::EchoFilterInstance(EchoFilter *aParent)
	{
		mParent = aParent;
		mBuffer = 0;
		mBufferLength = 0;
		mOffset = 0;
	}

	void EchoFilterInstance::filter(float *aBuffer, int aSamples, int aChannels, float aSamplerate, float aTime)
	{
		if (mBuffer == 0)
		{
			mBufferLength = (int)ceil(mParent->mDelay * aSamplerate);
			mBuffer = new float[mBufferLength * aChannels];
			int i;
			for (i = 0; i < mBufferLength * aChannels; i++)
			{
				mBuffer[i] = 0;
			}
		}

		float decay = mParent->mDecay;
		int i, j;
		for (i = 0; i < aSamples; i++)
		{
			for (j = 0; j < aChannels; j++)
			{
				int chofs = j * mBufferLength;
				int bchofs = j * aSamples;
				float n = aBuffer[i + bchofs] + mBuffer[mOffset + chofs] * decay;
				mBuffer[mOffset + chofs] = n;
				aBuffer[i + bchofs] = n;
			}
			mOffset = (mOffset + 1) % mBufferLength;
		}
	}

	EchoFilterInstance::~EchoFilterInstance()
	{
		delete[] mBuffer;
	}

	EchoFilter::EchoFilter()
	{
		mDelay = 1;
		mDecay = 0.5f;
	}

	void EchoFilter::setParams(float aDelay, float aDecay)
	{
		mDecay = aDecay;
		mDelay = aDelay;
	}


	FilterInstance *EchoFilter::createInstance()
	{
		return new EchoFilterInstance(this);
	}
}
