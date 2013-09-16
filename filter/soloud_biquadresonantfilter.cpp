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

--

Based on "Using the Biquad Resonant Filter", 
Phil Burk, Game Programming Gems 3, p. 606
*/

#include <math.h>
#include <string.h>
#include "soloud.h"
#include "soloud_biquadresonantfilter.h"

namespace SoLoud
{
	void BiquadResonantFilterInstance::calcBQRParams()
	{
		mDirty = 0;

		float omega = (float)((2.0f * M_PI * mFrequency) / mSampleRate);
		float sin_omega = (float)sin(omega);
		float cos_omega = (float)cos(omega);
		float alpha = sin_omega / (2.0f * mResonance);
		float scalar = 1.0f / (1.0f + alpha);

		mActive = 1;

		switch (mFilterType)
		{
		case BiquadResonantFilter::NONE:
			mActive = 0;
			break;
		case BiquadResonantFilter::LOWPASS:
			mA0 = 0.5f * (1.0f - cos_omega) * scalar;
			mA1 = (1.0f - cos_omega) * scalar;
			mA2 = mA0;
			mB1 = -2.0f * cos_omega * scalar;
			mB2 = (1.0f - alpha) * scalar;
			break;
		case BiquadResonantFilter::HIGHPASS:
			mA0 = 0.5f * (1.0f + cos_omega) * scalar;
			mA1 = -(1.0f + cos_omega) * scalar;
			mA2 = mA0;
			mB1 = -2.0f * cos_omega * scalar;
			mB2 = (1.0f - alpha) * scalar;
			break;
		case BiquadResonantFilter::BANDPASS:
			mA0 = alpha * scalar;
			mA1 = 0;
			mA2 = -mA0;
			mB1 = -2.0f * cos_omega * scalar;
			mB2 = (1.0f - alpha) * scalar;
			break;
		}
	}


	BiquadResonantFilterInstance::BiquadResonantFilterInstance(BiquadResonantFilter *aParent)
	{
		int i;
		for (i = 0; i < 2; i++)
		{
			mState[i].mX1 = 0;
			mState[i].mY1 = 0;
			mState[i].mX2 = 0;
			mState[i].mY2 = 0;
		}

		mParent = aParent;
		mFilterType = aParent->mFilterType;
		mSampleRate = aParent->mSampleRate;
		mResonance = aParent->mResonance;

		calcBQRParams();
	}

	void BiquadResonantFilterInstance::filterChannel(float *aBuffer, int aSamples, float aSamplerate, float aTime, int aChannel, int aChannels)
	{
		if (!mActive)
			return;

		if (mFrequencyFader.mActive > 0)
		{
			mDirty = 1;
			mFrequency = mFrequencyFader.get(aTime);
		}

		if (mResonanceFader.mActive > 0)
		{
			mDirty = 1;
			mResonance = mResonanceFader.get(aTime);
		}

		if (mSampleRateFader.mActive > 0)
		{
			mDirty = 1;
			mSampleRate = mSampleRateFader.get(aTime);
		}

		if (mDirty)
		{
			calcBQRParams();
		}

		float x;
		int i;
		int c = 0;

		BQRStateData &s = mState[aChannel];

		for (i = 0; i < aSamples; i +=2, c++)
		{
			// Generate outputs by filtering inputs.
			x = aBuffer[c];
			s.mY2 = (mA0 * x) + (mA1 * s.mX1) + (mA2 * s.mX2) - (mB1 * s.mY1) - (mB2 * s.mY2);
			aBuffer[c] = s.mY2;

			c++;

			// Permute filter operations to reduce data movement.
			// Just substitute variables instead of doing mX1=x, etc.
			s.mX2 = aBuffer[c];
			s.mY1 = (mA0 * s.mX2) + (mA1 * x) + (mA2 * s.mX1) - (mB1 * s.mY2) - (mB2 * s.mY1);
			aBuffer[c] = s.mY1;

			// Only move a little data.
			s.mX1 = s.mX2;
			s.mX2 = x;
		}

		// Apply a small impulse to filter to prevent arithmetic underflow,
		// which can cause the FPU to interrupt the CPU.
		s.mY1 += (float) 1.0E-26;		
	}

	void BiquadResonantFilterInstance::setFilterParameter(int aAttributeId, float aValue)
	{
		switch (aAttributeId)
		{
		case BiquadResonantFilter::FREQUENCY:
			mDirty = 1;
			mFrequencyFader.mActive = 0;
			mFrequency = aValue;
			break;
		case BiquadResonantFilter::SAMPLERATE:
			mDirty = 1;
			mSampleRateFader.mActive = 0;
			mSampleRate = aValue;
			break;
		case BiquadResonantFilter::RESONANCE:
			mDirty = 1;
			mResonanceFader.mActive = 0;
			mResonance = aValue;
			break;
		}
	}

	float BiquadResonantFilterInstance::getFilterParameter(int aAttributeId)
	{
		switch (aAttributeId)
		{
		case BiquadResonantFilter::FREQUENCY:
			return mFrequency;
		case BiquadResonantFilter::SAMPLERATE:
			return mSampleRate;
		case BiquadResonantFilter::RESONANCE:
			return mResonance;
		}
		return 0;
	}

	void BiquadResonantFilterInstance::fadeFilterParameter(int aAttributeId, float aFrom, float aTo, float aTime, float aStartTime)
	{
		if (aFrom == aTo || aTime <= 0) return;
		switch (aAttributeId)
		{
		case BiquadResonantFilter::FREQUENCY:
			mFrequencyFader.set(aFrom, aTo, aTime, aStartTime);
			break;
		case BiquadResonantFilter::SAMPLERATE:
			mSampleRateFader.set(aFrom, aTo, aTime, aStartTime);
			break;
		case BiquadResonantFilter::RESONANCE:
			mResonanceFader.set(aFrom, aTo, aTime, aStartTime);
			break;
		}
	}

	void BiquadResonantFilterInstance::oscillateFilterParameter(int aAttributeId, float aFrom, float aTo, float aTime, float aStartTime)
	{
		if (aFrom == aTo || aTime <= 0) return;
		switch (aAttributeId)
		{
		case BiquadResonantFilter::FREQUENCY:
			mFrequencyFader.setLFO(aFrom, aTo, aTime, aStartTime);
			break;
		case BiquadResonantFilter::SAMPLERATE:
			mSampleRateFader.setLFO(aFrom, aTo, aTime, aStartTime);
			break;
		case BiquadResonantFilter::RESONANCE:
			mResonanceFader.setLFO(aFrom, aTo, aTime, aStartTime);
			break;
		}
	}

	BiquadResonantFilterInstance::~BiquadResonantFilterInstance()
	{
	}

	BiquadResonantFilter::BiquadResonantFilter()
	{
		setParams(LOWPASS, 44100, 1000, 2);
	}

	void BiquadResonantFilter::setParams(int aType, float aSampleRate, float aFrequency, float aResonance)
	{
		mFilterType = aType;
		mSampleRate = aSampleRate;
		mFrequency = aFrequency;
		mResonance = aResonance;
	}

	BiquadResonantFilter::~BiquadResonantFilter()
	{
	}


	BiquadResonantFilterInstance *BiquadResonantFilter::createInstance()
	{
		return new BiquadResonantFilterInstance(this);
	}
}
