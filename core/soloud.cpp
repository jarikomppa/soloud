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

	Filter::~Filter()
	{
	}

	FilterInstance::~FilterInstance()
	{
	}

	void FilterInstance::setFilterParameter(int aAttributeId, float aValue)
	{
	}

	void FilterInstance::fadeFilterParameter(int aAttributeId, float aFrom, float aTo, float aTime, float aStartTime)
	{
	}

	void FilterInstance::oscillateFilterParameter(int aAttributeId, float aFrom, float aTo, float aTime, float aStartTime)
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

	void Fader::setLFO(float aFrom, float aTo, float aTime, float aStartTime)
	{
		mActive = 2;
		mCurrent = 0;
		mFrom = aFrom;
		mTo = aTo;
		mTime = aTime;
		mDelta = abs(aTo - aFrom) / 2;
		mStartTime = aStartTime;
		mEndTime = (float)M_PI * 2 / (mTime * 1000.0f);
	}

	float Fader::get(float aCurrentTime)
	{
		if (mActive == 2)
		{
			// LFO mode
			if (mStartTime > aCurrentTime)
			{
				// Time rolled over.
				mStartTime = aCurrentTime;
			}
			float t = aCurrentTime - mStartTime;
			mCurrent += t;
			return sin(mCurrent * mEndTime) * mDelta + (mFrom + mDelta);
			
		}
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

	AudioInstance::~AudioInstance()
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
		int i;
		for (i = 0; i < FILTERS_PER_STREAM; i++)
		{
			mFilter[i] = 0;
		}
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

	void AudioSource::setFilter(int aFilterId, Filter *aFilter)
	{
		if (aFilterId < 0 || aFilterId >= FILTERS_PER_STREAM)
			return;
		mFilter[aFilterId] = aFilter;
	}

	///////////////////////////////////////////////////////////////////////////

	Soloud::Soloud()
	{
		mScratch = NULL;
		mScratchSize = 0;
		mScratchNeeded = 0;
		mVoice = NULL;
		mVoiceCount = 0;
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
		int i;
		for (i = 0; i < FILTERS_PER_STREAM; i++)
		{
			mFilter[i] = NULL;
			mFilterInstance[i] = NULL;
		}
#ifdef SOLOUD_INCLUDE_FFT
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
		int i;
		for (i = 0; i < FILTERS_PER_STREAM; i++)
		{
			delete mFilterInstance[i];
		}
		delete[] mScratch;
		delete[] mVoice;
		deinit();
	}

	void Soloud::deinit()
	{
		if (mBackendCleanupFunc)
			mBackendCleanupFunc(this);
		mBackendCleanupFunc = 0;
	}

	void Soloud::init(int aVoices, int aSamplerate, int aBufferSize, int aFlags)
	{
		mGlobalVolume = 1;
		mVoice = new AudioInstance*[aVoices];
		mVoiceCount = aVoices;
		int i;
		for (i = 0; i < aVoices; i++)
		{
			mVoice[i] = 0;
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

	int Soloud::findFreeVoice()
	{
		int i;
		unsigned int lowest_play_index_value = 0xffffffff;
		int lowest_play_index = -1;
		for (i = 0; i < mVoiceCount; i++)
		{
			if (mVoice[i] == NULL)
			{
				return i;
			}
			if (((mVoice[i]->mFlags & AudioInstance::PROTECTED) == 0) && 
				mVoice[i]->mPlayIndex < lowest_play_index_value)
			{
				lowest_play_index_value = mVoice[i]->mPlayIndex;
				lowest_play_index = i;
			}
		}
		stopVoice(lowest_play_index);
		return lowest_play_index;
	}

	int Soloud::play(AudioSource &aSound, float aVolume, float aPan, int aPaused)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = findFreeVoice();
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
		mVoice[ch] = aSound.createInstance();
		mVoice[ch]->mAudioSourceID = aSound.mAudioSourceID;
		int handle = ch | (mPlayIndex << 12);

		mVoice[ch]->init(mPlayIndex, aSound.mBaseSamplerate, aSound.mFlags);
		
		if (aPaused)
		{
			mVoice[ch]->mFlags |= AudioInstance::PAUSED;
		}

		setVoicePan(ch, aPan);
		setVoiceVolume(ch, aVolume);
		setVoiceRelativePlaySpeed(ch, 1);

		int i;
		for (i = 0; i < FILTERS_PER_STREAM; i++)
		{
			if (aSound.mFilter[i])
			{
				mVoice[ch]->mFilter[i] = aSound.mFilter[i]->createInstance();
			}
		}

		mPlayIndex++;
		int scratchneeded = (int)ceil((mVoice[ch]->mSamplerate / mSamplerate) * mBufferSize);
		if (mScratchNeeded < scratchneeded)
		{
			int pot = 1024;
			while (pot < scratchneeded) pot <<= 1;
			mScratchNeeded = pot;
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return handle;
	}	

	int Soloud::getVoiceFromHandle(int aVoiceHandle) const
	{
		if (aVoiceHandle < 0) 
		{
			return -1;
		}
		int ch = aVoiceHandle & 0xff;
		unsigned int idx = aVoiceHandle >> 12;
		if (mVoice[ch] &&
			(mVoice[ch]->mPlayIndex & 0xfffff) == idx)
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
		for (i = 0; i < mVoiceCount; i++)
		{
			if (mVoice[i]) 
			{
				c++;
			}
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return c;
	}

	int Soloud::isValidVoiceHandle(int aVoiceHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		if (getVoiceFromHandle(aVoiceHandle) != -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return 1;
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return 0;
	}


	float Soloud::getVolume(int aVoiceHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return 0;
		}
		float v = mVoice[ch]->mVolume;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return v;
	}

	float Soloud::getPan(int aVoiceHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return 0;
		}
		float v = mVoice[ch]->mPan;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return v;
	}

	float Soloud::getStreamTime(int aVoiceHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return 0;
		}
		float v = mVoice[ch]->mStreamTime;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return v;
	}

	float Soloud::getRelativePlaySpeed(int aVoiceHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return 1;
		}
		float v = mVoice[ch]->mRelativePlaySpeed;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return v;
	}

	void Soloud::setVoiceRelativePlaySpeed(int aVoice, float aSpeed)
	{
		if (mVoice[aVoice])
		{
			mVoice[aVoice]->mRelativePlaySpeed = aSpeed;
			mVoice[aVoice]->mSamplerate = mVoice[aVoice]->mBaseSamplerate * mVoice[aVoice]->mRelativePlaySpeed;
			int scratchneeded = (int)ceil((mVoice[aVoice]->mSamplerate / mSamplerate) * mBufferSize);
			if (mScratchNeeded < scratchneeded)
			{
				int pot = 1024;
				while (pot < scratchneeded) pot <<= 1;
				mScratchNeeded = pot;
			}
		}
	}

	void Soloud::setRelativePlaySpeed(int aVoiceHandle, float aSpeed)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mRelativePlaySpeedFader.mActive = 0;
		setVoiceRelativePlaySpeed(ch, aSpeed);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	float Soloud::getSamplerate(int aVoiceHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return 0;
		}
		float v = mVoice[ch]->mBaseSamplerate;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return v;
	}

	void Soloud::setSamplerate(int aVoiceHandle, float aSamplerate)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mBaseSamplerate = aSamplerate;
		mVoice[ch]->mSamplerate = mVoice[ch]->mBaseSamplerate * mVoice[ch]->mRelativePlaySpeed;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::seek(int aVoiceHandle, float aSeconds)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->seek(aSeconds, mScratch, mScratchSize);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::setPause(int aVoiceHandle, int aPause)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}

		setVoicePause(ch, aPause);

		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::setPauseAll(int aPause)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch;
		for (ch = 0; ch < mVoiceCount; ch++)
		{
			setVoicePause(ch, aPause);
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}


	int Soloud::getPause(int aVoiceHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return 0;
		}
		int v = !!(mVoice[ch]->mFlags & AudioInstance::PAUSED);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return v;
	}

	int Soloud::getProtectVoice(int aVoiceHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return 0;
		}
		int v = !!(mVoice[ch]->mFlags & AudioInstance::PROTECTED);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		return v;
	}

	void Soloud::setProtectVoice(int aVoiceHandle, int aProtect)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		if (aProtect)
		{
			mVoice[ch]->mFlags |= AudioInstance::PROTECTED;
		}
		else
		{
			mVoice[ch]->mFlags &= ~AudioInstance::PROTECTED;
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::setVoicePause(int aVoice, int aPause)
	{
		if (mVoice[aVoice])
		{
			mVoice[aVoice]->mPauseScheduler.mActive = 0;

			if (aPause)
			{
				mVoice[aVoice]->mFlags |= AudioInstance::PAUSED;
			}
			else
			{
				mVoice[aVoice]->mFlags &= ~AudioInstance::PAUSED;
			}
		}
	}

	void Soloud::setVoicePan(int aVoice, float aPan)
	{
		if (mVoice[aVoice])
		{
			mVoice[aVoice]->mPan = aPan;
			mVoice[aVoice]->mLVolume = (float)cos((aPan + 1) * M_PI / 4);
			mVoice[aVoice]->mRVolume = (float)sin((aPan + 1) * M_PI / 4);
		}
	}

	void Soloud::setPan(int aVoiceHandle, float aPan)
	{		
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		setVoicePan(ch, aPan);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::setPanAbsolute(int aVoiceHandle, float aLVolume, float aRVolume)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mPanFader.mActive = 0;
		mVoice[ch]->mLVolume = aLVolume;
		mVoice[ch]->mRVolume = aRVolume;
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::setVoiceVolume(int aVoice, float aVolume)
	{
		if (mVoice[aVoice])
		{
			mVoice[aVoice]->mVolume = aVolume;
		}
	}

	void Soloud::setVolume(int aVoiceHandle, float aVolume)
	{
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mVolumeFader.mActive = 0;
		setVoiceVolume(ch, aVolume);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::stop(int aVoiceHandle)
	{
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			return;
		}
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		stopVoice(ch);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::stopVoice(int aVoice)
	{
		if (mVoice[aVoice])
		{
			delete mVoice[aVoice];
			mVoice[aVoice] = 0;			
		}
	}

	void Soloud::stopSound(AudioSource &aSound)
	{
		if (aSound.mAudioSourceID)
		{
			if (mLockMutexFunc) mLockMutexFunc(mMutex);
			
			int i;
			for (i = 0; i < mVoiceCount; i++)
			{
				if (mVoice[i] && mVoice[i]->mAudioSourceID == aSound.mAudioSourceID)
				{
					stopVoice(i);
				}
			}
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
		}
	}

	void Soloud::stopAll()
	{
		int i;
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		for (i = 0; i < mVoiceCount; i++)
		{
			stopVoice(i);
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::setFilterParameter(int aVoiceHandle, int aFilterId, int aAttributeId, float aValue)
	{
		if (aFilterId < 0 || aFilterId >= FILTERS_PER_STREAM)
			return;

		if (aVoiceHandle == 0)
		{
			if (mLockMutexFunc) mLockMutexFunc(mMutex);		
			if (mFilterInstance[aFilterId])
			{
				mFilterInstance[aFilterId]->setFilterParameter(aAttributeId, aValue);
			}
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}

		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			return;
		}
		if (mLockMutexFunc) mLockMutexFunc(mMutex);		
		if (mVoice[ch] &&
			mVoice[ch]->mFilter[aFilterId])
		{
			mVoice[ch]->mFilter[aFilterId]->setFilterParameter(aAttributeId, aValue);
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::fadeFilterParameter(int aVoiceHandle, int aFilterId, int aAttributeId, float aFrom, float aTo, float aTime)
	{
		if (aFilterId < 0 || aFilterId >= FILTERS_PER_STREAM)
			return;

		if (aVoiceHandle == 0)
		{
			if (mLockMutexFunc) mLockMutexFunc(mMutex);		
			if (mFilterInstance[aFilterId])
			{
				mFilterInstance[aFilterId]->fadeFilterParameter(aAttributeId, aFrom, aTo, aTime, mStreamTime);
			}
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}

		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			return;
		}
		if (mLockMutexFunc) mLockMutexFunc(mMutex);		
		if (mVoice[ch] &&
			mVoice[ch]->mFilter[aFilterId])
		{
			mVoice[ch]->mFilter[aFilterId]->fadeFilterParameter(aAttributeId, aFrom, aTo, aTime, mStreamTime);
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::oscillateFilterParameter(int aVoiceHandle, int aFilterId, int aAttributeId, float aFrom, float aTo, float aTime)
	{
		if (aFilterId < 0 || aFilterId >= FILTERS_PER_STREAM)
			return;

		if (aVoiceHandle == 0)
		{
			if (mLockMutexFunc) mLockMutexFunc(mMutex);		
			if (mFilterInstance[aFilterId])
			{
				mFilterInstance[aFilterId]->oscillateFilterParameter(aAttributeId, aFrom, aTo, aTime, mStreamTime);
			}
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}

		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			return;
		}
		if (mLockMutexFunc) mLockMutexFunc(mMutex);		
		if (mVoice[ch] &&
			mVoice[ch]->mFilter[aFilterId])
		{
			mVoice[ch]->mFilter[aFilterId]->oscillateFilterParameter(aAttributeId, aFrom, aTo, aTime, mStreamTime);
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::schedulePause(int aVoiceHandle, float aTime)
	{
		if (aTime <= 0)
		{
			setPause(aVoiceHandle, 1);
			return;
		}
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mPauseScheduler.set(1, 0, aTime, mVoice[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::scheduleStop(int aVoiceHandle, float aTime)
	{
		if (aTime <= 0)
		{
			stop(aVoiceHandle);
			return;
		}
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mStopScheduler.set(1, 0, aTime, mVoice[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::fadeVolume(int aVoiceHandle, float aFrom, float aTo, float aTime)
	{
		if (aTime <= 0 || aTo == aFrom)
		{
			setVolume(aVoiceHandle, aTo);
			return;
		}

		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mVolumeFader.set(aFrom, aTo, aTime, mVoice[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::fadePan(int aVoiceHandle, float aFrom, float aTo, float aTime)
	{
		if (aTime <= 0 || aTo == aFrom)
		{
			setPan(aVoiceHandle, aTo);
			return;
		}

		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mPanFader.set(aFrom, aTo, aTime, mVoice[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::fadeRelativePlaySpeed(int aVoiceHandle, float aFrom, float aTo, float aTime)
	{
		if (aTime <= 0 || aTo == aFrom)
		{
			setRelativePlaySpeed(aVoiceHandle, aTo);
			return;
		}
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mRelativePlaySpeedFader.set(aFrom, aTo, aTime, mVoice[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::fadeGlobalVolume(float aFrom, float aTo, float aTime)
	{
		if (aTime <= 0 || aTo == aFrom)
		{
			setGlobalVolume(aTo);
			return;
		}
		mStreamTime = 0; // avoid rollover (~6 days)
		mGlobalVolumeFader.set(aFrom, aTo, aTime, mStreamTime);
	}


	void Soloud::oscillateVolume(int aVoiceHandle, float aFrom, float aTo, float aTime)
	{
		if (aTime <= 0 || aTo == aFrom)
		{
			setVolume(aVoiceHandle, aTo);
			return;
		}

		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mVolumeFader.setLFO(aFrom, aTo, aTime, mVoice[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::oscillatePan(int aVoiceHandle, float aFrom, float aTo, float aTime)
	{
		if (aTime <= 0 || aTo == aFrom)
		{
			setPan(aVoiceHandle, aTo);
			return;
		}

		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mPanFader.setLFO(aFrom, aTo, aTime, mVoice[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::oscillateRelativePlaySpeed(int aVoiceHandle, float aFrom, float aTo, float aTime)
	{
		if (aTime <= 0 || aTo == aFrom)
		{
			setRelativePlaySpeed(aVoiceHandle, aTo);
			return;
		}
		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		int ch = getVoiceFromHandle(aVoiceHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
			return;
		}
		mVoice[ch]->mRelativePlaySpeedFader.setLFO(aFrom, aTo, aTime, mVoice[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
	}

	void Soloud::oscillateGlobalVolume(float aFrom, float aTo, float aTime)
	{
		if (aTime <= 0 || aTo == aFrom)
		{
			setGlobalVolume(aTo);
			return;
		}
		mStreamTime = 0; // avoid rollover (~6 days)
		mGlobalVolumeFader.setLFO(aFrom, aTo, aTime, mStreamTime);
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

	void Soloud::setGlobalFilter(int aFilterId, Filter *aFilter)
	{
		if (aFilterId < 0 || aFilterId >= FILTERS_PER_STREAM)
			return;

		if (mLockMutexFunc) mLockMutexFunc(mMutex);
		delete mFilterInstance[aFilterId];
		mFilterInstance[aFilterId] = 0;
		
		mFilter[aFilterId] = aFilter;
		if (aFilter)
		{
			mFilterInstance[aFilterId] = mFilter[aFilterId]->createInstance();
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc(mMutex);
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
		for (i = 0; i < mVoiceCount; i++)
		{
			if (mVoice[i] && !(mVoice[i]->mFlags & AudioInstance::PAUSED))
			{
				float volume[2];
				float panl[2];
				float panr[2];

				mVoice[i]->mActiveFader = 0;

				if (mGlobalVolumeFader.mActive > 0)
				{
					mVoice[i]->mActiveFader = 1;
				}

				mVoice[i]->mStreamTime += buffertime;

				if (mVoice[i]->mRelativePlaySpeedFader.mActive > 0)
				{
					float speed = mVoice[i]->mRelativePlaySpeedFader.get(mVoice[i]->mStreamTime);
					setVoiceRelativePlaySpeed(i, speed);
				}

				volume[0] = mVoice[i]->mVolume;
				if (mVoice[i]->mVolumeFader.mActive > 0)
				{
					mVoice[i]->mVolume = mVoice[i]->mVolumeFader.get(mVoice[i]->mStreamTime);
					mVoice[i]->mActiveFader = 1;
				}
				volume[1] = mVoice[i]->mVolume;


				panl[0] = mVoice[i]->mLVolume;
				panr[0] = mVoice[i]->mRVolume;
				if (mVoice[i]->mPanFader.mActive > 0)
				{
					float pan = mVoice[i]->mPanFader.get(mVoice[i]->mStreamTime);
					setVoicePan(i, pan);
					mVoice[i]->mActiveFader = 1;
				}
				panl[1] = mVoice[i]->mLVolume;
				panr[1] = mVoice[i]->mRVolume;

				if (mVoice[i]->mPauseScheduler.mActive)
				{
					mVoice[i]->mPauseScheduler.get(mVoice[i]->mStreamTime);
					if (mVoice[i]->mPauseScheduler.mActive == -1)
					{
						mVoice[i]->mPauseScheduler.mActive = 0;
						setVoicePause(i, 1);
					}
				}

				if (mVoice[i]->mActiveFader)
				{
					mVoice[i]->mFaderVolume[0*2+0] = panl[0] * volume[0] * globalVolume[0];
					mVoice[i]->mFaderVolume[0*2+1] = panl[1] * volume[1] * globalVolume[1];
					mVoice[i]->mFaderVolume[1*2+0] = panr[0] * volume[0] * globalVolume[0];
					mVoice[i]->mFaderVolume[1*2+1] = panr[1] * volume[1] * globalVolume[1];
				}

				if (mVoice[i]->mStopScheduler.mActive)
				{
					mVoice[i]->mStopScheduler.get(mVoice[i]->mStreamTime);
					if (mVoice[i]->mStopScheduler.mActive == -1)
					{
						mVoice[i]->mStopScheduler.mActive = 0;
						stopVoice(i);
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
		for (i = 0; i < mVoiceCount; i++)
		{
			if (mVoice[i] && !(mVoice[i]->mFlags & AudioInstance::PAUSED))
			{

				float stepratio = mVoice[i]->mSamplerate / mSamplerate;
				float step = 0;
				int j;

				int readsamples = (int)ceil(aSamples * stepratio);

				mVoice[i]->getAudio(mScratch, readsamples);	

				for (j = 0; j < FILTERS_PER_STREAM; j++)
				{
					if (mVoice[i]->mFilter[j])
					{
						mVoice[i]->mFilter[j]->filter(
							mScratch, 
							readsamples, 
							mVoice[i]->mFlags & AudioInstance::STEREO, 
							mVoice[i]->mSamplerate,
							mStreamTime);
					}
				}

				if (mVoice[i]->mActiveFader)
				{
					float lpan = mVoice[i]->mFaderVolume[0];
					float rpan = mVoice[i]->mFaderVolume[2];
					float lpani = (mVoice[i]->mFaderVolume[1] - mVoice[i]->mFaderVolume[0]) / aSamples;
					float rpani = (mVoice[i]->mFaderVolume[3] - mVoice[i]->mFaderVolume[2]) / aSamples;

					if (mVoice[i]->mFlags & AudioInstance::STEREO)
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
					float lpan = mVoice[i]->mLVolume * mVoice[i]->mVolume * mGlobalVolume;
					float rpan = mVoice[i]->mRVolume * mVoice[i]->mVolume * mGlobalVolume;
					if (mVoice[i]->mFlags & AudioInstance::STEREO)
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

				// clear voice if the sound is over
				if (!(mVoice[i]->mFlags & AudioInstance::LOOPING) && mVoice[i]->hasEnded())
				{
					stopVoice(i);
				}
			}
		}

		for (i = 0; i < FILTERS_PER_STREAM; i++)
		{
			if (mFilterInstance[i])
			{
				mFilterInstance[i]->filter(aBuffer, aSamples, 1, (float)mSamplerate, mStreamTime);
			}
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
