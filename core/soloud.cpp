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

	AudioProducer::AudioProducer()
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

	AudioProducer::~AudioProducer()
	{
	}

	void AudioProducer::init(int aPlayIndex, float aBaseSamplerate, int aFactoryFlags)
	{
		mPlayIndex = aPlayIndex;
		mBaseSamplerate = aBaseSamplerate;
		mSamplerate = mBaseSamplerate;
		mStreamTime = 0.0f;
		mFlags = 0;

		if (aFactoryFlags & AudioFactory::SHOULD_LOOP)
		{
			mFlags |= AudioProducer::LOOPING;
		}

		if (aFactoryFlags & AudioFactory::STEREO)
		{
			mFlags |= AudioProducer::STEREO;
		}
	}

	int AudioProducer::rewind()
	{
		return 0;
	}

	void AudioProducer::seek(float aSeconds, float *mScratch, int mScratchSize)
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
	}


	///////////////////////////////////////////////////////////////////////////

	AudioFactory::AudioFactory() 
	{ 
		mFlags = 0; 
		mBaseSamplerate = 44100; 
	}

	AudioFactory::~AudioFactory() 
	{
	}

	void AudioFactory::setLooping(int aLoop)
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
		lockMutex = NULL;
		unlockMutex = NULL;
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
		mChannel = new AudioProducer*[aChannels];
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
			if (((mChannel[i]->mFlags & AudioProducer::PROTECTED) == 0) && 
				mChannel[i]->mPlayIndex < lowest_play_index_value)
			{
				lowest_play_index_value = mChannel[i]->mPlayIndex;
				lowest_play_index = i;
			}
		}
		stop(lowest_play_index);
		return lowest_play_index;
	}

	int Soloud::play(AudioFactory &aSound, float aVolume, float aPan, int aPaused)
	{
		if (lockMutex) lockMutex();
		int ch = findFreeChannel();
		if (ch < 0) 
		{
			if (unlockMutex) unlockMutex();
			return -1;
		}
		mChannel[ch] = aSound.createProducer();
		int handle = ch | (mPlayIndex << 8);

		mChannel[ch]->init(mPlayIndex, aSound.mBaseSamplerate, aSound.mFlags);

		if (aPaused)
		{
			mChannel[ch]->mFlags |= AudioProducer::PAUSED;
		}

		// TODO: mutex is locked at this point, but the following lock/unlock it too..
		setPan(handle, aPan);
		setVolume(handle, aVolume);
		setRelativePlaySpeed(handle, 1);

		mPlayIndex++;
		int scratchneeded = (int)ceil((mChannel[ch]->mSamplerate / mSamplerate) * mBufferSize);
		if (mScratchNeeded < scratchneeded)
		{
			int pot = 1024;
			while (pot < scratchneeded) pot <<= 1;
			mScratchNeeded = pot;
		}
		if (unlockMutex) unlockMutex();
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
		if (lockMutex) lockMutex();
		int i;
		int c = 0;
		for (i = 0; i < mChannelCount; i++)
		{
			if (mChannel[i]) 
			{
				c++;
			}
		}
		if (unlockMutex) unlockMutex();
		return c;
	}

	int Soloud::isValidChannelHandle(int aChannelHandle) const
	{
		if (lockMutex) lockMutex();
		if (getChannelFromHandle(aChannelHandle) != -1) 
		{
			if (unlockMutex) unlockMutex();
			return 1;
		}
		if (unlockMutex) unlockMutex();
		return 0;
	}


	float Soloud::getVolume(int aChannelHandle) const
	{
		if (lockMutex) lockMutex();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (unlockMutex) unlockMutex();
			return 0;
		}
		float v = mChannel[ch]->mVolume;
		if (unlockMutex) unlockMutex();
		return v;
	}

	float Soloud::getStreamTime(int aChannelHandle) const
	{
		if (lockMutex) lockMutex();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (unlockMutex) unlockMutex();
			return 0;
		}
		float v = mChannel[ch]->mStreamTime;
		if (unlockMutex) unlockMutex();
		return v;
	}

	float Soloud::getRelativePlaySpeed(int aChannelHandle) const
	{
		if (lockMutex) lockMutex();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (unlockMutex) unlockMutex();
			return 1;
		}
		float v = mChannel[ch]->mRelativePlaySpeed;
		if (unlockMutex) unlockMutex();
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
		if (lockMutex) lockMutex();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (unlockMutex) unlockMutex();
			return;
		}
		setChannelRelativePlaySpeed(ch, aSpeed);
		if (unlockMutex) unlockMutex();
	}

	float Soloud::getSamplerate(int aChannelHandle) const
	{
		if (lockMutex) lockMutex();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (unlockMutex) unlockMutex();
			return 0;
		}
		float v = mChannel[ch]->mBaseSamplerate;
		if (unlockMutex) unlockMutex();
		return v;
	}

	void Soloud::setSamplerate(int aChannelHandle, float aSamplerate)
	{
		if (lockMutex) lockMutex();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (unlockMutex) unlockMutex();
			return;
		}
		mChannel[ch]->mBaseSamplerate = aSamplerate;
		mChannel[ch]->mSamplerate = mChannel[ch]->mBaseSamplerate * mChannel[ch]->mRelativePlaySpeed;
		if (unlockMutex) unlockMutex();
	}

	void Soloud::seek(int aChannelHandle, float aSeconds)
	{
		if (lockMutex) lockMutex();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (unlockMutex) unlockMutex();
			return;
		}
		mChannel[ch]->seek(aSeconds, mScratch, mScratchSize);
		if (unlockMutex) unlockMutex();
	}

	void Soloud::setPause(int aChannelHandle, int aPause)
	{
		if (lockMutex) lockMutex();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (unlockMutex) unlockMutex();
			return;
		}
		if (aPause)
		{
			mChannel[ch]->mFlags |= AudioProducer::PAUSED;
		}
		else
		{
			mChannel[ch]->mFlags &= ~AudioProducer::PAUSED;
		}
		if (unlockMutex) unlockMutex();
	}

	void Soloud::setPauseAll(int aPause)
	{
		if (lockMutex) lockMutex();
		int ch;
		for (ch = 0; ch < mChannelCount; ch++)
		{
			if (mChannel[ch])
			{
				if (aPause)
				{
					mChannel[ch]->mFlags |= AudioProducer::PAUSED;
				}
				else
				{
					mChannel[ch]->mFlags &= ~AudioProducer::PAUSED;
				}
			}
		}
		if (unlockMutex) unlockMutex();
	}


	int Soloud::getPause(int aChannelHandle) const
	{
		if (lockMutex) lockMutex();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (unlockMutex) unlockMutex();
			return 0;
		}
		int v = !!(mChannel[ch]->mFlags & AudioProducer::PAUSED);
		if (unlockMutex) unlockMutex();
		return v;
	}

	int Soloud::getProtectChannel(int aChannelHandle) const
	{
		if (lockMutex) lockMutex();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (unlockMutex) unlockMutex();
			return 0;
		}
		int v = !!(mChannel[ch]->mFlags & AudioProducer::PROTECTED);
		if (unlockMutex) unlockMutex();
		return v;
	}

	void Soloud::setProtectChannel(int aChannelHandle, int aProtect)
	{
		if (lockMutex) lockMutex();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (unlockMutex) unlockMutex();
			return;
		}
		if (aProtect)
		{
			mChannel[ch]->mFlags |= AudioProducer::PROTECTED;
		}
		else
		{
			mChannel[ch]->mFlags &= ~AudioProducer::PROTECTED;
		}
		if (unlockMutex) unlockMutex();
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
		if (lockMutex) lockMutex();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (unlockMutex) unlockMutex();
			return;
		}
		setChannelPan(ch, aPan);
		if (unlockMutex) unlockMutex();
	}

	void Soloud::setPanAbsolute(int aChannelHandle, float aLVolume, float aRVolume)
	{
		if (lockMutex) lockMutex();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (unlockMutex) unlockMutex();
			return;
		}
		mChannel[ch]->mLVolume = aLVolume;
		mChannel[ch]->mRVolume = aRVolume;
		if (unlockMutex) unlockMutex();
	}

	void Soloud::setVolume(int aChannelHandle, float aVolume)
	{
		if (lockMutex) lockMutex();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (unlockMutex) unlockMutex();
			return;
		}
		mChannel[ch]->mVolume = aVolume;
		if (unlockMutex) unlockMutex();
	}

	void Soloud::stop(int aChannelHandle)
	{
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			return;
		}
		if (lockMutex) lockMutex();
		stopChannel(ch);
		if (unlockMutex) unlockMutex();
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
		if (lockMutex) lockMutex();
		for (i = 0; i < mChannelCount; i++)
		{
			stopChannel(i);
		}
		if (unlockMutex) unlockMutex();
	}

	void Soloud::fadeVolume(int aChannelHandle, float aFrom, float aTo, float aTime)
	{
		if (lockMutex) lockMutex();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (unlockMutex) unlockMutex();
			return;
		}
		mChannel[ch]->mVolumeFader.set(aFrom, aTo, aTime, mChannel[ch]->mStreamTime);
		if (unlockMutex) unlockMutex();
	}

	void Soloud::fadePan(int aChannelHandle, float aFrom, float aTo, float aTime)
	{
		if (lockMutex) lockMutex();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (unlockMutex) unlockMutex();
			return;
		}
		mChannel[ch]->mPanFader.set(aFrom, aTo, aTime, mChannel[ch]->mStreamTime);
		if (unlockMutex) unlockMutex();
	}

	void Soloud::fadeRelativePlaySpeed(int aChannelHandle, float aFrom, float aTo, float aTime)
	{
		if (lockMutex) lockMutex();
		int ch = getChannelFromHandle(aChannelHandle);
		if (ch == -1) 
		{
			if (unlockMutex) unlockMutex();
			return;
		}
		mChannel[ch]->mRelativePlaySpeedFader.set(aFrom, aTo, aTime, mChannel[ch]->mStreamTime);
		if (unlockMutex) unlockMutex();
	}

	void Soloud::fadeGlobalVolume(float aFrom, float aTo, float aTime)
	{
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

		if (lockMutex) lockMutex();

		// Process faders. May change scratch size.
		int i;
		for (i = 0; i < mChannelCount; i++)
		{
			if (mChannel[i] && !(mChannel[i]->mFlags & AudioProducer::PAUSED))
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
			if (mChannel[i] && !(mChannel[i]->mFlags & AudioProducer::PAUSED))
			{
				float lpan = mChannel[i]->mLVolume * mChannel[i]->mVolume * mGlobalVolume;
				float rpan = mChannel[i]->mRVolume * mChannel[i]->mVolume * mGlobalVolume;

				float stepratio = mChannel[i]->mSamplerate / mSamplerate;
				mChannel[i]->getAudio(mScratch, (int)ceil(aSamples * stepratio));		

				int j;
				float step = 0;
				if (mChannel[i]->mFlags & AudioProducer::STEREO)
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
				if (!(mChannel[i]->mFlags & AudioProducer::LOOPING) && mChannel[i]->hasEnded())
				{
					stopChannel(i);
				}
			}
		}

		if (unlockMutex) unlockMutex();

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
