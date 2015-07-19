/*
SoLoud audio engine
Copyright (c) 2013-2015 Jari Komppa

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

#include <string.h>
#include "soloud_internal.h"

// Core "basic" operations - play, stop, etc

namespace SoLoud
{
	handle Soloud::play(AudioSource &aSound, float aVolume, float aPan, bool aPaused, unsigned int aBus)
	{
		if (aSound.mFlags & AudioSource::SINGLE_INSTANCE)
		{
			// Only one instance allowed, stop others
			aSound.stop();
		}

		// Creation of an audio instance may take significant amount of time,
		// so let's not do it inside the audio thread mutex.
		aSound.mSoloud = this;
		SoLoud::AudioSourceInstance *instance = aSound.createInstance();

		lockAudioMutex();
		int ch = findFreeVoice();
		if (ch < 0) 
		{
			unlockAudioMutex();
			delete instance;
			return UNKNOWN_ERROR;
		}
		if (!aSound.mAudioSourceID)
		{
			aSound.mAudioSourceID = mAudioSourceID;
			mAudioSourceID++;
		}
		mVoice[ch] = instance;
		mVoice[ch]->mAudioSourceID = aSound.mAudioSourceID;
		mVoice[ch]->mBusHandle = aBus;
		mVoice[ch]->init(aSound, mPlayIndex);
		m3dData[ch].init(aSound);

		mPlayIndex++;

		// 20 bits, skip the last one (top bits full = voice group)
		if (mPlayIndex == 0xfffff) 
		{
			mPlayIndex = 0;
		}

		if (aPaused)
		{
			mVoice[ch]->mFlags |= AudioSourceInstance::PAUSED;
		}

		setVoicePan(ch, aPan);
		if (aVolume < 0)
		{
			setVoiceVolume(ch, aSound.mVolume);
		}
		else
		{
			setVoiceVolume(ch, aVolume);
		}
		setVoiceRelativePlaySpeed(ch, 1);

		int i;
		for (i = 0; i < FILTERS_PER_STREAM; i++)
		{
			if (aSound.mFilter[i])
			{
				mVoice[ch]->mFilter[i] = aSound.mFilter[i]->createInstance();
			}
		}

		int scratchneeded = SAMPLE_GRANULARITY * mVoice[ch]->mChannels;

		mVoice[ch]->mResampleData[0]->mBuffer = new float[scratchneeded];
		mVoice[ch]->mResampleData[1]->mBuffer = new float[scratchneeded];

		// First buffer will be overwritten anyway; the second may be referenced by resampler
		memset(mVoice[ch]->mResampleData[0]->mBuffer, 0, sizeof(float) * scratchneeded);
		memset(mVoice[ch]->mResampleData[1]->mBuffer, 0, sizeof(float) * scratchneeded);

		unlockAudioMutex();

		int handle = getHandleFromVoice(ch);
		return handle;
	}

	handle Soloud::playClocked(time aSoundTime, AudioSource &aSound, float aVolume, float aPan, unsigned int aBus)
	{
		handle h = play(aSound, aVolume, aPan, 1, aBus);
		lockAudioMutex();
		time lasttime = mLastClockedTime;
		if (lasttime == 0) 
			mLastClockedTime = aSoundTime;
		unlockAudioMutex();
		int samples = 0;
		if (lasttime != 0)
		{
			samples = (int)floor((aSoundTime - lasttime) * mSamplerate);
		}
		setDelaySamples(h, samples);
		setPause(h, 0);
		return h;
	}

	void Soloud::seek(handle aVoiceHandle, time aSeconds)
	{
		FOR_ALL_VOICES_PRE
			mVoice[ch]->seek(aSeconds, mScratch.mData, mScratchSize);
		FOR_ALL_VOICES_POST
	}


	void Soloud::stop(handle aVoiceHandle)
	{
		FOR_ALL_VOICES_PRE
			stopVoice(ch);
		FOR_ALL_VOICES_POST
	}

	void Soloud::stopAudioSource(AudioSource &aSound)
	{
		if (aSound.mAudioSourceID)
		{
			lockAudioMutex();
			
			int i;
			for (i = 0; i < (signed)mHighestVoice; i++)
			{
				if (mVoice[i] && mVoice[i]->mAudioSourceID == aSound.mAudioSourceID)
				{
					stopVoice(i);
				}
			}
			unlockAudioMutex();
		}
	}

	void Soloud::stopAll()
	{
		int i;
		lockAudioMutex();
		for (i = 0; i < (signed)mHighestVoice; i++)
		{
			stopVoice(i);
		}
		unlockAudioMutex();
	}
}
