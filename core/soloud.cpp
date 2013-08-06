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

#include <stdlib.h> // rand
#include <math.h> // sin
#include "soloud.h"

namespace SoLoud
{
	///////////////////////////////////////////////////////////////////////////

	void Filter::init(AudioSource *aSource)
	{
	}

	///////////////////////////////////////////////////////////////////////////

	Fader::Fader()
	{
		mCurrent = mFrom = mTo = mDelta = mTime = mStartTime = mEndTime = 0;
		mActive = 0;
	}

	void Fader::set(float aFrom, float aTo, float aTime, float aStartTime)
	{
		mCurrent = mFrom;
		mFrom = aFrom;
		mTo = aTo;
		mTime = aTime;
		mStartTime = aStartTime;
		mDelta = aTo - aFrom;
		mEndTime = mStartTime + mTime;
		mActive = 1;
	}

	float Fader::get(float aCurrentTime)
	{
		if (mStartTime > aCurrentTime)
		{
			// Time rolled over.
			// Figure out where we were..
			float p = (mCurrent - mFrom) / mDelta; // 0..1
			mFrom = mCurrent;
			mStartTime = aCurrentTime;
			mTime = mTime * (1 - p); // time left
			mDelta = mTo - mFrom;
			mEndTime = mStartTime + mTime;
		}
		if (aCurrentTime > mEndTime)
		{
			mActive = -1;
			return mTo;
		}
		mCurrent = mFrom + mDelta * ((aCurrentTime - mStartTime) / mTime);
		return mCurrent;
	}

	///////////////////////////////////////////////////////////////////////////

