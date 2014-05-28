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
#include <stdio.h>
#include <stdlib.h>
#include "soloud.h"
#include "soloud_wavstream.h"
#include "stb_vorbis.h"

namespace SoLoud
{
	WavStreamInstance::WavStreamInstance(WavStream *aParent)
	{
		mParent = aParent;
		mOffset = 0;
		mOgg = 0;
		mFile = 0;
		if (aParent->mFilename == NULL)
			return;
		mFile = fopen(aParent->mFilename, "rb");
		if (mFile)
		{
			if (mParent->mOgg)
			{
				int e;
				mOgg = stb_vorbis_open_file(mFile, 0, &e, NULL);
				if (!mOgg)
				{
					fclose(mFile);
					mFile = 0;
				}
				mOggFrameSize = 0;
				mOggFrameOffset = 0;
				mOggOutputs = 0;
			}
			else
			{		
				fseek(mFile, aParent->mDataOffset, SEEK_SET);
			}
		}
	}

	WavStreamInstance::~WavStreamInstance()
	{
		if (mOgg)
		{
			stb_vorbis_close(mOgg);
		}
		if (mFile)
		{
			fclose(mFile);
		}
	}

	static int read32(FILE * f)
	{
		int i;
		fread(&i,1,4,f);
		return i;
	}

	static int read16(FILE * f)
	{
		short i;
		fread(&i,1,2,f);
		return i;
	}

	static int read8(FILE * f)
	{
		char i;
		fread(&i,1,1,f);
		return i;
	}

	static void getWavData(FILE * aFile, float * aBuffer, int aSamples, int aPitch, int aChannels, int aSrcChannels, int aBits)
	{
		int i, j;
		if (aBits == 8)
		{
			for (i = 0; i < aSamples; i++)
			{
				for (j = 0; j < aSrcChannels; j++)
				{
					if (j == 0)
					{
						aBuffer[i] = read8(aFile) / (float)0x80;
					}
					else
					{
						if (aChannels > 1 && j == 1)
						{
							aBuffer[i + aPitch] = read8(aFile) / (float)0x80;
						}
						else
						{
							read8(aFile);
						}
					}
				}
			}
		}
		else
		if (aBits == 16)
		{
			for (i = 0; i < aSamples; i++)
			{
				for (j = 0; j < aSrcChannels; j++)
				{
					if (j == 0)
					{
						aBuffer[i] = read16(aFile) / (float)0x8000;
					}
					else
					{
						if (aChannels > 1 && j == 1)
						{
							aBuffer[i + aPitch] = read16(aFile) / (float)0x8000;
						}
						else
						{
							read16(aFile);
						}
					}
				}
			}
		}
	}

	static int getOggData(float **aOggOutputs, float *aBuffer, int aSamples, int aPitch, int aFrameSize, int aFrameOffset, int aChannels)
	{			
		if (aFrameSize <= 0)
			return 0;

		int samples = aSamples;
		if (aFrameSize - aFrameOffset < samples)
		{
			samples = aFrameSize - aFrameOffset;
		}

		if (aChannels == 1)
		{
			memcpy(aBuffer, aOggOutputs[0] + aFrameOffset, sizeof(float) * samples);
		}
		else
		{
			memcpy(aBuffer, aOggOutputs[0] + aFrameOffset, sizeof(float) * samples);
			memcpy(aBuffer + aPitch, aOggOutputs[1] + aFrameOffset, sizeof(float) * samples);
		}
		return samples;
	}

	void WavStreamInstance::getAudio(float *aBuffer, int aSamples)
	{			
		int channels = mChannels;

		if (mFile == NULL)
			return;

		if (mOgg)
		{
			int offset = 0;			
			if (mOggFrameOffset < mOggFrameSize)
			{
				int b = getOggData(mOggOutputs, aBuffer, aSamples, aSamples, mOggFrameSize, mOggFrameOffset, channels);
				mOffset += b;
				offset += b;
				mOggFrameOffset += b;
			}

			while (offset < aSamples)
			{
				mOggFrameSize = stb_vorbis_get_frame_float(mOgg, NULL, &mOggOutputs);
				mOggFrameOffset = 0;
				int b;
				b = getOggData(mOggOutputs, aBuffer + offset, aSamples - offset, aSamples, mOggFrameSize, mOggFrameOffset, channels);
				mOffset += b;
				offset += b;
				mOggFrameOffset += b;
				if (mOffset >= mParent->mSampleCount)
				{
					if (mFlags & AudioSourceInstance::LOOPING)
					{
						stb_vorbis_seek_start(mOgg);
						mOffset = aSamples - offset;
					}
					else
					{
						int i;
						for (i = 0; i < channels; i++)
							memset(aBuffer + offset + i * aSamples, 0, sizeof(float) * (aSamples - offset));
						mOffset += aSamples - offset;
						offset = aSamples;
					}
				}
			}
		}
		else
		{
			int copysize = aSamples;
			if (copysize + mOffset > mParent->mSampleCount)
			{
				copysize = mParent->mSampleCount - mOffset;
			}

			getWavData(mFile, aBuffer, copysize, aSamples, channels, mParent->mChannels, mParent->mBits);
		
			if (copysize != aSamples)
			{
				if (mFlags & AudioSourceInstance::LOOPING)
				{
					fseek(mFile, mParent->mDataOffset, SEEK_SET);
					getWavData(mFile, aBuffer + copysize, aSamples - copysize, aSamples, channels, mParent->mChannels, mParent->mBits);
					mOffset = aSamples - copysize;
				}
				else
				{
					int i;
					for (i = 0; i < channels; i++)
						memset(aBuffer + copysize + i * aSamples, 0, sizeof(float) * (aSamples - copysize));
					mOffset += aSamples - copysize;
				}
			}
			else
			{
				mOffset += aSamples;
			}
		}
	}

