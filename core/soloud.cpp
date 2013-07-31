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

	Fader::Fader()
	{
		mFrom = mTo = mDelta = mTime = mStartTime = mEndTime = 0;
		mActive = 0;
	}

	void Fader::set(float aFrom, float aTo, float aTime, float aStartTime)
	{
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
		if (aCurrentTime > mEndTime)
		{
			mActive = 0;
			return mTo;
		}
		return mFrom + mDelta * ((aCurrentTime - mStartTime) / mTime);
	}

	///////////////////////////////////////////////////////////////////////////

	AudioInstance::AudioInstance()
	{
		mPlayIndex = 0;
		mFlags = 0;
		mLVolume = 1.0f / (float)sqrt(2.0);
		mRVolume = 1.0f / (float)sqrt(2.0);
		mVolume = 1.0f;
		mBaseSamplerate = 44100.0f;
		mSamplerate = 44100.0f;
		mRelativePlaySpeed = 1.0f;
		mStreamTime = 0.0f;
	}

	AudioInstance::~AudioInstance()
	{
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
	}

	AudioSource::~AudioSource() 
	{
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
		mMixerData = NULL;
		mPostClipScaler = 0;
		mLockMutexFunc = NULL;
		mUnlockMutexFunc = NULL;
		mStreamTime = 0;
	}

	Soloud::~Soloud()
	{
		stopAll();
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
		mScratchSize = 0;
		mScratchNeeded = 1;
		mBufferSize = aBufferSize;
		mFlags = aFlags;
		mPostClipScaler = 0.5f;
	}

	float Soloud::getPostClipScaler() const
	{
		return mPostClipScaler;
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
		if (mLockMutexFunc) mLockMutexFunc();
		int ch = findFreeChannel();
		if (ch < 0) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc();
			return -1;
		}
		mChannel[ch] = aSound.createInstance();
		int handle = ch | (mPlayIndex << 8);

		mChannel[ch]->init(mPlayIndex, aSound.mBaseSamplerate, aSound.mFlags);

		if (aPaused)
		{
			mChannel[ch]->mFlags |= AudioInstance::PAUSED;
		}

		setChannelPan(ch, aPan);
		setChannelVolume(ch, aVolume);
		setChannelRelativePlaySpeed(ch, 1);

		mPlayIndex++;
		int scratchneeded = (int)ceil((mChannel[ch]->mSamplerate / mSamplerate) * mBufferSize);
		if (mScratchNeeded < scratchneeded)
		{
			int pot = 1024;
			while (pot < scratchneeded) pot <<= 1;
			mScratchNeeded = pot;
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc();
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
		if (mLockMutexFunc) mLockMutexFunc();
		int i;
		int c = 0;
		for (i = 0; i < mChannelCount; i++)
		{
			if (mChannel[i]) 
			{
				c++;
			}
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc();
		return c;
	}

	int Soloud::isValidChannelHandle(int aChannelHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc();
		if (getChannelFromHandle(aChannelHandle) != -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc();
			return 1;
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc();
		return 0;
	}


	float Soloud::getVolume(int aChannelHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc();
			return 0;
		}
		float v = mChannel[ch]->mVolume;
		if (mUnlockMutexFunc) mUnlockMutexFunc();
		return v;
	}

	float Soloud::getStreamTime(int aChannelHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc();
			return 0;
		}
		float v = mChannel[ch]->mStreamTime;
		if (mUnlockMutexFunc) mUnlockMutexFunc();
		return v;
	}

	float Soloud::getRelativePlaySpeed(int aChannelHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc();
			return 1;
		}
		float v = mChannel[ch]->mRelativePlaySpeed;
		if (mUnlockMutexFunc) mUnlockMutexFunc();
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
		if (mLockMutexFunc) mLockMutexFunc();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc();
			return;
		}
		mChannel[ch]->mRelativePlaySpeedFader.mActive = 0;
		setChannelRelativePlaySpeed(ch, aSpeed);
		if (mUnlockMutexFunc) mUnlockMutexFunc();
	}

	float Soloud::getSamplerate(int aChannelHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc();
			return 0;
		}
		float v = mChannel[ch]->mBaseSamplerate;
		if (mUnlockMutexFunc) mUnlockMutexFunc();
		return v;
	}

	void Soloud::setSamplerate(int aChannelHandle, float aSamplerate)
	{
		if (mLockMutexFunc) mLockMutexFunc();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc();
			return;
		}
		mChannel[ch]->mBaseSamplerate = aSamplerate;
		mChannel[ch]->mSamplerate = mChannel[ch]->mBaseSamplerate * mChannel[ch]->mRelativePlaySpeed;
		if (mUnlockMutexFunc) mUnlockMutexFunc();
	}

	void Soloud::seek(int aChannelHandle, float aSeconds)
	{
		if (mLockMutexFunc) mLockMutexFunc();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc();
			return;
		}
		mChannel[ch]->seek(aSeconds, mScratch, mScratchSize);
		if (mUnlockMutexFunc) mUnlockMutexFunc();
	}

	void Soloud::setPause(int aChannelHandle, int aPause)
	{
		if (mLockMutexFunc) mLockMutexFunc();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc();
			return;
		}
		if (aPause)
		{
			mChannel[ch]->mFlags |= AudioInstance::PAUSED;
		}
		else
		{
			mChannel[ch]->mFlags &= ~AudioInstance::PAUSED;
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc();
	}

	void Soloud::setPauseAll(int aPause)
	{
		if (mLockMutexFunc) mLockMutexFunc();
		int ch;
		for (ch = 0; ch < mChannelCount; ch++)
		{
			if (mChannel[ch])
			{
				if (aPause)
				{
					mChannel[ch]->mFlags |= AudioInstance::PAUSED;
				}
				else
				{
					mChannel[ch]->mFlags &= ~AudioInstance::PAUSED;
				}
			}
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc();
	}


	int Soloud::getPause(int aChannelHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc();
			return 0;
		}
		int v = !!(mChannel[ch]->mFlags & AudioInstance::PAUSED);
		if (mUnlockMutexFunc) mUnlockMutexFunc();
		return v;
	}

	int Soloud::getProtectChannel(int aChannelHandle) const
	{
		if (mLockMutexFunc) mLockMutexFunc();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc();
			return 0;
		}
		int v = !!(mChannel[ch]->mFlags & AudioInstance::PROTECTED);
		if (mUnlockMutexFunc) mUnlockMutexFunc();
		return v;
	}

	void Soloud::setProtectChannel(int aChannelHandle, int aProtect)
	{
		if (mLockMutexFunc) mLockMutexFunc();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc();
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
		if (mUnlockMutexFunc) mUnlockMutexFunc();
	}

	void Soloud::setChannelPan(int aChannel, float aPan)
	{
		if (mChannel[aChannel])
		{
			mChannel[aChannel]->mLVolume = (float)cos((aPan + 1) * M_PI / 4);
			mChannel[aChannel]->mRVolume = (float)sin((aPan + 1) * M_PI / 4);
		}
	}

	void Soloud::setPan(int aChannelHandle, float aPan)
	{		
		if (mLockMutexFunc) mLockMutexFunc();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc();
			return;
		}
		setChannelPan(ch, aPan);
		if (mUnlockMutexFunc) mUnlockMutexFunc();
	}

	void Soloud::setPanAbsolute(int aChannelHandle, float aLVolume, float aRVolume)
	{
		if (mLockMutexFunc) mLockMutexFunc();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc();
			return;
		}
		mChannel[ch]->mPanFader.mActive = 0;
		mChannel[ch]->mLVolume = aLVolume;
		mChannel[ch]->mRVolume = aRVolume;
		if (mUnlockMutexFunc) mUnlockMutexFunc();
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
		if (mLockMutexFunc) mLockMutexFunc();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc();
			return;
		}
		mChannel[ch]->mVolumeFader.mActive = 0;
		setChannelVolume(ch, aVolume);
		if (mUnlockMutexFunc) mUnlockMutexFunc();
	}

	void Soloud::stop(int aChannelHandle)
	{
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			return;
		}
		if (mLockMutexFunc) mLockMutexFunc();
		stopChannel(ch);
		if (mUnlockMutexFunc) mUnlockMutexFunc();
	}

	void Soloud::stopChannel(int aChannel)
	{
		if (mChannel[aChannel])
		{
			delete mChannel[aChannel];
			mChannel[aChannel] = 0;			
		}
	}		

	void Soloud::stopAll()
	{
		int i;
		if (mLockMutexFunc) mLockMutexFunc();
		for (i = 0; i < mChannelCount; i++)
		{
			stopChannel(i);
		}
		if (mUnlockMutexFunc) mUnlockMutexFunc();
	}

	void Soloud::fadeVolume(int aChannelHandle, float aFrom, float aTo, float aTime)
	{
		if (mLockMutexFunc) mLockMutexFunc();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc();
			return;
		}
		mChannel[ch]->mVolumeFader.set(aFrom, aTo, aTime, mChannel[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc();
	}

	void Soloud::fadePan(int aChannelHandle, float aFrom, float aTo, float aTime)
	{
		if (mLockMutexFunc) mLockMutexFunc();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc();
			return;
		}
		mChannel[ch]->mPanFader.set(aFrom, aTo, aTime, mChannel[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc();
	}

	void Soloud::fadeRelativePlaySpeed(int aChannelHandle, float aFrom, float aTo, float aTime)
	{
		if (mLockMutexFunc) mLockMutexFunc();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (mUnlockMutexFunc) mUnlockMutexFunc();
			return;
		}
		mChannel[ch]->mRelativePlaySpeedFader.set(aFrom, aTo, aTime, mChannel[ch]->mStreamTime);
		if (mUnlockMutexFunc) mUnlockMutexFunc();
	}

	void Soloud::fadeGlobalVolume(float aFrom, float aTo, float aTime)
	{
		mStreamTime = 0; // avoid rollover (~6 days)
		mGlobalVolumeFader.set(aFrom, aTo, aTime, mStreamTime);
	}

	void Soloud::mix(float *aBuffer, int aSamples)
	{
		float buffertime = aSamples / (float)mSamplerate;
		mStreamTime += buffertime;

		if (mGlobalVolumeFader.mActive)
		{
			mGlobalVolume = mGlobalVolumeFader.get(mStreamTime);
		}

		if (mLockMutexFunc) mLockMutexFunc();

		// Process faders. May change scratch size.
		int i;
		for (i = 0; i < mChannelCount; i++)
		{
			if (mChannel[i] && !(mChannel[i]->mFlags & AudioInstance::PAUSED))
			{
				mChannel[i]->mStreamTime += buffertime;

				if (mChannel[i]->mVolumeFader.mActive)
				{
					mChannel[i]->mVolume = mChannel[i]->mVolumeFader.get(mChannel[i]->mStreamTime);
				}

				if (mChannel[i]->mRelativePlaySpeedFader.mActive)
				{
					float speed = mChannel[i]->mRelativePlaySpeedFader.get(mChannel[i]->mStreamTime);
					setChannelRelativePlaySpeed(i, speed);
				}

				if (mChannel[i]->mPanFader.mActive)
				{
					float pan = mChannel[i]->mPanFader.get(mChannel[i]->mStreamTime);
					setChannelPan(i, pan);
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
				float lpan = mChannel[i]->mLVolume * mChannel[i]->mVolume * mGlobalVolume;
				float rpan = mChannel[i]->mRVolume * mChannel[i]->mVolume * mGlobalVolume;

				float stepratio = mChannel[i]->mSamplerate / mSamplerate;
				mChannel[i]->getAudio(mScratch, (int)ceil(aSamples * stepratio));		

				int j;
				float step = 0;
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

				// chear channel if the sound is over
				if (!(mChannel[i]->mFlags & AudioInstance::LOOPING) && mChannel[i]->hasEnded())
				{
					stopChannel(i);
				}
			}
		}

		if (mUnlockMutexFunc) mUnlockMutexFunc();

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
	}

};
