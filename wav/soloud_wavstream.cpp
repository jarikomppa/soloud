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
#include "soloud_wavstream.h"
#include "stb_vorbis.h"

namespace SoLoud
{
	WavStreamProducer::WavStreamProducer(WavStream *aParent)
	{
		mParent = aParent;
		mOffset = 0;
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

	WavStreamProducer::~WavStreamProducer()
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

	static void getWavData(FILE * aFile, float * aBuffer, int aSamples, int aChannels, int aSrcChannels, int aChannelOffset, int aBits)
	{
		int i, j;
		if (aBits == 8)
		{
			for (i = 0; i < aSamples; i++)
			{
				for (j = 0; j < aSrcChannels; j++)
				{
					if (j == aChannelOffset)
					{
						aBuffer[i * aChannels] = read8(aFile) / (float)0x80;
					}
					else
					{
						if (aChannels > 1 && j == aChannelOffset + 1)
						{
							aBuffer[i * aChannels + 1] = read8(aFile) / (float)0x80;
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
					if (j == aChannelOffset)
					{
						aBuffer[i * aChannels] = read16(aFile) / (float)0x8000;
					}
					else
					{
						if (aChannels > 1 && j == aChannelOffset + 1)
						{
							aBuffer[i * aChannels + 1] = read16(aFile) / (float)0x8000;
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

	static int getOggData(float **aOggOutputs, float *aBuffer, int aSamples, int aFrameSize, int &aFrameOffset, int aChannelOffset, int aChannels)
	{			
		if (aFrameSize == 0)
			return 0;
		int samples = aSamples;
		if (aFrameSize - aFrameOffset < samples)
		{
			samples = aFrameSize - aFrameOffset;
		}

		if (aChannels == 1)
		{
			memcpy(aBuffer, aOggOutputs[aChannelOffset] + aFrameOffset, sizeof(float) * samples);
		}
		else
		{
			int i;
			for (i = 0; i < samples; i++)
			{
				aBuffer[i * 2] = aOggOutputs[aChannelOffset][i + aFrameOffset];
				aBuffer[i * 2 + 1] = aOggOutputs[aChannelOffset + 1][i + aFrameOffset];
			}
		}
		aFrameOffset += samples;
		return samples;
	}

	void WavStreamProducer::getAudio(float *aBuffer, int aSamples)
	{			
		int channels = 1;
		if (mFlags & STEREO)
			channels = 2;

		if (mFile == NULL)
			return;

		if (mOgg)
		{
			int offset = 0;			
			if (mOggFrameOffset < mOggFrameSize)
			{
				int b = getOggData(mOggOutputs, aBuffer, aSamples, mOggFrameSize, mOggFrameOffset, mParent->mChannelOffset, channels);
				offset += b;
				mOffset += b;
			}

			while (offset < aSamples)
			{
				mOggFrameSize = stb_vorbis_get_frame_float(mOgg, NULL, &mOggOutputs);
				mOggFrameOffset = 0;
				int b;
				b = getOggData(mOggOutputs, aBuffer + offset * channels, aSamples - offset, mOggFrameSize, mOggFrameOffset, mParent->mChannelOffset, channels);
				offset += b;
				mOffset += b;
				if (mOffset >= mParent->mSampleCount)
				{
					if (mFlags & AudioProducer::LOOPING)
					{
						stb_vorbis_close(mOgg);
						fseek(mFile, 0, SEEK_SET);
						int e;
						mOgg = stb_vorbis_open_file(mFile, 0, &e, NULL);
						mOffset = aSamples - offset;
						mStreamTime = mOffset / mSamplerate;
					}
					else
					{
						memset(aBuffer + offset * channels, 0, sizeof(float) * (aSamples - offset) * channels);
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

			getWavData(mFile, aBuffer, copysize, channels, mParent->mChannels, mParent->mChannelOffset, mParent->mBits);
		
			if (copysize != aSamples)
			{
				if (mFlags & AudioProducer::LOOPING)
				{
					fseek(mFile, mParent->mDataOffset, SEEK_SET);
					getWavData(mFile, aBuffer + copysize * channels, aSamples - copysize, channels, mParent->mChannels, mParent->mChannelOffset, mParent->mBits);
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
	}

	int WavStreamProducer::rewind()
	{
		if (mFile)
		{
			fseek(mFile, mParent->mDataOffset, SEEK_SET);
		}
		mOffset = 0;
		mStreamTime = 0;
		return 1;
	}

	int WavStreamProducer::hasEnded()
	{
		if (mOffset >= mParent->mSampleCount)
		{
			return 1;
		}
		return 0;
	}

	WavStream::WavStream()
	{
		mFilename = 0;
		mSampleCount = 0;
	}
	
	WavStream::~WavStream()
	{
		delete[] mFilename;
	}
	
#define MAKEDWORD(a,b,c,d) (((d) << 24) | ((c) << 16) | ((b) << 8) | (a))

	void WavStream::loadwav(FILE * fp, int aStereo, int aChannel)
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
		
		mDataOffset = ftell(fp);
		mChannelOffset = aChannel;
		mBits = bitspersample;
		mChannels = channels;	
		mBaseSamplerate = (float)samplerate;
		mSampleCount = samples;
		mOgg = 0;
	}

	void WavStream::loadogg(FILE * fp, int aStereo, int aChannel)
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
		mOgg = 1;

		mSampleCount = samples;
		
/*
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
*/
	}

	void WavStream::load(const char *aFilename, int aStereo, int aChannel)
	{
		delete[] mFilename;
		mFilename = 0;
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

		int len = strlen(aFilename);
		mFilename = new char[len+1];
		memcpy(mFilename, aFilename, len);
		mFilename[len] = 0;

		fclose(fp);
	}

	AudioProducer *WavStream::createProducer()
	{
		return new WavStreamProducer(this);
	}
};