	int WavStreamInstance::rewind()
	{
		if (mOgg)
		{
			stb_vorbis_seek_start(mOgg);
		}
		else
		if (mFile)
		{
			fseek(mFile, mParent->mDataOffset, SEEK_SET);
		}
		mOffset = 0;
		mStreamTime = 0;
		return 1;
	}

	bool WavStreamInstance::hasEnded()
	{
		if (mOffset >= mParent->mSampleCount && !(mFlags & AudioSourceInstance::LOOPING))
		{
			return 1;
		}
		return 0;
	}

	WavStream::WavStream()
	{
		mFilename = 0;
		mSampleCount = 0;
		mOgg = NULL;
		mDataOffset = 0;
		mBits = 0;
		mChannels = 0;
	}
	
	WavStream::~WavStream()
	{
		delete[] mFilename;
	}
	
#define MAKEDWORD(a,b,c,d) (((d) << 24) | ((c) << 16) | ((b) << 8) | (a))

	int WavStream::loadwav(FILE * fp)
	{
		int wavsize = read32(fp);
		if (read32(fp) != MAKEDWORD('W','A','V','E'))
		{
			return FILE_LOAD_FAILED;
		}
		if (read32(fp) != MAKEDWORD('f','m','t',' '))
		{
			return FILE_LOAD_FAILED;
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
			return FILE_LOAD_FAILED;
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
			return FILE_LOAD_FAILED;
		}

		int readchannels = 1;

		if (channels > 1)
		{
			readchannels = 2;
			mChannels = 2;
		}

		int subchunk2size = read32(fp);
		
		int samples = (subchunk2size / (bitspersample / 8)) / channels;
		
		mDataOffset = ftell(fp);
		mBits = bitspersample;
		mChannels = channels;	
		mBaseSamplerate = (float)samplerate;
		mSampleCount = samples;
		mOgg = 0;

		return 0;
	}

	int WavStream::loadogg(FILE * fp)
	{
		fseek(fp,0,SEEK_SET);
		int e;
		stb_vorbis *v = stb_vorbis_open_file(fp, 0, &e, NULL);
		if (!v) return FILE_LOAD_FAILED;
		stb_vorbis_info info = stb_vorbis_get_info(v);
		if (info.channels > 1)
		{
			mChannels = 2;
		}
		mBaseSamplerate = (float)info.sample_rate;
		int samples = stb_vorbis_stream_length_in_samples(v);
		stb_vorbis_close(v);
		mOgg = 1;

		mSampleCount = samples;

		return 0;
	}

	int WavStream::load(const char *aFilename)
	{
		delete[] mFilename;
		mFilename = 0;
		mSampleCount = 0;
		FILE * fp = fopen(aFilename, "rb");
		if (!fp) return FILE_NOT_FOUND;
		
		int len = strlen(aFilename);
		mFilename = new char[len+1];		
		memcpy(mFilename, aFilename, len);
		mFilename[len] = 0;
		
		int tag = read32(fp);
		int res = 0;
		if (tag == MAKEDWORD('O','g','g','S'))
		{
			res = loadogg(fp);
		}
		else
		if (tag == MAKEDWORD('R','I','F','F'))
		{
			res = loadwav(fp);
		}
		else
		{
			res = FILE_LOAD_FAILED;
		}

		if (res)
		{
			delete[] mFilename;
			mFilename = 0;
			fclose(fp);
			return res;
		}

		fclose(fp);
		return 0;
	}

	AudioSourceInstance *WavStream::createInstance()
	{
		return new WavStreamInstance(this);
	}

	double WavStream::getLength()
	{
		if (mBaseSamplerate == 0)
			return 0;
		return mSampleCount / mBaseSamplerate;
	}

};
