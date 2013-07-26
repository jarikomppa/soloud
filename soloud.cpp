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

	float Soloud::getPostClipScaler()
	{
		return mPostClipScaler;
	}

	void Soloud::setPostClipScaler(float aScaler)
	{
		mPostClipScaler = aScaler;
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

	void Soloud::setVolume(float aVolume)
	{
		mGlobalVolume = aVolume;
	}		

	int Soloud::findFreeChannel()
	{
		int i;
		unsigned int lpi = 0xffffffff;
		int lpii = -1;
		for (i = 0; i < mChannelCount; i++)
		{
			if (mChannel[i] == NULL)
			{
				return i;
			}
			if (((mChannel[i]->mFlags & AudioProducer::PROTECTED) == 0) && 
				mChannel[i]->mPlayIndex < lpi)
			{
				lpii = i;
			}
		}
		stop(lpii);
		return lpii;
	}

	void AudioProducer::init(int aPlayIndex, float aBaseSamplerate, int aFactoryFlags)
	{
		mPlayIndex = aPlayIndex;
		mBaseSamplerate = aBaseSamplerate;
		mSamplerate = mBaseSamplerate;
		mFlags = 0;

		if (aFactoryFlags & AudioFactory::SHOULD_LOOP)
		{
			mFlags |= AudioProducer::LOOPING;
		}
	}

	int Soloud::play(AudioFactory &aSound, float aVolume, float aPan)
	{
		int ch = findFreeChannel();
		mChannel[ch] = aSound.createProducer();
		int handle = ch | (mPlayIndex << 8);

		mChannel[ch]->init(mPlayIndex, aSound.mBaseSamplerate, aSound.mFlags);

		setPan(ch, aPan);
		setVolume(ch, aVolume);
		setRelativePlaySpeed(ch, 1);

		mPlayIndex++;
		int scratchneeded = (int)ceil((mChannel[ch]->mSamplerate / mSamplerate) * mBufferSize);
		if (mScratchNeeded < scratchneeded)
		{
			int pot = 1024;
			while (pot < scratchneeded) pot <<= 1;
			mScratchNeeded = pot;
		}
		return handle;
	}	

	int Soloud::getAbsoluteChannelFromHandle(int aChannel)
	{
		int ch = aChannel & 0xff;
		unsigned int idx = aChannel >> 8;
		if (mChannel[ch] &&
			(mChannel[ch]->mPlayIndex & 0xffffff) == idx)
			return ch;
		return -1;		
	}

	int Soloud::getActiveVoices()
	{
		int i;
		int c = 0;
		for (i = 0; i < mChannelCount; i++)
		{
			if (mChannel[i]) 
			{
				c++;
			}
		}
		return c;
	}

	int Soloud::isValidChannelHandle(int aChannel)
	{
		if (getAbsoluteChannelFromHandle(aChannel) != -1) 
		{
			return 1;
		}
		return 0;
	}


	float Soloud::getVolume(int aChannel)
	{
		int ch = getAbsoluteChannelFromHandle(aChannel);
		if (ch == -1) return 0;
		return mChannel[ch]->mVolume;
	}

	float Soloud::getRelativePlaySpeed(int aChannel)
	{
		int ch = getAbsoluteChannelFromHandle(aChannel);
		if (ch == -1) return 1;
		return mChannel[ch]->mRelativePlaySpeed;
	}

	void Soloud::setRelativePlaySpeed(int aChannel, float aSpeed)
	{
		int ch = getAbsoluteChannelFromHandle(aChannel);
		if (ch == -1) return;
		mChannel[ch]->mRelativePlaySpeed = aSpeed;
		mChannel[ch]->mSamplerate = mChannel[ch]->mBaseSamplerate * mChannel[ch]->mRelativePlaySpeed;
	}

	float Soloud::getSamplerate(int aChannel)
	{
		int ch = getAbsoluteChannelFromHandle(aChannel);
		if (ch == -1) return 0;
		return mChannel[ch]->mBaseSamplerate;
	}

	void Soloud::setSamplerate(int aChannel, float aSamplerate)
	{
		int ch = getAbsoluteChannelFromHandle(aChannel);
		if (ch == -1) return;
		mChannel[ch]->mBaseSamplerate = aSamplerate;
		mChannel[ch]->mSamplerate = mChannel[ch]->mBaseSamplerate * mChannel[ch]->mRelativePlaySpeed;
	}

	int Soloud::getProtectChannel(int aChannel)
	{
		int ch = getAbsoluteChannelFromHandle(aChannel);
		if (ch == -1) return 0;
		return !!(mChannel[ch]->mFlags & AudioProducer::PROTECTED);
	}

	void Soloud::setProtectChannel(int aChannel, int aProtect)
	{
		int ch = getAbsoluteChannelFromHandle(aChannel);
		if (ch == -1) return;
		if (aProtect)
		{
			mChannel[ch]->mFlags |= AudioProducer::PROTECTED;
		}
		else
		{
			mChannel[ch]->mFlags &= ~AudioProducer::PROTECTED;
		}
	}

	void Soloud::setPan(int aChannel, float aPan)
	{
		setPanAbsolute(
			aChannel,
			(float)cos((aPan + 1) * M_PI / 4),
			(float)sin((aPan + 1) * M_PI / 4));
	}

	void Soloud::setPanAbsolute(int aChannel, float aLVolume, float aRVolume)
	{
		int ch = getAbsoluteChannelFromHandle(aChannel);
		if (ch == -1) return;
		mChannel[ch]->mLVolume = aLVolume;
		mChannel[ch]->mRVolume = aRVolume;
	}

	void Soloud::setVolume(int aChannel, float aVolume)
	{
		int ch = getAbsoluteChannelFromHandle(aChannel);
		if (ch == -1) return;
		mChannel[ch]->mVolume = aVolume;
	}

	void Soloud::stop(int aChannel)
	{
		int ch = getAbsoluteChannelFromHandle(aChannel);
		if (ch == -1) return;
		stopAbsolute(ch);
	}

	void Soloud::stopAbsolute(int aAbsoluteChannel)
	{
		if (mChannel[aAbsoluteChannel])
		{
			delete mChannel[aAbsoluteChannel];
			mChannel[aAbsoluteChannel] = 0;			
		}
	}		

	void Soloud::stopAll()
	{
		int i;
		for (i = 0; i < mChannelCount; i++)
		{
			stopAbsolute(i);
		}
	}

	void Soloud::mix(float *aBuffer, int aSamples)
	{
		if (mScratchSize < mScratchNeeded)
		{
			mScratchSize = mScratchNeeded;
			delete[] mScratch;
			mScratch = new float[mScratchSize];
		}

		int i;

		// Clear accumulation buffer
		for (i = 0; i < aSamples*2; i++)
		{
			aBuffer[i] = 0;
		}

		// Accumulate sound sources
		for (i = 0; i < mChannelCount; i++)
		{
			if (mChannel[i])
			{
				float lpan = mChannel[i]->mLVolume * mChannel[i]->mVolume * mGlobalVolume;
				float rpan = mChannel[i]->mRVolume * mChannel[i]->mVolume * mGlobalVolume;

				float stepratio = mChannel[i]->mSamplerate / mSamplerate;
				mChannel[i]->getAudio(mScratch, (int)ceil(aSamples * stepratio));					

				int j;
				float step = 0;
				for (j = 0; j < aSamples; j++, step += stepratio)
				{
					float s = mScratch[(int)floor(step)];
					aBuffer[j * 2 + 0] += s * lpan;
					aBuffer[j * 2 + 1] += s * rpan;
				}

				// chear channel if the sound is over
				if (!(mChannel[i]->mFlags & AudioProducer::LOOPING) && mChannel[i]->hasEnded())
				{
					stopAbsolute(i);
				}
			}
		}

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
	}

	Soloud::~Soloud()
	{
		stopAll();
		delete[] mScratch;
		delete[] mChannel;
	}
};
