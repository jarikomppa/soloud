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
	WavInstance::WavInstance(Wav *aParent)
	{
		mParent = aParent;
		mOffset = 0;
	}

	void WavInstance::getAudio(float *aBuffer, int aSamples)
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
			if (mFlags & AudioInstance::LOOPING)
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

	int WavInstance::rewind()
	{
		mOffset = 0;
		mStreamTime = 0;
		return 1;
	}

	int WavInstance::hasEnded()
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
	
	static int read32File(void **f)
	{
		int i;
		fread(&i,1,4,*reinterpret_cast<FILE**>(f));
		return i;
	}

	static int read16File(void **f)
	{
		short i;
		fread(&i,1,2,*reinterpret_cast<FILE**>(f));
		return i;
	}

	static int read8File(void **f)
	{
		char i;
		fread(&i,1,1,*reinterpret_cast<FILE**>(f));
		return i;
	}

	static int read32Mem(void **m)
	{
		int **ptr = reinterpret_cast<int**>(m);
        int i = (**ptr);
        ++(*ptr);
		return i;
	}

	static int read16Mem(void **m)
	{
		short **ptr = reinterpret_cast<short**>(m);
        short i = (**ptr);
        ++(*ptr);
		return i;
	}

	static int read8Mem(void **m)
	{
		char **ptr = reinterpret_cast<char**>(m);
        char i = (**ptr);
        ++(*ptr);
		return i;
	}

#define MAKEDWORD(a,b,c,d) (((d) << 24) | ((c) << 16) | ((b) << 8) | (a))

	void Wav::loadwav(void *src, bool isMem, int aStereo, int aChannel)
	{
        void **fp = &src;
        int (*read8)(void **) = read8File;
        int (*read16)(void **) = read16File;
        int (*read32)(void **) = read32File;
        if (isMem) {
            read8 = read8Mem;
            read16 = read16Mem;
            read32 = read32Mem;
        }
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

	void Wav::loadogg(stb_vorbis *v, int aStereo, int aChannel)
	{
		stb_vorbis_info info = stb_vorbis_get_info(v);
		mBaseSamplerate = (float)info.sample_rate;
		int samples = stb_vorbis_stream_length_in_samples(v);

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
		FILE * fp = fopen(aFilename, "rb");
		if (!fp) return;
		delete[] mData;
		mData = NULL;
		mSampleCount = 0;
        int tag = read32File(reinterpret_cast<void**>(&fp));
		if (tag == MAKEDWORD('O','g','g','S'))
		{
		    fseek(fp,0,SEEK_SET);
		    int e;
		    stb_vorbis *v = stb_vorbis_open_file(fp, 0, &e, NULL);
		    if (0 != v)
			    loadogg(v, aStereo, aChannel);
		}
		if (tag == MAKEDWORD('R','I','F','F'))
		{
			loadwav(fp, false, aStereo, aChannel);
		}
		fclose(fp);
	}

    void Wav::loadMem(unsigned char *mem, int len, int aStereo, int aChannel)
    {
		delete[] mData;
		mData = 0;
		mSampleCount = 0;
        int tag = *reinterpret_cast<const int*>(mem);
		if (tag == MAKEDWORD('O','g','g','S'))
		{
            int e = 0;
            stb_vorbis *v = stb_vorbis_open_memory(mem, len, &e, 0);
            if (0 != v)
			    loadogg(v, aStereo, aChannel);
		}
		if (tag == MAKEDWORD('R','I','F','F'))
		{
			loadwav(mem+4, true, aStereo, aChannel);
		}
    }

	AudioInstance *Wav::createInstance()
	{
		return new WavInstance(this);
	}
};
