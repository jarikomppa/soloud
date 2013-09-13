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

namespace SoLoud
{
	BusInstance::BusInstance(Bus *aParent)
	{
		mParent = aParent;
		mScratch = 0;
		mScratchSize = 0;
		mFlags |= PROTECTED;
	}
	
	void BusInstance::getAudio(float *aBuffer, int aSamples)
	{
		int handle = mParent->mChannelHandle;
		if (handle == 0) 
			return;
		
		Soloud *s = mParent->mSoloud;
		if (s->mScratchNeeded != mScratchSize)
		{
			mScratchSize = s->mScratchNeeded;
			delete[] mScratch;
			mScratch = new float[mScratchSize];
		}
		
		s->mixBus(aBuffer, aSamples, mScratch, handle);
	}

	int BusInstance::hasEnded()
	{
		return 0;
	}

	BusInstance::~BusInstance()
	{
		Soloud *s = mParent->mSoloud;
		int i;
		for (i = 0; i < s->mVoiceCount; i++)
		{
			if (s->mVoice[i] && s->mVoice[i]->mBusHandle == mParent->mChannelHandle)
			{
				s->stopVoice(i);
			}
		}
		delete[] mScratch;
	}

	Bus::Bus()
	{
		mChannelHandle = 0;
		mInstance = 0;
	}
	
	BusInstance * Bus::createInstance()
	{
		if (mChannelHandle)
		{
			// We should be inside the mutex at the moment, so stop using the "internal" functions
			mSoloud->stopVoice(mSoloud->getVoiceFromHandle(mChannelHandle));
			mChannelHandle = 0;
			mInstance = 0;
		}
		mInstance = new BusInstance(this);
		return mInstance;
	}

	int Bus::play(AudioSource &aSound, float aVolume, float aPan, int aPaused)
	{
		if (!mInstance || !mSoloud)
			return 0;
		if (mChannelHandle == 0)
		{
			// Find the channel the bus is playing on to calculate handle..
			int i;
			for (i = 0; mChannelHandle == 0 && i < mSoloud->mVoiceCount; i++)
			{
				if (mSoloud->mVoice[i] == mInstance)
				{
					mChannelHandle = mSoloud->getHandleFromVoice(i);
				}
			}
			if (mChannelHandle == 0)
			{
				return 0;
			}
		}
		return mSoloud->play(aSound, aVolume, aPan, aPaused, mChannelHandle);
	}	
};
