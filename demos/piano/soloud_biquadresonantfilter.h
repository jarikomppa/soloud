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

#ifndef SOLOUD_BQRFILTER_H
#define SOLOUD_BQRFILTER_H

namespace SoLoud
{
	class BiquadResonantFilter;

	class BiquadResonantFilterInstance : public FilterInstance
	{
		int mActive;
		float mY1[2], mY2[2], mX1[2], mX2[2];
		float mA0, mA1, mA2, mB1, mB2;
		int mDirty;
		int mFilterType;
		float mSampleRate;
		float mFrequency;
		float mResonance;
		BiquadResonantFilter *mParent;
		void calcBQRParams();
	public:
		virtual void filter(float *aBuffer, int aSamples, int aStereo, float aSamplerate);
		virtual ~BiquadResonantFilterInstance();
		BiquadResonantFilterInstance(BiquadResonantFilter *aParent);
	};

	class BiquadResonantFilter : public Filter
	{
	public:
		enum FILTERTYPE
		{
			NONE = 0,
			LOWPASS = 1,
			HIGHPASS = 2,
			BANDPASS = 3
		};
		int mFilterType;
		float mSampleRate;
		float mFrequency;
		float mResonance;
		virtual void init(AudioSource *aSource);
		virtual BiquadResonantFilterInstance *createInstance();
		BiquadResonantFilter();
		void setParams(int aType, float aSampleRate, float aFrequency, float aResonance);
		virtual ~BiquadResonantFilter();
	};
}

#endif