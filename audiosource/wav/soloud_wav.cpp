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

		int channels = mChannels;

		int copysize = aSamples;
		if (copysize + mOffset > mParent->mSampleCount)
		{
			copysize = mParent->mSampleCount - mOffset;
		}

		int i;
		for (i = 0; i < channels; i++)
		{
			memcpy(aBuffer + i * aSamples, mParent->mData + mOffset + i * mParent->mSampleCount, sizeof(float) * copysize);
		}
		
		if (copysize != aSamples)
		{
			if (mFlags & AudioSourceInstance::LOOPING)
			{
				for (i = 0; i < channels; i++)
				{
					memcpy(aBuffer + copysize + i * aSamples, mParent->mData + i * mParent->mSampleCount, sizeof(float) * (aSamples - copysize));
				}
				mOffset = aSamples - copysize;
				mStreamTime = mOffset / mSamplerate;
			}
			else
			{
				for (i = 0; i < channels; i++)
				{
					memset(aBuffer + copysize + i * aSamples, 0, sizeof(float) * (aSamples - copysize));
				}
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
            : m_originalPtr(0), m_currentPtr(0), m_filePtr(0), m_isMem(false), m_length(0) {}
        virtual ~DataReader() 
        {
            if (0 != m_filePtr)
                fclose(m_filePtr);
        }
        bool open(const char *filename)
        {
            m_isMem = false;
            m_filePtr = fopen(filename, "rb");
            return (0 != m_filePtr);
        }
        bool open(unsigned char *data, int len)
        {
            m_originalPtr = data;
            m_currentPtr = data;
            m_length = len;
            m_isMem = true;
            return true;
        }
        int read8()
        {
            char i = 0;
            if (m_isMem) {
                i = *m_currentPtr;
                ++m_currentPtr;
            } else {
                fread(&i, sizeof(char), 1, m_filePtr);
            }
            return i;
        }
        int read16()
        {
            short i = 0;
            if (m_isMem) {
                i = *reinterpret_cast<short*>(m_currentPtr);
                m_currentPtr += sizeof(short);
            } else {
                fread(&i, sizeof(short), 1, m_filePtr);
            }
            return i;
        }
        int read32()
        {
            int i = 0;
            if (m_isMem) {
                i = *reinterpret_cast<int*>(m_currentPtr);
                m_currentPtr += sizeof(int);
            } else {
                fread(&i, sizeof(int), 1, m_filePtr);
            }
            return i;
        }
        void seek(int offset)
        {
            if (m_isMem) {
                offset = offset > (m_length - 1) ? m_length - 1 : offset;
                offset = offset < 0 ? 0 : offset;
                m_currentPtr = m_originalPtr + offset;
            } else {
                fseek(m_filePtr, offset, SEEK_SET);
            }
        }
        bool isMemoryFile() const { return m_isMem; }
        unsigned char* currentData() const { return m_currentPtr; }
        int dataLength() const { return m_length; }
        FILE* filePtr() const { return m_filePtr; }
    private:
        unsigned char *m_originalPtr;
        unsigned char *m_currentPtr;
        FILE *m_filePtr;
        bool m_isMem;
        int m_length;
    };

#define MAKEDWORD(a,b,c,d) (((d) << 24) | ((c) << 16) | ((b) << 8) | (a))

    void Wav::loadwav(DataReader *dr)
	{
		/*int wavsize =*/ dr->read32();
		if (dr->read32() != MAKEDWORD('W','A','V','E'))
		{
			return;
		}
		if (dr->read32() != MAKEDWORD('f','m','t',' '))
		{
			return;
		}
		int subchunk1size = dr->read32();
		int audioformat = dr->read16();
		int channels = dr->read16();
		int samplerate = dr->read32();
		/*int byterate =*/ dr->read32();
		/*int blockalign =*/ dr->read16();
		int bitspersample = dr->read16();

		if (audioformat != 1 ||
			subchunk1size != 16 ||
			(bitspersample != 8 && bitspersample != 16))
		{
			return;
		}
		
		int chunk = dr->read32();
		
		if (chunk == MAKEDWORD('L','I','S','T'))
		{
			int size = dr->read32();
			int i;
			for (i = 0; i < size; i++)
				dr->read8();
			chunk = dr->read32();
		}
		
		if (chunk != MAKEDWORD('d','a','t','a'))
		{
			return;
		}

		int readchannels = 1;

		if (channels > 1)
		{
			readchannels = 2;
			mChannels = 2;
		}

		int subchunk2size = dr->read32();
		
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
						mData[i * readchannels] = dr->read8() / (float)0x80;
					}
					else
					{
						if (readchannels > 1 && j == 1)
						{
							mData[i + samples] = dr->read8() / (float)0x80;
						}
						else
						{
							dr->read8();
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
						mData[i] = dr->read16() / (float)0x8000;
					}
					else
					{
						if (readchannels > 1 && j == 1)
						{
							mData[i + samples] = dr->read16() / (float)0x8000;
						}
						else
						{
							dr->read16();
						}
					}
				}
			}
		}
		mBaseSamplerate = (float)samplerate;
		mSampleCount = samples;
	}

	void Wav::loadogg(stb_vorbis *v)
	{
		stb_vorbis_info info = stb_vorbis_get_info(v);
		mBaseSamplerate = (float)info.sample_rate;
		int samples = stb_vorbis_stream_length_in_samples(v);

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
			int n = stb_vorbis_get_frame_float(v, NULL, &outputs);
			if (n == 0)
				break;
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
		stb_vorbis_close(v);
	}

    void Wav::testAndLoadFile(DataReader *dr)
    {
		delete[] mData;
		mData = 0;
		mSampleCount = 0;
		int tag = dr->read32();
		if (tag == MAKEDWORD('O','g','g','S')) {
		 	dr->seek(0);
			int e = 0;
			stb_vorbis *v = 0;
			if (dr->isMemoryFile())
				v = stb_vorbis_open_memory(dr->currentData(), dr->dataLength(), &e, 0);
			else
				v = stb_vorbis_open_file(dr->filePtr(), 0, &e, 0);
			if (0 != v)
				loadogg(v);
		} else if (tag == MAKEDWORD('R','I','F','F')) {
			loadwav(dr);
		}
    }

	void Wav::load(const char *aFilename)
	{
		DataReader dr;
		if (!dr.open(aFilename))
			return;
		testAndLoadFile(&dr);
	}

	void Wav::loadMem(unsigned char *mem, int len)
	{
		DataReader dr;
		dr.open(mem, len);
		testAndLoadFile(&dr);
	}

	AudioSourceInstance *Wav::createInstance()
	{
		return new WavInstance(this);
	}
};
