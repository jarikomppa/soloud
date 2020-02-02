/*
SoLoud audio engine
Copyright (c) 2013-2020 Jari Komppa

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

		float omega = (float)((2.0f * M_PI * mParam[FREQUENCY]) / mParam[SAMPLERATE]);
		float sin_omega = (float)sin(omega);
		float cos_omega = (float)cos(omega);
		float alpha = sin_omega / (2.0f * mParam[RESONANCE]);
		float scalar = 1.0f / (1.0f + alpha);

		switch ((int)(mParam[TYPE]))
		{
		default:
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

		initParams(5);
		
		mParam[SAMPLERATE] = aParent->mSampleRate;
		mParam[RESONANCE] = aParent->mResonance;
		mParam[FREQUENCY] = aParent->mFrequency;
		mParam[TYPE] = aParent->mFilterType;
		mParam[WET] = 1;

		calcBQRParams();
	}

	void BiquadResonantFilterInstance::filterChannel(float *aBuffer, unsigned int aSamples, float /*aSamplerate*/, double aTime, unsigned int aChannel, unsigned int /*aChannels*/)
	{
		if (aChannel == 0)
		{
			updateParams(aTime);

			if (mParamChanged & ((1 << FREQUENCY) | (1 << RESONANCE) | (1 << SAMPLERATE) | (1 << TYPE)))
			{
				calcBQRParams();
			}
			mParamChanged = 0;
		}

		float x;
		unsigned int i;
		int c = 0;

		BQRStateData &s = mState[aChannel];

		for (i = 0; i < aSamples; i +=2, c++)
		{
			// Generate outputs by filtering inputs.
			x = aBuffer[c];
			s.mY2 = (mA0 * x) + (mA1 * s.mX1) + (mA2 * s.mX2) - (mB1 * s.mY1) - (mB2 * s.mY2);
			aBuffer[c] += (s.mY2 - aBuffer[c]) * mParam[WET];

			c++;

			// Permute filter operations to reduce data movement.
			// Just substitute variables instead of doing mX1=x, etc.
			s.mX2 = aBuffer[c];
			s.mY1 = (mA0 * s.mX2) + (mA1 * x) + (mA2 * s.mX1) - (mB1 * s.mY2) - (mB2 * s.mY1);
			aBuffer[c] += (s.mY1 - aBuffer[c]) * mParam[WET];

			// Only move a little data.
			s.mX1 = s.mX2;
			s.mX2 = x;
		}

		// Apply a small impulse to filter to prevent arithmetic underflow,
		// which can cause the FPU to interrupt the CPU.
		s.mY1 += (float) 1.0E-26;		
	}


	BiquadResonantFilterInstance::~BiquadResonantFilterInstance()
	{
	}

	BiquadResonantFilter::BiquadResonantFilter()
	{
		setParams(LOWPASS, 44100, 1000, 2);
	}

	result BiquadResonantFilter::setParams(int aType, float aSampleRate, float aFrequency, float aResonance)
	{
		if (aType < 0 || aType > 3 || aSampleRate <= 0 || aFrequency <= 0 || aResonance <= 0)
			return INVALID_PARAMETER;

		mFilterType = aType;
		mSampleRate = aSampleRate;
		mFrequency = aFrequency;
		mResonance = aResonance;

		return 0;
	}

	int BiquadResonantFilter::getParamCount()
	{
		return 5;
	}

	const char* BiquadResonantFilter::getParamName(unsigned int aParamIndex)
	{
		if (aParamIndex < 0 || aParamIndex > 4)
			return 0;

		const char* name[5] = {
			"Wet",
			"Type",
			"Samplerate",
			"Frequency",
			"Resonance"
		};
		return name[aParamIndex];
	}
	
	unsigned int BiquadResonantFilter::getParamType(unsigned int aParamIndex)
	{
		if (aParamIndex == TYPE)
			return INT_PARAM;
		return FLOAT_PARAM;
	}

	float BiquadResonantFilter::getParamMax(unsigned int aParamIndex)
	{
		switch (aParamIndex)
		{
		case WET: return 1;
		case TYPE: return 2;
		case SAMPLERATE: return 44100;
		case FREQUENCY: return 22000;
		case RESONANCE: return 20;
		}
		return 1;
	}
	
	float BiquadResonantFilter::getParamMin(unsigned int aParamIndex)
	{
		switch (aParamIndex)
		{
		case SAMPLERATE: return 100;
		case FREQUENCY: return 1;
		case RESONANCE: return 0.1;
		}
		return 0;
	}


	BiquadResonantFilter::~BiquadResonantFilter()
	{
	}


	BiquadResonantFilterInstance *BiquadResonantFilter::createInstance()
	{
		return new BiquadResonantFilterInstance(this);
	}
}
