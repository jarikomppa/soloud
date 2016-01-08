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
#include "soloud_wavstream.h"
#include "soloud_file.h"
#include "stb_vorbis.h"

namespace SoLoud
{
	WavStreamInstance::WavStreamInstance(WavStream *aParent)
	{
		mParent = aParent;
		mOffset = 0;
		mOgg = 0;
		mFile = 0;
		if (aParent->mMemFile)
		{
			MemoryFile *mf = new MemoryFile();
			mFile = mf;
			mf->openMem(aParent->mMemFile->getMemPtr(), aParent->mMemFile->length(), false, false);
		}
		else
		if (aParent->mFilename)
		{
			DiskFile *df = new DiskFile;
			mFile = df;
			df->open(aParent->mFilename);
		}
		else
		if (aParent->mStreamFile)
		{
			mFile = aParent->mStreamFile;
			mFile->seek(0); // stb_vorbis assumes file offset to be at start of ogg
		}
		else
		{
			return;
		}
		
		if (mFile)
		{
			if (mParent->mOgg)
			{
				int e;

				mOgg = stb_vorbis_open_file((Soloud_Filehack *)mFile, 0, &e, 0);

				if (!mOgg)
				{
					if (mFile != mParent->mStreamFile)
						delete mFile;
					mFile = 0;
				}
				mOggFrameSize = 0;
				mOggFrameOffset = 0;
				mOggOutputs = 0;
			}
			else
			{		
				mFile->seek(aParent->mDataOffset);
			}
		}
	}

	WavStreamInstance::~WavStreamInstance()
	{
		if (mOgg)
		{
			stb_vorbis_close(mOgg);
		}
		if (mFile != mParent->mStreamFile)
		{
			delete mFile;
		}
	}

