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

#include <math.h>
#include <string.h>
#include "soloud.h"
#include "soloud_lofifilter.h"

namespace SoLoud
{

	LofiFilterInstance::LofiFilterInstance(LofiFilter *aParent)
	{
		mParent = aParent;
		mSampleRate = aParent->mSampleRate;
		mBitdepth = aParent->mBitdepth;
		mChannelData[0].mSample = 0;
		mChannelData[0].mSamplesToSkip = 0;
		mChannelData[1].mSample = 0;
		mChannelData[1].mSamplesToSkip = 0;
	}

	void LofiFilterInstance::filterChannel(float *aBuffer, int aSamples, float aSamplerate, float aTime, int aChannel, int aChannels)
	{
		if (mSampleRateFader.mActive > 0)
		{
			mSampleRate = mSampleRateFader.get(aTime);
		}
		if (mBitdepthFader.mActive > 0)
		{
			mBitdepth = mBitdepthFader.get(aTime);
		}

		int i;
		for (i = 0; i < aSamples; i++)
		{
			if (mChannelData[aChannel].mSamplesToSkip <= 0)
			{
				mChannelData[aChannel].mSamplesToSkip += (aSamplerate / mSampleRate) - 1;
				float q = pow(2, mBitdepth);
				mChannelData[aChannel].mSample = floor(q*aBuffer[i])/q;
			}
			else
			{
				mChannelData[aChannel].mSamplesToSkip--;
			}
			aBuffer[i] = mChannelData[aChannel].mSample;			
		}

	}

	void LofiFilterInstance::setFilterParameter(int aAttributeId, float aValue)
	{
		switch (aAttributeId)
		{
		case LofiFilter::SAMPLERATE:
			mSampleRateFader.mActive = 0;
			mSampleRate = aValue;
			break;
		case LofiFilter::BITDEPTH:
			mBitdepthFader.mActive = 0;
			mBitdepth = aValue;
			break;
		}
	}

	float LofiFilterInstance::getFilterParameter(int aAttributeId)
	{
		switch (aAttributeId)
		{
		case LofiFilter::BITDEPTH:
			return mBitdepth;
		case LofiFilter::SAMPLERATE:
			return mSampleRate;
		}
		return 0;
	}

	void LofiFilterInstance::fadeFilterParameter(int aAttributeId, float aTo, float aTime, float aStartTime)
	{
		if (aTime <= 0) return;
		switch (aAttributeId)
		{
		case LofiFilter::SAMPLERATE:
			if (mSampleRate == aTo) return;
			mSampleRateFader.set(mSampleRate, aTo, aTime, aStartTime);
			break;
		case LofiFilter::BITDEPTH:
			if (mBitdepth == aTo) return;
			mBitdepthFader.set(mBitdepth, aTo, aTime, aStartTime);
			break;
		}
	}

	void LofiFilterInstance::oscillateFilterParameter(int aAttributeId, float aFrom, float aTo, float aTime, float aStartTime)
	{
		if (aFrom == aTo || aTime <= 0) return;
		switch (aAttributeId)
		{
		case LofiFilter::SAMPLERATE:
			mSampleRateFader.setLFO(aFrom, aTo, aTime, aStartTime);
			break;
		case LofiFilter::BITDEPTH:
			mBitdepthFader.setLFO(aFrom, aTo, aTime, aStartTime);
			break;
		}
	}

	LofiFilterInstance::~LofiFilterInstance()
	{
	}

	LofiFilter::LofiFilter()
	{
		setParams(4000, 3);
	}

	void LofiFilter::setParams(float aSampleRate, float aBitdepth)
	{
		mSampleRate = aSampleRate;
		mBitdepth = aBitdepth;
	}

	LofiFilter::~LofiFilter()
	{
	}


	LofiFilterInstance *LofiFilter::createInstance()
	{
		return new LofiFilterInstance(this);
	}
}
