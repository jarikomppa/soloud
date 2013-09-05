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
	EchoFilterInstance::EchoFilterInstance(EchoFilter *aParent)
	{
		mParent = aParent;
		mBuffer = 0;
		mBufferLength = 0;
		mOffset = 0;
	}

	void EchoFilterInstance::filter(float *aBuffer, int aSamples, int aStereo, float aSamplerate)
	{
		if (mBuffer == 0)
		{
			mBufferLength = (int)ceil(mParent->mDelay * aSamplerate) * (aStereo?2:1);
			mBuffer = new float[mBufferLength];
			int i;
			for (i = 0; i < mBufferLength; i++)
			{
				mBuffer[i] = 0;
			}
		}
		int process = aSamples * (aStereo?2:1);
		float decay = mParent->mDecay;
		int i;
		for (i = 0; i < process; i++)
		{
			float n = aBuffer[i] + mBuffer[mOffset];
			mBuffer[mOffset] = n * decay;
			aBuffer[i] = n;
			mOffset = (mOffset + 1) % mBufferLength;
		}
	}

	EchoFilterInstance::~EchoFilterInstance()
	{
		delete[] mBuffer;
	}

	void EchoFilter::init(AudioSource *aSource)
	{
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
