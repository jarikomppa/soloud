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
#include "soloud_fft.h"

namespace SoLoud
{
	BusInstance::BusInstance(Bus *aParent)
	{
		mParent = aParent;
		mScratch = 0;
		mScratchSize = 0;
		mFlags |= PROTECTED;
	}
	
	void BusInstance::getAudio(float *aBuffer, unsigned int aSamples)
	{
		int handle = mParent->mChannelHandle;
		if (handle == 0) 
		{
			// Avoid reuse of scratch data if this bus hasn't played anything yet
			unsigned int i;
			for (i = 0; i < aSamples * mChannels; i++)
				aBuffer[i] = 0;
			return;
		}
		
		Soloud *s = mParent->mSoloud;
		if (s->mScratchNeeded != mScratchSize)
		{
			mScratchSize = s->mScratchNeeded;
			delete[] mScratch;
			mScratch = new float[mScratchSize];
		}
		
		s->mixBus(aBuffer, aSamples, mScratch, handle, mSamplerate);

		int i;
		if (mParent->mFlags & AudioSource::VISUALIZATION_DATA)
		{
			if (aSamples > 255)
			{
				for (i = 0; i < 256; i++)
				{
					mVisualizationWaveData[i] = aBuffer[i] + aBuffer[i+aSamples];
				}
			}
			else
			{
				// Very unlikely failsafe branch
				for (i = 0; i < 256; i++)
				{
					mVisualizationWaveData[i] = aBuffer[i % aSamples] + aBuffer[(i % aSamples) + aSamples];
				}
			}
		}

	}

	bool BusInstance::hasEnded()
	{
		// Busses never stop for fear of going under 50mph.
		return 0;
	}

	BusInstance::~BusInstance()
	{
		Soloud *s = mParent->mSoloud;
		int i;
		for (i = 0; i < VOICE_COUNT; i++)
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
		mChannels = 2;
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

	void Bus::findBusHandle()
	{
		if (mChannelHandle == 0)
		{
			// Find the channel the bus is playing on to calculate handle..
			int i;
			for (i = 0; mChannelHandle == 0 && i < VOICE_COUNT; i++)
			{
				if (mSoloud->mVoice[i] == mInstance)
				{
					mChannelHandle = mSoloud->getHandleFromVoice(i);
				}
			}
		}
	}

	handle Bus::play(AudioSource &aSound, float aVolume, float aPan, bool aPaused)
	{
		if (!mInstance || !mSoloud)
		{
			return 0;
		}

		findBusHandle();

		if (mChannelHandle == 0)
		{
			return 0;
		}
		return mSoloud->play(aSound, aVolume, aPan, aPaused, mChannelHandle);
	}	


	handle Bus::playClocked(time aSoundTime, AudioSource &aSound, float aVolume, float aPan)
	{
		if (!mInstance || !mSoloud)
		{
			return 0;
		}

		findBusHandle();

		if (mChannelHandle == 0)
		{
			return 0;
		}

		return mSoloud->playClocked(aSoundTime, aSound, aVolume, aPan, mChannelHandle);
	}	


	void Bus::setFilter(unsigned int aFilterId, Filter *aFilter)
	{
		if (aFilterId < 0 || aFilterId >= FILTERS_PER_STREAM)
			return;

		mFilter[aFilterId] = aFilter;

		if (mInstance)
		{
			if (mSoloud->mLockMutexFunc) mSoloud->mLockMutexFunc(mSoloud->mMutex);
			delete mInstance->mFilter[aFilterId];
			mInstance->mFilter[aFilterId] = 0;
		
			if (aFilter)
			{
				mInstance->mFilter[aFilterId] = mFilter[aFilterId]->createInstance();
			}
			if (mSoloud->mUnlockMutexFunc) mSoloud->mUnlockMutexFunc(mSoloud->mMutex);
		}
	}

	void Bus::setVisualizationEnable(bool aEnable)
	{
		if (aEnable)
		{
			mFlags |= AudioSource::VISUALIZATION_DATA;
		}
		else
		{
			mFlags &= ~AudioSource::VISUALIZATION_DATA;
		}
	}
		
	float * Bus::calcFFT()
	{
		if (mInstance && mSoloud)
		{
			if (mSoloud->mLockMutexFunc) mSoloud->mLockMutexFunc(mSoloud->mMutex);
			float temp[1024];
			int i;
			for (i = 0; i < 256; i++)
			{
				temp[i*2] = mInstance->mVisualizationWaveData[i];
				temp[i*2+1] = 0;
				temp[i+512] = 0;
				temp[i+768] = 0;
			}
			if (mSoloud->mUnlockMutexFunc) mSoloud->mUnlockMutexFunc(mSoloud->mMutex);

			SoLoud::FFT::fft512(temp);

			for (i = 0; i < 256; i++)
			{
				float real = temp[i*2];
				float imag = temp[i*2+1];
				mFFTData[i] = sqrt(real*real+imag*imag);
			}
		}

		return mFFTData;
	}

	float * Bus::getWave()
	{
		if (mInstance && mSoloud)
		{
			int i;
			if (mSoloud->mLockMutexFunc) mSoloud->mLockMutexFunc(mSoloud->mMutex);
			for (i = 0; i < 256; i++)
				mWaveData[i] = mInstance->mVisualizationWaveData[i];
			if (mSoloud->mUnlockMutexFunc) mSoloud->mUnlockMutexFunc(mSoloud->mMutex);
		}
		return mWaveData;
	}

};