	static void getWavData(File * aFile, float * aBuffer, int aSamples, int aPitch, int aChannels, int aSrcChannels, int aBits)
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
						aBuffer[i] = ((signed)aFile->read8() - 128) / (float)0x80;
					}
					else
					{
						if (aChannels > 1 && j == 1)
						{
							aBuffer[i + aPitch] = ((signed)aFile->read8() - 128) / (float)0x80;
						}
						else
						{
							aFile->read8();
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
						aBuffer[i] = ((signed short)aFile->read16()) / (float)0x8000;
					}
					else
					{
						if (aChannels > 1 && j == 1)
						{
							aBuffer[i + aPitch] = ((signed short)aFile->read16()) / (float)0x8000;
						}
						else
						{
							aFile->read16();
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

	void WavStreamInstance::getAudio(float *aBuffer, unsigned int aSamples)
	{			
		unsigned int channels = mChannels;

		if (mFile == NULL)
			return;

		if (mOgg)
		{
			unsigned int offset = 0;			
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
						mLoopCount++;
					}
					else
					{
						unsigned int i;
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
			unsigned int copysize = aSamples;
			if (copysize + mOffset > mParent->mSampleCount)
			{
				copysize = mParent->mSampleCount - mOffset;
			}

			getWavData(mFile, aBuffer, copysize, aSamples, channels, mParent->mChannels, mParent->mBits);
		
			if (copysize != aSamples)
			{
				if (mFlags & AudioSourceInstance::LOOPING)
				{
					mFile->seek(mParent->mDataOffset);
					getWavData(mFile, aBuffer + copysize, aSamples - copysize, aSamples, channels, mParent->mChannels, mParent->mBits);
					mOffset = aSamples - copysize;
					mLoopCount++;
				}
				else
				{					
					unsigned int i;
					for (i = 0; i < channels; i++)
						memset(aBuffer + copysize + i * aSamples, 0, sizeof(float) * (aSamples - copysize));
						
					mOffset += aSamples;
				}
			}
			else
			{
				mOffset += aSamples;
			}
		}
	}

	result WavStreamInstance::rewind()
	{
		if (mOgg)
		{
			stb_vorbis_seek_start(mOgg);
		}
		else
		if (mFile)
		{
			mFile->seek(mParent->mDataOffset);
		}
		mOffset = 0;
		mStreamTime = 0;
		return 0;
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
		mOgg = 0;
		mDataOffset = 0;
		mBits = 0;
		mMemFile = 0;
		mStreamFile = 0;
	}
	
	WavStream::~WavStream()
	{
		stop();
		delete[] mFilename;
		delete mMemFile;
	}
	
#define MAKEDWORD(a,b,c,d) (((d) << 24) | ((c) << 16) | ((b) << 8) | (a))

	result WavStream::loadwav(File * fp)
	{
		fp->seek(4);
		int wavsize = fp->read32();
		if (fp->read32() != MAKEDWORD('W', 'A', 'V', 'E'))
		{
			return FILE_LOAD_FAILED;
		}
		if (fp->read32() != MAKEDWORD('f', 'm', 't', ' '))
		{
			return FILE_LOAD_FAILED;
		}
		int subchunk1size = fp->read32();
		int audioformat = fp->read16();
		int channels = fp->read16();
		int samplerate = fp->read32();
		int byterate = fp->read32();
		int blockalign = fp->read16();
		int bitspersample = fp->read16();

		if (audioformat != 1 ||
			subchunk1size != 16 ||
			(bitspersample != 8 && bitspersample != 16))
		{
			return FILE_LOAD_FAILED;
		}
		
		int chunk = fp->read32();
		
		if (chunk == MAKEDWORD('L','I','S','T'))
		{
			int size = fp->read32();
			int i;
			for (i = 0; i < size; i++)
				fp->read8();
			chunk = fp->read32();
		}
		
		if (chunk != MAKEDWORD('d','a','t','a'))
		{
			return FILE_LOAD_FAILED;
		}

		int readchannels = 1;

		mChannels = channels;

		if (channels > 1)
		{
			readchannels = 2;
			mChannels = 2;
		}

		int subchunk2size = fp->read32();
		
		int samples = (subchunk2size / (bitspersample / 8)) / channels;
		
		mDataOffset = fp->pos();
		mBits = bitspersample;
		mBaseSamplerate = (float)samplerate;
		mSampleCount = samples;
		mOgg = 0;

		return 0;
	}

	result WavStream::loadogg(File * fp)
	{
		fp->seek(0);
		int e;
		stb_vorbis *v;
		v = stb_vorbis_open_file((Soloud_Filehack *)fp, 0, &e, 0);
		if (v == NULL)
			return FILE_LOAD_FAILED;
		stb_vorbis_info info = stb_vorbis_get_info(v);
		mChannels = 1;
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

	result WavStream::load(const char *aFilename)
	{
		delete[] mFilename;
		delete mMemFile;
		mMemFile = 0;
		mFilename = 0;
		mSampleCount = 0;
		DiskFile fp;
		int res = fp.open(aFilename);
		if (res != SO_NO_ERROR)
			return res;
		
		int len = (int)strlen(aFilename);
		mFilename = new char[len+1];		
		memcpy(mFilename, aFilename, len);
		mFilename[len] = 0;
		
		res = parse(&fp);

		if (res != SO_NO_ERROR)
		{
			delete[] mFilename;
			mFilename = 0;
			return res;
		}

		return 0;
	}

	result WavStream::loadMem(unsigned char *aData, unsigned int aDataLen, bool aCopy, bool aTakeOwnership)
	{
		delete[] mFilename;
		delete mMemFile;
		mStreamFile = 0;
		mMemFile = 0;
		mFilename = 0;
		mSampleCount = 0;

		if (aData == NULL || aDataLen == 0)
			return INVALID_PARAMETER;

		MemoryFile *mf = new MemoryFile();
		int res = mf->openMem(aData, aDataLen, aCopy, aTakeOwnership);
		if (res != SO_NO_ERROR)
		{
			delete mf;
			return res;
		}

		res = parse(mf);

		if (res != SO_NO_ERROR)
		{
			delete mf;
			return res;
		}

		mMemFile = mf;

		return 0;
	}

	result WavStream::loadToMem(const char *aFilename)
	{
		DiskFile df;
		int res = df.open(aFilename);
		if (res == SO_NO_ERROR)
		{
			res = loadFileToMem(&df);
		}
		return res;
	}

	result WavStream::loadFile(File *aFile)
	{
		delete[] mFilename;
		delete mMemFile;
		mStreamFile = 0;
		mMemFile = 0;
		mFilename = 0;
		mSampleCount = 0;

		int res = parse(aFile);

		if (res != SO_NO_ERROR)
		{
			return res;
		}

		mStreamFile = aFile;

		return 0;
	}

	result WavStream::loadFileToMem(File *aFile)
	{
		delete[] mFilename;
		delete mMemFile;
		mStreamFile = 0;
		mMemFile = 0;
		mFilename = 0;
		mSampleCount = 0;

		MemoryFile *mf = new MemoryFile();
		int res = mf->openFileToMem(aFile);
		if (res != SO_NO_ERROR)
		{
			delete mf;
			return res;
		}

		res = parse(mf);

		if (res != SO_NO_ERROR)
		{
			delete mf;
			return res;
		}

		mMemFile = mf;

		return res;
	}


	result WavStream::parse(File *aFile)
	{
		int tag = aFile->read32();
		int res = SO_NO_ERROR;
		if (tag == MAKEDWORD('O', 'g', 'g', 'S'))
		{
			res = loadogg(aFile);
		}
		else
		if (tag == MAKEDWORD('R', 'I', 'F', 'F'))
		{
			res = loadwav(aFile);
		}
		else
		{
			res = FILE_LOAD_FAILED;
		}
		return res;
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