	AudioInstance::AudioInstance()
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
		mFilter = 0;
		int i;
		for (i = 0; i < 4; i++)
		{
			mFaderVolume[i] = 0;
		}
	}

	AudioInstance::~AudioInstance()
	{
		if (mFilter)
		{
			delete mFilter;
		}
	}

	void AudioInstance::init(int aPlayIndex, float aBaseSamplerate, int aSourceFlags)
	{
		mPlayIndex = aPlayIndex;
		mBaseSamplerate = aBaseSamplerate;
		mSamplerate = mBaseSamplerate;
		mStreamTime = 0.0f;
		mFlags = 0;

		if (aSourceFlags & AudioSource::SHOULD_LOOP)
		{
			mFlags |= AudioInstance::LOOPING;
		}

		if (aSourceFlags & AudioSource::STEREO)
		{
			mFlags |= AudioInstance::STEREO;
		}
	}

	int AudioInstance::rewind()
	{
		return 0;
	}

	void AudioInstance::seek(float aSeconds, float *mScratch, int mScratchSize)
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


	///////////////////////////////////////////////////////////////////////////

	AudioSource::AudioSource() 
	{ 
		mFlags = 0; 
		mBaseSamplerate = 44100; 
		mAudioSourceID = 0;
		mSoloud = 0;
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

	void AudioSource::setFilter(Filter &aFilter)
	{
		mFilter = &aFilter;
		if (mFilter)
		{
			mFilter->init(this);
		}
	}

	///////////////////////////////////////////////////////////////////////////

	Soloud::Soloud()
	{
		mScratch = NULL;
		mScratchSize = 0;
		mScratchNeeded = 0;
		mChannel = NULL;
		mChannelCount = 0;
		mSamplerate = 0;
		mBufferSize = 0;
		mFlags = 0;
		mGlobalVolume = 0;
		mPlayIndex = 0;
		mBackendData = NULL;
		mMutex = NULL;
		mPostClipScaler = 0;
		mLockMutexFunc = NULL;
		mUnlockMutexFunc = NULL;
		mStreamTime = 0;
		mAudioSourceID = 1;
		mFilter = 0;
		mFilterInstance = 0;
#ifdef SOLOUD_INCLUDE_FFT
		int i;
		for (i = 0; i < 512; i++)
		{
			mFFTInput[i] = 0;
		}
		for (i = 0; i < 256; i++)
		{
			mFFTData[i] = 0;
		}
#endif
	}

	Soloud::~Soloud()
	{
		stopAll();
		delete mFilterInstance;
		delete[] mScratch;
		delete[] mChannel;
	}

	void Soloud::init(int aChannels, int aSamplerate, int aBufferSize, int aFlags)
	{
		mGlobalVolume = 1;
		mChannel = new AudioInstance*[aChannels];
		mChannelCount = aChannels;
		int i;
		for (i = 0; i < aChannels; i++)
		{
			mChannel[i] = 0;
		}
		mSamplerate = aSamplerate;
		mScratchSize = 2048;
		mScratchNeeded = 2048;
		mScratch = new float[mScratchSize * 2];
		mBufferSize = aBufferSize;
		mFlags = aFlags;
		mPostClipScaler = 0.5f;
	}

	float Soloud::getPostClipScaler() const
	{
		return mPostClipScaler;
	}

	float Soloud::getGlobalVolume() const
	{
		return mGlobalVolume;
	}

	void Soloud::setPostClipScaler(float aScaler)
	{
		mPostClipScaler = aScaler;
	}

	void Soloud::setGlobalVolume(float aVolume)
	{
		mGlobalVolumeFader.mActive = 0;
		mGlobalVolume = aVolume;
	}		

	int Soloud::findFreeChannel()
	{
		int i;
		unsigned int lowest_play_index_value = 0xffffffff;
		int lowest_play_index = -1;
		for (i = 0; i < mChannelCount; i++)
		{
			if (mChannel[i] == NULL)
			{
				return i;
			}
			if (((mChannel[i]->mFlags & AudioInstance::PROTECTED) == 0) && 
				mChannel[i]->mPlayIndex < lowest_play_index_value)
			{
				lowest_play_index_value = mChannel[i]->mPlayIndex;
				lowest_play_index = i;
			}
		}
		stop(lowest_play_index);
		return lowest_play_index;
	}

	int Soloud::play(AudioSource &aSound, float aVolume, float aPan, int aPaused)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = findFreeChannel();
		if (ch < 0) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return -1;
		}
		if (!aSound.mAudioSourceID)
		{
			aSound.mAudioSourceID = mAudioSourceID;
			mAudioSourceID++;
			aSound.mSoloud = this;
		}
		mChannel[ch] = aSound.createInstance();
		mChannel[ch]->mAudioSourceID = aSound.mAudioSourceID;
		int handle = ch | (mPlayIndex << 8);

		mChannel[ch]->init(mPlayIndex, aSound.mBaseSamplerate, aSound.mFlags);
		
		if (aPaused)
		{
			mChannel[ch]->mFlags |= AudioInstance::PAUSED;
		}

		setChannelPan(ch, aPan);
		setChannelVolume(ch, aVolume);
		setChannelRelativePlaySpeed(ch, 1);

		if (aSound.mFilter)
		{
			mChannel[ch]->mFilter = aSound.mFilter->createInstance();
		}

		mPlayIndex++;
		int scratchneeded = (int)ceil((mChannel[ch]->mSamplerate / mSamplerate) * mBufferSize);
		if (mScratchNeeded < scratchneeded)
		{
			int pot = 1024;
			while (pot < scratchneeded) pot <<= 1;
			mScratchNeeded = pot;
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return handle;
	}	

	int Soloud::getChannelFromHandle(int aChannelHandle) const
	{
		int ch = aChannelHandle & 0xff;
		unsigned int idx = aChannelHandle >> 8;
		if (mChannel[ch] &&
			(mChannel[ch]->mPlayIndex & 0xffffff) == idx)
		{
			return ch;
		}
		return -1;		
	}

	int Soloud::getActiveVoiceCount() const
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int i;
		int c = 0;
		for (i = 0; i < mChannelCount; i++)
		{
			if (mChannel[i]) 
			{
				c++;
			}
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return c;
	}

	int Soloud::isValidChannelHandle(int aChannelHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		if (getChannelFromHandle(aChannelHandle) != -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return 1;
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return 0;
	}


	float Soloud::getVolume(int aChannelHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return 0;
		}
		float v = mChannel[ch]->mVolume;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return v;
	}

	float Soloud::getPan(int aChannelHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return 0;
		}
		float v = mChannel[ch]->mPan;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return v;
	}

	float Soloud::getStreamTime(int aChannelHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return 0;
		}
		float v = mChannel[ch]->mStreamTime;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return v;
	}

	float Soloud::getRelativePlaySpeed(int aChannelHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return 1;
		}
		float v = mChannel[ch]->mRelativePlaySpeed;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return v;
	}

	void Soloud::setChannelRelativePlaySpeed(int aChannel, float aSpeed)
	{
		if (mChannel[aChannel])
		{
			mChannel[aChannel]->mRelativePlaySpeed = aSpeed;
			mChannel[aChannel]->mSamplerate = mChannel[aChannel]->mBaseSamplerate * mChannel[aChannel]->mRelativePlaySpeed;
			int scratchneeded = (int)ceil((mChannel[aChannel]->mSamplerate / mSamplerate) * mBufferSize);
			if (mScratchNeeded < scratchneeded)
			{
				int pot = 1024;
				while (pot < scratchneeded) pot <<= 1;
				mScratchNeeded = pot;
			}
		}
	}

	void Soloud::setRelativePlaySpeed(int aChannelHandle, float aSpeed)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mChannel[ch]->mRelativePlaySpeedFader.mActive = 0;
		setChannelRelativePlaySpeed(ch, aSpeed);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	float Soloud::getSamplerate(int aChannelHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return 0;
		}
		float v = mChannel[ch]->mBaseSamplerate;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return v;
	}

	void Soloud::setSamplerate(int aChannelHandle, float aSamplerate)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mChannel[ch]->mBaseSamplerate = aSamplerate;
		mChannel[ch]->mSamplerate = mChannel[ch]->mBaseSamplerate * mChannel[ch]->mRelativePlaySpeed;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::seek(int aChannelHandle, float aSeconds)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mChannel[ch]->seek(aSeconds, mScratch, mScratchSize);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::setPause(int aChannelHandle, int aPause)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}

		setChannelPause(ch, aPause);

		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::setPauseAll(int aPause)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch;
		for (ch = 0; ch < mChannelCount; ch++)
		{
			setChannelPause(ch, aPause);
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}


	int Soloud::getPause(int aChannelHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return 0;
		}
		int v = !!(mChannel[ch]->mFlags & AudioInstance::PAUSED);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return v;
	}

	int Soloud::getProtectChannel(int aChannelHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return 0;
		}
		int v = !!(mChannel[ch]->mFlags & AudioInstance::PROTECTED);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return v;
	}

	void Soloud::setProtectChannel(int aChannelHandle, int aProtect)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		if (aProtect)
		{
			mChannel[ch]->mFlags |= AudioInstance::PROTECTED;
		}
		else
		{
			mChannel[ch]->mFlags &= ~AudioInstance::PROTECTED;
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::setChannelPause(int aChannel, int aPause)
	{
		if (mChannel[aChannel])
		{
			mChannel[aChannel]->mPauseScheduler.mActive = 0;

			if (aPause)
			{
				mChannel[aChannel]->mFlags |= AudioInstance::PAUSED;
			}
			else
			{
				mChannel[aChannel]->mFlags &= ~AudioInstance::PAUSED;
			}
		}
	}

	void Soloud::setChannelPan(int aChannel, float aPan)
	{
		if (mChannel[aChannel])
		{
			mChannel[aChannel]->mPan = aPan;
			mChannel[aChannel]->mLVolume = (float)cos((aPan + 1) * M_PI / 4);
			mChannel[aChannel]->mRVolume = (float)sin((aPan + 1) * M_PI / 4);
		}
	}

	void Soloud::setPan(int aChannelHandle, float aPan)
	{		
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		setChannelPan(ch, aPan);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::setPanAbsolute(int aChannelHandle, float aLVolume, float aRVolume)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mChannel[ch]->mPanFader.mActive = 0;
		mChannel[ch]->mLVolume = aLVolume;
		mChannel[ch]->mRVolume = aRVolume;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::setChannelVolume(int aChannel, float aVolume)
	{
		if (mChannel[aChannel])
		{
			mChannel[aChannel]->mVolume = aVolume;
		}
	}

	void Soloud::setVolume(int aChannelHandle, float aVolume)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mChannel[ch]->mVolumeFader.mActive = 0;
		setChannelVolume(ch, aVolume);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::stop(int aChannelHandle)
	{
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			return;
		}
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		stopChannel(ch);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::stopChannel(int aChannel)
	{
		if (mChannel[aChannel])
		{
			delete mChannel[aChannel];
			mChannel[aChannel] = 0;			
		}
	}		

	void Soloud::stopSound(AudioSource &aSound)
	{
		if (aSound.mAudioSourceID)
		{
			if (mLockMutexFunc) mLockMutexFunc(mMutex);
			
			int i;
			for (i = 0; i < mChannelCount; i++)
			{
				if (mChannel[i] && mChannel[i]->mAudioSourceID == aSound.mAudioSourceID)
				{
					stopChannel(i);
				}
			}
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		}
	}

	void Soloud::stopAll()
	{
		int i;
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		for (i = 0; i < mChannelCount; i++)
		{
			stopChannel(i);
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::schedulePause(int aChannelHandle, float aTime)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mChannel[ch]->mPauseScheduler.set(1, 0, aTime, mChannel[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::scheduleStop(int aChannelHandle, float aTime)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mChannel[ch]->mStopScheduler.set(1, 0, aTime, mChannel[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::fadeVolume(int aChannelHandle, float aFrom, float aTo, float aTime)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mChannel[ch]->mVolumeFader.set(aFrom, aTo, aTime, mChannel[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::fadePan(int aChannelHandle, float aFrom, float aTo, float aTime)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mChannel[ch]->mPanFader.set(aFrom, aTo, aTime, mChannel[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::fadeRelativePlaySpeed(int aChannelHandle, float aFrom, float aTo, float aTime)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mChannel[ch]->mRelativePlaySpeedFader.set(aFrom, aTo, aTime, mChannel[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::fadeGlobalVolume(float aFrom, float aTo, float aTime)
	{
		mStreamTime = 0; // avoid rollover (~6 days)
		mGlobalVolumeFader.set(aFrom, aTo, aTime, mStreamTime);
	}

#ifdef SOLOUD_INCLUDE_FFT
	float * Soloud::calcFFT()
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		mFFT.fft(mFFTInput, mScratch);

		int i;
		for (i = 0; i < 256; i++)
		{
			float real = mScratch[i];
			float imag = mScratch[i+256];
			mFFTData[i] = sqrt(real*real + imag*imag);
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return mFFTData;
	}
#endif

	void Soloud::setGlobalFilter(Filter &aFilter)
	{
		delete mFilterInstance;
		
		mFilter = &aFilter;
		mFilter->init(NULL);
		mFilterInstance = mFilter->createInstance();
	}

	void Soloud::mix(float *aBuffer, int aSamples)
	{
		float buffertime = aSamples / (float)mSamplerate;
		float globalVolume[2];
		mStreamTime += buffertime;

		globalVolume[0] = mGlobalVolume;
		if (mGlobalVolumeFader.mActive)
		{
			mGlobalVolume = mGlobalVolumeFader.get(mStreamTime);
		}
		globalVolume[1] = mGlobalVolume;

		if (mLockMutexFunc) mLockMutexFunc(mMutex);

		// Process faders. May change scratch size.
		int i;
		for (i = 0; i < mChannelCount; i++)
		{
			if (mChannel[i] && !(mChannel[i]->mFlags & AudioInstance::PAUSED))
			{
				float volume[2];
				float panl[2];
				float panr[2];

				mChannel[i]->mActiveFader = 0;

				if (mGlobalVolumeFader.mActive)
				{
					mChannel[i]->mActiveFader = 1;
				}

				mChannel[i]->mStreamTime += buffertime;

				if (mChannel[i]->mRelativePlaySpeedFader.mActive == 1)
				{
					float speed = mChannel[i]->mRelativePlaySpeedFader.get(mChannel[i]->mStreamTime);
					setChannelRelativePlaySpeed(i, speed);
				}

				volume[0] = mChannel[i]->mVolume;
				if (mChannel[i]->mVolumeFader.mActive == 1)
				{
					mChannel[i]->mVolume = mChannel[i]->mVolumeFader.get(mChannel[i]->mStreamTime);
					mChannel[i]->mActiveFader = 1;
				}
				volume[1] = mChannel[i]->mVolume;


				panl[0] = mChannel[i]->mLVolume;
				panr[0] = mChannel[i]->mRVolume;
				if (mChannel[i]->mPanFader.mActive == 1)
				{
					float pan = mChannel[i]->mPanFader.get(mChannel[i]->mStreamTime);
					setChannelPan(i, pan);
					mChannel[i]->mActiveFader = 1;
				}
				panl[1] = mChannel[i]->mLVolume;
				panr[1] = mChannel[i]->mRVolume;

				if (mChannel[i]->mPauseScheduler.mActive)
				{
					mChannel[i]->mPauseScheduler.get(mChannel[i]->mStreamTime);
					if (mChannel[i]->mPauseScheduler.mActive == -1)
					{
						mChannel[i]->mPauseScheduler.mActive = 0;
						setChannelPause(i, 1);
					}
				}

				if (mChannel[i]->mActiveFader)
				{
					mChannel[i]->mFaderVolume[0*2+0] = panl[0] * volume[0] * globalVolume[0];
					mChannel[i]->mFaderVolume[0*2+1] = panl[1] * volume[1] * globalVolume[1];
					mChannel[i]->mFaderVolume[1*2+0] = panr[0] * volume[0] * globalVolume[0];
					mChannel[i]->mFaderVolume[1*2+1] = panr[1] * volume[1] * globalVolume[1];
				}

				if (mChannel[i]->mStopScheduler.mActive)
				{
					mChannel[i]->mStopScheduler.get(mChannel[i]->mStreamTime);
					if (mChannel[i]->mStopScheduler.mActive == -1)
					{
						mChannel[i]->mStopScheduler.mActive = 0;
						stopChannel(i);
					}
				}
			}
		}

		// Resize scratch if needed.
		if (mScratchSize < mScratchNeeded)
		{
			mScratchSize = mScratchNeeded;
			delete[] mScratch;
			mScratch = new float[mScratchSize];
		}
		
		// Clear accumulation buffer
		for (i = 0; i < aSamples*2; i++)
		{
			aBuffer[i] = 0;
		}

		// Accumulate sound sources
		for (i = 0; i < mChannelCount; i++)
		{
			if (mChannel[i] && !(mChannel[i]->mFlags & AudioInstance::PAUSED))
			{

				float stepratio = mChannel[i]->mSamplerate / mSamplerate;
				float step = 0;
				int j;

				int readsamples = (int)ceil(aSamples * stepratio);

				mChannel[i]->getAudio(mScratch, readsamples);	

				if (mChannel[i]->mFilter)
				{
					mChannel[i]->mFilter->filter(mScratch, readsamples, mChannel[i]->mFlags & AudioInstance::STEREO, mChannel[i]->mSamplerate);
				}

				if (mChannel[i]->mActiveFader)
				{
					float lpan = mChannel[i]->mFaderVolume[0];
					float rpan = mChannel[i]->mFaderVolume[2];
					float lpani = (mChannel[i]->mFaderVolume[1] - mChannel[i]->mFaderVolume[0]) / aSamples;
					float rpani = (mChannel[i]->mFaderVolume[3] - mChannel[i]->mFaderVolume[2]) / aSamples;

					if (mChannel[i]->mFlags & AudioInstance::STEREO)
					{
						for (j = 0; j < aSamples; j++, step += stepratio, lpan += lpani, rpan += rpani)
						{
							float s1 = mScratch[(int)floor(step)*2];
							float s2 = mScratch[(int)floor(step)*2+1];
							aBuffer[j * 2 + 0] += s1 * lpan;
							aBuffer[j * 2 + 1] += s2 * rpan;
						}
					}
					else
					{
						for (j = 0; j < aSamples; j++, step += stepratio, lpan += lpani, rpan += rpani)
						{
							float s = mScratch[(int)floor(step)];
							aBuffer[j * 2 + 0] += s * lpan;
							aBuffer[j * 2 + 1] += s * rpan;
						}
					}
				}
				else
				{
					float lpan = mChannel[i]->mLVolume * mChannel[i]->mVolume * mGlobalVolume;
					float rpan = mChannel[i]->mRVolume * mChannel[i]->mVolume * mGlobalVolume;
					if (mChannel[i]->mFlags & AudioInstance::STEREO)
					{
						for (j = 0; j < aSamples; j++, step += stepratio)
						{
							float s1 = mScratch[(int)floor(step)*2];
							float s2 = mScratch[(int)floor(step)*2+1];
							aBuffer[j * 2 + 0] += s1 * lpan;
							aBuffer[j * 2 + 1] += s2 * rpan;
						}
					}
					else
					{
						for (j = 0; j < aSamples; j++, step += stepratio)
						{
							float s = mScratch[(int)floor(step)];
							aBuffer[j * 2 + 0] += s * lpan;
							aBuffer[j * 2 + 1] += s * rpan;
						}
					}
				}

				// chear channel if the sound is over
				if (!(mChannel[i]->mFlags & AudioInstance::LOOPING) && mChannel[i]->hasEnded())
				{
					stopChannel(i);
				}
			}
		}

		if (mFilterInstance)
		{
			mFilterInstance->filter(aBuffer, aSamples, 1, (float)mSamplerate);
		}

		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);

		// Clip
		if (mFlags & CLIP_ROUNDOFF)
		{
			for (i = 0; i < aSamples*2; i++)
			{
				float f = aBuffer[i];
				if (f <= -1.65f)
				{
					f = -0.9862875f;
				}
				else
				if (f >= 1.65f)
				{
					f = 0.9862875f;
				}
				else
				{
					f =  0.87f * f - 0.1f * f * f * f;
				}
				aBuffer[i] = f * mPostClipScaler;
			}
		}
		else
		{
			for (i = 0; i < aSamples; i++)
			{
				float f = aBuffer[i];
				if (f < -1.0f)
				{
					f = -1.0f;
				}
				else
				if (f > 1.0f)
				{
					f = 1.0f;
				}
				aBuffer[i] = f * mPostClipScaler;
			}
		}
#ifdef SOLOUD_INCLUDE_FFT
		if (mFlags & ENABLE_FFT)
		{
			if (aSamples > 511)
			{
				for (i = 0; i < 512; i++)
				{
					mFFTInput[i] = aBuffer[i*2+0] + aBuffer[i*2+1];
				}
			}
			else
			{
				for (i = 0; i < 512; i++)
				{
					mFFTInput[i] = aBuffer[((i % aSamples) * 2) + 0] + aBuffer[((i % aSamples) * 2) + 1];
				}
			}
		}
#endif
	}
};
