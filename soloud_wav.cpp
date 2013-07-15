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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "soloud.h"
#include "soloud_wav.h"

namespace SoLoud
{
	WavProducer::WavProducer(Wav *aParent)
	{
		mParent = aParent;
		mSamplerate = (float)aParent->mSamplerate;
		mOffset = 0;
	}

	void WavProducer::getAudio(float *aBuffer, int aSamples)
	{
		int copysize = aSamples;
		if (copysize + mOffset > mParent->mSamples)
		{
			copysize = mParent->mSamples - mOffset;
		}
		memcpy(aBuffer, mParent->mData + mOffset, sizeof(float) * copysize);
		if (copysize != aSamples)
		{
			if (mFlags & AudioProducer::LOOPING)
			{
				memcpy(aBuffer + copysize, mParent->mData, sizeof(float) * (aSamples - copysize));
				mOffset = aSamples - copysize;
			}
			else
			{
				memset(aBuffer + copysize, 0, sizeof(float) * (aSamples - copysize));
				mOffset += aSamples;
			}
		}
		else
		{
			mOffset += aSamples;
		}
	}

	int WavProducer::hasEnded()
	{
		if (mOffset >= mParent->mSamples)
		{
			return 1;
		}
		return 0;
	}

	Wav::Wav()
	{
		mData = NULL;
	}
	
	Wav::~Wav()
	{
		delete[] mData;
	}
	
	int read32(FILE * f)
	{
		int i;
		fread(&i,1,4,f);
		return i;
	}

	int read16(FILE * f)
	{
		short i;
		fread(&i,1,2,f);
		return i;
	}

	int read8(FILE * f)
	{
		char i;
		fread(&i,1,2,f);
		return i;
	}

#define MAKEDWORD(a,b,c,d) (((d) << 24) | ((c) << 16) | ((b) << 8) | (a))

	void Wav::load(const char *aFilename, int aChannel)
	{
		delete[] mData;
		mData = NULL;
		mSamples = 0;
		mSamplerate = 1;
		FILE * fp = fopen(aFilename, "rb");
		if (!fp) return;
		if (read32(fp) != MAKEDWORD('R','I','F','F'))
		{
			fclose(fp);
			return;
		}
		int wavsize = read32(fp);
		if (read32(fp) != MAKEDWORD('W','A','V','E'))
		{
			fclose(fp);
			return;
		}
		if (read32(fp) != MAKEDWORD('f','m','t',' '))
		{
			fclose(fp);
			return;
		}
		int subchunk1size = read32(fp);
		int audioformat = read16(fp);
		int channels = read16(fp);
		int samplerate = read32(fp);
		int byterate = read32(fp);
		int blockalign = read16(fp);
		int bitspersample = read16(fp);
		if (audioformat != 1 ||
			subchunk1size != 16 ||
			(bitspersample != 8 && bitspersample != 16))
		{
			fclose(fp);
			return;
		}
		if (read32(fp) != MAKEDWORD('d','a','t','a'))
		{
			fclose(fp);
			return;
		}
		int subchunk2size = read32(fp);
		int samples = (subchunk2size / (bitspersample / 8)) / channels;
		mData = new float[samples];
		int i, j;

		if (bitspersample == 8)
		{
			for (i = 0; i < samples; i++)
			{
				for (j = 0; j < channels; j++)
				{
					if (j == aChannel)
						mData[i] = read8(fp) / (float)0x80;
					else
						read8(fp);
				}
			}
		}
		else
		if (bitspersample == 16)
		{
			for (i = 0; i < samples; i++)
			{
				for (j = 0; j < channels; j++)
				{
					if (j == aChannel)
						mData[i] = read16(fp) / (float)0x8000;
					else
						read16(fp);
				}
			}
		}
		fclose(fp);
		mSamplerate = samplerate;
		mSamples = samples;
	}

	AudioProducer *Wav::createProducer()
	{
		return new WavProducer(this);
	}
};
