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

#include <string.h>
#include "soloud.h"
#include "soloud_fftfilter.h"

namespace SoLoud
{
	FFTFilterInstance::FFTFilterInstance(FFTFilter *aParent)
	{
		mParent = aParent;
		mBuffer = 0;
		initParams(1);
	}


	static void smbFft(float *fftBuffer, int fftFrameSizeLog, int sign)
	/* 
	 * COPYRIGHT 1996 Stephan M. Bernsee <smb [AT] dspdimension [DOT] com>
	 *
	 * 						The Wide Open License (WOL)
	 *
	 * Permission to use, copy, modify, distribute and sell this software and its
	 * documentation for any purpose is hereby granted without fee, provided that
	 * the above copyright notice and this license appear in all source copies. 
	 * THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF
	 * ANY KIND. See http://www.dspguru.com/wol.htm for more information.
	 *
	 * Sign = -1 is FFT, 1 is iFFT (inverse)
	 * Fills fftBuffer[0...2*fftFrameSize-1] with the Fourier transform of the
	 * time domain data in fftBuffer[0...2*fftFrameSize-1]. 
	 * The FFT array takes and returns the cosine and sine parts in an interleaved 
	 * manner, ie.	fftBuffer[0] = cosPart[0], fftBuffer[1] = sinPart[0], asf. 
	 * fftFrameSize	must be a power of 2. 
	 * It expects a complex input signal (see footnote 2), ie. when working with 
	 * 'common' audio signals our input signal has to be passed as 
	 * {in[0],0.,in[1],0.,in[2],0.,...} asf. 
	 * In that case, the transform of the frequencies of interest is in 
	 * fftBuffer[0...fftFrameSize].
	*/
	{
		float wr, wi, arg, *p1, *p2, temp;
		float tr, ti, ur, ui, *p1r, *p1i, *p2r, *p2i;
		int i, bitm, j, le, le2, k;
		int fftFrameSize = 1 << fftFrameSizeLog;

		for (i = 2; i < 2 * fftFrameSize - 2; i += 2) 
		{
			for (bitm = 2, j = 0; bitm < 2 * fftFrameSize; bitm <<= 1) 
			{
				if (i & bitm) j++;
				j <<= 1;
			}

			if (i < j) 
			{
				p1 = fftBuffer+i; 
				p2 = fftBuffer+j;
				temp = *p1; 
				*(p1++) = *p2;
				*(p2++) = temp; 
				temp = *p1;
				*p1 = *p2; 
				*p2 = temp;
			}
		}
		for (k = 0, le = 2; k < fftFrameSizeLog; k++) 
		{
			le <<= 1;
			le2 = le >> 1;
			ur = 1.0;
			ui = 0.0;
			arg = (float)(M_PI / (le2 >> 1));
			wr = cos(arg);
			wi = sign * sin(arg);
			for (j = 0; j < le2; j += 2) 
			{
				p1r = fftBuffer + j; 
				p1i = p1r + 1;
				p2r = p1r + le2; 
				p2i = p2r + 1;
				for (i = j; i < 2 * fftFrameSize; i += le) 
				{
					tr = *p2r * ur - *p2i * ui;
					ti = *p2r * ui + *p2i * ur;
					*p2r = *p1r - tr; 
					*p2i = *p1i - ti;
					*p1r += tr; 
					*p1i += ti;
					p1r += le; 
					p1i += le;
					p2r += le; 
					p2i += le;
				}
				tr = ur * wr - ui * wi;
				ui = ur * wi + ui * wr;
				ur = tr;
			}
		}
	}

	void FFTFilterInstance::filterChannel(float *aBuffer, int aSamples, float aSamplerate, float aTime, int aChannel, int aChannels)
	{
		if (aChannel == 0)
		{
			updateParams(aTime);
		}

		if (mBuffer == 0)
		{
			mBuffer = new float[SAMPLE_GRANULARITY * 2];
		}

		float * b = mBuffer;

		int i;
		for (i = 0; i < SAMPLE_GRANULARITY; i++)
		{
			b[i*2+0] = aBuffer[i];
			b[i*2+1] = 0;
		}

		static int l = (long)(log((float)SAMPLE_GRANULARITY)/log(2.)+.5);

		smbFft(b,l,-1);
		
		float temp[SAMPLE_GRANULARITY * 2];
		memcpy(temp, b, sizeof(float) * SAMPLE_GRANULARITY * 2);

		for (i = 0; i < SAMPLE_GRANULARITY * 2; i++)
		{
			b[i] = temp[(i + SAMPLE_GRANULARITY * 2 + mParent->mShift) % (SAMPLE_GRANULARITY * 2)];
		}

		smbFft(b,l,1);

		float n = 0;

		switch (mParent->mCombine)
		{
		case FFTFilter::OVER:
			for (i = 0; i < SAMPLE_GRANULARITY; i++)
			{
				n = aBuffer[i];
				n = b[i*2+0] * mParent->mScale;
				aBuffer[i] += (n - aBuffer[i]) * mParam[0];
			}
			break;
		case FFTFilter::SUBSTRACT:
			for (i = 0; i < SAMPLE_GRANULARITY; i++)
			{
				n = aBuffer[i];
				n -= b[i*2+0] * mParent->mScale;
				aBuffer[i] += (n - aBuffer[i]) * mParam[0];
			}
			break;
		case FFTFilter::MULTIPLY:
			for (i = 0; i < SAMPLE_GRANULARITY; i++)
			{
				n = aBuffer[i];
				n *= b[i*2+0] * mParent->mScale;
				aBuffer[i] += (n - aBuffer[i]) * mParam[0];
			}
			break;
		}
	}

	FFTFilterInstance::~FFTFilterInstance()
	{
		delete[] mBuffer;
	}

	FFTFilter::FFTFilter()
	{
		mScale = 0.002f;
		mCombine = SUBSTRACT;
		mShift = -15;
	}

	FilterInstance *FFTFilter::createInstance()
	{
		return new FFTFilterInstance(this);
	}

	void FFTFilter::setParameters(int aShift, int aCombine, float aScale)
	{
		mShift = aShift;
		mCombine = aCombine;
		mScale = aScale;
	}
}
