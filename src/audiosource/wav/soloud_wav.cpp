/*
SoLoud audio engine
Copyright (c) 2013-2015 Jari Komppa

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
#include "soloud_file.h"
#include "stb_vorbis.h"

namespace SoLoud
{
	WavInstance::WavInstance(Wav *aParent)
	{
		mParent = aParent;
		mOffset = 0;
	}

	void WavInstance::getAudio(float *aBuffer, unsigned int aSamples)
	{		
		if (mParent->mData == NULL)
			return;

		// Buffer size may be bigger than samples, and samples may loop..

		unsigned int written = 0;
		unsigned int maxwrite = (aSamples > mParent->mSampleCount) ?  mParent->mSampleCount : aSamples;
		unsigned int channels = mChannels;

		while (written < aSamples)
		{
			unsigned int copysize = maxwrite;
			if (copysize + mOffset > mParent->mSampleCount)
			{
				copysize = mParent->mSampleCount - mOffset;
			}

			if (copysize + written > aSamples)
			{
				copysize = aSamples - written;
			}

			unsigned int i;
			for (i = 0; i < channels; i++)
			{
				memcpy(aBuffer + i * aSamples + written, mParent->mData + mOffset + i * mParent->mSampleCount, sizeof(float) * copysize);
			}

			written += copysize;
			mOffset += copysize;				
		
			if (copysize != maxwrite)
			{
				if (mFlags & AudioSourceInstance::LOOPING)
				{
					if (mOffset == mParent->mSampleCount)
					{
						mOffset = 0;
						mLoopCount++;
					}
				}
				else
				{
					for (i = 0; i < channels; i++)
					{
						memset(aBuffer + copysize + i * aSamples, 0, sizeof(float) * (aSamples - written));
					}
					mOffset += aSamples - written;
					written = aSamples;
				}
			}
		}
	}

	result WavInstance::rewind()
	{
		mOffset = 0;
		mStreamTime = 0;
		return 0;
	}

	bool WavInstance::hasEnded()
	{
		if (!(mFlags & AudioSourceInstance::LOOPING) && mOffset >= mParent->mSampleCount)
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
		stop();
		delete[] mData;
	}

#define MAKEDWORD(a,b,c,d) (((d) << 24) | ((c) << 16) | ((b) << 8) | (a))

    result Wav::loadwav(File *aReader)
	{
		/*int wavsize =*/ aReader->read32();
		if (aReader->read32() != MAKEDWORD('W','A','V','E'))
		{
			return FILE_LOAD_FAILED;
		}
		if (aReader->read32() != MAKEDWORD('f','m','t',' '))
		{
			return FILE_LOAD_FAILED;
		}
		int subchunk1size = aReader->read32();
		int audioformat = aReader->read16();
		int channels = aReader->read16();
		int samplerate = aReader->read32();
		/*int byterate =*/ aReader->read32();
		/*int blockalign =*/ aReader->read16();
		int bitspersample = aReader->read16();

		if (audioformat != 1 ||
			subchunk1size != 16 ||
			(bitspersample != 8 && bitspersample != 16))
		{
			return FILE_LOAD_FAILED;
		}
		
		int chunk = aReader->read32();
		
		if (chunk == MAKEDWORD('L','I','S','T'))
		{
			int size = aReader->read32();
			int i;
			for (i = 0; i < size; i++)
				aReader->read8();
			chunk = aReader->read32();
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

		int subchunk2size = aReader->read32();
		
		int samples = (subchunk2size / (bitspersample / 8)) / channels;
		
		mData = new float[samples * readchannels];
		
		int i, j;
		if (bitspersample == 8)
		{
			for (i = 0; i < samples; i++)
			{
				for (j = 0; j < channels; j++)
				{
					if (j == 0)
					{
						mData[i] = ((signed)aReader->read8() - 128) / (float)0x80;
					}
					else
					{
						if (readchannels > 1 && j == 1)
						{
							mData[i + samples] = ((signed)aReader->read8() - 128) / (float)0x80;
						}
						else
						{
							aReader->read8();
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
					if (j == 0)
					{
						mData[i] = ((signed short)aReader->read16()) / (float)0x8000;
					}
					else
					{
						if (readchannels > 1 && j == 1)
						{
							mData[i + samples] = ((signed short)aReader->read16()) / (float)0x8000;
						}
						else
						{
							aReader->read16();
						}
					}
				}
			}
		}
		mBaseSamplerate = (float)samplerate;
		mSampleCount = samples;

		return 0;
	}

	result Wav::loadogg(stb_vorbis *aVorbis)
	{
        stb_vorbis_info info = stb_vorbis_get_info(aVorbis);
		mBaseSamplerate = (float)info.sample_rate;
        int samples = stb_vorbis_stream_length_in_samples(aVorbis);

		int readchannels = 1;
		if (info.channels > 1)
		{
			readchannels = 2;
			mChannels = 2;
		}
		mData = new float[samples * readchannels];
		mSampleCount = samples;
		samples = 0;
		while(1)
		{
			float **outputs;
            int n = stb_vorbis_get_frame_float(aVorbis, NULL, &outputs);
			if (n == 0)
            {
				break;
            }
			if (readchannels == 1)
			{
				memcpy(mData + samples, outputs[0],sizeof(float) * n);
			}
			else
			{
				memcpy(mData + samples, outputs[0],sizeof(float) * n);
				memcpy(mData + samples + mSampleCount, outputs[1],sizeof(float) * n);
			}
			samples += n;
		}
        stb_vorbis_close(aVorbis);

		return 0;
	}

    result Wav::testAndLoadFile(File *aReader)
    {
		delete[] mData;
		mData = 0;
		mSampleCount = 0;
        int tag = aReader->read32();
		if (tag == MAKEDWORD('O','g','g','S')) 
        {
		 	aReader->seek(0);
			int e = 0;
			stb_vorbis *v = 0;
			v = stb_vorbis_open_file((Soloud_Filehack*)aReader, 0, &e, 0);

			if (0 != v)
            {
				return loadogg(v);
            }
			return FILE_LOAD_FAILED;
		} 
        else if (tag == MAKEDWORD('R','I','F','F')) 
        {
			return loadwav(aReader);
		}
		return FILE_LOAD_FAILED;
    }

	result Wav::load(const char *aFilename)
	{
		DiskFile dr;
		int res = dr.open(aFilename);
		if (res != SO_NO_ERROR)
        {
			return res;
        }
		return testAndLoadFile(&dr);
	}

	result Wav::loadMem(unsigned char *aMem, unsigned int aLength, bool aCopy, bool aTakeOwnership)
	{
		if (aMem == NULL || aLength == 0)
			return INVALID_PARAMETER;

		MemoryFile dr;
        dr.openMem(aMem, aLength, aCopy, aTakeOwnership);
		return testAndLoadFile(&dr);
	}

	result Wav::loadFile(File *aFile)
	{
		return testAndLoadFile(aFile);
	}

	AudioSourceInstance *Wav::createInstance()
	{
		return new WavInstance(this);
	}

	double Wav::getLength()
	{
		if (mBaseSamplerate == 0)
			return 0;
		return mSampleCount / mBaseSamplerate;
	}
};
