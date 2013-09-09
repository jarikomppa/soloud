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
#include "soloud.h"
#include "soloud_biquadresonantfilter.h"

namespace SoLoud
{
	BiquadResonantFilterInstance::BiquadResonantFilterInstance(BiquadResonantFilter *aParent)
	{
		mParent = aParent;
		mY1[0] = mY2[0] = mX1[0] = mX2[0] = mY1[1] = mY2[1] = mX1[1] = mX2[1] = 0;
	}

	void BiquadResonantFilterInstance::filter(float *aBuffer, int aSamples, int aStereo, float aSamplerate)
	{
		int i, pitch, s;
		float x;

		pitch = aStereo ? 2 : 1;

		for (s = 0; s < pitch; s++)
		{
			for (i = 0; i < aSamples; i += 2)
			{
				// Generate outputs by filtering inputs.
				x = aBuffer[i * pitch + s];
				mY2[s] = (mParent->mA0 * x) + (mParent->mA1 * mX1[s]) + (mParent->mA2 * mX2[s]) - (mParent->mB1 * mY1[s]) - (mParent->mB2 * mY2[s]);
				aBuffer[i * pitch + s] = mY2[s];

				// Permute filter operations to reduce data movement.
				// Just substitute variables instead of doing mX1=x, etc.
				mX2[s] = aBuffer[(i+1) * pitch + s];
				mY1[s] = (mParent->mA0 * mX2[s]) + (mParent->mA1 * x) + (mParent->mA2 * mX1[s]) - (mParent->mB1 * mY2[s]) - (mParent->mB2 * mY1[s]);
				aBuffer[(i+1) * pitch + s] = mY1[s];

				// Only move a little data.
				mX1[s] = mX2[s];
				mX2[s] = x;
			}

			// Apply a small impulse to filter to prevent arithmetic underflow,
			// which can cause the FPU to interrupt the CPU.
			mY1[s] += (float) 1.0E-26;
		}
	}

	BiquadResonantFilterInstance::~BiquadResonantFilterInstance()
	{
	}

	void BiquadResonantFilter::init(AudioSource *aSource)
	{
	}

	BiquadResonantFilter::BiquadResonantFilter()
	{
		setParams(LOWPASS, 44100, 1000, 2);
	}

	void BiquadResonantFilter::setParams(int aType, float aSampleRate, float aFrequency, float aResonance)
	{
		float omega = (2.0f * M_PI * aFrequency) / aSampleRate;
		float sin_omega = (float)sin(omega);
		float cos_omega = (float)cos(omega);
		float alpha = sin_omega / (2.0f * aResonance);
		float scalar = 1.0f / (1.0f + alpha);

		switch (aType)
		{
		case LOWPASS:
			mA0 = 0.5f * (1.0f - cos_omega) * scalar;
			mA1 = (1.0f - cos_omega) * scalar;
			mA2 = mA0;
			mB1 = -2.0f * cos_omega * scalar;
			mB2 = (1.0f - alpha) * scalar;
			break;
		case HIGHPASS:
			mA0 = 0.5f * (1.0f + cos_omega) * scalar;
			mA1 = -(1.0f + cos_omega) * scalar;
			mA2 = mA0;
			mB1 = -2.0f * cos_omega * scalar;
			mB2 = (1.0f - alpha) * scalar;
			break;
		case BANDPASS:
			mA0 = alpha * scalar;
			mA1 = 0;
			mA2 = -mA0;
			mB1 = -2.0f * cos_omega * scalar;
			mB2 = (1.0f - alpha) * scalar;
			break;
		}
	}

	BiquadResonantFilter::~BiquadResonantFilter()
	{
	}


	BiquadResonantFilterInstance *BiquadResonantFilter::createInstance()
	{
		return new BiquadResonantFilterInstance(this);
	}
}
