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
	AudioSourceInstance::AudioSourceInstance()
	{
		mPlayIndex = 0;
		mFlags = 0;
		mPan = 0;
		mLVolume = 1.0f / (float)sqrt(2.0);
		mRVolume = 1.0f / (float)sqrt(2.0);
		mVolume = 1.0f;
		mBaseSamplerate = 44100.0f;
		mSamplerate = 44100.0f;
		mRelativePlaySpeed = 1.0f;
		mStreamTime = 0.0f;
		mAudioSourceID = 0;
		mActiveFader = 0;
		mChannels = 1;
		int i;
		for (i = 0; i < FILTERS_PER_STREAM; i++)
		{
			mFilter[i] = NULL;
		}
		for (i = 0; i < 4; i++)
		{
			mFaderVolume[i] = 0;
		}
	}

	AudioSourceInstance::~AudioSourceInstance()
	{
		if (mFilter)
		{
			int i;
			for (i = 0; i < FILTERS_PER_STREAM; i++)
			{
				delete mFilter[i];
			}
		}
	}

	void AudioSourceInstance::init(int aPlayIndex, float aBaseSamplerate, int aChannels, int aSourceFlags)
	{
		mPlayIndex = aPlayIndex;
		mBaseSamplerate = aBaseSamplerate;
		mSamplerate = mBaseSamplerate;
		mChannels = aChannels;
		mStreamTime = 0.0f;

		if (aSourceFlags & AudioSource::SHOULD_LOOP)
		{
			mFlags |= AudioSourceInstance::LOOPING;
		}
	}

	int AudioSourceInstance::rewind()
	{
		return 0;
	}

	void AudioSourceInstance::seek(float aSeconds, float *mScratch, int mScratchSize)
	{
		float offset = aSeconds - mStreamTime;
		if (offset < 0)
		{
			if (rewind() == 0)
			{
				// can't do generic seek backwards unless we can rewind.
				return;
			}
			offset = aSeconds;
		}
		int samples_to_discard = (int)floor(mSamplerate * offset);

		while (samples_to_discard)
		{
			int samples = mScratchSize / 2;
			if (samples > samples_to_discard)
				samples = samples_to_discard;
			getAudio(mScratch, samples);
			samples_to_discard -= samples;
		}

		mStreamTime = aSeconds;
	}


	AudioSource::AudioSource() 
	{ 
		int i;
		for (i = 0; i < FILTERS_PER_STREAM; i++)
		{
			mFilter[i] = 0;
		}
		mFlags = 0; 
		mBaseSamplerate = 44100; 
		mAudioSourceID = 0;
		mSoloud = 0;
		mChannels = 1;
	}

	AudioSource::~AudioSource() 
	{
		if (mSoloud)
		{
			mSoloud->stopSound(*this);
		}
	}

	void AudioSource::setLooping(int aLoop)
	{
		if (aLoop)
		{
			mFlags |= SHOULD_LOOP;
		}
		else
		{
			mFlags &= ~SHOULD_LOOP;
		}
	}

	void AudioSource::setFilter(int aFilterId, Filter *aFilter)
	{
		if (aFilterId < 0 || aFilterId >= FILTERS_PER_STREAM)
			return;
		mFilter[aFilterId] = aFilter;
	}
};

