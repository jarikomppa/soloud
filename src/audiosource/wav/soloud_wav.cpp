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

		// Buffer size may be bigger than samples, and samples may loop..

		int written = 0;
		int maxwrite = (aSamples > mParent->mSampleCount) ?  mParent->mSampleCount : aSamples;
		int channels = mChannels;

		while (written < aSamples)
		{
			int copysize = maxwrite;
			if (copysize + mOffset > mParent->mSampleCount)
			{
				copysize = mParent->mSampleCount - mOffset;
			}

			if (copysize + written > aSamples)
			{
				copysize = aSamples - written;
			}

			int i;
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
						mOffset = 0;
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

	int WavInstance::rewind()
	{
		mOffset = 0;
		mStreamTime = 0;
		return 1;
	}

	int WavInstance::hasEnded()
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
		delete[] mData;
	}

    // if this proves to be useful in general, we probably should make an interface
    // called DataReader. then we could implement all kinds of fancy readers
    class DataReader
    {
    public:
        DataReader()
            : mOriginalPtr(0), mCurrentPtr(0), mFilePtr(0), mIsMem(false), mLength(0) {}
        virtual ~DataReader() 
        {
            if (0 != mFilePtr) 
            {
                fclose(mFilePtr);
            }
        }
        bool open(const char *filename)
        {
            mIsMem = false;
            mFilePtr = fopen(filename, "rb");
            return (0 != mFilePtr);
        }
        bool open(unsigned char *data, int len)
        {
            mOriginalPtr = data;
            mCurrentPtr = data;
            mLength = len;
            mIsMem = true;
            return true;
        }
        int read8()
        {
            char i = 0;
            if (mIsMem) 
            {
                i = *mCurrentPtr;
                ++mCurrentPtr;
            } 
            else 
            {
                fread(&i, sizeof(char), 1, mFilePtr);
            }
            return i;
        }
        int read16()
        {
            short i = 0;
            if (mIsMem) 
            {
                i = *reinterpret_cast<short*>(mCurrentPtr);
                mCurrentPtr += sizeof(short);
            } 
            else 
            {
                fread(&i, sizeof(short), 1, mFilePtr);
            }
            return i;
        }
        int read32()
        {
            int i = 0;
            if (mIsMem) 
            {
                i = *reinterpret_cast<int*>(mCurrentPtr);
                mCurrentPtr += sizeof(int);
            } 
            else 
            {
                fread(&i, sizeof(int), 1, mFilePtr);
            }
            return i;
        }
        void seek(int offset)
        {
            if (mIsMem) 
            {
                offset = offset > (mLength - 1) ? mLength - 1 : offset;
                offset = offset < 0 ? 0 : offset;
                mCurrentPtr = mOriginalPtr + offset;
            } 
            else 
            {
                fseek(mFilePtr, offset, SEEK_SET);
            }
        }
        bool isMemoryFile() const { return mIsMem; }
        unsigned char* currentData() const { return mCurrentPtr; }
        int dataLength() const { return mLength; }
        FILE* filePtr() const { return mFilePtr; }
    private:
        unsigned char *mOriginalPtr;
        unsigned char *mCurrentPtr;
        FILE *mFilePtr;
        bool mIsMem;
        int mLength;
    };

#define MAKEDWORD(a,b,c,d) (((d) << 24) | ((c) << 16) | ((b) << 8) | (a))

    int Wav::loadwav(DataReader *aReader)
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
						mData[i] = aReader->read8() / (float)0x80;
					}
					else
					{
						if (readchannels > 1 && j == 1)
						{
							mData[i + samples] = aReader->read8() / (float)0x80;
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
						mData[i] = aReader->read16() / (float)0x8000;
					}
					else
					{
						if (readchannels > 1 && j == 1)
						{
							mData[i + samples] = aReader->read16() / (float)0x8000;
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

	int Wav::loadogg(stb_vorbis *aVorbis)
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

    int Wav::testAndLoadFile(DataReader *aReader)
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
			if (aReader->isMemoryFile())
            {
				v = stb_vorbis_open_memory(aReader->currentData(), aReader->dataLength(), &e, 0);
            }
			else
            {
				v = stb_vorbis_open_file(aReader->filePtr(), 0, &e, 0);
            }

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

	int Wav::load(const char *aFilename)
	{
		DataReader dr;
		if (!dr.open(aFilename))
        {
			return FILE_NOT_FOUND;
        }
		return testAndLoadFile(&dr);
	}

	int Wav::loadMem(unsigned char *aMem, int aLength)
	{
		if (aMem == NULL || aLength <= 0)
			return INVALID_PARAMETER;

		DataReader dr;
        dr.open(aMem, aLength);
		return testAndLoadFile(&dr);
	}

	AudioSourceInstance *Wav::createInstance()
	{
		return new WavInstance(this);
	}

	float Wav::getLength()
	{
		if (mBaseSamplerate == 0)
			return 0;
		return mSampleCount / mBaseSamplerate;
	}
};
