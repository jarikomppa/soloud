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

	unsigned int WavInstance::getAudio(float *aBuffer, unsigned int aSamplesToRead, unsigned int aBufferSize)
	{		
		if (mParent->mData == NULL)
			return 0;

		unsigned int dataleft = mParent->mSampleCount - mOffset;
		unsigned int copylen = dataleft;
		if (copylen > aSamplesToRead)
			copylen = aSamplesToRead;

		unsigned int i;
		for (i = 0; i < mChannels; i++)
		{
			memcpy(aBuffer + i * aBufferSize, mParent->mData + mOffset + i * mParent->mSampleCount, sizeof(float) * copylen);
		}

		mOffset += copylen;
		return copylen;
	}

	result WavInstance::rewind()
	{
		mOffset = 0;
		mStreamPosition = 0.0f;
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

	result Wav::loadwav(MemoryFile *aReader)
	{
		int filesize = aReader->read32();
		if (aReader->read32() != MAKEDWORD('W', 'A', 'V', 'E'))
		{
			return FILE_LOAD_FAILED;
		}
		filesize -= 4;

		int channels = 0;
		int bitspersample = 0;

		while (filesize > 0)
		{
			int id = aReader->read32();
			int chunkSize = aReader->read32();
			if (chunkSize & 1)
			{
				chunkSize++;
			}
			filesize -= 8;

			if (aReader->length() < aReader->pos() + chunkSize)
			{
				return FILE_LOAD_FAILED;
			}

			int chunkStart = aReader->pos();

			if (id == MAKEDWORD('f', 'm', 't', ' '))
			{
				int audioformat = aReader->read16();
				channels = aReader->read16();
				mBaseSamplerate = (float)aReader->read32();
				/*int byterate =*/ aReader->read32();
				/*int blockalign =*/ aReader->read16();
				bitspersample = aReader->read16();

				if (audioformat != 1 || (bitspersample != 8 && bitspersample != 16))
				{
					return FILE_LOAD_FAILED;
				}
			}
			else if (id == MAKEDWORD('d', 'a', 't', 'a'))
			{
				if (channels == 0 || bitspersample == 0)
					return FILE_LOAD_FAILED;

				int readchannels = 1;

				if (channels > 1)
				{
					readchannels = 2;
					mChannels = 2;
				}

				int samples = (chunkSize / (bitspersample / 8)) / channels;

				mData = new float[samples * readchannels];
				mSampleCount = samples;
				int i, j;
				if (bitspersample == 8)
				{
					unsigned char * dataptr = (unsigned char*)(aReader->getMemPtr() + aReader->pos());
					for (i = 0; i < samples; i++)
					{
						for (j = 0; j < channels; j++)
						{
							if (j == 0)
							{
								mData[i] = ((signed)*dataptr - 128) / (float)0x80;
							}
							else
							{
								if (readchannels > 1 && j == 1)
								{
									mData[i + samples] = ((signed)*dataptr - 128) / (float)0x80;
								}
							}
							dataptr++;
						}
					}
				}
				else if (bitspersample == 16)
				{
					unsigned short * dataptr = (unsigned short*)(aReader->getMemPtr() + aReader->pos());
					for (i = 0; i < samples; i++)
					{
						for (j = 0; j < channels; j++)
						{
							if (j == 0)
							{
								mData[i] = ((signed short)*dataptr) / (float)0x8000;
							}
							else
							{
								if (readchannels > 1 && j == 1)
								{
									mData[i + samples] = ((signed short)*dataptr) / (float)0x8000;
								}
							}
							dataptr++;
						}
					}
				}
			}

			// skip rest of chunk
			aReader->seek(chunkStart + chunkSize);

			filesize -= chunkSize;
		}

		return 0;
	}

	result Wav::loadogg(MemoryFile *aReader)
	{	
		int e = 0;
		stb_vorbis *vorbis = 0;
		vorbis = stb_vorbis_open_memory(aReader->getMemPtr(), aReader->length(), &e, 0);

		if (0 == vorbis)
		{
			return FILE_LOAD_FAILED;
		}

        stb_vorbis_info info = stb_vorbis_get_info(vorbis);
		mBaseSamplerate = (float)info.sample_rate;
        int samples = stb_vorbis_stream_length_in_samples(vorbis);

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
            int n = stb_vorbis_get_frame_float(vorbis, NULL, &outputs);
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
        stb_vorbis_close(vorbis);

		return 0;
	}

    result Wav::testAndLoadFile(MemoryFile *aReader)
    {
		delete[] mData;
		mData = 0;
		mSampleCount = 0;
		mChannels = 1;
        int tag = aReader->read32();
		if (tag == MAKEDWORD('O','g','g','S')) 
        {
			return loadogg(aReader);

		} 
        else if (tag == MAKEDWORD('R','I','F','F')) 
        {
			return loadwav(aReader);
		}
		return FILE_LOAD_FAILED;
    }

	result Wav::load(const char *aFilename)
	{
		if (aFilename == 0)
			return INVALID_PARAMETER;
		stop();
		DiskFile dr;
		int res = dr.open(aFilename);
		if (res == SO_NO_ERROR)
			return loadFile(&dr);
		return FILE_LOAD_FAILED;
	}

	result Wav::loadMem(unsigned char *aMem, unsigned int aLength, bool aCopy, bool aTakeOwnership)
	{
		if (aMem == NULL || aLength == 0)
			return INVALID_PARAMETER;
		stop();

		MemoryFile dr;
        dr.openMem(aMem, aLength, aCopy, aTakeOwnership);
		return testAndLoadFile(&dr);
	}

	result Wav::loadFile(File *aFile)
	{
		if (!aFile)
			return INVALID_PARAMETER;
		stop();

		MemoryFile mr;
		result res = mr.openFileToMem(aFile);

		if (res != SO_NO_ERROR)
		{
			return res;
		}
		return testAndLoadFile(&mr);
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

	result Wav::loadRawWave8(unsigned char *aMem, unsigned int aLength, float aSamplerate, unsigned int aChannels)
	{
		if (aMem == 0 || aLength == 0 || aSamplerate <= 0 || aChannels < 1)
			return INVALID_PARAMETER;
		stop();
		delete[] mData;
		mData = new float[aLength];	
		mSampleCount = aLength / aChannels;
		mChannels = aChannels;
		mBaseSamplerate = aSamplerate;
		unsigned int i;
		for (i = 0; i < aLength; i++)
			mData[i] = ((signed)aMem[i] - 128) / (float)0x80;
		return SO_NO_ERROR;
	}

	result Wav::loadRawWave16(short *aMem, unsigned int aLength, float aSamplerate, unsigned int aChannels)
	{
		if (aMem == 0 || aLength == 0 || aSamplerate <= 0 || aChannels < 1)
			return INVALID_PARAMETER;
		stop();
		delete[] mData;
		mData = new float[aLength];
		mSampleCount = aLength / aChannels;
		mChannels = aChannels;
		mBaseSamplerate = aSamplerate;
		unsigned int i;
		for (i = 0; i < aLength; i++)
			mData[i] = ((signed short)aMem[i]) / (float)0x8000;
		return SO_NO_ERROR;
	}

	result Wav::loadRawWave(float *aMem, unsigned int aLength, float aSamplerate, unsigned int aChannels, bool aCopy, bool aTakeOwndership)
	{
		if (aMem == 0 || aLength == 0 || aSamplerate <= 0 || aChannels < 1)
			return INVALID_PARAMETER;
		stop();
		delete[] mData;
		if (aCopy == true || aTakeOwndership == false)
		{
			mData = new float[aLength];
			memcpy(mData, aMem, sizeof(float) * aLength);
		}
		else
		{
			mData = aMem;
		}
		mSampleCount = aLength / aChannels;
		mChannels = aChannels;
		mBaseSamplerate = aSamplerate;
		return SO_NO_ERROR;
	}
};
