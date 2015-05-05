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

namespace SoLoud
{

	AudioSourceResampleData::AudioSourceResampleData()
	{
		mBuffer = 0;
	}
	
	AudioSourceResampleData::~AudioSourceResampleData()
	{
		delete[] mBuffer;
	}

	AudioSourceInstance::AudioSourceInstance()
	{
		mPlayIndex = 0;
		mFlags = 0;
		mPan = 0;
		mChannelVolume[0] = 1.0f / (float)sqrt(2.0);
		mChannelVolume[1] = 1.0f / (float)sqrt(2.0);
		mVolume = 1.0f;
		mBaseSamplerate = 44100.0f;
		mSamplerate = 44100.0f;
		mRelativePlaySpeed = 1.0f;
		mStreamTime = 0.0f;
		mAudioSourceID = 0;
		mActiveFader = 0;
		mChannels = 1;
		mBusHandle = ~0u;
		mLoopCount = 0;
		int i;
		for (i = 0; i < FILTERS_PER_STREAM; i++)
		{
			mFilter[i] = NULL;
		}
		for (i = 0; i < MAX_CHANNELS; i++)
		{
			mCurrentChannelVolume[i] = 0;
		}
		mResampleData[0] = new AudioSourceResampleData;
		mResampleData[1] = new AudioSourceResampleData;
		mSrcOffset = 0;
		mLeftoverSamples = 0;
		mDelaySamples = 0;
		m3dAttenuationModel = 0;
		m3dAttenuationRolloff = 1;
		m3dDopplerFactor = 1.0;
		m3dMaxDistance = 1000000.0f;
		m3dMinDistance = 0.0f;
		m3dPosition[0] = 0;
		m3dPosition[1] = 0;
		m3dPosition[2] = 0;
		m3dVelocity[0] = 0;
		m3dVelocity[1] = 0;
		m3dVelocity[2] = 0;		
		mCollider = 0;
		mColliderData = 0;
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
		delete mResampleData[0];
		delete mResampleData[1];
	}

	void AudioSourceInstance::init(AudioSource &aSource, int aPlayIndex)
	{
		mPlayIndex = aPlayIndex;
		mBaseSamplerate = aSource.mBaseSamplerate;
		mSamplerate = mBaseSamplerate;
		mChannels = aSource.mChannels;
		mStreamTime = 0.0f;
		m3dAttenuationModel = aSource.m3dAttenuationModel;
		m3dAttenuationRolloff = aSource.m3dAttenuationRolloff;
		m3dDopplerFactor = aSource.m3dDopplerFactor;
		m3dMaxDistance = aSource.m3dMaxDistance;
		m3dMinDistance = aSource.m3dMinDistance;		

		if (aSource.mFlags & AudioSource::SHOULD_LOOP)
		{
			mFlags |= AudioSourceInstance::LOOPING;
		}
		if (aSource.mFlags & AudioSource::PROCESS_3D)
		{
			mFlags |= AudioSourceInstance::PROCESS_3D;
		}
		if (aSource.mFlags & AudioSource::LISTENER_RELATIVE)
		{
			mFlags |= AudioSourceInstance::LISTENER_RELATIVE;
		}
		if (aSource.mFlags & AudioSource::INAUDIBLE_KILL)
		{
			mFlags |= AudioSourceInstance::INAUDIBLE_KILL;
		}
		if (aSource.mFlags & AudioSource::INAUDIBLE_TICK)
		{
			mFlags |= AudioSourceInstance::INAUDIBLE_TICK;
		}

		mCollider = aSource.mCollider;
		mColliderData = aSource.mColliderData;
	}

	result AudioSourceInstance::rewind()
	{
		return NOT_IMPLEMENTED;
	}

	void AudioSourceInstance::seek(double aSeconds, float *mScratch, unsigned int mScratchSize)
	{
		double offset = aSeconds - mStreamTime;
		if (offset < 0)
		{
			if (rewind() < 0)
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
		m3dMinDistance = 1;
		m3dMaxDistance = 1000000.0f;
		m3dAttenuationRolloff = 1.0f;
		m3dAttenuationModel = NO_ATTENUATION;
		m3dDopplerFactor = 1.0f;
		mCollider = 0;
		mColliderData = 0;
	}

	AudioSource::~AudioSource() 
	{
		stop();
	}

	void AudioSource::setLooping(bool aLoop)
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

	void AudioSource::setSingleInstance(bool aSingleInstance)
	{
		if (aSingleInstance)
		{
			mFlags |= SINGLE_INSTANCE;
		}
		else
		{
			mFlags &= ~SINGLE_INSTANCE;
		}
	}

	void AudioSource::setFilter(unsigned int aFilterId, Filter *aFilter)
	{
		if (aFilterId < 0 || aFilterId >= FILTERS_PER_STREAM)
			return;
		mFilter[aFilterId] = aFilter;
	}

	void AudioSource::stop()
	{
		if (mSoloud)
		{
			mSoloud->stopAudioSource(*this);
		}
	}

	void AudioSource::set3dMinMaxDistance(float aMinDistance, float aMaxDistance)
	{
		m3dMinDistance = aMinDistance;
		m3dMaxDistance = aMaxDistance;
	}

	void AudioSource::set3dAttenuation(unsigned int aAttenuationModel, float aAttenuationRolloffFactor)
	{
		m3dAttenuationModel = aAttenuationModel;
		m3dAttenuationRolloff = aAttenuationRolloffFactor;
	}

	void AudioSource::set3dDopplerFactor(float aDopplerFactor)
	{
		m3dDopplerFactor = aDopplerFactor;
	}

	void AudioSource::set3dProcessing(bool aDo3dProcessing)
	{
		if (aDo3dProcessing)
		{
			mFlags |= PROCESS_3D;
		}
		else
		{
			mFlags &= ~PROCESS_3D;
		}
	}

	void AudioSource::set3dListenerRelative(bool aListenerRelative)
	{
		if (aListenerRelative)
		{
			mFlags |= LISTENER_RELATIVE;
		}
		else
		{
			mFlags &= ~LISTENER_RELATIVE;
		}
	}


	void AudioSource::set3dDistanceDelay(bool aDistanceDelay)
	{
		if (aDistanceDelay)
		{
			mFlags |= DISTANCE_DELAY;
		}
		else
		{
			mFlags &= ~DISTANCE_DELAY;
		}
	}

	void AudioSource::set3dCollider(AudioCollider *aCollider, int aUserData)
	{
		mCollider = aCollider;
		mColliderData = aUserData;
	}
	
	float AudioSourceInstance::getInfo(unsigned int aInfoKey)
	{
	    return 0;
	}

};

