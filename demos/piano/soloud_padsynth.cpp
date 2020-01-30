/*
SoLoud audio engine
Copyright (c) 2013-2018 Jari Komppa

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

#include "soloud.h"
#include "soloud_padsynth.h"
#include "soloud_fft.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

class PADsynth {
    public:
	/*  PADsynth:
		N                - is the samplesize (eg: 262144)
		samplerate 	 - samplerate (eg. 44100)
		number_harmonics - the number of harmonics that are computed */
	PADsynth(int aSampleCount, float aSamplerate, int aHarmonicsCount);

	~PADsynth();

	/* set the amplitude of the n'th harmonic */
	void setharmonic(int n,float value);

	/* get the amplitude of the n'th harmonic */
	float getharmonic(int n);

	/*  synth() generates the wavetable
	    f		- the fundamental frequency (eg. 440 Hz)
	    bw		- bandwidth in cents of the fundamental frequency (eg. 25 cents)
	    bwscale	- how the bandwidth increase on the higher harmonics (recomanded value: 1.0)
	    *smp	- a pointer to allocated memory that can hold N samples */
	void synth(float aFundamental, float aBandwidth, float aBandwidthScale, float *aBuf);
    protected:

	/* relF():
	    This method returns the N'th overtone's position relative 
	    to the fundamental frequency.
	    By default it returns N.
	    You may override it to make metallic sounds or other 
	    instruments where the overtones are not harmonic.  */
	virtual float relF(int N);

	/* profile():
	    This is the profile of one harmonic
	    In this case is a Gaussian distribution (e^(-x^2))
            The amplitude is divided by the bandwidth to ensure that the harmonic
	    keeps the same amplitude regardless of the bandwidth */
	virtual float profile(float fi, float bwi);

	/* RND() - a random number generator that 
	    returns values between 0 and 1
	*/
	virtual float RND();

    private:
	PADsynth(const PADsynth&); // disable copy
	PADsynth& operator=(PADsynth const&);
	float *mHarmonics;		//Amplitude of the harmonics
	float *mFreqAmp;	//Amplitude spectrum
	float mSamplerate;
	int mHarmonicsCount;
	int mSampleCount;			//Size of the sample
};



PADsynth::PADsynth(int aSampleCount, float aSamplerate, int aHarmonicsCount)
{
    mSampleCount = aSampleCount;
    mSamplerate = aSamplerate;
    mHarmonicsCount = aHarmonicsCount;
    mHarmonics = new float[mHarmonicsCount];
	int i;
	for (i = 0; i < mHarmonicsCount; i++) 
		mHarmonics[i] = 0.0f;
	mHarmonics[1]=1.0;//default, the first harmonic has the amplitude 1.0

    mFreqAmp = new float[mSampleCount / 2];
};

PADsynth::~PADsynth()
{
    delete[] mHarmonics;
    delete[] mFreqAmp;
};

float PADsynth::relF(int N)
{
    return (float)N;
};

void PADsynth::setharmonic(int n,float value)
{
    if (n < 1 || n >= mHarmonicsCount) return;
    mHarmonics[n] = value;
};

float PADsynth::getharmonic(int n)
{
    if (n < 1 || n >= mHarmonicsCount) 
		return 0.0f;
    return mHarmonics[n];
};

float PADsynth::profile(float fi, float bwi)
{
    float x = fi / bwi;
    x *= x;
    if (x > 14.71280603f) 
		return 0.0f; //this avoids computing the e^(-x^2) where it's results are very close to zero
    return (float)exp(-x) / bwi;
};

void PADsynth::synth(float f,float bw,float bwscale,float *smp)
{
    int i, nh;
    
    for (i = 0; i < mSampleCount / 2; i++) 
		mFreqAmp[i] = 0.0f; //default, all the frequency amplitudes are zero

    for (nh = 1; nh < mHarmonicsCount; nh++)
	{ //for each harmonic
		float bw_Hz;//bandwidth of the current harmonic measured in Hz
		float bwi;
		float fi;
		float rF = f * relF(nh);
	
        bw_Hz = (float)((pow(2.0f,bw / 1200.0f) - 1.0f) * f * pow(relF(nh), bwscale));
	
		bwi = (float)(bw_Hz / (2.0f * mSamplerate));
		fi = rF / mSamplerate;
		for (i = 0; i < mSampleCount / 2; i++)
		{   //here you can optimize, by avoiding to compute the profile for the full frequency (usually it's zero or very close to zero)
			float hprofile = profile((i / (float)mSampleCount) - fi, bwi);
			mFreqAmp[i] += hprofile * mHarmonics[nh];
		}
    }
	
    //Convert the freq_amp array to complex array (real/imaginary) by making the phases random
    for (i = 0; i < mSampleCount / 2; i++)
	{
		float phase = RND() * 2.0f * 3.14159265358979f;
		smp[i * 2 + 0] = (float)(mFreqAmp[i] * cos(phase));
		smp[i * 2 + 1] = (float)(mFreqAmp[i] * sin(phase));
    };

	SoLoud::FFT::ifft(smp, mSampleCount);

    //normalize the output
    float max = 0.0;
	for (i = 0; i < mSampleCount; i++)
	{
		float amp = (float)fabs(smp[i]);
		if (amp > max)
		{
			max = amp;
		}
	}
    if (max < 0.000001f) max = 0.000001f;
	for (i = 0; i < mSampleCount; i++) 
		smp[i] /= max * 0.5f;
    
};

float PADsynth::RND()
{
    return (rand() / (RAND_MAX + 1.0f));
};


namespace SoLoud
{
	result generatePadsynth(
		SoLoud::Wav &aTarget, 
		unsigned int aHarmonicCount, 
		float *aHarmonics, 
		float aBandwidth,
		float aBandwidthScale,
		float aPrincipalFreq,
		float aSampleRate,
		int aSizePow)
	{
		if (aHarmonicCount < 1 || aHarmonics == NULL || aSizePow < 8 || aSizePow > 24)
			return INVALID_PARAMETER;
		int len = 1 << aSizePow;
		float *buf = new float[len];
		PADsynth p(len, aSampleRate, aHarmonicCount);
		unsigned int i;
		for (i = 0; i < aHarmonicCount; i++)
			p.setharmonic(i, aHarmonics[i]);
		p.synth(aPrincipalFreq, aBandwidth, aBandwidthScale, buf);
		aTarget.loadRawWave(buf, len, aSampleRate);
		aTarget.setLooping(true);
		return SO_NO_ERROR;
	}
}