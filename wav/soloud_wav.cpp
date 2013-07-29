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
#include "stb_vorbis.h"

namespace SoLoud
{
	WavProducer::WavProducer(Wav *aParent)
	{
		mParent = aParent;
		mOffset = 0;
	}

	void WavProducer::getAudio(float *aBuffer, int aSamples)
	{		
		if (mParent->mData == NULL)
			return;

		int channels = 1;
		if (mFlags & STEREO)
			channels = 2;

		int copysize = aSamples;
		if (copysize + mOffset > mParent->mSampleCount)
		{
			copysize = mParent->mSampleCount - mOffset;
		}

		memcpy(aBuffer, mParent->mData + mOffset * channels, sizeof(float) * copysize * channels);
		
		if (copysize != aSamples)
		{
			if (mFlags & AudioProducer::LOOPING)
			{
				memcpy(aBuffer + copysize * channels, mParent->mData, sizeof(float) * (aSamples - copysize) * channels);
				mOffset = aSamples - copysize;
				mStreamTime = mOffset / mSamplerate;
			}
			else
			{
				memset(aBuffer + copysize * channels, 0, sizeof(float) * (aSamples - copysize) * channels);
				mOffset += aSamples - copysize;
			}
		}
		else
		{
			mOffset += aSamples;
		}
	}

	int WavProducer::rewind()
	{
		mOffset = 0;
		mStreamTime = 0;
		return 1;
	}

	int WavProducer::hasEnded()
	{
		if (mOffset >= mParent->mSampleCount)
		{
			return 1;
		}
		return 0;
	}

	Wav::Wav()
	{
		mData = NULL;
		mSampleCount = 0;
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
		fread(&i,1,1,f);
		return i;
	}

#define MAKEDWORD(a,b,c,d) (((d) << 24) | ((c) << 16) | ((b) << 8) | (a))

	void Wav::loadwav(FILE * fp, int aStereo, int aChannel)
	{
		int wavsize = read32(fp);
		if (read32(fp) != MAKEDWORD('W','A','V','E'))
		{
			return;
		}
		if (read32(fp) != MAKEDWORD('f','m','t',' '))
		{
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
			return;
		}
		
		int chunk = read32(fp);
		
		if (chunk == MAKEDWORD('L','I','S','T'))
		{
			int size = read32(fp);
			int i;
			for (i = 0; i < size; i++)
				read8(fp);
			chunk = read32(fp);
		}
		
		if (chunk != MAKEDWORD('d','a','t','a'))
		{
			return;
		}

		int readchannels = 1;

		if (aStereo && channels > 1)
		{
			readchannels = 2;
			mFlags |= STEREO;
		}

		int subchunk2size = read32(fp);
		
		int samples = (subchunk2size / (bitspersample / 8)) / channels;
		
		mData = new float[samples * readchannels];
		
		int i, j;
		if (bitspersample == 8)
		{
			for (i = 0; i < samples; i++)
			{
				for (j = 0; j < channels; j++)
				{
					if (j == aChannel)
					{
						mData[i * readchannels] = read8(fp) / (float)0x80;
					}
					else
					{
						if (readchannels > 1 && j == aChannel + 1)
						{
							mData[i * readchannels+1] = read8(fp) / (float)0x80;
						}
						else
						{
							read8(fp);
						}
					}
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
					{
						mData[i * readchannels] = read16(fp) / (float)0x8000;
					}
					else
					{
						if (readchannels > 1 && j == aChannel + 1)
						{
							mData[i * readchannels + 1] = read16(fp) / (float)0x8000;
						}
						else
						{
							read16(fp);
						}
					}
				}
			}
		}
		mBaseSamplerate = (float)samplerate;
		mSampleCount = samples;
	}

	void Wav::loadogg(FILE * fp, int aStereo, int aChannel)
	{
		// There's no way to know how many samples the ogg has beforehand (via stb, anyway), 
		// so we have to decode twice.
		fseek(fp,0,SEEK_SET);
		int e;
		stb_vorbis *v = stb_vorbis_open_file(fp, 0, &e, NULL);
		if (!v) return;
		stb_vorbis_info info = stb_vorbis_get_info(v);
		mBaseSamplerate = (float)info.sample_rate;
		int samples = 0;
		while(1)
		{
			float **outputs;
			int n = stb_vorbis_get_frame_float(v, NULL, &outputs);
			samples += n;
			if (n == 0)
				break;
		}
		stb_vorbis_close(v);

		// and now, again, with feeling
		fseek(fp, 0, SEEK_SET);
		v = stb_vorbis_open_file(fp, 0, &e, NULL);
		if (!v) return;

		int readchannels = 1;
		if (aStereo)
		{
			readchannels = 2;
			mFlags |= STEREO;
		}

		mData = new float[samples * readchannels];
		mSampleCount = samples;
		samples = 0;
		while(1)
		{
			float **outputs;
			int n = stb_vorbis_get_frame_float(v, NULL, &outputs);
			if (n == 0)
				break;
			if (readchannels == 1)
			{
				memcpy(mData + samples, outputs[aChannel],sizeof(float) * n);
			}
			else
			{
				int i;
				for (i = 0; i < n; i++)
				{
					mData[(samples + i) * 2] = outputs[aChannel][i];
					mData[(samples + i) * 2 + 1] = outputs[aChannel + 1][i];
				}
			}
			samples += n;
		}
		stb_vorbis_close(v);
	}

	void Wav::load(const char *aFilename, int aStereo, int aChannel)
	{
		delete[] mData;
		mData = NULL;
		mSampleCount = 0;
		FILE * fp = fopen(aFilename, "rb");
		if (!fp) return;
		int tag = read32(fp);
		if (tag == MAKEDWORD('O','g','g','S'))
		{
			loadogg(fp, aStereo, aChannel);
		}
		if (tag == MAKEDWORD('R','I','F','F'))
		{
			loadwav(fp, aStereo, aChannel);
		}
		fclose(fp);
	}

	AudioProducer *Wav::createProducer()
	{
		return new WavProducer(this);
	}
};
